if (formData.isNew === 'Yes') {
  const mapping = productGroupMap[formData.productGroup];

  if (!mapping) {
    alert('❌ Unknown Product Group — cannot determine project or field mapping.');
    return;
  }

  formData.projectKey = mapping.projectKey;
  formData.cf19600 = mapping.cf19600;
  formData.cf30000 = cf30000_val;

  // Also update hidden fields
  document.getElementById('projectKeyField').value = mapping.projectKey;
  document.getElementById('cf19600Field').value = mapping.cf19600;
  document.getElementById('cf30000Field').value = JSON.stringify(cf30000_val);
}
