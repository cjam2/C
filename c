const JIRA_BASE_URL = 'https://your-jira-domain.com';
const JIRA_PAT = 'YOUR_PERSONAL_ACCESS_TOKEN';

async function createJiraTicket(formData) {
  const productGroupMap = {
    'Group A1': { projectKey: 'ABC', cf19600: '1234' },
    'Group A2': { projectKey: 'XYZ', cf19600: '5678' },
    'Group B1': { projectKey: 'ETE', cf19600: '89345' }
    // Add more product group mappings here
  };

  const cf30000_val = formData.requestType === 'env_type'
    ? { value: "deploy", child: { value: "ENV" } }
    : { value: "deploy", child: { value: "build" } };

  const mapping = productGroupMap[formData.productGroup];
  if (!mapping) {
    throw new Error('Unknown Product Group — cannot determine Jira project and cf19600');
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
    payload.fields.customfield_24100 = formData.startDate; // e.g., 2025-06-10
    payload.fields.customfield_24101 = formData.endDate;
  }

  const response = await fetch(`${JIRA_BASE_URL}/rest/api/3/issue`, {
    method: 'POST',
    headers: {
      'Authorization': `Bearer ${JIRA_PAT}`,
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(payload)
  });

  if (!response.ok) {
    const error = await response.json();
    throw new Error(`Jira API Error: ${response.status} ${JSON.stringify(error.errorMessages || error)}`);
  }

  const data = await response.json();
  return data.key; // Returns ticket key like "ABC-123"
}
