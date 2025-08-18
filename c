<script>
AJS.toInit(function () {
  var $ = AJS.$;

  // ---- selectors ----
  var PG_SEL  = '[name="ProductGroup"]';
  var PLT_SEL = '[name="Platform"]';

  // ---- your existing update function (call this) ----
  function updateFromCurrentProductGroup(reason) {
    var $pg = $(PG_SEL);
    if (!$pg.length) { console.warn('[EpicPanel] ProductGroup not found'); return; }
    var val  = $pg.val();
    var text = $pg.find('option:selected').text();

    console.log('[EpicPanel] trigger:', reason,
                '| raw value =', val,
                '| selected text =', text);

    // >>> call your existing panel update here <<<
    // e.g. updatePanelWithValue(val);  // whatever you named it
    if (typeof updatePanel === 'function') {
      updatePanel(); // if your updatePanel reads from PG directly
    }
  }

  // ---- 1) normal events: both fields ----
  $(document).on('change input', PG_SEL, function(){
    updateFromCurrentProductGroup('PG:change/input');
  });
  $(document).on('change input', PLT_SEL, function(){
    // Platform changed; ProductGroup may get set programmatically a moment later
    setTimeout(function(){ updateFromCurrentProductGroup('PLATFORM:change→read PG'); }, 120);
  });

  // ---- 2) mutation observer: catches programmatic selection/option updates ----
  function attachObserver() {
    var pgEl = document.querySelector(PG_SEL.replace(/^\[name="/,'[name="'));
    if (!pgEl) { setTimeout(attachObserver, 500); return; }

    new MutationObserver(function(mutations){
      // Any selected/attribute/child change in the PG select will retrigger
      updateFromCurrentProductGroup('PG:MutationObserver');
    }).observe(pgEl, { attributes:true, childList:true, subtree:true, characterData:true });
    console.log('[EpicPanel] MutationObserver attached on ProductGroup');
  }
  attachObserver();

  // ---- 3) tiny poll as last resort (handles edge cases in some ConfiForms versions) ----
  (function poll(){
    var last = null;
    setInterval(function(){
      var $pg = $(PG_SEL);
      if (!$pg.length) return;
      var current = $pg.val();
      if (current !== last) {
        last = current;
        if (current != null) updateFromCurrentProductGroup('PG:poll change');
      }
    }, 700);
  })();

  // ---- 4) initial kick if PG already has a value ----
  if ($(PG_SEL).val()) updateFromCurrentProductGroup('PG:initial');
});
</script>