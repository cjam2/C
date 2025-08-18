<script>
AJS.toInit(function () {
  var $ = AJS.$;

  // ====== CONFIG ======
  var JIRA_BASE      = "https://track.td.com";            // your Jira base URL
  var SUMMARY_FILTER = "RAD";                              // optional: set "" to disable
  var STATUS_LIST    = ['"In Progress"', '"Refining"'];    // optional: [] to disable

  // If ProductGroup values are labels not keys, map them here
  var PROJECT_MAP = {
    // "UI Label" : "PROJECTKEY"
    // "Credit-SecuredConsumerCredit": "CRPHUCC"
  };

  // ====== SELECTORS ======
  var PG_SEL  = '[name="ProductGroup"]';  // ConfiForms ProductGroup
  var PLT_SEL = '[name="Platform"]';      // ConfiForms Platform (may drive PG)
  var JQL_EL  = '#epic-jql';
  var LINK_EL = '#epic-link';
  var IFRAME_EL = '#epic-preview';

  // ====== HELPERS ======
  function toProjectKey(rawVal, labelText) {
    if (!rawVal) return "";
    var v = String(rawVal).trim();

    // Common ConfiForms pattern: "KEY\Label" or "KEY|Label"
    var parts = v.split(/\\|\|/);
    var key = (parts[0] || "").trim();

    // If no explicit key, try map by label
    if (!key && labelText) key = PROJECT_MAP[labelText.trim()] || "";

    // Ignore platform headers / non-projects
    if (/^platform\s*:?/i.test(key)) return "";

    return key;
  }

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

  function issueNavUrl(jql) {
    if (!jql) return '#';
    // Cache-buster so the iframe always reloads
    var url = JIRA_BASE + '/issues/?jql=' + encodeURIComponent(jql);
    return url + (url.indexOf('?') > -1 ? '&' : '?') + '_ts=' + Date.now();
  }

  function applyToPanel(jql) {
    var url = issueNavUrl(jql);
    $(JQL_EL).text(jql || '(select a ProductGroup)');
    $(LINK_EL).attr('href', url);
    $(IFRAME_EL).attr('src', jql ? url : 'about:blank');
  }

  function updatePanelFromCurrentPG(reason) {
    var $pg = $(PG_SEL);
    if (!$pg.length) { console.warn('[EpicPanel] ProductGroup not found'); return; }

    var raw = $pg.val();
    var txt = $pg.find('option:selected').text() || '';
    var key = toProjectKey(raw, txt);
    var jql = buildJql(key);

    console.log('[EpicPanel] trigger:', reason,
                '| raw value =', raw,
                '| selected text =', txt,
                '| projectKey =', key,
                '| JQL =', jql);

    applyToPanel(jql);
  }

  // ====== WIRING ======
  function attach() {
    // 1) Normal events on PG
    $(document).on('change input', PG_SEL, function () {
      updatePanelFromCurrentPG('PG:change/input');
    });

    // 2) Platform may change PG programmatically; read PG shortly after
    $(document).on('change input', PLT_SEL, function () {
      setTimeout(function(){ updatePanelFromCurrentPG('PLATFORM:change→read PG'); }, 120);
    });

    // 3) MutationObserver on PG (catches programmatic option/selection changes)
    var pgEl = document.querySelector(PG_SEL.replace(/^\[name="/,'[name="'));
    if (pgEl) {
      new MutationObserver(function(){
        updatePanelFromCurrentPG('PG:MutationObserver');
      }).observe(pgEl, { attributes:true, childList:true, subtree:true, characterData:true });
      console.log('[EpicPanel] MutationObserver attached on ProductGroup');
    } else {
      console.warn('[EpicPanel] PG element not found for observer (will rely on poll)');
    }

    // 4) Small poll fallback (handles edge cases)
    (function poll(){
      var last = null;
      setInterval(function(){
        var $pg = $(PG_SEL);
        if (!$pg.length) return;
        var val = $pg.val();
        if (val !== last) {
          last = val;
          if (val != null) updatePanelFromCurrentPG('PG:poll change');
        }
      }, 700);
    })();

    // 5) Initial render
    updatePanelFromCurrentPG('PG:initial');
  }

  // Wait until the fields exist, then attach
  (function waitForFields(){
    if ($(PG_SEL).length) { attach(); }
    else { setTimeout(waitForFields, 300); }
  })();
});
</script>