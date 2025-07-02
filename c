run: |
  RAW_INPUT="${{ github.event.inputs.servers }}"
  CLEANED_SERVERS=$(printf "%s" "$RAW_INPUT" | tr '__NEWLINE__' '\n')

  echo "Parsed server list:"
  echo "$CLEANED_SERVERS"

  echo "$CLEANED_SERVERS" > servers.txt

  chmod +x ./scripts/run.sh
  ./scripts/run.sh "$(cat servers.txt)" "${{ github.event.inputs.ssh_user }}" "${{ github.event.inputs.ssh_pass }}"