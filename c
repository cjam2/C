function loadJiraEpicsFromProductGroup(productGroupValue) {
  var $epic = AJS.$('[name="jiraEpic"]'); // Change to your actual ConfiForms field name for the Jira Epic dropdown

  function setLoading(isLoading) {
    $epic.prop('disabled', isLoading);
    if (isLoading) {
      $epic.html('<option>Loading…</option>');
    }
  }

  function fillOptions(issues) {
    if (!issues || !issues.length) {
      $epic.html('<option value="">No epics found</option>');
      return;
    }
    var opts = issues.map(function (it) {
      var key = it.key;
      var summary = (it.fields && it.fields.summary) || '';
      var label = key + ' — ' + summary;
      return '<option value="' + key + '">' + AJS.escapeHtml(label) + '</option>';
    }).join('');
    $epic.html('<option value="">Select an epic…</option>' + opts);
  }

  // In your case, ProductGroup value will match a Jira project key
  function buildJql(productGroupValue) {
    return 'project=' + productGroupValue + ' AND issuetype=Epic AND statusCategory != Done ORDER BY updated DESC';
  }

  if (!productGroupValue) {
    $epic.html('<option value="">Pick a product group first</option>');
    return;
  }

  setLoading(true);

  AP.request({
    url: '/rest/api/2/search',
    type: 'GET',
    data: {
      jql: buildJql(productGroupValue),
      fields: 'key,summary',
      maxResults: 200
    },
    success: function (resp) {
      try {
        var json = JSON.parse(resp);
        fillOptions(json.issues || []);
      } catch (e) {
        console.error('Parse error', e);
        $epic.html('<option value="">Error parsing response</option>');
      } finally {
        setLoading(false);
      }
    },
    error: function (xhr) {
      console.error('Jira search failed', xhr && xhr.responseText);
      $epic.html('<option value="">Failed to load epics</option>');
      setLoading(false);
    }
  });
}