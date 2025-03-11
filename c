pipeline {
    agent any
    parameters {
        string(name: 'CONFLUENCE_URL', defaultValue: 'https://your-confluence-instance', description: 'Base Confluence URL')
        string(name: 'PAGE_ID', defaultValue: '123456', description: 'ID of the Confluence page to extract data from')
        string(name: 'USERNAME', defaultValue: 'your-username', description: 'Confluence username')
        password(name: 'PASSWORD', defaultValue: '', description: 'Confluence API token')
        string(name: 'RECIPIENT_EMAIL', defaultValue: 'recipient@example.com', description: 'Email to receive the extracted codes')
    }
    environment {
        API_URL = "${params.CONFLUENCE_URL}/rest/api/content/${params.PAGE_ID}?expand=body.storage"
    }
    stages {
        stage('Extract Data from Confluence') {
            steps {
                script {
                    echo "Fetching data from Confluence page ${params.PAGE_ID}..."

                    def response = sh(script: """
                        curl -u ${params.USERNAME}:${params.PASSWORD} -X GET "${env.API_URL}" -H "Accept: application/json" -s
                    """, returnStdout: true).trim()

                    def jsonResponse = readJSON(text: response)
                    def htmlContent = jsonResponse.body.storage.value
                    
                    // Save the HTML content to a file
                    writeFile file: 'confluence_content.html', text: htmlContent

                    echo "Extracting table data..."

                    // Parse table data using Python
                    def extractedCodes = sh(script: """
                        python3 -c '
import sys
from bs4 import BeautifulSoup

with open("confluence_content.html", "r") as f:
    soup = BeautifulSoup(f.read(), "html.parser")

table = soup.find("table")
codes = []

if table:
    for row in table.find_all("tr"):
        cols = row.find_all("td")
        if cols:
            codes.append(cols[0].text.strip())

print("\\n".join(codes))
                        '""", returnStdout: true).trim()

                    echo "Extracted Codes:\n${extractedCodes}"
                    writeFile file: 'codes.txt', text: extractedCodes
                }
            }
        }

        stage('Send Email') {
            steps {
                script {
                    echo "Sending email with extracted codes..."

                    emailext (
                        subject: "Extracted Confluence Codes",
                        body: "Here are the extracted codes from Confluence:\n\n${readFile('codes.txt')}",
                        to: "${params.RECIPIENT_EMAIL}",
                        from: "jenkins@yourdomain.com"
                    )
                }
            }
        }
    }
}