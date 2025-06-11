<!-- Conditional Fields for "Is NEW" = No -->
<div id="nonNewFields" style="display:none; margin-top:20px;">

  <div style="display:flex; align-items:center; margin-bottom:20px;">
    <label style="flex:0 0 240px;"></label>
    <label><input type="checkbox" id="confirmCheckbox" style="margin-right:8px;"> Yes, I confirm</label>
  </div>

  <div style="display:flex; margin-bottom:20px;">
    <label style="flex:0 0 240px; font-weight:600;">Product Family<span style="color:red;"> *</span></label>
    <select id="productFamily" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
      <option value="">Select product family</option>
      <option value="Family A">Family A</option>
      <option value="Family B">Family B</option>
    </select>
  </div>

  <div style="display:flex; margin-bottom:20px;">
    <label style="flex:0 0 240px; font-weight:600;">LOB<span style="color:red;"> *</span></label>
    <select id="lob" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
      <option value="">Select LOB</option>
    </select>
  </div>

  <div style="display:flex; margin-bottom:20px;">
    <label style="flex:0 0 240px; font-weight:600;">Project Name<span style="color:red;"> *</span></label>
    <input type="text" id="projectName" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;" placeholder="Enter project name">
  </div>

  <div style="display:flex; margin-bottom:20px;">
    <label style="flex:0 0 240px; font-weight:600;">Additional Details</label>
    <input type="text" id="additionalDetails" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;" placeholder="Enter additional info">
  </div>

</div>
















  const nonNewFields = document.getElementById('nonNewFields');
const productFamily = document.getElementById('productFamily');
const lob = document.getElementById('lob');
const projectName = document.getElementById('projectName');

const lobMap = {
  'Family A': ['LOB A1', 'LOB A2'],
  'Family B': ['LOB B1', 'LOB B2']
};

isNew.addEventListener('change', () => {
  const isNo = isNew.value === 'No';
  const isYes = isNew.value === 'Yes';

  // Show/hide relevant sections
  nonNewFields.style.display = isNo ? 'block' : 'none';

  // Reset dependent values
  if (!isNo) {
    document.getElementById('confirmCheckbox').checked = false;
    productFamily.value = '';
    lob.innerHTML = '<option value="">Select LOB</option>';
    lob.value = '';
    projectName.value = '';
    document.getElementById('additionalDetails').value = '';
  }
});

productFamily.addEventListener('change', () => {
  const selected = productFamily.value;
  const lobOptions = lobMap[selected] || [];
  lob.innerHTML = '<option value="">Select LOB</option>';
  lobOptions.forEach(opt => {
    const option = document.createElement('option');
    option.value = opt;
    option.textContent = opt;
    lob.appendChild(option);
  });
});
