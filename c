<script>
AJS.toInit(function () {
  console.log("Running AJS script...");

  setTimeout(function () {
    const emailInput = document.querySelector('input[name="userEmail"]');
    if (emailInput) {
      console.log("User email found:", emailInput.value);
    } else {
      console.warn("Email field not found");
    }
  }, 1000); // wait for ConfiForms to render
});
</script>

