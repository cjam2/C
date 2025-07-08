curl --url "smtps://smtp.office365.com:465" \
  --ssl-reqd \
  --mail-from "your_email@outlook.com" \
  --mail-rcpt "recipient@example.com" \
  --user "your_email@outlook.com:your_app_password" \
  --upload-file email.html \
  --header "Subject: Test Email from curl (Outlook)" \
  --header "From: Your Name <your_email@outlook.com>" \
  --header "Content-Type: text/html"