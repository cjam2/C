AJS.toInit(function () {
    // Wait for DOM and ConfiForms to render
    var checkExist = setInterval(function () {
        var $productGroup = $('[name="ProductGroup"]');

        if ($productGroup.length) {
            clearInterval(checkExist);
            console.log("[Debug] ProductGroup dropdown found");

            $productGroup.on("change", function () {
                var value = $(this).val();
                var text = $(this).find("option:selected").text();

                // Ignore if "Platform" is selected
                if (text.trim().toLowerCase() === "platform") {
                    console.log("[Debug] Platform selection detected → Ignoring");
                    return;
                }

                console.log("[Debug] ProductGroup changed → Updating");
                console.log("Selected Value:", value);
                console.log("Selected Text:", text);

                // Your update JQL logic here
                updateJiraJQL(value);
            });
        }
    }, 500);

    function updateJiraJQL(productGroupValue) {
        // Example: dynamically set your JQL based on ProductGroup
        var jql = `project = CORE AND "Product Group" = "${productGroupValue}" ORDER BY updated DESC`;
        console.log("[Debug] New JQL:", jql);

        // TODO: update your Jira macro or iframe here
    }
});