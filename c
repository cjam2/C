AJS.toInit(function () {
    var $productGroup = $('[name="ProductGroup"]');

    if ($productGroup.length) {
        console.log("[Debug] ProductGroup dropdown found:", $productGroup.length);

        $productGroup.on("change", function () {
            var rawValue = $(this).val();
            var rawHtml = $(this).html();
            console.log("=== ProductGroup Change Detected ===");
            console.log("Selected value (raw):", rawValue);
            console.log("Selected option text:", $(this).find("option:selected").text());
            console.log("Full HTML of dropdown:\n", rawHtml);
        });
    } else {
        console.warn("[Debug] ProductGroup dropdown not found");
    }
});