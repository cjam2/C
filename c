<script>
AJS.toInit(function () {
  var $ = AJS.$;

  // ---------- CONFIG ----------
  var JIRA_BASE = "https://track.td.com";

  var PRODUCT_SEL  = '[name="ProductGroup"]';   // ConfiForms ProductGroup field (project key)
  var PLATFORM_SEL = '[name="Platform"]';       // ConfiForms Platform field

  var SUMMARY_FILTER = 'RAD';                   // optional; set '' to disable
  var STATUS_LIST    = ['"In Progress"', '"Refining"']; // optional; set [] to disable

  // If you want Platform to be REQUIRED before we build the JQL:
  var REQUIRE_PLATFORM = true;

  // If you want Platform to be INCLUDED in JQL, set the Jira field name here.
  // Examples: 'Platform' (for a system/custom field with that name) or 'cf[12345]'
  // Leave empty ('') to not include Platform in JQL.
  var PLATFORM_FIELD_IN_JQL = ''; // e.g. 'Platform' or 'cf[12345]'

  // If ProductGroup isn't the actual project key, map it here:
  var PROJECT_MAP = {
    // "UI label": "PROJECTKEY"
  };

  // ---------- ELEMENTS ----------
  var $pg   = $(PRODUCT_SEL);
  var $plat = $(PLATFORM_SEL);
  var $jql  = $("#epic-jql");
  var $link = $("#epic-link");
  var $frame= $("#epic-preview");

  if (!$pg.length)   { console.error('[EpicPanel] ProductGroup field not found:', PRODUCT_SEL); return; }
  if (!$plat.length) { console.warn('[EpicPanel] Platform field not found:', PLATFORM_SEL, '(continuing)'); }

  // ---------- HELPERS ----------
  function resolveProject(val){ return (PROJECT_MAP[val] || val || '').trim(); }

  function buildJql(projectKey, platformVal){
    if (!projectKey) return '';
    var jql = 'issuetype=Epic AND project="' + projectKey + '"';
    if (SUMMARY_FILTER && SUMMARY_FILTER.trim()){
      jql += ' AND summary ~ "' + SUMMARY_FILTER + '"';
    }
    if (STATUS_LIST && STATUS_LIST.length){
      jql += ' AND status in (' + STATUS_LIST.join(',') + ')';
    }
    // Optional: include Platform in JQL
    if (PLATFORM_FIELD_IN_JQL && platformVal){
      // exact match; tweak to ~ for contains if you prefer
      jql += ' AND "' + PLATFORM_FIELD_IN_JQL + '" = "' + platformVal + '"';
    }
    jql += ' ORDER BY updated DESC';
    return jql;
  }

  function issueNavUrl(jql){ return JIRA_BASE + '/issues/?jql=' + encodeURIComponent(jql); }

  var lastApplied = { pg:null, plat:null };

  function maybeUpdate(){
    var pgVal   = resolveProject($pg.val());
    var platVal = ($plat.val() || '').trim();

    // Gate: if Platform is required, do nothing until it's chosen
    if (!pgVal) {
      $jql.text('(select a ProductGroup)');
      $link.attr('href', '#'); $frame.attr('src','about:blank');
      return;
    }
    if (REQUIRE_PLATFORM && !$plat.length){
      console.warn('[EpicPanel] REQUIRE_PLATFORM is true but Platform field not found.');
    }
    if (REQUIRE_PLATFORM && !platVal){
      $jql.text('(select a Platform)');
      $link.attr('href', '#'); $frame.attr('src','about:blank');
      return;
    }

    // Avoid redundant reloads
    if (lastApplied.pg === pgVal && lastApplied.plat === platVal) return;

    var jql = buildJql(pgVal, platVal);
    if (!jql){ return; }

    var url = issueNavUrl(jql);
    $jql.text(jql);
    $link.attr('href', url);
    $frame.attr('src', url); // may be blocked by X-Frame-Options; link still works

    lastApplied.pg   = pgVal;
    lastApplied.plat = platVal;
  }

  // ---------- EVENTS ----------
  var t=null;
  $pg.on('change', function(){ clearTimeout(t); t=setTimeout(maybeUpdate, 120); });
  $plat.on('change', function(){ clearTimeout(t); t=setTimeout(maybeUpdate, 120); });

  // Do NOT auto-update on initial load—only after user picks fields
  // (If you want initial attempt, uncomment next line)
  // maybeUpdate();
});
</script>