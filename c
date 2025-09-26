#!/usr/bin/env bash
set -euo pipefail

############################################
#                 CONFIG                   #
############################################
JIRA_BASE_URL="https://mm.atlassian.net"
JIRA_EMAIL="your.name@mm.com"
JIRA_API_TOKEN="PUT_API_TOKEN_HERE"

PROJECT_KEY="ENG"                 # e.g., ENG, OPS, IT
PARENT_ISSUE_TYPE="Task"          # Must exist for the project
SUBTASK_ISSUE_TYPE="Sub-task"     # Must exist for the project
ASSIGNEE_ACCOUNT_ID=""            # Optional (leave blank for Unassigned)

# Optional Advanced Roadmaps “Target start / Target end” custom field IDs.
# Leave blank if you don’t use them.
START_CF_ID=""    # e.g. customfield_12345  (Target start)
END_CF_ID=""      # e.g. customfield_67890  (Target end)

# Title helpers you can customize
PREFIX="Monthly Ops Cycle -"
INCLUDE_MONTH_NAME=true           # true -> “September”, false -> “09”
DESCRIPTION="Automation-created issue for this month's operational tasks."

# Define your subtasks here (no brackets needed)
SUBTASKS=(
  "Cutover prep"
  "Deploy"
  "Validation"
  "Reporting"
)

# Debug: set to true to see payloads and responses
DEBUG=false
############################################

########## Utils: dates, text, logging ##########
month_name() { date +%B; }      # e.g., September
month_num()  { date +%m; }      # e.g., 09
today()      { date +%Y-%m-%d; }  # e.g., 2025-09-26

month_end() {
  # End of current month (Linux/macOS compatible)
  if date -v 1d >/dev/null 2>&1; then
    # BSD/macOS date
    date -v1d -v+1m -v-1d "+%Y-%m-%d"
  else
    # GNU date
    date -d "$(date +%Y-%m-01) +1 month -1 day" +%Y-%m-%d
  fi
}

log()   { echo "[INFO] $*"; }
err()   { echo "[ERROR] $*" >&2; }
debug() { "$DEBUG" && echo "[DEBUG] $*" >&2 || true; }

########## HTTP / Jira helpers ##########
auth_header() {
  printf "%s" "$(printf "%s:%s" "$JIRA_EMAIL" "$JIRA_API_TOKEN" | base64)"
}

# POST JSON and capture response body + HTTP code cleanly
post_json() {
  # $1 path (e.g., /rest/api/3/issue), $2 json
  local path="$1" json="$2"
  local resp http
  resp="$(curl -sS -w '\n%{http_code}' -X POST \
      "$JIRA_BASE_URL$path" \
      -H "Authorization: Basic $(auth_header)" \
      -H "Accept: application/json" \
      -H "Content-Type: application/json" \
      --data "$json")"
  http="${resp##*$'\n'}"
  body="${resp%$'\n'*}"
  echo "$http"
  echo "$body"
}

########## Payload builders ##########
base_issue_fields() {
  # args: summary, description, issuetype_name [, parent_key]
  local summary="$1" desc="$2" itype="$3" parent="${4-}"

  # Build base fields
  local j
  if [[ -n "$parent" ]]; then
    j=$(jq -n --arg project "$PROJECT_KEY" \
              --arg summary "$summary" \
              --arg desc "$desc" \
              --arg itype "$itype" \
              --arg parent "$parent" \
          '{
            fields:{
              project:{key:$project},
              parent:{key:$parent},
              summary:$summary,
              description:$desc,
              issuetype:{name:$itype}
            }
          }')
  else
    j=$(jq -n --arg project "$PROJECT_KEY" \
              --arg summary "$summary" \
              --arg desc "$desc" \
              --arg itype "$itype" \
          '{
            fields:{
              project:{key:$project},
              summary:$summary,
              description:$desc,
              issuetype:{name:$itype}
            }
          }')
  fi

  # Optional assignee
  if [[ -n "$ASSIGNEE_ACCOUNT_ID" ]]; then
    j=$(jq --arg id "$ASSIGNEE_ACCOUNT_ID" '.fields.assignee={accountId:$id}' <<<"$j")
  fi

  echo "$j"
}

add_dates() {
  # $1 JSON with fields, adds duedate + optional roadmap fields
  local j="$1"
  local start end
  start="$(today)"
  end="$(month_end)"

  j=$(jq --arg due "$end" '.fields.duedate=$due' <<<"$j")

  if [[ -n "$START_CF_ID" ]]; then
    j=$(jq --arg k "$START_CF_ID" --arg v "$start" '.fields[$k]=$v' <<<"$j")
  fi
  if [[ -n "$END_CF_ID" ]]; then
    j=$(jq --arg k "$END_CF_ID" --arg v "$end" '.fields[$k]=$v' <<<"$j")
  fi

  echo "$j"
}

########## Jira actions ##########
create_parent_issue() {
  local month_str summary payload http body key

  if "$INCLUDE_MONTH_NAME"; then
    month_str="$(month_name)"
  else
    month_str="$(month_num)"
  fi

  summary="$PREFIX $month_str"

  payload="$(base_issue_fields "$summary" "$DESCRIPTION" "$PARENT_ISSUE_TYPE")"
  payload="$(add_dates "$payload")"

  debug "Parent payload: $payload"
  read -r http body < <(post_json "/rest/api/3/issue" "$payload")
  debug "Parent HTTP: $http"
  debug "Parent body: $body"

  if [[ "$http" != 201 ]]; then
    err "Failed to create parent issue (HTTP $http)"
    echo "$body" >&2
    return 1
  fi

  key="$(jq -r '.key' <<<"$body")"
  if [[ -z "$key" || "$key" == "null" ]]; then
    err "Parent issue created but no key in response"
    echo "$body" >&2
    return 1
  fi

  echo "$key"
}

create_subtask() {
  # $1 parent_key, $2 summary
  local parent="$1" st_summary="$2" payload http body key

  payload="$(base_issue_fields "$st_summary" "$DESCRIPTION" "$SUBTASK_ISSUE_TYPE" "$parent")"
  payload="$(add_dates "$payload")"

  debug "Subtask payload: $payload"
  read -r http body < <(post_json "/rest/api/3/issue" "$payload")
  debug "Subtask HTTP: $http"
  debug "Subtask body: $body"

  if [[ "$http" != 201 ]]; then
    err "Failed to create subtask '$st_summary' (HTTP $http)"
    echo "$body" >&2
    return 1
  fi

  key="$(jq -r '.key' <<<"$body")"
  if [[ -z "$key" || "$key" == "null" ]]; then
    err "Subtask created but no key in response for '$st_summary'"
    echo "$body" >&2
    return 1
  fi

  echo "$key"
}

########## Main ##########
main() {
  # sanity checks
  for req in curl jq; do
    command -v "$req" >/dev/null || { err "Missing dependency: $req"; exit 1; }
  done

  log "Creating parent issue in project $PROJECT_KEY…"
  local parent_key
  parent_key="$(create_parent_issue)" || exit 1
  log "Parent created: $parent_key"

  # Create subtasks
  if ((${#SUBTASKS[@]}==0)); then
    log "No subtasks configured. Done."
    exit 0
  fi

  for st in "${SUBTASKS[@]}"; do
    log "Creating subtask: $st"
    if sub_key="$(create_subtask "$parent_key" "$st")"; then
      log "  Subtask created: $sub_key"
    else
      err "  Failed to create subtask: $st"
    fi
  done

  log "All done."
}

main "$@"