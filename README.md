# Sample JIRA Form Application

This repository now includes a small Node.js application that demonstrates how to
collect user input for creating a JIRA ticket and sending an email with the
resulting ticket number.

## Running the application

1. (Optional) install `nodemailer` if you want email sending:
   ```bash
   npm install nodemailer
   ```

2. Set environment variables for JIRA and email access as needed. For example:
   ```bash
   export JIRA_BASE_URL=https://jira.example.com
   export JIRA_USER=user@example.com
   export JIRA_TOKEN=yourapitoken
   export JIRA_PROJECT_KEY=DEMO
   export SMTP_HOST=smtp.example.com
   export SMTP_USER=me@example.com
   export SMTP_PASS=secret
   export EMAIL_RECIPIENTS="team@example.com"
   ```

3. Start the server:
   ```bash
   node app/server.js
   ```

Open `http://localhost:3000` in your browser to use the form.

This application only demonstrates the flow. In environments without JIRA or
SMTP configured, it falls back to a placeholder ticket number and skips sending
email.
