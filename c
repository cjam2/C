<p>
    <ac:structured-macro ac:name="html">
        <ac:plain-text-body><![CDATA[
            <script>
            // Define the function globally or in a way that ConfiForms can see it
            // AJS.toInit is NOT needed here if afterLoadInit is handling the timing.
            // However, defining it within a script tag might still be necessary.
            function initializeEmailLogic() {
              console.log("Custom JS initialized via afterLoadInit.");

              var nameFieldName = "ProductGroup";
              var emailFieldName = "recipientEmail";

              var emailMap = {
                "CreditDecisioning:1": "CRPH01_RAD-Pod@td.com",
                "CreditDecisioning:2": "CRPH02_RAD-Pod@td.com",
                "CreditDecisioning:3": "CRPHCR_RAD-Pod@td.com"
              };

              // IMPORTANT: Fields must be found. Use a more specific selector if needed,
              // e.g., if you have multiple forms on the page.
              var nameField = document.querySelector('[name="' + nameFieldName + '"]');
              var emailField = document.querySelector('[name="' + emailFieldName + '"]');

              if (nameField && emailField) {
                console.log("ConfiForms afterLoadInit: Both fields found.");
                function updateEmail() {
                  var selectedID = nameField.value;
                  var email = emailMap[selectedID] || "";
                  console.log("ConfiForms afterLoadInit: Selected ID:", selectedID, "Mapped Email:", email);
                  emailField.value = email;
                }

                nameField.addEventListener('change', updateEmail);
                updateEmail(); // Call on form load to initialize the email field
              } else {
                console.warn("ConfiForms afterLoadInit: One or both fields were not found!");
              }
            }
            </script>
        ]]></ac:plain-text-body>
    </ac:structured-macro>
</p>

<p>
    <ac:structured-macro ac:name="confiform" ac:schema-version="1">
        <ac:parameter ac:name="formName">YourFormName</ac:parameter>
        <ac:rich-text-body>
            <ac:structured-macro ac:name="confiform-entry-register" ac:schema-version="1">
                <ac:parameter ac:name="formName">YourFormName</ac:parameter>
                <ac:parameter ac:name="afterLoadInit">initializeEmailLogic</ac:parameter>
                </ac:structured-macro>

            <ac:structured-macro ac:name="confiform-field" ac:schema-version="1">
                <ac:parameter ac:name="name">ProductGroup</ac:parameter>
                <ac:parameter ac:name="type">select</ac:parameter>
                <ac:parameter ac:name="options">CreditDecisioning:1|Option 1,CreditDecisioning:2|Option 2,CreditDecisioning:3|Option 3</ac:parameter>
                <ac:parameter ac:name="label">Product Group</ac:parameter>
            </ac:structured-macro>
            <ac:structured-macro ac:name="confiform-field" ac:schema-version="1">
                <ac:parameter ac:name="name">recipientEmail</ac:parameter>
                <ac:parameter ac:name="type">text</ac:parameter>
                <ac:parameter ac:name="label">Recipient Email</ac:parameter>
                <ac:parameter ac:name="readonly">true</ac:parameter>
            </ac:structured-macro>
            <ac:structured-macro ac:name="confiform-button" ac:schema-version="1">
                <ac:parameter ac:name="type">submit</ac:parameter>
                <ac:parameter ac:name="label">Submit</ac:parameter>
            </ac:structured-macro>
        </ac:rich-text-body>
    </ac:structured-macro>
</p>
