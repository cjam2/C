<script>
AJS.toInit(function () {
  // Wait for ConfiForms to render the input
  setTimeout(function () {
    var emailField = document.querySelector('input[name="userEmail"]');
    if (emailField) {
      var userEmail = emailField.value;
      console.log("User's email from ConfiForms:", userEmail);

      // You can now use the email in your logic
      // For example, fill another field or display it
      // document.getElementById("myField").value = userEmail;
    } else {
      console.warn("Email field not found — did ConfiForms render correctly?");
    }
  }, 1000); // Delay to ensure ConfiForms has loaded
});
</script>



  {confiforms:name=emailForm}
{confiforms-field:userEmail|type=text|default=[entry._user.email]|readonly=true}
{confiforms-registration}
<div id="confiform-wrapper">
  <input type="hidden" name="userEmail" />
</div>
{confiforms-registration}
{confiforms-view:emailForm}
{confiforms-view}
