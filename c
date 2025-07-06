#!/bin/bash

# List of programs to check
TOOLS=("awk" "sshpass" "curl" "wget" "sed" "grep" "git" "expect" "ssh" "python3" "node")

echo "🔍 Checking installed programs..."

for tool in "${TOOLS[@]}"; do
  if command -v "$tool" &>/dev/null; then
    version=$("$tool" --version 2>/dev/null | head -n 1)
    echo "$tool is installed — $version"
  else
    echo " $tool is NOT installed"
  fi
done

echo " Program check complete."
