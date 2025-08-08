<script>
AJS.toInit(function () {
  var $ = AJS.$;
  var SOURCE = '[name="ProductGroup"]';
  var TARGET = '[name="jiraEpic"]';
  var RAW    = '[name="jiraRaw"]';

  function setLoading($sel, on) {
    $sel.prop("disabled", on);
    $sel.html('<option>' + (on ? 'Loading…' : 'Select an epic…') + '</option>');
  }

  function fillFromRaw() {
    var raw = $(RAW).val();
    if (!raw) return false;
    try {
      var data = JSON.parse(raw);
      var issues = (data && data.issues) || [];
      var html = ['<option value="">Select an epic…</option>'];
      for (var i = 0; i < issues.length; i++) {
        var it = issues[i], key = it.key, summary = (it.fields && it.fields.summary) || '';
        html.push('<option value="' + key + '">' + key + ' — ' + summary + '</option>');
      }
      var $target = $(TARGET);
      $target.html(html.join(''));
      $target.prop('disabled', false);
      console.log('[EpicLoader] Loaded', issues.length, 'epics');
      return true;
    } catch (e) {
      console.error('[EpicLoader] JSON parse failed for jiraRaw', e);
      $(TARGET).html('<option value="">Error parsing Jira data</option>').prop('disabled', false);
      return true;
    }
  }

  // Poller that waits for IFTTT to write jiraRaw, with timeout
  function waitAndFill(maxMs) {
    var start = Date.now();
    (function tick() {
      if (fillFromRaw()) return;
      if (Date.now() - start > maxMs) {
        $(TARGET).html('<option value="">Timed out waiting for Jira</option>').prop('disabled', false);
        console.warn('[EpicLoader] Timed out waiting for jiraRaw');
        return;
      }
      setTimeout(tick, 200);
    })();
  }

  // Wire up
  var $src = $(SOURCE), $tgt = $(TARGET), $raw = $(RAW);
  if (!$src.length || !$tgt.length || !$raw.length) {
    console.error('[EpicLoader] Missing fields. Found:', {src:$src.length, tgt:$tgt.length, raw:$raw.length});
    return;
  }

  // When ProductGroup changes -> show spinner, clear old data, wait for IFTTT result
  $src.on('change', function () {
    // Clear jiraRaw so we know when new data arrives
    $(RAW).val('');
    setLoading($tgt, true);
    waitAndFill(15000); // wait up to 15s
  });

  // Optional: if page loads with an existing ProductGroup, try once
  if ($src.val()) {
    setLoading($tgt, true);
    waitAndFill(15000);
  }
});
</script>