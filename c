name: Run on Second Monday

on:
  schedule:
    # Runs every Monday at 09:00 UTC
    - cron: "0 9 * * 1"

jobs:
  run-on-second-monday:
    runs-on: ubuntu-latest
    steps:
      - name: Check if today is the second Monday
        id: check
        run: |
          day=$(date +%d)
          if [ "$day" -ge 8 ] && [ "$day" -le 14 ]; then
            echo "is_second_monday=true" >> $GITHUB_ENV
          else
            echo "is_second_monday=false" >> $GITHUB_ENV
          fi

      - name: Run job only on second Monday
        if: env.is_second_monday == 'true'
        run: |
          echo "✅ This is the second Monday! Running job..."
          # Your actual job steps go here















#!/usr/bin/env bash
set -euo pipefail

# --- Configuration (env vars) ---
: "${JIRA_BASE_URL:?Set JIRA_BASE_URL, e.g. https://your-domain.atlassian.net}"
: "${JIRA_EMAIL:?Set JIRA_EMAIL (your Atlassian account email)}"
: "${JIRA_API_TOKEN:?Set JIRA_API_TOKEN (from https://id.atlassian.com/manage-profile/security/api-tokens)}"
: "${PROJECT_KEY:?Set PROJECT_KEY (e.g., ENG)}"

# Optional: custom field IDs for Target start / Target end (Advanced Roadmaps)
# e.g. export START_CF_ID="customfield_12345"
# e.g. export END_CF_ID="customfield_67890"
: "${START_CF_ID:=}"
: "${END_CF_ID:=}"

# Optional: customize issue types (must exist in your project)
PARENT_ISSUE_TYPE="${PARENT_ISSUE_TYPE:-Task}"
SUBTASK_ISSUE_TYPE="${SUBTASK_ISSUE_TYPE:-Sub-task}"

# Optional: assign issues (accountId), or leave empty to unassigned
ASSIGNEE_ACCOUNT_ID="${ASSIGNEE_ACCOUNT_ID:-}"

# --- Helpers ---
auth_header() {
  printf "%s" "$(printf "%s:%s" "$JIRA_EMAIL" "$JIRA_API_TOKEN" | base64)"
}

json_base_fields() {
  # $1 summary, $2 description
  jq -n --arg project "$PROJECT_KEY" \
        --arg summary "$1" \
        --arg desc "$2" \
        --arg issType "$3" \
        --arg assignee "$ASSIGNEE_ACCOUNT_ID" \
  '
  {
    fields: {
      project: { key: $project },
      summary: $summary,
      description: $desc,
      issuetype: { name: $issType }
    }
  }
  + ( $assignee | length > 0
      ? { fields: { assignee: { accountId: $assignee } } }
      : {} )
  '
}

date_today() {
  # YYYY-MM-DD in local time
  date "+%Y-%m-%d"
}

month_end() {
  # End of current month (works with GNU date). If not GNU, falls back to python.
  if date -v 1d >/dev/null 2>&1; then
    # macOS BSD date: go to 1st of next month, then -1 day
    date -j -f "%Y-%m-%d" "$(date "+%Y-%m-01")" "+%Y-%m-%d" >/dev/null 2>&1 || true
    first_next_month=$(date -v1d -v+1m "+%Y-%m-%d")
    date -j -f "%Y-%m-%d" "$first_next_month" "+%Y-%m-%d" >/dev/null 2>&1 || true
    date -j -v1d -v+1m -v-1d "+%Y-%m-%d"
  elif date -d "today" >/dev/null 2>&1; then
    # GNU date (Linux)
    date -d "$(date +%Y-%m-01) +1 month -1 day" +%Y-%m-%d
  else
    # Fallback: python
    python - "$@" <<'PY'
from datetime import date, timedelta
today = date.today()
first = today.replace(day=1)
if first.month == 12:
    first_next = first.replace(year=first.year+1, month=1)
else:
    first_next = first.replace(month=first.month+1)
last_this = first_next - timedelta(days=1)
print(last_this.isoformat())
PY
  fi
}

add_target_dates() {
  # $1 JSON doc, adds duedate + optional custom fields
  local json="$1"
  local start_date end_date
  start_date="$(date_today)"
  end_date="$(month_end)"

  # Build date fields block
  local extra='{"fields":{ "duedate":"'"$end_date"'" }}'

  if [[ -n "$START_CF_ID" ]]; then
    extra=$(echo "$extra" | jq --arg key "$START_CF_ID" --arg val "$start_date" '.fields += {($key): $val}')
  fi
  if [[ -n "$END_CF_ID" ]]; then
    extra=$(echo "$extra" | jq --arg key "$END_CF_ID" --arg val "$end_date" '.fields += {($key): $val}')
  fi

  jq -n --argjson a "$json" --argjson b "$extra" '$a * $b'
}

