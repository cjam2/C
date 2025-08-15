<!-- Dynamic JQL Panel -->
<div id="epic-query-panel" style="border:1px solid #ddd;border-radius:8px;padding:12px;margin:12px 0;">
  <div style="display:flex;gap:12px;align-items:center;flex-wrap:wrap;">
    <strong>Current JQL:</strong>
    <code id="epic-jql" style="background:#f7f7f7;padding:4px 6px;border-radius:4px;max-width:100%;overflow:auto;">
      (select a ProductGroup)
    </code>
    <a id="epic-link" href="#" target="_blank" rel="noopener" style="margin-left:auto;">Open in Jira ↗</a>
  </div>

  <!-- Optional inline preview (may be blocked by Jira's X-Frame-Options) -->
  <div style="margin-top:10px;">
    <iframe id="epic-preview" src="about:blank"
            style="width:100%;height:520px;border:1px solid #eee;border-radius:6px;"></iframe>
    <div style="font-size:12px;color:#666;margin-top:6px;">
      If the preview doesn’t load, click “Open in Jira.” Some Jira instances block embedding.
    </div>
  </div>
</div>

<script>
AJS.toInit(function () {
  var $ = AJS.$;

  // ----- CONFIG -----
  var JIRA_BASE = "https://track.td.com";                 // your Jira base URL
  var SOURCE_SEL = '[name="ProductGroup"]';               // ConfiForms ProductGroup field
  var SUMMARY_FILTER = 'RAD';                             // optional text filter
  var STATUS_LIST = ['"In Progress"', '"Refining"'];      // optional status filter

  // If ProductGroup values are *labels* not keys, map them here:
  var PROJECT_MAP = {
    // "UI Label" : "PROJECTKEY"
    // "Core-IM1": "COREIM1"
  };

  // ----- ELEMENTS -----
  var $src   = $(SOURCE_SEL);
  var $jqlEl = $("#epic-jql");
  var $link  = $("#epic-link");
  var $frame = $("#epic-preview");

  function toProjectKey(v) { return PROJECT_MAP[v] || v || ""; }

  function buildJql(projectKey) {
    if (!projectKey) return "";
    var jql = 'issuetype=Epic AND project="' + projectKey + '"';
    if (SUMMARY_FILTER && SUMMARY_FILTER.trim()) {
      jql += ' AND summary ~ "' + SUMMARY_FILTER + '"';
    }
    if (STATUS_LIST && STATUS_LIST.length) {
      jql += ' AND status in (' + STATUS_LIST.join(',') + ')';
    }
    return jql + ' ORDER BY updated DESC';
  }

  function buildIssueNavUrl(jql) {
    // Use Issue Navigator with a cache-busting param so the iframe refreshes
    var url = JIRA_BASE + '/issues/?jql=' + encodeURIComponent(jql);
    return url + (url.indexOf('?')>-1 ? '&' : '?') + '_ts=' + Date.now();
  }

  function updatePanel() {
    var raw = ($src.val() || "").trim();
    var projectKey = toProjectKey(raw);

    if (!projectKey) {
      $jqlEl.text('(select a ProductGroup)');
      $link.attr('href', '#');
      $frame.attr('src', 'about:blank');
      return;
    }

    var jql = buildJql(projectKey);
    var url = buildIssueNavUrl(jql);

    // Update visible JQL, link, and iframe preview
    $jqlEl.text(jql);
    $link.attr('href', url);
    $frame.attr('src', url);
    console.log('[EpicPanel] Updated JQL:', jql);
  }

  if (!$src.length) {
    console.error('[EpicPanel] ProductGroup field not found:', SOURCE_SEL);
    return;
  }

  // Debounced change handler
  var t = null;
  $src.on('change', function () {
    clearTimeout(t);
    t = setTimeout(updatePanel, 120);
  });

  // Initial paint (in case a value is preselected)
  updatePanel();
});
</script>