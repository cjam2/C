function toProjectKey(raw) {
  if (!raw) return "";
  // ConfiForms often gives "KEY\Label" or "KEY|Label"
  var v = String(raw).trim();

  // Split on common separators between id and label
  var parts = v.split(/\\|\|/); // backslash OR pipe
  var key = (parts[0] || "").trim();

  // Some setups send "KEY Label" or "KEY — Label"
  if (!key && v.indexOf(" ") > -1) key = v.split(" ")[0].trim();
  if (!key && v.indexOf("—") > -1) key = v.split("—")[0].trim();

  // Guard: ignore group headers like "Platform:Core"
  if (/^platform\s*:?/i.test(key)) return "";

  return key;
}





function updatePanel() {
  var raw = ($src.val() || "").trim();
  var projectKey = toProjectKey(raw);
  console.log("[EpicPanel] raw ProductGroup:", raw, "→ projectKey:", projectKey);

  if (!projectKey) {
    $jqlEl.text('(select a ProductGroup)');
    $link.attr('href', '#');
    $frame.attr('src', 'about:blank');
    return;
  }

  var jql = buildJql(projectKey);
  var url = buildIssueNavUrl(jql);
  $jqlEl.text(jql);
  $link.attr('href', url);
  $frame.attr('src', url);
}