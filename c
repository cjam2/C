<html>
<head>
<script>
function updateEmail() {
  // Mapping of names (or IDs) to emails - adjust this!
  var emailMap = {
    "John Doe": "john.doe@example.com",
    "Jane Smith": "jane.smith@example.com",
    "Bob Brown": "bob.brown@example.com"
  };

  // Get selected name from dropdown (adjust field name)
  var dropdown = document.querySelector('[name="nameDropdown"]'); // Replace with your field name if different
  var selectedName = dropdown.options[dropdown.selectedIndex].text;

  // Get the email based on the selected name
  var email = emailMap[selectedName] || "";

  // Set the email in the read-only box
  var emailField = document.querySelector('[name="emailBox"]'); // Replace with your field name
  if (emailField) {
    emailField.value = email;
  }
}

// Attach event listener on page load
window.addEventListener('DOMContentLoaded', function() {
  var dropdown = document.querySelector('[name="nameDropdown"]');
  if (dropdown) {
    dropdown.addEventListener('change', updateEmail);
    updateEmail(); // Initialize on load
  }
});
</script>
</head>
<body>
</body>
</html>