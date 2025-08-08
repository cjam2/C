<script>
AJS.toInit(function () {
  var $ = AJS.$;

  var SOURCE_FIELD = '[name="ProductGroup"]';
  var TARGET_FIELD = '[name="jiraEpic"]';

  // Utility
  function setLoading($sel, on) {
    $sel.prop("disabled", on);
    $sel.html('<option>' + (on ? 'Loading…' : 'Select an epic…') + '</option>');
  }

  // 1) Wait for the Jira Issue/Filter macro table to appear
  function waitForJiraTable(cb, tries) {
    tries = tries || 0;
    // The macro typically renders a table with issue keys as links
    var $table = $('table:has(a[href*="/browse/"])').first();
    if ($table.length) return cb($table);
    if (tries > 80) { console.error("[EpicLoader] Jira table not found"); return; } // ~24s max
    setTimeout(function(){ waitForJiraTable(cb, tries+1); }, 300);
  }

  // 2) Parse all rows into an in-memory list
  function parseEpics($table) {
    var rows = [];
    $table.find('tbody tr').each(function () {
      var $row = $(this);
      // Find key & summary from typical Jira macro table structure
      var $keyLink = $row.find('a[href*="/browse/"]').first();
      var key = ($keyLink.text() || "").trim();
      var summary = "";
      // Summary is usually in the same row; try common cells
      var $cells = $row.find('td');
      $cells.each(function(){
        var txt = $(this).text().trim();
        // Heuristic: longest non-key text in row is often the summary
        if (txt && txt !== key && txt.length > summary.length) summary = txt;
      });

      if (key) {
        // Project key prefix is before the hyphen
        var proj = key.split("-")[0];
        rows.push({ key: key, summary: summary, project: proj });
      }
    });
    console.log("[EpicLoader] Parsed epics:", rows.length);
    return rows;
  }

  // 3) Fill the target select with filtered epics
  function fillTarget($target, epics) {
    if (!epics.length) {
      $target.html('<option value="">No epics found</option>');
      return;
    }
    var html = ['<option value="">Select an epic…</option>'];
    epics.forEach(function (e) {
      html.push('<option value="' + e.key + '">' + e.key + " — " + e.summary + '</option>');
    });
    $target.html(html.join(""));
  }

  // Boot
  var allEpics = [];
  var $source = $(SOURCE_FIELD);
  var $target = $(TARGET_FIELD);

  if (!$source.length || !$target.length) {
    console.error("[EpicLoader] Could not find fields", SOURCE_FIELD, TARGET_FIELD);
    return;
  }

  setLoading($target, true);

  waitForJiraTable(function ($table) {
    allEpics = parseEpics($table);
    setLoading($target, false);

    function refresh() {
      var pg = ($source.val() || "").trim();   // assume this is the Jira project key, e.g., COREIM1
      var list = pg ? allEpics.filter(e => e.project === pg) : allEpics;
      fillTarget($target, list);
      console.log("[EpicLoader] Filled", list.length, "epics for ProductGroup:", pg || "(all)");
    }

    // Wire change + do initial fill
    $source.on("change", refresh);
    refresh();
  });
});
</script>