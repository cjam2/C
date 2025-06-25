<script>
  AJS.toInit(function() {
    // Wait for DOM to load and grab the hidden email field by name
    var emailField = document.querySelector('[name="userEmail"]');
    if (emailField) {
      console.log("Logged-in user email:", emailField.value);
      // Now you can use the email value in your script
    }
  });
</script>
{confiforms-field:userEmail|type=text|default=[entry._user.email]|readonly=true}
