<script>
window.addEventListener('DOMContentLoaded', function() {

  // Your field names from ConfiForms
  var nameFieldName = "ProductGroup"; // Dropdown field (ID stored, label shown)
  var emailFieldName = "recipientEmail"; // Read-only email field

  // Define your mapping: ID → Email
  var emailMap = {
    "john": "john.doe@example.com",
    "jane": "jane.smith@example.com",
    "bob": "bob.brown@example.com"
  };

  // Get the fields
  var nameField = document.querySelector('[name="' + nameFieldName + '"]');
  var emailField = document.querySelector('[name="' + emailFieldName + '"]');

  if (nameField && emailField) {

    function updateEmail() {
      var selectedID = nameField.value; // Get the ID stored in the dropdown
      var email = emailMap[selectedID] || ""; // Lookup the email from the map
      emailField.value = email; // Set the email in the read-only field
    }

    // Trigger when dropdown changes
    nameField.addEventListener('change', updateEmail);

    // Set initial value on page load
    updateEmail();
  }

});
</script>