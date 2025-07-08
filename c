curl --url "smtp://smtp-relay.company.com:25" \
  --mail-from "jenkins@company.com" \
  --mail-rcpt "you@example.com" \
  --upload-file "${GITHUB_WORKSPACE}/email.html" \
  --header "Subject: Build Notification" \
  --header "Content-Type: text/html"