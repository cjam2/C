#!/usr/bin/env bash
set -euo pipefail

############################################
#                 CONFIG                   #
############################################
# Jira base URL (no trailing slash)
JIRA_BASE_URL="https://track.td.com"

# Personal Access Token (Server/Data Center) or OAuth access token
JIRA_PAT="PUT_YOUR_PAT_HERE"
AUTH_HEADER="Authorization: Bearer $JIRA_PAT"

# Project and types
PROJECT_KEY="COREIM"                # e.g., COREIM
PARENT_ISSUE_TYPE_NAME="Task"       # parent type
SUBTASK_ISSUE_TYPE_NAME="Sub-task"  # subtask type

# Assignee (Server/DC uses "name"; if your instance requires accountId, switch format below)
ASSIGNEE_NAME="jdoe"

# Optional Advanced Roadmaps fields (leave empty if not used)
TARGET_START_CF="customfield_24100"
TARGET_END_CF="customfield_24101"

# Title/description
PREFIX="Monthly Ops Cycle -"
INCLUDE_MONTH_NAME=true            # true => "September", false => "09"
DESCRIPTION="Automation-created issue for this month's operational tasks."

# Subtasks to create under the parent
SUBTASKS=(
  "Cutover prep"
  "Deploy"
  "Validation"
  "Reporting"
)

############################################
#                HELPERS                   #
############################################
today()      { date +%Y-%m-%d; }
month_name() { date +%B; }
month_num()  { date +%m; }
month_end() {
  if date -v 1d >/dev/null 2>&1; then
    date -v1d -v+1m -v-1d +%Y-%m-%d   # macOS/BSD
  else
    date -d "$(date +%Y-%m-01) +1 month -1 day" +%Y-%m-%d  # GNU
  fi
}

require_cmd() {
  for c in "$@"; do command -v "$c" >/dev/null || { echo "[ERROR] Missing $c" >&2; exit 1; }; done
}

############################################
#           PAYLOAD BUILDERS               #
############################################
build_parent_payload() {
  local start_date end_date month_str summary
  start_date="$(today)"
  end_date="$(month_end)"
  if "$INCLUDE_MONTH_NAME"; then month_str="$(month_name)"; else month_str="$(month_num)"; fi
  summary="$PREFIX $month_str"

  # If you don't use Target fields, leave variables empty; JSON lines will be harmless
  cat <<EOF
{
  "fields": {
    "project":   { "key": "$PROJECT_KEY" },
    "summary":   "$summary",
    "description":"$DESCRIPTION",
    "issuetype": { "name": "$PARENT_ISSUE_TYPE_NAME" },
    "duedate":   "$end_date",
    ${TARGET_START_CF:+\"$TARGET_START_CF\": \"$start_date\",}
    ${TARGET_END_CF:+\"$TARGET_END_CF\": \"$end_date\",}
    "assignee":  { "name": "$ASSIGNEE_NAME" }
  }
}
EOF
}

build_subtask_payload() {
  # $1 = parent key, $2 = subtask title
  local parent_key="$1" st_title="$2" start_date end_date
  start_date="$(today)"
  end_date="$(month_end)"

  cat <<EOF
{
  "fields": {
    "project":   { "key": "$PROJECT_KEY" },
    "parent":    { "key": "$parent_key" },
    "summary":   "$st_title",
    "description":"$DESCRIPTION",
    "issuetype": { "name": "$SUBTASK_ISSUE_TYPE_NAME" },
    "duedate":   "$end_date",
    ${TARGET_START_CF:+\"$TARGET_START_CF\": \"$start_date\",}
    ${TARGET_END_CF:+\"$TARGET_END_CF\": \"$end_date\",}
    "assignee":  { "name": "$ASSIGNEE_NAME" }
  }
}
EOF
}

############################################
#               API CALLS                  #
############################################
create_parent_issue() {
  local payload response
  payload="$(build_parent_payload)"

  # Create parent
  response="$(curl -sS -H "$AUTH_HEADER" \
    -H "Content-Type: application/json" \
    -X POST -d "$payload" \
    "$JIRA_BASE_URL/rest/api/2/issue")"

  echo "$response"
}

create_subtask() {
  local parent_key="$1" st_title="$2" payload response
  payload="$(build_subtask_payload "$parent_key" "$st_title")"

  response="$(curl -sS -H "$AUTH_HEADER" \
    -H "Content-Type: application/json" \
    -X POST -d "$payload" \
    "$JIRA_BASE_URL/rest/api/2/issue")"

  echo "$response"
}

############################################
#                  MAIN                    #
############################################
main() {
  require_cmd curl jq

  echo "[INFO] Creating parent in $PROJECT_KEY…"
  parent_resp="$(create_parent_issue)"
  echo "[DEBUG] Parent response: $parent_resp"

  parent_key="$(echo "$parent_resp" | jq -r '.key // empty')"
  if [[ -z "$parent_key" ]]; then
    echo "[ERROR] Could not read parent key from response" >&2
    exit 1
  fi
  echo "[INFO] Parent created: $parent_key"

  for st in "${SUBTASKS[@]}"; do
    echo "[INFO] Creating subtask: $st"
    sub_resp="$(create_subtask "$parent_key" "$st")"
    echo "[DEBUG] Subtask response: $sub_resp"
    sub_key="$(echo "$sub_resp" | jq -r '.key // empty')"
    if [[ -n "$sub_key" ]]; then
      echo "  -> $sub_key"
    else
      echo "  -> [WARN] No key in response (see debug above)"
    fi
  done

  echo "[INFO] All done."
}

main "$@"