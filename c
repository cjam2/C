<script>
AJS.toInit(function() {
  console.log("Test script running...");
  var emailFieldName = "recipientEmail";
  var emailField = document.querySelector('[name="' + emailFieldName + '"]');

  if (emailField) {
    emailField.value = "It's working!";
    console.log("Email field found. Value set.");
  } else {
    console.log("Email field not found.");
  }
});
</script>