<script>
AJS.toInit(function() {
  console.log("Custom JS for ProductGroup → recipientEmail is active");

  var emailMap = {
    "CreditDecisioning:1": "CRPH01_RAD-Pod@td.com",
    "CreditDecisioning:2": "CRPH02_RAD-Pod@td.com",
    "CreditDecisioning:3": "CRPHCR_RAD-Pod@td.com"
  };

  function updateEmail() {
    var nameField = document.querySelector('[name="ProductGroup"]');
    var emailField = document.querySelector('[name="recipientEmail"]');

    if (nameField && emailField) {
      var selectedID = nameField.value;
      var email = emailMap[selectedID] || "";
      emailField.value = email;
      console.log("Email updated to:", email);
    } else {
      console.log("Fields not found.");
    }
  }

  // Attach event after ensuring DOM is ready
  var interval = setInterval(function() {
    var nameField = document.querySelector('[name="ProductGroup"]');
    if (nameField) {
      nameField.addEventListener('change', updateEmail);
      updateEmail(); // Initial run
      clearInterval(interval);
    }
  }, 100); // retry every 100ms until field appears (then stop)
});
</script>