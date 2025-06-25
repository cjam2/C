<button onclick="copyToClipboard()">Copy Jenkins Trigger</button>

<script>
function copyToClipboard() {
  const cmd = `curl -X POST https://jenkins.example.com/job/your-job-name/build?token=YOURTOKEN`;
  navigator.clipboard.writeText(cmd)
    .then(() => alert("Command copied. Paste it into your terminal."))
    .catch(() => alert("Failed to copy"));
}
</script>
