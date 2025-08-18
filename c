function updatePanelWithValue(pgValue) {
  // If value looks like "CRPHUCC\CreditSomething", keep only key
  var key = pgValue ? pgValue.split('\\')[0] : '';
  if (!key) return;

  var newJql = 'project = EPIC and "Product Group" = "' + key + '" ORDER BY created DESC';

  console.log('[EpicPanel] updating JQL:', newJql);

  // Replace the existing iframe/panel src
  var $iframe = $('#epic-panel-iframe'); // your panel iframe id
  if ($iframe.length) {
    $iframe.attr('src', '/jira/secure/IssueNavigator.jspa?jql=' + encodeURIComponent(newJql));
  }
}




function updateFromCurrentProductGroup(reason) {
  var $pg = $(PG_SEL);
  var val  = $pg.val();
  var text = $pg.find('option:selected').text();

  console.log('[EpicPanel] trigger:', reason, '| raw value =', val, '| selected text =', text);

  updatePanelWithValue(val);
}