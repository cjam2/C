name: Run Shell Script on Schedule

on:
  schedule:
    - cron: '0 3 * * *'  # runs daily at 3 AM UTC
  workflow_dispatch:      # allows manual run (optional)

jobs:
  run-script:
    runs-on: ubuntu-latest

    steps:
    - name: Checkout repository
      uses: actions/checkout@v3

    - name: Make script executable
      run: chmod +x ./scripts/run_me.sh  # update path if needed

    - name: Run script with parameters
      run: ./scripts/run_me.sh param1 param2