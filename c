<script>
AJS.toInit(function () {
  var $ = AJS.$;

  // ---------- CONFIG ----------
  var JIRA_BASE = "https://track.td.com";

  var PLATFORM_SEL   = '[name="Platform"]';       // pick this FIRST
  var PRODUCT_SEL    = '[name="ProductGroup"]';   // then this one

  // Optional filters; set '' or [] to disable
  var SUMMARY_FILTER = 'RAD';
  var STATUS_LIST    = ['"In Progress"', '"Refining"'];

  // Include Platform as a JQL clause? (set to Jira field name or custom field id, e.g. 'Platform' or 'cf[12345]')
  var PLATFORM_FIELD_IN_JQL = ''; // leave blank if Platform is just a gate, not a filter

  // If ProductGroup values are NOT the Jira project key, map them here:
  var PROJECT_MAP = {
    // "UI label": "PROJECTKEY"
  };

  // ---------- ELEMENTS ----------
  var $plat = $(PLATFORM_SEL);
  var $pg   = $(PRODUCT_SEL);
  var $jql  = $("#epic-jql");
  var $link = $("#epic-link");
  var $frame= $("#epic-preview");

  if (!$plat.length) { console.error('[EpicPanel] Platform field not found:', PLATFORM_SEL); return; }
  if (!$pg.length)   { console.error('[EpicPanel] ProductGroup field not found:', PRODUCT_SEL); return; }

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
    if (PLATFORM_FIELD_IN_JQL && platformVal){
      jql += ' AND "' + PLATFORM_FIELD_IN_JQL + '" = "' + platformVal + '"';
    }
    jql += ' ORDER BY updated DESC';
    return jql;
  }

  function issueNavUrl(jql){ return JIRA_BASE + '/issues/?jql=' + encodeURIComponent(jql); }

  var last = { pg:null, plat:null };

  function updatePanel(){
    var platVal = ($plat.val() || '').trim();
    if (!platVal){
      // Require Platform first; show hint
      $jql.text('(select a Platform first)');
      $link.attr('href', '#');
      $frame.attr('src', 'about:blank');
      last = { pg:null, plat:null };
      return;
    }

    var pgVal = resolveProject($pg.val());
    if (!pgVal){
      // Platform is set, but no ProductGroup yet
      $jql.text('(select a ProductGroup)');
      $link.attr('href', '#');
      $frame.attr('src', 'about:blank');
      last = { pg:null, plat:platVal };
      return;
    }

    // Skip redundant refresh
    if (last.pg === pgVal && last.plat === platVal) return;

    var jql = buildJql(pgVal, platVal);
    var url = issueNavUrl(jql);

    $jql.text(jql);
    $link.attr('href', url);
    $frame.attr('src', url); // may be blocked by X-Frame-Options; link still works

    last = { pg: pgVal, plat: platVal };
  }

  // ---------- EVENTS ----------
  var t=null;
  $plat.on('change', function(){ clearTimeout(t); t=setTimeout(updatePanel, 120); });
  $pg.on('change',   function(){ clearTimeout(t); t=setTimeout(updatePanel, 120); });

  // Do NOT update on load; we wait for Platform first.
});
</script>