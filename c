<script>
window.addEventListener('DOMContentLoaded', function() {
  var nameFieldName = "ProductGroup";
  var emailFieldName = "recipientEmail";

  console.log("Script loaded!");

  var nameField = document.querySelector('[name="' + nameFieldName + '"]');
  var emailField = document.querySelector('[name="' + emailFieldName + '"]');

  console.log("Dropdown field found:", nameField);
  console.log("Email field found:", emailField);

  var emailMap = {
    "CreditDecisioning:1": "CRPH01_RAD-Pod@td.com",
    "CreditDecisioning:2": "CRPH02_RAD-Pod@td.com",
    "CreditDecisioning:3": "CRPHCR_RAD-Pod@td.com"
  };

  if (nameField && emailField) {
    function updateEmail() {
      console.log("Dropdown changed. Value:", nameField.value);
      var email = emailMap[nameField.value] || "";
      console.log("Setting email field to:", email);
      emailField.value = email;
    }

    nameField.addEventListener('change', updateEmail);
    updateEmail();
  } else {
    console.log("Fields not found. Please check the field names in ConfiForms.");
  }
});
</script>