if (typeof AP !== 'undefined' && AP.user) {
  AP.user.getCurrentUser(function(user) {
    const emailInput = document.getElementById('emailField');
    emailInput.value = user.email || '';
    emailInput.style.border = '1px solid #ccc'; // visually reset if empty
  });
}
