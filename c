/* ConfiForms → Jira Epic loader
   - Runs on load
   - Uses AP.request (no CORS creds in browser)
   - Source field:  name="ProductGroup"
   - Target field:  name="jiraEpic"
*/

(function () {
  // ---- CONFIG ----
  var SOURCE_FIELD_NAME = "ProductGroup"; // ConfiForms field name of the first dropdown
  var TARGET_FIELD_NAME = "jiraEpic";     // ConfiForms field name of the Jira Epic dropdown
  var SEARCH_MODE = "project";            // "project" | "label" | "summary"
  var INCLUDE_DONE = false;               // include epics in Done status

  // ---- BOOT LOG ----
  console.log("[EpicLoader] Script loaded ✓");

  // Utility: HTML-escape using jQuery
  function escapeHtml(str) { return AJS.$("<div>").text(String(str || "")).html(); }

  // Build JQL based on your ProductGroup meaning
  function buildJql(productGroupValue) {
    var base;
    if (SEARCH_MODE === "label") {
      base = 'issuetype=Epic AND labels = "' + productGroupValue + '"';
    } else if (SEARCH_MODE === "summary") {
      base = 'issuetype=Epic AND summary ~ "' + productGroupValue + '"';
    } else {
      // default: treat as Jira project key
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
    for (var i = 0; i < issues.length; i++) {
      var it = issues[i];
      var key = it.key;
      var summary = (it.fields && it.fields.summary) || "";
      html.push('<option value="' + escapeHtml(key) + '">' + escapeHtml(key + " — " + summary) + "</option>");
    }
    $sel.html(html.join(""));
  }

  function loadEpics(productGroupValue, $target) {
    if (!productGroupValue) {
      $target.html('<option value="">Pick a product group first</option>');
      return;
    }
    if (typeof AP === "undefined" || !AP.request) {
      console.error("[EpicLoader] AP.request is unavailable. Are you running inside Confluence?");
      $target.html('<option value="">AP.request unavailable</option>');
      return;
    }

    setLoading($target, true);
    var jql = buildJql(productGroupValue);

    console.log("[EpicLoader] Querying Jira with JQL:", jql);

    AP.request({
      url: "/rest/api/2/search",
      type: "GET",
      data: {
        jql: jql,
        fields: "key,summary",
        maxResults: 200
      },
      success: function (resp) {
        try {
          var data = JSON.parse(resp);
          fillOptions($target, (data && data.issues) || []);
          console.log("[EpicLoader] Loaded", (data && data.issues ? data.issues.length : 0), "epics");
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
  }

  // Run when Confluence is ready
  AJS.toInit(function () {
    var $ = AJS.$;
    var $source = $('[name="' + SOURCE_FIELD_NAME + '"]');
    var $target = $('[name="' + TARGET_FIELD_NAME + '"]');

    if ($source.length === 0) {
      console.error('[EpicLoader] Source field not found: name="' + SOURCE_FIELD_NAME + '"');
      return;
    }
    if ($target.length === 0) {
      console.error('[EpicLoader] Target field not found: name="' + TARGET_FIELD_NAME + '"');
      return;
    }

    console.log("[EpicLoader] Wiring change handler");
    var t = null;
    $source.on("change", function () {
      clearTimeout(t);
      var val = $(this).val();
      t = setTimeout(function () { loadEpics(val, $target); }, 120);
    });

    // Initial load if a value already exists
    var initial = $source.val();
    if (initial) {
      console.log("[EpicLoader] Initial load for value:", initial);
      loadEpics(initial, $target);
    } else {
      console.log("[EpicLoader] Waiting for ProductGroup selection…");
    }
  });
})();