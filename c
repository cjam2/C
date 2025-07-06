- name: Check and install sshpass using expect if needed
  run: |
    if ! command -v sshpass &> /dev/null; then
      echo "sshpass not found. Installing using expect..."

      sudo -k  # force sudo to prompt for password

      expect <<EOF
      spawn sudo apt-get update && sudo apt-get install -y sshpass
      expect "password for"
      send "${{ secrets.SUDO_PASS }}\r"
      expect eof
EOF

    else
      echo "sshpass is already installed. Skipping installation."
    fi
