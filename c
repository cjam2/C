<script>
AJS.toInit(function() {
  console.log("Script loaded and running in AJS context!");

  var nameFieldName = "ProductGroup";
  var emailFieldName = "recipientEmail";

  var emailMap = {
    "CreditDecisioning:1": "CRPH01_RAD-Pod@td.com",
    "CreditDecisioning:2": "CRPH02_RAD-Pod@td.com",
    "CreditDecisioning:3": "CRPHCR_RAD-Pod@td.com"
  };

  var nameField = document.querySelector('[name="' + nameFieldName + '"]');
  var emailField = document.querySelector('[name="' + emailFieldName + '"]');

  console.log("Dropdown field:", nameField);
  console.log("Email field:", emailField);

  if (nameField && emailField) {
    function updateEmail() {
      var selectedID = nameField.value;
      var email = emailMap[selectedID] || "";
      console.log("Selected:", selectedID, "Setting email:", email);
      emailField.value = email;
    }

    nameField.addEventListener('change', updateEmail);
    updateEmail();
  } else {
    console.log("Fields not found. Double-check field names.");
  }
});
</script>