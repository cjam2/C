function loadJiraEpics(productId) {
  var $epic = AJS.$('[name="jiraEpic"]'); // your ConfiForms field name

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

  function buildJql(id) {
    // Change this if productId is label or summary match
    return 'project=' + id + ' AND issuetype=Epic AND statusCategory != Done ORDER BY updated DESC';
  }

  if (!productId) {
    $epic.html('<option value="">Pick a product first</option>');
    return;
  }

  setLoading(true);

  AP.request({
    url: '/rest/api/2/search',
    type: 'GET',
    data: {
      jql: buildJql(productId),
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