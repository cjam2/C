<script>
AJS.toInit(function() {
  const emailMapping = {
    "sr_john": "john@example.com",
    "br_mary": "mary@example.com",
    "tr_team": "team@example.com"
  };

  const $dropdown = AJS.$("select[name='recipientName']");
  const $emailField = AJS.$("input[name='recipientEmail']");

  function updateEmail() {
    const selectedId = $dropdown.val();
    const email = emailMapping[selectedId] || "";
    console.log("Setting email for", selectedId, "to", email);
    $emailField.val(email);
  }

  $dropdown.on("change", updateEmail);
  updateEmail(); // Populate on load
});
</script>