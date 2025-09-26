build_parent_payload() {
  local start end month_str summary desc
  start="$(today)"
  end="$(month_end)"
  if "$INCLUDE_MONTH_NAME"; then month_str="$(month_name)"; else month_str="$(month_num)"; fi
  summary="$PREFIX $month_str"
  desc="$DESCRIPTION"

  jq -n \
    --arg project "$PROJECT_KEY" \
    --arg summary "$summary" \
    --arg desc    "$desc" \
    --arg itype   "$PARENT_ISSUE_TYPE_NAME" \
    --arg due     "$end" \
    --arg assg    "$ASSIGNEE_ACCOUNT_ID" \
    --arg start   "$start" \
    --arg endd    "$end" \
    --arg startcf "$START_CF_ID" \
    --arg endcf   "$END_CF_ID" \
    --argjson useStartCF "$( [[ -n "$START_CF_ID" ]] && echo true || echo false )" \
    --argjson useEndCF   "$( [[ -n "$END_CF_ID"   ]] && echo true || echo false )" \
'{
   fields: {
     project:   { key: $project },
     summary:   $summary,
     description: $desc,
     issuetype: { name: $itype },
     duedate:   $due
   }
 }
 | if ($assg|length) > 0
     then .fields.assignee = {accountId:$assg}
     else .
   end
 | if $useStartCF
     then .fields += {($startcf): $start}
     else .
   end
 | if $useEndCF
     then .fields += {($endcf): $endd}
     else .
   end'
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
'{
   fields: {
     project:   { key: $project },
     parent:    { key: $parent },
     summary:   $summary,
     description: $desc,
     issuetype: { name: $itype },
     duedate:   $due
   }
 }
 | if ($assg|length) > 0
     then .fields.assignee = {accountId:$assg}
     else .
   end
 | if $useStartCF
     then .fields += {($startcf): $start}
     else .
   end
 | if $useEndCF
     then .fields += {($endcf): $endd}
     else .
   end'
}

