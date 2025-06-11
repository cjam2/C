function submitForm() {
  const fields = document.querySelectorAll('.required');
  let valid = true;

  fields.forEach(f => {
    if (!f.value) {
      f.style.border = '1px solid red';
      valid = false;
    } else {
      f.style.border = '1px solid #ccc';
    }
  });

  if (!valid) {
    alert('Please fill in all required fields.');
    return;
  }

  // ✅ Submission logic
  alert('Form submitted successfully!');

  // ✅ Reset form logic
  document.querySelectorAll('input[type="text"], input[type="email"], textarea').forEach(input => input.value = '');
  document.querySelectorAll('select').forEach(select => select.value = '');
  document.querySelectorAll('input[type="checkbox"]').forEach(box => box.checked = false);
  fields.forEach(f => f.style.border = '1px solid #ccc');

  // ✅ Reset conditional visibility
  document.getElementById('platformContainer').style.display = 'none';
  document.getElementById('productGroupContainer').style.display = 'none';
  document.getElementById('nonNewFields').style.display = 'none';

  // ✅ Reset dependent dropdowns
  document.getElementById('productGroup').innerHTML = '<option value="">Select product group</option>';
  document.getElementById('lob').innerHTML = '<option value="">Select LOB</option>';

  // ✅ Clear EML field
  document.getElementById('emlField').value = '';
}
