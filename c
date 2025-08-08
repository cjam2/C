AJS.toInit(function() {
  var $ = AJS.$;

  function fillEpics() {
    var raw = $('[name="jiraRaw"]').val();
    if (!raw) {
      console.warn("[EpicLoader] jiraRaw is empty");
      return;
    }
    try {
      var data = JSON.parse(raw);
      var $target = $('[name="jiraEpic"]');
      var html = ['<option value="">Select an epic…</option>'];
      (data.issues || []).forEach(function(it) {
        var key = it.key;
        var summary = it.fields.summary || "";
        html.push('<option value="' + key + '">' + key + " — " + summary + "</option>");
      });
      $target.html(html.join(""));
      console.log("[EpicLoader] Loaded", (data.issues || []).length, "epics");
    } catch (e) {
      console.error("[EpicLoader] Failed to parse jiraRaw", e);
    }
  }

  $('[name="ProductGroup"]').on("change", function() {
    console.log("[EpicLoader] ProductGroup changed:", $(this).val());
    setTimeout(fillEpics, 1000); // wait for IFTTT to update jiraRaw
  });
});