name: Two Runner Example

on:
  workflow_dispatch:   # trigger manually (you can change to push, schedule, etc.)

jobs:
  main_program:
    name: Run Main Program
    runs-on: [self-hosted, Linux, organization]   # 1st runner
    steps:
      - name: Checkout repo
        uses: actions/checkout@v4

      - name: Run program
        run: |
          echo "Running main program on Linux organization runner"
          ./run_main.sh

  send_email:
    name: Send Email
    needs: main_program     # ensures this waits for main program
    runs-on: [self-hosted, Linux, email_runner]   # 2nd runner
    steps:
      - name: Checkout repo
        uses: actions/checkout@v4

      - name: Send email
        run: |
          echo "Sending email from dedicated email runner"
          ./send_email.sh