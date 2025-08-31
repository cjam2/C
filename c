curl -v --url smtp://relay.cloud.td.com:25 --mail-from alfred@yourcompany.com --mail-rcpt youremail@yourcompany.com --upload-file <(echo -e "Subject: Test Email\n\nHello, this is a test via curl.\n")


echo Subject: Test Email > message.txt
echo. >> message.txt
echo Hello, this is a test via curl. >> message.txt

curl -v --url smtp://relay.cloud.td.com:25 --mail-from alfred@yourcompany.com --mail-rcpt youremail@yourcompany.com --upload-file message.txt