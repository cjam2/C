async function submitForm() {
  // 🔁 Validation logic here...
  const isValid = validateForm(); // You already have this or similar logic

  if (!isValid) {
    alert("Please fill in all required fields.");
    return;
  }

  const formData = {
    isNew: document.getElementById('isNew').value,
    requestType: document.getElementById('requestType').value,
    platform: document.getElementById('platform')?.value || '',
    productGroup: document.getElementById('productGroup')?.value || '',
    startDate: document.getElementById('startDate').value,
    endDate: document.getElementById('endDate').value,
    summary: document.getElementById('summary').value,
    details: document.getElementById('details').value
  };

  try {
    const ticketKey = await createJiraTicket(formData);
    alert(`✅ Jira ticket created: ${ticketKey}`);
    // 🔁 Call resetForm() or clear fields here if needed
  } catch (error) {
    console.error(error);
    alert(`❌ Failed to create ticket: ${error.message}`);
  }
}
