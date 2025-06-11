const JIRA_USERNAME = 'your-email@example.com'; // your Jira username
const JIRA_TOKEN = 'YOUR_PERSONAL_ACCESS_TOKEN';


async function createJiraTicket(formData) {
  const auth = btoa(`${JIRA_USERNAME}:${JIRA_TOKEN}`);
  
  // Mapping of productGroup to customfield_19600 values
  const productGroupMap = {
    'Group A1': '1234',
    'Group A2': '5678',
    'Group B1': '9012',
    // ... add your 20 mappings here
  };

  // Determine request-type-based customfield_30000
  const cf30000_val = formData.requestType === 'env_type'
    ? { value: "deploy", child: { value: "ENV" } }
    : { value: "deploy", child: { value: "build" } };

  // Build base payload
  const payload = {
    fields: {
      project: { key: "YOUR_PROJECT_KEY" },
      summary: formData.summary,
      description: `## Description  
${formData.details}

**Platform**: ${formData.platform}`,
      issuetype: { name: "Task" },
      customfield_19600: productGroupMap[formData.productGroup],
      customfield_30000: cf30000_val,
    }
  };

  // Only if NEW = Yes: add target dates
  if (formData.isNew === 'Yes') {
    payload.fields.customfield_24100 = formData.startDate; // YYYY-MM-DD
    payload.fields.customfield_24101 = formData.endDate;   // YYYY-MM-DD
  }

  const resp = await fetch(`${JIRA_BASE_URL}/rest/api/3/issue`, {
    method: 'POST',
    headers: {
      'Authorization': `Basic ${auth}`,
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(payload)
  });

  if (!resp.ok) {
    const err = await resp.json();
    throw new Error(`Jira API error: ${err.errorMessages || resp.statusText}`);
  }

  const data = await resp.json();
  return data.key;
}

















const formData = {
  isNew: document.getElementById('isNew').value,
  requestType: document.getElementById('requestType').value,
  environment: document.getElementById('environment').value,
  appName: document.getElementById('appName').value,
  malcode: document.getElementById('malcode').value,
  testSet: document.getElementById('testSet').value,
  platform: document.getElementById('platform')?.value || '',
  productGroup: document.getElementById('productGroup')?.value || '',
  summary: document.getElementById('summary').value,
  details: document.getElementById('details').value,
  startDate: document.getElementById('startDate').value, // YYYY-MM-DD
  endDate: document.getElementById('endDate').value      // YYYY-MM-DD
};

// Then:
const ticketKey = await createJiraTicket(formData);












async function createJiraTicket(formData) {
  const auth = btoa(`${JIRA_USERNAME}:${JIRA_TOKEN}`);

  const productGroupMap = {
    'Group A1': { projectKey: 'ABC', cf19600: '1234' },
    'Group A2': { projectKey: 'XYZ', cf19600: '5678' },
    'Group B1': { projectKey: 'ETE', cf19600: '89345' },
    // ... more entries
  };

  const cf30000_val = formData.requestType === 'env_type'
    ? { value: "deploy", child: { value: "ENV" } }
    : { value: "deploy", child: { value: "build" } };

  const mapping = productGroupMap[formData.productGroup];
  if (!mapping) {
    throw new Error('Unknown product group mapping');
  }

  const payload = {
    fields: {
      project: { key: mapping.projectKey },
      summary: formData.summary,
      description: `## Description\n${formData.details}\n\n**Platform**: ${formData.platform}`,
      issuetype: { name: "Task" },
      customfield_19600: mapping.cf19600,
      customfield_30000: cf30000_val
    }
  };

  if (formData.isNew === 'Yes') {
    payload.fields.customfield_24100 = formData.startDate;
    payload.fields.customfield_24101 = formData.endDate;
  }

  const resp = await fetch(`${JIRA_BASE_URL}/rest/api/3/issue`, {
    method: 'POST',
    headers: {
      'Authorization': `Basic ${auth}`,
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(payload)
  });

  if (!resp.ok) {
    const err = await resp.json();
    throw new Error(`Jira API error: ${err.errorMessages?.join('; ')}`);
  }

  const data = await resp.json();
  return data.key;
}
