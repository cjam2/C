#!/usr/bin/env bash
set -euo pipefail

############################################
#                 CONFIG                   #
############################################
JIRA_BASE_URL="https://mm.atlassian.net"
JIRA_EMAIL="your.name@mm.com"
JIRA_API_TOKEN="PUT_API_TOKEN_HERE"

PROJECT_KEY="ENG"                 # e.g., ENG, OPS, IT
PARENT_ISSUE_TYPE_NAME="Task"     # Must exist in the project
SUBTASK_ISSUE_TYPE_NAME="Sub-task"# Must exist and be a Sub-task type
ASSIGNEE_ACCOUNT_ID=""            # Optional; leave empty for Unassigned

# Optional Advanced Roadmaps custom fields (Target start / Target end)
START_CF_ID=""    # e.g. customfield_12345 (Target start)
END_CF_ID=""      # e.g. customfield_67890 (Target end)

# Title/description
PREFIX="Monthly Ops Cycle -"
INCLUDE_MONTH_NAME=true           # true -> “September”, false -> “09”
DESCRIPTION="Automation-created issue for this month's operational tasks."

# Define your subtasks here
SUBTASKS=(
  "Cutover prep"
  "Deploy"
  "Validation"
  "Reporting"
)

# Debug switch (prints payloads and responses)
DEBUG=false

# If you’re behind a proxy you want to bypass for Atlassian, uncomment:
# export NO_PROXY=".atlassian.net,atlassian.net"
############################################

########## Utilities ##########
log()   { echo "[INFO] $*"; }
err()   { echo "[ERROR] $*" >&2; }
debug() { "$DEBUG" && echo "[DEBUG] $*" >&2 || true; }

month_name() { date +%B; }       # e.g., September
month_num()  { date +%m; }       # e.g., 09
today()      { date +%Y-%m-%d; } # e.g., 2025-09-26

month_end() {
  # End of current month (GNU/BSD compatible)
  if date -v 1d >/dev/null 2>&1; then
    # macOS/BSD date
    date -v1d -v+1m -v-1d "+%Y-%m-%d"
  else
    # GNU date
    date -d "$(date +%Y-%m-01) +1 month -1 day" +%Y-%m-%d
  fi
}

