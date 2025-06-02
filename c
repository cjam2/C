<script>
AJS.toInit(function() {
  console.log("Script loaded, checking page...");

  // Replace this with your page ID
  var targetPageId = "12345678";

  // Get current page ID from meta tag
  var currentPageId = document.querySelector('meta[name="ajs-page-id"]')?.getAttribute("content");

  if (currentPageId === targetPageId) {
    console.log("Page ID matched, running script...");

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
    } else {
      console.log("Fields not found. Check field names.");
    }

  } else {
    console.log("Script skipped: Not the target page.");
  }
});
</script>