create_issue() {
  # args: summary, description, issueType
  local summary="$1" desc="$2" issueType="$3"
  local payload base enriched
  base=$(json_base_fields "$summary" "$desc" "$issueType")
  enriched=$(add_target_dates "$base")

  curl -sS -X POST "$JIRA_BASE_URL/rest/api/3/issue" \
    -H "Authorization: Basic $(auth_header)" \
    -H "Accept: application/json" \
    -H "Content-Type: application/json" \
    --data "$enriched"
}

create_subtask() {
  # args: parentKey, summary
  local parentKey="$1" summary="$2"
  local base enriched payload

  base=$(jq -n \
    --arg project "$PROJECT_KEY" \
    --arg parent "$parentKey" \
    --arg summary "$summary" \
    --arg issType "$SUBTASK_ISSUE_TYPE" \
    '
    {
      fields: {
        project: { key: $project },
        parent: { key: $parent },
        summary: $summary,
        issuetype: { name: $issType }
      }
    }
    '
  )

  enriched=$(add_target_dates "$base")

  curl -sS -X POST "$JIRA_BASE_URL/rest/api/3/issue" \
    -H "Authorization: Basic $(auth_header)" \
    -H "Accept: application/json" \
    -H "Content-Type: application/json" \
    --data "$enriched"
}

extract_issue_key() {
  jq -r '.key'
}

# --- Usage / Args ---
usage() {
  cat <<'USAGE'
Usage:
  create_jira_with_subtasks.sh "<Parent Summary>" "<Parent Description>" "[Subtask 1]" "[Subtask 2]" ...

Notes:
- Subtask titles are passed in square brackets, separated by spaces, e.g.:
    "[Cutover prep]" "[Deploy]" "[Validation]"
- Required env vars: JIRA_BASE_URL, JIRA_EMAIL, JIRA_API_TOKEN, PROJECT_KEY
- Optional env vars:
    START_CF_ID     customfield id for Target start (e.g. customfield_12345)
    END_CF_ID       customfield id for Target end   (e.g. customfield_67890)
    PARENT_ISSUE_TYPE (default: Task)
    SUBTASK_ISSUE_TYPE (default: Sub-task)
    ASSIGNEE_ACCOUNT_ID (accountId to assign parent + subtasks)

USAGE
}

if [[ $# -lt 2 ]]; then
  usage
  exit 1
fi

PARENT_SUMMARY="$1"
PARENT_DESC="$2"
shift 2

# Collect bracketed subtask titles from remaining args
SUBTASKS=()
for arg in "$@"; do
  # Extract content inside [ ... ]
  if [[ "$arg" =~ ^\[(.*)\]$ ]]; then
    title="${BASH_REMATCH[1]}"
    [[ -n "$title" ]] && SUBTASKS+=( "$title" )
  fi
done

echo "Creating parent issue in $PROJECT_KEY ..."
parent_resp="$(create_issue "$PARENT_SUMMARY" "$PARENT_DESC" "$PARENT_ISSUE_TYPE")"
parent_key="$(echo "$parent_resp" | extract_issue_key)"

if [[ "$parent_key" == "null" || -z "$parent_key" ]]; then
  echo "Error creating parent issue:"
  echo "$parent_resp"
  exit 1
fi

echo "Created parent: $parent_key"

# Create subtasks
for st in "${SUBTASKS[@]}"; do
  echo "Creating subtask: $st"
  sub_resp="$(create_subtask "$parent_key" "$st")"
  sub_key="$(echo "$sub_resp" | extract_issue_key)"
  if [[ "$sub_key" == "null" || -z "$sub_key" ]]; then
    echo "  Failed to create subtask. Response:"
    echo "$sub_resp"
  else
    echo "  Created subtask: $sub_key"
  fi
done

echo "Done."





chmod +x create_jira_with_subtasks.sh

./create_jira_with_subtasks.sh \
  "Monthly Ops Cycle" \
  "Automation-created issue for this month's operational tasks." \
  "[Cutover prep]" "[Deploy]" "[Validation]" "[Reporting]"