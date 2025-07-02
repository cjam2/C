- name: Save output to file
  run: |
    echo "Some log output" > output.log

- name: Upload log as artifact
  uses: actions/upload-artifact@v4
  with:
    name: logs
    path: output.log



- name: Save log to file
  run: |
    echo "Hello log data" > logs.txt

- name: Send email with attachment
  uses: dawidd6/action-send-mail@v3
  with:
    server_address: smtp.gmail.com
    server_port: 465
    username: ${{ secrets.EMAIL_USERNAME }}
    password: ${{ secrets.EMAIL_PASSWORD }}
    subject: GitHub Action Log
    to: your@email.com
    from: GitHub Actions <${{ secrets.EMAIL_USERNAME }}>
    body: |
      Hello, your job has completed.
      Please see the attached log file.
    attachments: logs.txt



