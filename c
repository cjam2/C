#!/usr/bin/env bash
# batch_runner.sh — upload a local .sh to many servers, run it, fetch logs, clean up.

set -u

# ======== CONFIG (edit as needed) ========
USER="${USER:-ubuntu}"          # remote user
PASS="${PASS:-}"                # remote password (optional; uses sshpass if set)
PORT="${PORT:-22}"              # SSH port
SUDO_MODE="${SUDO_MODE:-0}"     # 0=no sudo, 1=run the script with sudo
STRICT_HOST="${STRICT_HOST:-accept-new}"  # StrictHostKeyChecking: no|accept-new|yes
LOCAL_SH="${LOCAL_SH:-$(pwd)/my_script.sh}"
SERVERS_FILE="${SERVERS_FILE:-$(pwd)/servers.txt}"
OUT_DIR="${OUT_DIR:-$(pwd)/outputs}"
REMOTE_DIR="${REMOTE_DIR:-/tmp}"
REMOTE_SH_NAME="${REMOTE_SH_NAME:-runner.sh}"

# ======== SSH/ SCP wrappers (support password or key) ========
ssh_cmd_base=(ssh -p "$PORT" -o StrictHostKeyChecking="$STRICT_HOST" -o LogLevel=ERROR)
scp_cmd_base=(scp -P "$PORT" -o StrictHostKeyChecking="$STRICT_HOST" -o LogLevel=ERROR)

if [[ -n "$PASS" ]]; then
  if ! command -v sshpass >/dev/null 2>&1; then
    echo "[ERROR] sshpass not found but PASS is set. Install sshpass or unset PASS." >&2
    exit 1
  fi
  ssh_cmd_base=(sshpass -p "$PASS" "${ssh_cmd_base[@]}")
  scp_cmd_base=(sshpass -p "$PASS" "${scp_cmd_base[@]}")
fi

# ======== Checks ========
[[ -f "$LOCAL_SH" ]] || { echo "[ERROR] Local script not found: $LOCAL_SH"; exit 1; }
[[ -f "$SERVERS_FILE" ]] || { echo "[ERROR] Servers file not found: $SERVERS_FILE"; exit 1; }
mkdir -p "$OUT_DIR" || { echo "[ERROR] Could not create output dir: $OUT_DIR"; exit 1; }

# ======== Helpers ========
run_ssh() { "${ssh_cmd_base[@]}" "$@"; }
run_scp() { "${scp_cmd_base[@]}" "$@"; }

# ======== Main loop ========
while IFS= read -r SERVER || [[ -n "$SERVER" ]]; do
  # Skip blanks and comments
  [[ -z "$SERVER" || "${SERVER:0:1}" == "#" ]] && continue

  echo
  echo "--- Processing $SERVER ---"

  # 1) Upload script
  if ! run_scp "$LOCAL_SH" "${USER}@${SERVER}:${REMOTE_DIR}/${REMOTE_SH_NAME}"; then
    echo "[ERROR] Upload failed for $SERVER"
    continue
  fi

  # 2) Build remote command: chmod +x; run (optionally sudo); write to timestamped log; echo log path
  REMOTE_RUN="bash -lc 'TS=\$(date +%Y%m%d_%H%M%S);
    OUT=\"${REMOTE_DIR}/${SERVER}_\${TS}.log\";
    chmod +x \"${REMOTE_DIR}/${REMOTE_SH_NAME}\";
    %RUNNER% >\"\$OUT\" 2>&1;
    echo \"\$OUT\"'"

  if [[ "$SUDO_MODE" == "1" ]]; then
    REMOTE_RUN="${REMOTE_RUN//%RUNNER%/sudo \"${REMOTE_DIR}/${REMOTE_SH_NAME}\"}"
  else
    REMOTE_RUN="${REMOTE_RUN//%RUNNER%/\"${REMOTE_DIR}/${REMOTE_SH_NAME}\"}"
  fi

  # 3) Execute and capture printed OUT path
  if ! REMOTE_OUT_PATH=$(run_ssh "${USER}@${SERVER}" "$REMOTE_RUN"); then
    echo "[ERROR] Execution failed on $SERVER"
    # attempt cleanup of uploaded script
    run_ssh "${USER}@${SERVER}" "rm -f '${REMOTE_DIR}/${REMOTE_SH_NAME}'" >/dev/null 2>&1 || true
    continue
  fi

  if [[ -z "$REMOTE_OUT_PATH" ]]; then
    echo "[ERROR] No output path returned from remote on $SERVER"
    run_ssh "${USER}@${SERVER}" "rm -f '${REMOTE_DIR}/${REMOTE_SH_NAME}'" >/dev/null 2>&1 || true
    continue
  fi

  echo "[INFO] Remote output file: $REMOTE_OUT_PATH"

  # 4) Download the output file
  if ! run_scp "${USER}@${SERVER}:${REMOTE_OUT_PATH}" "$OUT_DIR/"; then
    echo "[ERROR] Download failed for $SERVER"
    # still try to clean
    run_ssh "${USER}@${SERVER}" "rm -f '${REMOTE_DIR}/${REMOTE_SH_NAME}' '${REMOTE_OUT_PATH}'" >/dev/null 2>&1 || true
    continue
  fi

  # 5) Cleanup on server
  echo "[STEP] Cleaning up remote files on $SERVER ..."
  run_ssh "${USER}@${SERVER}" "rm -f '${REMOTE_DIR}/${REMOTE_SH_NAME}' '${REMOTE_OUT_PATH}'" >/dev/null 2>&1 || \
    echo "[WARN] Cleanup may have failed on $SERVER"

  echo "[DONE] Finished $SERVER"
done < "$SERVERS_FILE"

echo
echo "All done. Logs saved in: $OUT_DIR"