# Minimal JSON escaper (no jq/python needed)
json_escape() {
  # Usage: json_escape "string"; prints escaped string (no quotes)
  local s="$1"
  s=${s//\\/\\\\}
  s=${s//\"/\\\"}
  s=${s//$'\n'/\\n}
  s=${s//$'\r'/}
  s=${s//$'\t'/\\t}
  printf '%s' "$s"
}

########## HTTP helper ##########
post_json() {
  # $1 path (e.g., /rest/api/2/issue), $2 json
  local path="$1" json="$2"
  local resp http body
  resp="$(curl -sS -w $'\n%{http_code}' -u "$JIRA_EMAIL:$JIRA_API_TOKEN" \
          -H "Accept: application/json" \
          -H "Content-Type: application/json" \
          -X POST --data "$json" \
          "$JIRA_BASE_URL$path")"
  http="${resp##*$'\n'}"
  body="${resp%$'\n'*}"
  printf '%s\n' "$http"
  printf '%s\n' "$body"
}

# Extract "key":"ENG-123" from a JSON body without jq
extract_issue_key() {
  # stdin: JSON; stdout: key or empty
  grep -o '"key"[[:space:]]*:[[:space:]]*"[^"]*"' 2>/dev/null \
    | head -n1 \
    | sed -E 's/.*"key"[[:space:]]*:[[:space:]]*"([^"]*)".*/\1/'
}

########## Payload builders (v2) ##########
build_parent_payload() {
  local month_str summary desc esc_summary esc_desc start end
  if "$INCLUDE_MONTH_NAME"; then month_str="$(month_name)"; else month_str="$(month_num)"; fi
  summary="$PREFIX $month_str"
  desc="$DESCRIPTION"
  esc_summary="$(json_escape "$summary")"
  esc_desc="$(json_escape "$desc")"
  start="$(today)"
  end="$(month_end)"

  # Build optional fragments
  local assignee_block="" start_cf_block="" end_cf_block=""
  [[ -n "$ASSIGNEE_ACCOUNT_ID" ]] && assignee_block=', "assignee": { "accountId": "'"$ASSIGNEE_ACCOUNT_ID"'" }'
  [[ -n "$START_CF_ID" ]] && start_cf_block=', "'"$START_CF_ID"'": "'"$start"'"'
  [[ -n "$END_CF_ID"   ]] && end_cf_block=', "'"$END_CF_ID"'": "'"$end"'"'

  cat <<JSON
{
  "fields": {
    "project":   { "key": "$PROJECT_KEY" },
    "summary":   "$esc_summary",
    "description":"$esc_desc",
    "issuetype": { "name": "$PARENT_ISSUE_TYPE_NAME" },
    "duedate":   "$end"
    $assignee_block
    $(
      # Insert custom fields inside "fields" object if provided
      if [[ -n "$START_CF_ID$END_CF_ID" ]]; then
        printf '%s' "$start_cf_block$end_cf_block"
      fi
    )
  }
}
JSON
}

build_subtask_payload() {
  # $1 parent_key, $2 subtask summary
  local parent_key="$1" st_summary="$2"
  local esc_summary esc_desc start end
  esc_summary="$(json_escape "$st_summary")"
  esc_desc="$(json_escape "$DESCRIPTION")"
  start="$(today)"
  end="$(month_end)"

  local assignee_block="" start_cf_block="" end_cf_block=""
  [[ -n "$ASSIGNEE_ACCOUNT_ID" ]] && assignee_block=', "assignee": { "accountId": "'"$ASSIGNEE_ACCOUNT_ID"'" }'
  [[ -n "$START_CF_ID" ]] && start_cf_block=', "'"$START_CF_ID"'": "'"$start"'"'
  [[ -n "$END_CF_ID"   ]] && end_cf_block=', "'"$END_CF_ID"'": "'"$end"'"'

  cat <<JSON
{
  "fields": {
    "project":   { "key": "$PROJECT_KEY" },
    "parent":    { "key": "$parent_key" },
    "summary":   "$esc_summary",
    "description":"$esc_desc",
    "issuetype": { "name": "$SUBTASK_ISSUE_TYPE_NAME" },
    "duedate":   "$end"
    $assignee_block
    $(
      if [[ -n "$START_CF_ID$END_CF_ID" ]]; then
        printf '%s' "$start_cf_block$end_cf_block"
      fi
    )
  }
}
JSON
}

########## Jira actions (v2) ##########
create_parent_issue() {
  local payload http body key
  payload="$(build_parent_payload)"
  debug "Parent payload: $payload"

  read -r http body < <(post_json "/rest/api/2/issue" "$payload")
  debug "Parent HTTP: $http"
  debug "Parent body: $body"

  if [[ "$http" != "201" ]]; then
    err "Failed to create parent issue (HTTP $http)"
    printf '%s\n' "$body" >&2
    return 1
  fi

  key="$(printf '%s' "$body" | extract_issue_key)"
  if [[ -z "$key" ]]; then
    err "Parent issue created but no key found"
    printf '%s\n' "$body" >&2
    return 1
  fi
  printf '%s\n' "$key"
}

create_subtask() {
  # $1 parent_key, $2 subtask title
  local parent="$1" title="$2" payload http body key
  payload="$(build_subtask_payload "$parent" "$title")"
  debug "Subtask payload: $payload"

  read -r http body < <(post_json "/rest/api/2/issue" "$payload")
  debug "Subtask HTTP: $http"
  debug "Subtask body: $body"

  if [[ "$http" != "201" ]]; then
    err "Failed to create subtask '$title' (HTTP $http)"
    printf '%s\n' "$body" >&2
    return 1
  fi

  key="$(printf '%s' "$body" | extract_issue_key)"
  if [[ -z "$key" ]]; then
    err "Subtask created but no key found for '$title'"
    printf '%s\n' "$body" >&2
    return 1
  fi
  printf '%s\n' "$key"
}

########## Main ##########
main() {
  for dep in curl; do
    command -v "$dep" >/dev/null || { err "Missing dependency: $dep"; exit 1; }
  done

  log "Creating parent issue in project $PROJECT_KEY…"
  local parent_key
  parent_key="$(create_parent_issue)" || exit 1
  log "Parent created: $parent_key"

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