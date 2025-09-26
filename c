#!/usr/bin/env bash
set -euo pipefail

############################################
#                CONFIG                    #
############################################
JIRA_BASE_URL="https://track.td.com"      # <-- your Jira base URL (no trailing /)
JIRA_PAT="PUT_YOUR_PAT_HERE"              # <-- Personal Access Token (Server/DC)
AUTH_HEADER="Authorization: Bearer $JIRA_PAT"

PROJECT_KEY="COREIM"                      # e.g., COREIM
PARENT_ISSUE_TYPE_NAME="Task"             # must exist in the project
SUBTASK_ISSUE_TYPE_NAME="Sub-task"        # must be the Sub-task type

# Optional roadmap fields (leave empty if not used)
START_CF_ID=""    # e.g., customfield_24100 (Target start)
END_CF_ID=""      # e.g., customfield_24101 (Target end)

PREFIX="Monthly Ops Cycle -"
INCLUDE_MONTH_NAME=true                   # true => “September”, false => “09”
DESCRIPTION="Automation-created issue for this month's operational tasks."

SUBTASKS=("Cutover prep" "Deploy" "Validation" "Reporting")
############################################

# ---------- small utils ----------
today()      { date +%Y-%m-%d; }
month_name() { date +%B; }
month_num()  { date +%m; }
month_end() {
  if date -v 1d >/dev/null 2>&1; then date -v1d -v+1m -v-1d +%Y-%m-%d
  else date -d "$(date +%Y-%m-01) +1 month -1 day" +%Y-%m-%d; fi
}

# ---------- HTTP helpers (Bearer) ----------
get_json() {
  # $1: path like /rest/api/2/issue/KEY?fields=parent
  curl -sS -H "$AUTH_HEADER" -H "Accept: application/json" \
       "$JIRA_BASE_URL$1"
}
post_json() {
  # $1: path, $2: json payload
  curl -sS --fail-with-body \
       -H "$AUTH_HEADER" -H "Accept: application/json" -H "Content-Type: application/json" \
       -X POST --data "$2" "$JIRA_BASE_URL$1"
}

# ---------- jq payload builders ----------
build_parent_payload() {
  local start end month_str summary
  start="$(today)"; end="$(month_end)"
  $INCLUDE_MONTH_NAME && month_str="$(month_name)" || month_str="$(month_num)"
  summary="$PREFIX $month_str"

  jq -n \
    --arg project "$PROJECT_KEY" \
    --arg summary "$summary" \
    --arg desc    "$DESCRIPTION" \
    --arg itype   "$PARENT_ISSUE_TYPE_NAME" \
    --arg due     "$end" \
    --arg startcf "$START_CF_ID" \
    --arg endcf   "$END_CF_ID" \
    --arg start   "$start" \
    --arg endd    "$end" \
    --argjson useStartCF "$( [[ -n "$START_CF_ID" ]] && echo true || echo false )" \
    --argjson useEndCF   "$( [[ -n "$END_CF_ID"   ]] && echo true || echo false )" \
'{
   fields:{
     project:{key:$project},
     summary:$summary,
     description:$desc,
     issuetype:{name:$itype},
     duedate:$due
   }
 }
 | if $useStartCF then .fields += {($startcf):$start} else . end
 | if $useEndCF   then .fields += {($endcf):$endd}   else . end'
}

build_subtask_payload() {
  # $1 parent_key, $2 subtask summary
  local parent="$1" stsum="$2" start end
  start="$(today)"; end="$(month_end)"

  jq -n \
    --arg project "$PROJECT_KEY" \
    --arg parent  "$parent" \
    --arg summary "$stsum" \
    --arg desc    "$DESCRIPTION" \
    --arg itype   "$SUBTASK_ISSUE_TYPE_NAME" \
    --arg due     "$end" \
    --arg startcf "$START_CF_ID" \
    --arg endcf   "$END_CF_ID" \
    --arg start   "$start" \
    --arg endd    "$end" \
    --argjson useStartCF "$( [[ -n "$START_CF_ID" ]] && echo true || echo false )" \
    --argjson useEndCF   "$( [[ -n "$END_CF_ID"   ]] && echo true || echo false )" \
'{
   fields:{
     project:{key:$project},
     parent:{key:$parent},
     summary:$summary,
     description:$desc,
     issuetype:{name:$itype},
     duedate:$due
   }
 }
 | if $useStartCF then .fields += {($startcf):$start} else . end
 | if $useEndCF   then .fields += {($endcf):$endd}   else . end'
}

# ---------- actions ----------
create_parent_issue() {
  local payload body key
  payload="$(build_parent_payload)"
  body="$(post_json "/rest/api/2/issue" "$payload")"
  key="$(jq -r '.key // empty' <<<"$body")"
  [[ -z "$key" ]] && { echo "[ERROR] No key in create response"; echo "$body" >&2; return 1; }
  echo "$key"
}

create_subtask() {
  local parent="$1" title="$2" payload body key
  payload="$(build_subtask_payload "$parent" "$title")"
  body="$(post_json "/rest/api/2/issue" "$payload")"
  key="$(jq -r '.key // empty' <<<"$body")"
  [[ -z "$key" ]] && { echo "[ERROR] No key for subtask '$title'"; echo "$body" >&2; return 1; }
  echo "$key"
}

# ---------- helpers you asked for ----------
get_custom_field() {
  # usage: get_custom_field ISSUE_KEY customfield_24100
  local ticket_id="$1" custom_field="$2"
  local response value
  response="$(get_json "/rest/api/2/issue/${ticket_id}?fields=${custom_field}")"
  value="$(jq -r --arg f "$custom_field" '.fields[$f] // empty' <<<"$response")"
  [[ -z "$value" || "$value" == "null" ]] && echo "" || echo "$value"
}

get_parent_key_of_issue() {
  # for a subtask, fetch its parent key
  local issue_key="$1"
  get_json "/rest/api/2/issue/${issue_key}?fields=parent" \
    | jq -r '.fields.parent.key // empty'
}

# ---------- main ----------
main() {
  # quick auth probe (Bearer; should return 200 JSON)
  get_json "/rest/api/2/myself" >/dev/null

  echo "[INFO] Creating parent in $PROJECT_KEY…"
  parent_key="$(create_parent_issue)"
  echo "[INFO] Parent: $parent_key"

  for st in "${SUBTASKS[@]}"; do
    echo "[INFO] Creating subtask: $st"
    sub_key="$(create_subtask "$parent_key" "$st")"
    echo "  -> $sub_key"
  done

  echo "[INFO] Done."
}

main "$@"