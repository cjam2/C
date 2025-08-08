(function () {
  var jiraUrl = 'https://track.td.com/rest/api/2/search?jql=' +
                encodeURIComponent('issuetype=Epic AND project=COREIM1 AND summary~"RAD" AND status in ("In Progress","Refining")') +
                '&fields=key,summary&maxResults=200';

  var proxyUrl = AJS.contextPath() + '/rest/proxy/latest?url=' + encodeURIComponent(jiraUrl);

  AJS.$.ajax({
    url: proxyUrl,
    type: 'GET',
    dataType: 'json',
    success: function (data) {
      console.log('Proxied Jira JSON:', data);
      // populate your dropdown here from data.issues
    },
    error: function (xhr) {
      console.error('Proxy call failed:', xhr);
    }
  });
})();