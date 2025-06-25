<script>
AJS.toInit(function() {
  // Get username if it's rendered somewhere in the DOM (not email)
  var userInfo = AJS.params.remoteUser;  // Only works in some Server versions
  console.log("Logged-in user:", userInfo); // May be blank in Cloud
});
</script>
