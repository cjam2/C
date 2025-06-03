<ac:structured-macro ac:name="html">
    <ac:plain-text-body><![CDATA[
        <script>
        // Define the function globally so ConfiForms can find it
        function updateEmailFromProductGroup() {
          console.log("updateEmailFromProductGroup function called by IFTTT.");

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
            var selectedID = nameField.value;
            var email = emailMap[selectedID] || "";
            console.log("IFTTT Trigger: Selected ID:", selectedID, "Mapped Email:", email);
            emailField.value = email;
          } else {
            console.warn("IFTTT Trigger: One or both fields were not found! nameField:", nameField, "emailField:", emailField);
          }
        }

        // You can add a direct call here if you want to try to force initial population
        // outside of IFTTT, but only if your environment allows general inline scripts.
        // If not, rely solely on IFTTT fieldchange.
        AJS.toInit(function() {
            console.log("Attempting direct AJS.toInit run for initial populate (might be blocked).");
            updateEmailFromProductGroup(); // Call once on page load
        });

        </script>
    ]]></ac:plain-text-body>
</ac:structured-macro>
