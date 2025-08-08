<script>
AJS.toInit(function () {
  var $ = AJS.$;
  var SOURCE = '[name="ProductGroup"]';
  var TARGET = '[name$="jiraEpic"]';
  var RAW    = 'textarea[name$="jiraRaw"],input[name$="jiraRaw"]';

  function setLoading(on){
    $(TARGET).prop('disabled', on)
      .html('<option>'+(on?'Loading…':'Select an epic…')+'</option>');
  }

  function tryFill(){
    var $raw = $(RAW); 
    if (!$raw.length) return false;

    var raw = $raw.val(); 
    if (!raw) return false;

    // 👇 Added: log raw JSON string from hidden field
    console.log('[EpicLoader] jiraRaw field value:', raw);

    try {
      var data = JSON.parse(raw);
      var issues = (data.issues || []);
      var html = ['<option value="">Select an epic…</option>'];
      for (var i=0; i<issues.length; i++){
        var it = issues[i];
        var k = it.key;
        var s = (it.fields && it.fields.summary) || '';
        html.push('<option value="'+k+'">'+k+' — '+s+'</option>');
      }
      $(TARGET).html(html.join('')).prop('disabled', false);
      console.log('[EpicLoader] Loaded', issues.length, 'epics');
    } catch(e){
      console.error('[EpicLoader] JSON parse failed; first 200 chars:', (raw||'').slice(0,200));
      $(TARGET).html('<option value="">Error parsing Jira data</option>').prop('disabled', false);
    }
    return true;
  }

  function waitAndFill(ms){
    var t0 = Date.now();
    (function loop(){
      if (tryFill()) return;
      if (Date.now() - t0 > ms){
        $(TARGET).html('<option value="">Timed out waiting for Jira</option>').prop('disabled', false);
        return;
      }
      setTimeout(loop, 200);
    })();
  }

  $(SOURCE).on('change', function(){
    var $raw = $(RAW); 
    if ($raw.length) $raw.val('');
    setLoading(true);
    waitAndFill(20000);
  });

  if ($(SOURCE).val()) $(SOURCE).trigger('change');
});
</script>