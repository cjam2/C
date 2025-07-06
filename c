node {
    def mailer = Jenkins.instance.getDescriptor(hudson.tasks.Mailer.class)

    echo "=== SMTP Configuration ==="
    echo "SMTP Host      : ${mailer.smtpHost}"
    echo "SMTP Port      : ${mailer.smtpPort}"
    echo "Use SSL        : ${mailer.useSsl}"
    echo "Reply-To Addr  : ${mailer.replyToAddress}"
    echo "SMTP Username  : ${mailer.smtpAuthUsername}"
    echo "SMTP Password  : ${hudson.util.Secret.toString(mailer.smtpAuthPassword)}"
}
