<script>
AJS.toInit(function () {
  var $ = AJS.$;

  var SOURCE = '[name="ProductGroup"]';  // ConfiForms dropdown
  var TARGET = '[name="jiraEpic"]';      // ConfiForms dropdown you want to fill
  var RAW    = '[name="jiraRaw"]';       // Hidden field where WebService saves JSON

  function setLoading(on) {
    var $t = $(TARGET);
    $t.prop('disabled', on)
      .html('<option>' + (on ? 'Loading…' : 'Select an epic…') + '</option>');
  }

  function fillFromRaw() {
    var raw = $(RAW).val();
    if (!raw) return; // nothing yet
    try {
      var data = JSON.parse(raw);
      var issues = (data && data.issues) || [];
      var opts = ['<option value="">Select an epic…</option>'];
      for (var i = 0; i < issues.length; i++) {
        var it = issues[i];
        var key = it.key;
        var summary = (it.fields && it.fields.summary) || '';
        opts.push('<option value="'+ key +'">'+ key +' — '+ summary +'</option>');
      }
      $(TARGET).html(opts.join('')).prop('disabled', false);
      console.log('[EpicLoader] Loaded', issues.length, 'epics');
    } catch (e) {
      console.error('[EpicLoader] JSON parse failed', e);
      $(TARGET).html('<option value="">Error parsing Jira data</option>').prop('disabled', false);
    }
  }

  // When ProductGroup changes:
  //  - clear old jiraRaw
  //  - show spinner
  //  - IFTTT WebService will run and write fresh JSON into jiraRaw
  $(SOURCE).on('change', function () {
    $(RAW).val('');         // clear old payload so we know when new arrives
    setLoading(true);
  });

  // React as soon as jiraRaw gets updated (even though it’s hidden)
  // 1) change/input listener (works on many setups)
  $(document).on('change input', RAW, fillFromRaw);

  // 2) mutation observer as a safety net
  var el = document.querySelector(RAW.replace(/^\[name="/,'[name="'));
  if (el) {
    new MutationObserver(function () { fillFromRaw(); })
      .observe(el, { attributes:true, childList:true, characterData:true, subtree:true });
  }

  // If ProductGroup already has a value on load, trigger the flow
  if ($(SOURCE).val()) {
    $(SOURCE).trigger('change');
  }
});
</script>