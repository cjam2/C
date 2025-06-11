<div id="envExtraFields" style="display: none; margin-top: 10px;">
  <label for="environmentRequired">Is Environment Needed?</label>
  <select id="environmentRequired" class="required-env">
    <option value="">Select an option</option>
    <option value="Yes">Yes</option>
    <option value="No">No</option>
  </select>

  <label for="environmentDescription" style="margin-top: 8px;">Environment Description</label>
  <input type="text" id="environmentDescription" class="required-env" placeholder="Describe the environment" />
</div>




  document.getElementById('requestType').addEventListener('change', function() {
  const value = this.value;
  const envSection = document.getElementById('envExtraFields');
  
  if (value === 'env_type') {
    envSection.style.display = 'block';
  } else {
    envSection.style.display = 'none';
    // Clear values if hidden
    document.getElementById('environmentRequired').value = '';
    document.getElementById('environmentDescription').value = '';
  }
});





const extraEnvFields = document.querySelectorAll('.required-env');

if (document.getElementById('requestType').value === 'env_type') {
  extraEnvFields.forEach(field => {
    if (!field.value.trim()) {
      field.style.border = '1px solid red';
      valid = false;
    } else {
      field.style.border = '1px solid #ccc';
    }
  });
}
