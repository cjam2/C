<input type="hidden" id="projectKeyField" name="projectKey" />
<input type="hidden" id="cf19600Field" name="custom19600" />
<input type="hidden" id="cf30000Field" name="custom30000" />







  // Mappings
const productGroupMap = {
  'Group A1': { projectKey: 'ABC', cf19600: '1234' },
  'Group A2': { projectKey: 'XYZ', cf19600: '5678' },
  'Group B1': { projectKey: 'ETE', cf19600: '89345' }
  // Add your 20 mappings here
};

// Request type mapping
const cf30000_val = formData.requestType === 'env_type'
  ? { value: "deploy", child: { value: "ENV" } }
  : { value: "deploy", child: { value: "build" } };

const mapping = productGroupMap[formData.productGroup];

if (!mapping) {
  alert('❌ Unknown Product Group — cannot determine project or field mapping.');
  return;
}

// Set hidden fields
document.getElementById('projectKeyField').value = mapping.projectKey;
document.getElementById('cf19600Field').value = mapping.cf19600;
document.getElementById('cf30000Field').value = JSON.stringify(cf30000_val);
