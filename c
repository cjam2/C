<script>
AJS.toInit(function () {
  var $ = AJS.$;

  // ---- CONFIG ----
  var JIRA_BASE = "https://track.td.com";
  var PLATFORM_SEL   = '[name="Platform"]';       // select this first (but we don't listen to it)
  var PRODUCT_SEL    = '[name="ProductGroup"]';   // we ONLY listen to this

  // Optional filters; set '' or [] to disable
  var SUMMARY_FILTER = 'RAD';
  var STATUS_LIST    = ['"In Progress"', '"Refining"'];

  // Include Platform in JQL? put Jira field name or custom field id, else leave ''
  var PLATFORM_FIELD_IN_JQL = ''; // e.g. 'Platform' or 'cf[12345]'

  // If ProductGroup values aren't actual project keys, map here:
  var PROJECT_MAP = {
    // "UI label": "PROJECTKEY"
  };

  // ---- ELEMENTS ----
  var $plat = $(PLATFORM_SEL);
  var $pg   = $(PRODUCT_SEL);
  var $jql  = $("#epic-jql");
  var $link = $("#epic-link");
  var $frame= $("#epic-preview");

  if (!$pg.length)   { console.error('[EpicPanel] ProductGroup field not found:', PRODUCT_SEL); return; }
  if (!$plat.length) { console.warn('[EpicPanel] Platform field not found:', PLATFORM_SEL); }

  // ---- HELPERS ----
  function resolveProject(val){ return (PROJECT_MAP[val] || val || '').trim(); }

  function buildJql(projectKey, platformVal){
    var jql = 'issuetype=Epic AND project="' + projectKey + '"';
    if (SUMMARY_FILTER && SUMMARY_FILTER.trim()){
      jql += ' AND summary ~ "' + SUMMARY_FILTER + '"';
    }
    if (STATUS_LIST && STATUS_LIST.length){
      jql += ' AND status in (' + STATUS_LIST.join(',') + ')';
    }
    if (PLATFORM_FIELD_IN_JQL && platformVal){
      jql += ' AND "' + PLATFORM_FIELD_IN_JQL + '" = "' + platformVal + '"';
    }
    return jql + ' ORDER BY updated DESC';
  }

  function issueNavUrl(jql){ return JIRA_BASE + '/issues/?jql=' + encodeURIComponent(jql); }

  // ---- MAIN: only on ProductGroup change ----
  function onProductGroupChange(){
    var pgVal   = resolveProject($pg.val());
    var platVal = ($plat.val() || '').trim();

    if (!platVal){
      // Platform not selected yet → tell user and wait
      $jql.text('(select a Platform first)');
      $link.attr('href', '#');
      $frame.attr('src','about:blank');
      return;
    }
    if (!pgVal){
      // ProductGroup missing → tell user
      $jql.text('(select a ProductGroup)');
      $link.attr('href', '#');
      $frame.attr('src','about:blank');
      return;
    }

    var jql = buildJql(pgVal, platVal);
    var url = issueNavUrl(jql);

    $jql.text(jql);
    $link.attr('href', url);
    $frame.attr('src', url); // may be blocked by X-Frame-Options; link still works
  }

  // Listen ONLY to ProductGroup changes (per your flow)
  var t=null;
  $pg.on('change', function(){ clearTimeout(t); t = setTimeout(onProductGroupChange, 120); });

  // On initial page load, show a neutral hint (don’t auto-run)
  $jql.text('(select a Platform, then select a ProductGroup)');
  $link.attr('href', '#');
  $frame.attr('src', 'about:blank');
});
</script>