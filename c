<input type="hidden" id="labelField" name="label" />

  if (formData.isNew === 'Yes' && formData.requestType === 'env_type') {
  formData.label = 'RETER';
  document.getElementById('labelField').value = 'RETER';
}
