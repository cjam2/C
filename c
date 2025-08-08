<script>
(function () {
  // ---- CONFIG ----
  var JIRA_BASE_URL = "https://track.td.com";
  var SOURCE_FIELD_NAME = "ProductGroup"; // ConfiForms source dropdown
  var TARGET_FIELD_NAME = "jiraEpic";     // ConfiForms target dropdown
  var SUMMARY_FILTER = "RAD";             // text to match in epic summary
  var STATUS_LIST = ['"In Progress"', '"Refining"']; // Jira statuses to include

  console.log("[EpicLoader] Script loaded ✓");

  function escapeHtml(str) { return AJS.$("<div>").text(String(str || "")).html(); }

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

  function buildApiUrl(productGroupValue) {
    // Build the JQL exactly like in your screenshot
    var jql = 'issuetype=Epic AND project="' + productGroupValue +
              '" AND summary ~ "' + SUMMARY_FILTER + 
              '" AND status in (' + STATUS_LIST.join(",") + ')';
    return JIRA_BASE_URL + "/rest/api/2/search?jql=" +
           encodeURIComponent(jql) + "&fields=key,summary&maxResults=200";
  }

  function loadEpics(productGroupValue, $target) {
    if (!productGroupValue) {
      $target.html('<option value="">Pick a product group first</option>');
      return;
    }
    var apiUrl = buildApiUrl(productGroupValue);
    setLoading($target, true);

    console.log("[EpicLoader] Fetching from:", apiUrl);

    fetch(apiUrl, {
      method: "GET",
      credentials: "include", // uses your logged-in Jira session
      headers: {
        "Accept": "application/json"
      }
    })
    .then(res => {
      if (!res.ok) throw new Error("HTTP " + res.status);
      return res.json();
    })
    .then(data => {
      console.log("[EpicLoader] JSON response:", data);
      fillOptions($target, data.issues || []);
      console.log("[EpicLoader] Loaded", (data.issues || []).length, "epics");
    })
    .catch(err => {
      console.error("[EpicLoader] Fetch failed:", err);
      $target.html('<option value="">Failed to load epics</option>');
    })
    .finally(() => {
      setLoading($target, false);
    });
  }

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