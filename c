- name: Parse server list using bash
  run: |
    bash -c '
      RAW_INPUT="${{ github.event.inputs.servers }}"
      CLEANED_SERVERS="${RAW_INPUT//__NEWLINE__/$'\n'}"

      echo "Parsed servers:"
      echo "$CLEANED_SERVERS"

      echo "$CLEANED_SERVERS" > servers.txt

      chmod +x ./scripts/run.sh
      ./scripts/run.sh "$(cat servers.txt)" "${{ github.event.inputs.ssh_user }}" "${{ github.event.inputs.ssh_pass }}"
    '