get_json() {
  # $1: path like /rest/api/2/issue/KEY?fields=parent
  curl -sS -H "$AUTH_HEADER" -H "Accept: application/json" \
       "$JIRA_BASE_URL$1"
}

post_json() {
  # $1: path, $2: json payload
  local path="$1" json="$2"
  local resp http body

  resp="$(
    curl -sS -w $'\n%{http_code}' \
      -H "$AUTH_HEADER" -H "Accept: application/json" -H "Content-Type: application/json" \
      -X POST --data "$json" "$JIRA_BASE_URL$path"
  )"

  http="${resp##*$'\n'}"
  body="${resp%$'\n'*}"

  if [[ "$http" != "201" ]]; then
    echo "[ERROR] Jira returned HTTP $http" >&2
    echo "$body" >&2
    return 1
  fi

  echo "$body"
}