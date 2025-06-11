<div style="display:flex; margin-bottom:20px;">
  <label style="flex:0 0 240px; font-weight:600;">EML</label>
  <input type="text" id="emlField" readonly placeholder="Auto-selected email" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc; background:#f4f4f4;">
</div>





  const emlField = document.getElementById('emlField');

const emailMap = {
  'Group A1': 'team-a1@example.com',
  'Group A2': 'team-a2@example.com',
  'Group B1': 'team-b1@example.com',
  'Group B2': 'team-b2@example.com',
  'Group C1': 'team-c1@example.com',
  'Group C2': 'team-c2@example.com',
  'fallback': 'non-new-projects@example.com'
};

// Update EML on product group change
productGroup.addEventListener('change', () => {
  const isNewVal = isNew.value;
  const group = productGroup.value;

  if (isNewVal === 'Yes' && emailMap[group]) {
    emlField.value = emailMap[group];
  } else if (isNewVal === 'No') {
    emlField.value = emailMap['fallback'];
  } else {
    emlField.value = '';
  }
});

// Update EML when "Is New" changes (if No is selected early)
isNew.addEventListener('change', () => {
  if (isNew.value === 'No') {
    emlField.value = emailMap['fallback'];
  } else {
    emlField.value = ''; // reset until productGroup is selected
  }
});
