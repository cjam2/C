isNew.addEventListener('change', () => {
  if (isNew.value === 'Yes') {
    platformContainer.style.display = 'flex';
    platform.classList.add('required');
  } else {
    platformContainer.style.display = 'none';
    productGroupContainer.style.display = 'none';

    // 👇 Reset the dropdown values
    platform.value = '';
    productGroup.innerHTML = '<option value="">Select product group</option>';
    productGroup.value = '';

    // 👇 Remove required flag
    platform.classList.remove('required');
    productGroup.classList.remove('required');
  }
});
