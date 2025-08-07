<script src="https://connect-cdn.atl-paas.net/all.js"></script>
<script>
/* ConfiForms → Jira Epic loader (Confluence version)
   - Runs on load
   - Uses AP.request if available, else fetch() directly
   - Jira base URL: https://track.td.com
*/

(function () {
  var JIRA_BASE_URL = "https://track.td.com"; // fixed Jira URL
  var SOURCE_FIELD_NAME = "ProductGroup"; // ConfiForms source dropdown
  var TARGET_FIELD_NAME = "jiraEpic";     // ConfiForms target dropdown
  var SEARCH_MODE = "project";            // "project" | "label" | "summary"
  var INCLUDE_DONE = false;

  console.log("[EpicLoader] Script loaded ✓");

  function escapeHtml(str) { return AJS.$("<div>").text(String(str || "")).html(); }

  function buildJql(productGroupValue) {
    var base;
    if (SEARCH_MODE === "label") {
      base = 'issuetype=Epic AND labels = "' + productGroupValue + '"';
    } else if (SEARCH_MODE === "summary") {
      base = 'issuetype=Epic AND summary ~ "' + productGroupValue + '"';
    } else {
      base = 'project=' + productGroupValue + ' AND issuetype=Epic';
    }
    if (!INCLUDE_DONE) base += ' AND statusCategory != Done';
    return base + ' ORDER BY updated DESC';
  }

  function setLoading($sel, isLoading) {
    $sel.prop("disabled", isLoading);
    $sel.html('<option>' + (isLoading ? 'Loading…' : 'Select an epic…') + '</option>');
  }

  function fillOptions($sel, issues) {
    if (!issues || !issues.length) {
      $sel.html('<option value="">No epics found</option>');
      return;
    }
    var html = ['<option value="">Select an epic…</option>'];
    issues.forEach(function (it) {
      var key = it.key;
      var summary = (it.fields && it.fields.summary) || "";
      html.push('<option value="' + escapeHtml(key) + '">' + escapeHtml(key + " — " + summary) + "</option>");
    });
    $sel.html(html.join(""));
  }

  function loadEpics(productGroupValue, $target) {
    if (!productGroupValue) {
      $target.html('<option value="">Pick a product group first</option>');
      return;
    }
    setLoading($target, true);
    var jql = buildJql(productGroupValue);
    console.log("[EpicLoader] Querying Jira with JQL:", jql);

    // --- Prefer AP.request inside Confluence ---
    if (typeof AP !== "undefined" && AP.request) {
      AP.request({
        url: JIRA_BASE_URL + "/rest/api/2/search?jql=" + encodeURIComponent(jql) + "&fields=key,summary&maxResults=200",
        type: "GET",
        success: function (resp) {
          try {
            var data = JSON.parse(resp);
            fillOptions($target, data.issues || []);
            console.log("[EpicLoader] Loaded", (data.issues || []).length, "epics");
          } catch (e) {
            console.error("[EpicLoader] Parse error:", e);
            $target.html('<option value="">Error parsing response</option>');
          } finally {
            setLoading($target, false);
          }
        },
        error: function (xhr) {
          console.error("[EpicLoader] Jira search failed:", xhr && xhr.responseText);
          $target.html('<option value="">Failed to load epics</option>');
          setLoading($target, false);
        }
      });
    } else {
      // --- Fallback: direct fetch() ---
      fetch(JIRA_BASE_URL + "/rest/api/2/search?jql=" + encodeURIComponent(jql) + "&fields=key,summary&maxResults=200", {
        method: "GET",
        credentials: "include"
      })
      .then(function (r) { return r.json(); })
      .then(function (data) {
        fillOptions($target, data.issues || []);
        console.log("[EpicLoader] Loaded", (data.issues || []).length, "epics");
      })
      .catch(function (err) {
        console.error("[EpicLoader] Fetch error:", err);
        $target.html('<option value="">Failed to load epics</option>');
      })
      .finally(function () {
        setLoading($target, false);
      });
    }
  }

  // Wait for fields to appear, then wire events
  function initWhenReady() {
    var $ = AJS.$;
    var $source = $('[name="' + SOURCE_FIELD_NAME + '"]');
    var $target = $('[name="' + TARGET_FIELD_NAME + '"]');

    if ($source.length && $target.length) {
      console.log("[EpicLoader] Wiring change handler");
      var t = null;
      $source.on("change", function () {
        clearTimeout(t);
        var val = $(this).val();
        t = setTimeout(function () { loadEpics(val, $target); }, 150);
      });
      var initial = $source.val();
      if (initial) {
        console.log("[EpicLoader] Initial load for:", initial);
        loadEpics(initial, $target);
      }
    } else {
      console.log("[EpicLoader] Waiting for fields...");
      setTimeout(initWhenReady, 300);
    }
  }

  AJS.toInit(initWhenReady);
})();
</script>