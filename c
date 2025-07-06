- name: Check and install sshpass if needed
  run: |
    if ! command -v sshpass &> /dev/null; then
      echo "sshpass not found. Installing..."
      sudo apt-get update && sudo apt-get install -y sshpass
    else
      echo "sshpass is already installed. Skipping installation."
    fi
