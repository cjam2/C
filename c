#!/usr/bin/env bash
set -euo pipefail

############################################
#                 CONFIG                   #
############################################
JIRA_BASE_URL="https://mm.atlassian.net"
JIRA_EMAIL="your.name@mm.com"
JIRA_API_TOKEN="PUT_API_TOKEN_HERE"

PROJECT_KEY="ENG"                   # e.g., ENG, OPS, IT
PARENT_ISSUE_TYPE_NAME="Task"       # Must exist for the project
SUBTASK_ISSUE_TYPE_NAME="Sub-task"  # Must be the Sub-task type name
ASSIGNEE_ACCOUNT_ID=""              # Optional (leave empty for Unassigned)

# Optional Advanced Roadmaps fields (if your site uses them)
START_CF_ID=""    # e.g. customfield_12345  (Target start)
END_CF_ID=""      # e.g. customfield_67890  (Target end)

# Titles/description
PREFIX="Monthly Ops Cycle -"
INCLUDE_MONTH_NAME=true             # true ⇒ “September”; false ⇒ “09”
DESCRIPTION="Automation-created issue for this month's operational tasks."

# Subtasks to create (edit freely)
SUBTASKS=(
  "Cutover prep"
  "Deploy"
  "Validation"
  "Reporting"
)

# Optional labels (uncomment & edit if you want)
# LABELS=("automation" "monthly")

# Debug: print payloads & responses
DEBUG=false
############################################

log()   { echo "[INFO] $*"; }
err()   { echo "[ERROR] $*" >&2; }
debug() { "$DEBUG" && echo "[DEBUG] $*" >&2 || true; }

month_name() { date +%B; }        # September
month_num()  { date +%m; }        # 09
today()      { date +%Y-%m-%d; }  # 2025-09-26

month_end() {
  # End of current month (GNU/BSD compatible)
  if date -v 1d >/dev/null 2>&1; then
    date -v1d -v+1m -v-1d "+%Y-%m-%d"     # macOS/BSD
  else
    date -d "$(date +%Y-%m-01) +1 month -1 day" +%Y-%m-%d  # GNU
  fi
}

require_cmd() {
  for c in "$@"; do command -v "$c" >/dev/null || { err "Missing dependency: $c"; exit 1; }; done
}

# POST JSON and capture body + HTTP code
post_json() {
  # $1 path, $2 json
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

# ---------- Payload builders (v2) ----------
build_parent_payload() {
  local start end month_str summary desc
  start="$(today)"
  end="$(month_end)"
  if "$INCLUDE_MONTH_NAME"; then month_str="$(month_name)"; else month_str="$(month_num)"; fi
  summary="$PREFIX $month_str"
  desc="$DESCRIPTION"

  # Build with jq; add optional fields conditionally
  jq -n \
    --arg project "$PROJECT_KEY" \
    --arg summary "$summary" \
    --arg desc "$desc" \
    --arg itype "$PARENT_ISSUE_TYPE_NAME" \
    --arg due   "$end" \
    --arg assg  "$ASSIGNEE_ACCOUNT_ID" \
    --arg start "$start" \
    --arg endd  "$end" \
    --arg startcf "$START_CF_ID" \
    --arg endcf   "$END_CF_ID" \
    --argjson useStartCF "$( [[ -n "$START_CF_ID" ]] && echo true || echo false )" \
    --argjson useEndCF   "$( [[ -n "$END_CF_ID"   ]] && echo true || echo false )" \
    --argjson labels     "$( if [[ ${#LABELS[@]:-0} -gt 0 ]]; then printf '%s\n' "$(printf '%s\n' "${LABELS[@]}" | jq -R . | jq -s .)"; else echo '[]'; fi )" \
  '
  {
    fields: {
      project:   { key: $project },
      summary:   $summary,
      description: $desc,
      issuetype: { name: $itype },
      duedate:   $due
    }
  }
  # assignee (optional)
  | ( if ($assg|length) > 0
      then .fields.assignee = {accountId:$assg}
      else .
    end )
  # labels (optional)
  | ( if ($labels|length) > 0
      then .fields.labels = $labels
      else .
    end )
  # custom fields (optional)
  | ( if $useStartCF then .fields += {($startcf): $start} else . end )
  | ( if $useEndCF   then .fields += {($endcf):   $endd} else . end )
  '
}

build_subtask_payload() {
  # $1 parent_key, $2 subtask summary
  local parent="$1" stsum="$2" start end
  start="$(today)"
  end="$(month_end)"

  jq -n \
    --arg project "$PROJECT_KEY" \
    --arg parent  "$parent" \
    --arg summary "$stsum" \
    --arg desc    "$DESCRIPTION" \
    --arg itype   "$SUBTASK_ISSUE_TYPE_NAME" \
    --arg due     "$end" \
    --arg assg    "$ASSIGNEE_ACCOUNT_ID" \
    --arg start   "$start" \
    --arg endd    "$end" \
    --arg startcf "$START_CF_ID" \
    --arg endcf   "$END_CF_ID" \
    --argjson useStartCF "$( [[ -n "$START_CF_ID" ]] && echo true || echo false )" \
    --argjson useEndCF   "$( [[ -n "$END_CF_ID"   ]] && echo true || echo false )" \
    --argjson labels     "$( if [[ ${#LABELS[@]:-0} -gt 0 ]]; then printf '%s\n' "$(printf '%s\n' "${LABELS[@]}" | jq -R . | jq -s .)"; else echo '[]'; fi )" \
  '
  {
    fields: {
      project:   { key: $project },
      parent:    { key: $parent },
      summary:   $summary,
      description: $desc,
      issuetype: { name: $itype },
      duedate:   $due
    }
  }
  | ( if ($assg|length) > 0
      then .fields.assignee = {accountId:$assg}
      else .
    end )
  | ( if ($labels|length) > 0
      then .fields.labels = $labels
      else .
    end )
  | ( if $useStartCF then .fields += {($startcf): $start} else . end )
  | ( if $useEndCF   then .fields += {($endcf):   $endd} else . end )
  '
}

# ---------- Jira actions (v2) ----------
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
  key="$(jq -r '.key // empty' <<<"$body")"
  [[ -z "$key" ]] && { err "Parent created but no key found"; printf '%s\n' "$body" >&2; return 1; }
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
  key="$(jq -r '.key // empty' <<<"$body")"
  [[ -z "$key" ]] && { err "Subtask created but no key found for '$title'"; printf '%s\n' "$body" >&2; return 1; }
  printf '%s\n' "$key"
}

main() {
  require_cmd curl jq

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