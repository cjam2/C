name: Run Script with Multi-Line Servers

on:
  workflow_dispatch:
    inputs:
      servers:
        description: "List of servers separated by __NEWLINE__"
        required: true
        default: "10.8.97.654__NEWLINE__10.9.97.777__NEWLINE__10.8.98.8575"
      ssh_user:
        description: "SSH username"
        required: true
        default: "admin"
      ssh_pass:
        description: "SSH password"
        required: true

jobs:
  process-servers:
    runs-on: ubuntu-latest

    steps:
    - name: Checkout repo
      uses: actions/checkout@v3

    - name: Prepare server list and run script
      run: |
        # Convert __NEWLINE__ to real newlines
        CLEANED_SERVERS=$(echo "${{ github.event.inputs.servers }}" | sed 's/__NEWLINE__/\
/g')

        echo "Parsed server list:"
        echo "$CLEANED_SERVERS"

        # Save to file for script consumption
        echo "$CLEANED_SERVERS" > servers.txt

        # (Optional) make your script executable
        chmod +x ./scripts/run.sh

        # Run your script and pass values
        ./scripts/run.sh "$(cat servers.txt)" "${{ github.event.inputs.ssh_user }}" "${{ github.event.inputs.ssh_pass }}"