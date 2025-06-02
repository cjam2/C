<script type="text/javascript">
  AJS.toInit(function() {
    var nameFieldName = "ProductGroup";
    var emailFieldName = "recipientEmail";

    var emailMap = {
      "CreditDecisioning:1": "CRPH01_RAD-Pod@td.com",
      "CreditDecisioning:2": "CRPH02_RAD-Pod@td.com",
      "CreditDecisioning:3": "CRPHCR_RAD-Pod@td.com"
    };

    var nameField = AJS.$('[name="' + nameFieldName + '"]');
    var emailField = AJS.$('[name="' + emailFieldName + '"]');

    if (nameField.length && emailField.length) {
      var selectedID = nameField.val();
      var email = emailMap[selectedID] || "";
      emailField.val(email);
      console.log("Email field updated to:", email);
    } else {
      console.log("Fields not found. Check field names.");
    }
  });
</script>