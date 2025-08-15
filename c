<!-- Dynamic Epic Query Panel -->
<div id="epic-query-panel" style="border:1px solid #ddd;border-radius:8px;padding:12px;margin:12px 0;">
  <div style="display:flex;gap:12px;align-items:center;flex-wrap:wrap;">
    <strong>Current JQL:</strong>
    <code id="epic-jql" style="background:#f7f7f7;padding:4px 6px;border-radius:4px;max-width:100%;overflow:auto;">
      (select a ProductGroup)
    </code>
    <a id="epic-link" href="#" target="_blank" rel="noopener" style="margin-left:auto;">
      Open in Jira ↗
    </a>
  </div>

  <!-- Optional inline preview (might be blocked by X-Frame-Options on your Jira) -->
  <div id="epic-preview-wrap" style="margin-top:10px;">
    <iframe id="epic-preview" src="about:blank" style="width:100%;height:520px;border:1px solid #eee;border-radius:6px;"></iframe>
    <div style="font-size:12px;color:#666;margin-top:6px;">
      If the preview doesn’t load, click “Open in Jira.” Some Jira instances block embedding.
    </div>
  </div>
</div>

<script>
AJS.toInit(function () {
  var $ = AJS.$;

  // ---- CONFIG ----
  var JIRA_BASE = "https://track.td.com";                 // your Jira base
  var SOURCE_SEL = '[name="ProductGroup"]';               // ConfiForms field (adjust if your name differs)
  var SUMMARY_FILTER = 'RAD';                             // remove or change if not needed
  var STATUS_LIST = ['"In Progress"', '"Refining"'];      // remove or edit if needed

  // If ProductGroup values are NOT project keys, map them here:
  var PROJECT_MAP = {
    // "UI label" : "PROJECTKEY"
    // e.g. "Core-IM1": "COREIM1"
  };

  // ---- ELEMENTS ----
  var $src   = $(SOURCE_SEL);
  var $jqlEl = $("#epic-jql");
  var $link  = $("#epic-link");
  var $frame = $("#epic-preview");

  function resolveProject(val) {
    if (!val) return '';
    return PROJECT_MAP[val] || val; // assume it's already a key if not in map
  }

  function buildJql(projectKey) {
    if (!projectKey) return '';
    var jql = 'issuetype=Epic AND project="' + projectKey + '"';
    if (SUMMARY_FILTER && SUMMARY_FILTER.trim()) {
      jql += ' AND summary ~ "' + SUMMARY_FILTER + '"';
    }
    if (STATUS_LIST && STATUS_LIST.length) {
      jql += ' AND status in (' + STATUS_LIST.join(',') + ')';
    }
    jql += ' ORDER BY updated DESC';
    return jql;
  }

  function issueNavUrl(jql) {
    return JIRA_BASE + '/issues/?jql=' + encodeURIComponent(jql);
  }

  function updatePanel() {
    var srcVal = ($src.val() || '').trim();
    var projectKey = resolveProject(srcVal);

    if (!projectKey) {
      $jqlEl.text('(select a ProductGroup)');
      $link.attr('href', '#');
      $frame.attr('src', 'about:blank');
      return;
    }

    var jql = buildJql(projectKey);
    var url = issueNavUrl(jql);

    $jqlEl.text(jql);
    $link.attr('href', url);
    $frame.attr('src', url); // may be blocked by X-Frame-Options on Jira
  }

  if (!$src.length) {
    console.error('[EpicPanel] Could not find ProductGroup field:', SOURCE_SEL);
    return;
  }

  var t = null;
  $src.on('change', function(){
    clearTimeout(t);
    t = setTimeout(updatePanel, 120);
  });

  // Initial render
  updatePanel();
});
</script>