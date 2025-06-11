<input type="email" id="emailField" class="required" readonly placeholder="Loading user email..." style="...">

<script>
  AP.require(["user"], function(user) {
    user.getCurrentUser(function(currentUser) {
      const e = document.getElementById("emailField");
      if (e) {
        e.value = currentUser.email || "";
        e.style.border = "1px solid #ccc";
      }
    });
  });
</script>
