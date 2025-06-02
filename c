<button id="updateEmailButton" style="padding: 6px 12px; background-color: #0052cc; color: white; border: none; border-radius: 4px; cursor: pointer;">
  Update Email
</button>



<script>
AJS.toInit(function() {
  console.log("Script loaded, waiting for button click...");

  var nameFieldName = "ProductGroup";    // Your dropdown field name
  var emailFieldName = "recipientEmail"; // Your read-only field name

  var emailMap = {
    "CreditDecisioning:1": "CRPH01_RAD-Pod@td.com",
    "CreditDecisioning:2": "CRPH02_RAD-Pod@td.com",
    "CreditDecisioning:3": "CRPHCR_RAD-Pod@td.com"
  };

  function updateEmail() {
    var nameField = document.querySelector('[name="' + nameFieldName + '"]');
    var emailField = document.querySelector('[name="' + emailFieldName + '"]');

    if (nameField && emailField) {
      var selectedID = nameField.value;
      var email = emailMap[selectedID] || "";
      emailField.value = email;
      console.log("Email field updated to:", email);
    } else {
      console.log("Fields not found. Check field names.");
    }
  }

  var button = document.querySelector("#updateEmailButton");
  if (button) {
    button.addEventListener("click", updateEmail);
    console.log("Button event listener attached.");
  } else {
    console.log("Button not found on the page.");
  }
});
</script>


