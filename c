import hudson.util.Secret
import jenkins.model.*
import hudson.tasks.Mailer

def mailer = Jenkins.instance.getDescriptor(Mailer.class)

println "=== SMTP Configuration ==="
println "SMTP Host      : ${mailer.smtpHost}"
println "SMTP Port      : ${mailer.smtpPort}"
println "Use SSL        : ${mailer.useSsl}"
println "Reply-To Addr  : ${mailer.replyToAddress}"
println "Charset        : ${mailer.charset ?: 'default'}"

def username = mailer.smtpAuthUsername
def password = Secret.toString(mailer.smtpAuthPassword)

if (username) {
    println "SMTP Username  : ${username}"
} else {
    println "SMTP Username  : (not set)"
}

if (password) {
    println "SMTP Password  : ${password}"
} else {
    println "SMTP Password  : (not set)"
}
