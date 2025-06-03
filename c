<script>
AJS.toInit(function() {
  console.log("Custom JS injected via admin panel.");

  var nameFieldName = "ProductGroup";
  var emailFieldName = "recipientEmail";

  var emailMap = {
    "CreditDecisioning:1": "CRPH01_RAD-Pod@td.com",
    "CreditDecisioning:2": "CRPH02_RAD-Pod@td.com",
    "CreditDecisioning:3": "CRPHCR_RAD-Pod@td.com"
  };

  var nameField = document.querySelector('[name="' + nameFieldName + '"]');
  var emailField = document.querySelector('[name="' + emailFieldName + '"]');

  if (nameField && emailField) {
    function updateEmail() {
      var selectedID = nameField.value;
      var email = emailMap[selectedID] || "";
      emailField.value = email;
    }

    nameField.addEventListener('change', updateEmail);
    updateEmail();
  }
});
</script>