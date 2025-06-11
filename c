createJiraTicket(formData)
  .then(ticketKey => {
    alert(`✅ Jira ticket created: ${ticketKey}`);
    // Optionally clear form here
  })
  .catch(error => {
    console.error(error);
    alert(`❌ Failed to create ticket: ${error.message}`);
  });
