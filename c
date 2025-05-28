<script>
AJS.toInit(function() {
  // Mapping of recipientName id to email
  const emailMapping = {
    "sr_john": "john@example.com",
    "br_mary": "mary@example.com",
    "tr_team": "team@example.com"
  };

  // Update the email field when dropdown changes
  AJS.$("select[name='recipientName']").on("change", function() {
    const selectedValue = AJS.$(this).val(); // Get the id selected
    const email = emailMapping[selectedValue] || "";
    console.log("Setting email for", selectedValue, "to", email);
    AJS.$("input[name='recipientEmail']").val(email);
  });

  // Optional: Populate email field on page load (if a value is pre-selected)
  const initialValue = AJS.$("select[name='recipientName']").val();
  if (initialValue) {
    const initialEmail = emailMapping[initialValue] || "";
    AJS.$("input[name='recipientEmail']").val(initialEmail);
  }
});
</script>