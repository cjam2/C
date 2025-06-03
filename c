<script>
AJS.toInit(function () {
  console.log("Email autofill JS is running...");

  var emailMap = {
    "AFT001": "alpha@pod.com",
    "AFT002": "bravo@pod.com",
    "BFT001": "charlie@pod.com"
  };

  function bindProductGroupListener() {
    var productGroupField = document.querySelector('[name="ProductGroup"]');
    var emailField = document.querySelector('[name="recipientEmail"]');

    if (!productGroupField || !emailField) {
      console.log("Fields not found yet. Waiting...");
      return;
    }

    // Avoid attaching multiple listeners
    if (productGroupField.dataset.listenerAttached === "true") return;
    productGroupField.dataset.listenerAttached = "true";

    productGroupField.addEventListener("change", function () {
      var selectedID = productGroupField.value;
      var email = emailMap[selectedID] || "";
      emailField.value = email;
      console.log("Updated email for:", selectedID, "→", email);
    });

    // Run once to initialize
    var selectedID = productGroupField.value;
    emailField.value = emailMap[selectedID] || "";
    console.log("Initialized email for:", selectedID, "→", emailField.value);
  }

  // Watch the form for dynamic field re-rendering
  const observer = new MutationObserver(function (mutationsList) {
    for (const mutation of mutationsList) {
      if (mutation.type === "childList") {
        bindProductGroupListener();
      }
    }
  });

  // Observe entire form container
  observer.observe(document.body, {
    childList: true,
    subtree: true
  });

  // Initial binding attempt
  bindProductGroupListener();
});
</script>