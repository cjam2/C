<script>
AJS.toInit(function () {
  var $ = AJS.$;
  var SOURCE = '[name="ProductGroup"]';
  var TARGET = '[name="jiraEpic"]';
  var RAW    = '[name="jiraRaw"]';

  function setLoading(on){
    var $t = $(TARGET);
    $t.prop('disabled', on)
      .html('<option>' + (on ? 'Loading…' : 'Select an epic…') + '</option>');
  }

  function fillFromRaw(){
    var raw = $(RAW).val();
    if (!raw) return;
    try{
      var data = JSON.parse(raw);
      var issues = (data && data.issues) || [];
      var opts = ['<option value="">Select an epic…</option>'];
      for (var i=0;i<issues.length;i++){
        var it = issues[i], key = it.key, summary = (it.fields && it.fields.summary) || '';
        opts.push('<option value="'+key+'">'+key+' — '+summary+'</option>');
      }
      $(TARGET).html(opts.join('')).prop('disabled', false);
      console.log('[EpicLoader] Loaded', issues.length, 'epics');
    }catch(e){
      console.error('[EpicLoader] Parse failed', e, raw);
      $(TARGET).html('<option value="">Error parsing Jira data</option>').prop('disabled', false);
    }
  }

  // Watch jiraRaw for changes (fires even though hidden)
  $(document).on('change input', RAW, fillFromRaw);

  // Also observe attribute/value changes for safety
  var el = document.querySelector(RAW.replace(/^\[name="/,'[name="'));
  if (el) {
    new MutationObserver(function(){ fillFromRaw(); })
      .observe(el, { attributes:true, childList:true, characterData:true, subtree:true });
  }

  // When ProductGroup changes, clear and show spinner
  $(SOURCE).on('change', function(){
    $(RAW).val('');           // clear old data; IFTTT will overwrite
    setLoading(true);
    // If nothing arrives after 30s, show timeout
    setTimeout(function(){
      if (!$(RAW).val()){
        $(TARGET).html('<option value="">Timed out waiting for Jira</option>').prop('disabled', false);
        console.warn('[EpicLoader] Timed out waiting for jiraRaw');
      }
    }, 30000);
  });

  // If ProductGroup already set on load, kick it off
  if ($(SOURCE).val()){
    $(SOURCE).trigger('change');
  }
});
</script>