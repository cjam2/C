#!/usr/bin/env bash
set -euo pipefail

# ======= CONFIG =======
TARGETS_FILE="${1:-targets.txt}"   # optional arg; default ./targets.txt
OUT_DIR="${OUT_DIR:-ssl_logs}"
PARALLEL="${PARALLEL:-6}"          # how many concurrent checks
TIMEOUT_SECS="${TIMEOUT_SECS:-8}"  # per-target timeout
OPENSSL_BIN="${OPENSSL_BIN:-openssl}"  # path to openssl if not in PATH
# ======================

mkdir -p "$OUT_DIR"
SUMMARY="$OUT_DIR/summary_$(date +%Y%m%d_%H%M%S).csv"
echo "target,status,rt_ms,tls_version,cipher" > "$SUMMARY"

# simple job limiter without GNU parallel
joblim() { while [ "$(jobs -pr | wc -l)" -ge "$PARALLEL" ]; do sleep 0.1; done; }

run_one() {
  local target="$1" ip port sni file t0 t1 rt_ms
  IFS=',' read -r hostport sni <<<"$target"
  IFS=':' read -r ip port <<<"$hostport"
  file="$OUT_DIR/${ip}_${port}.log"

  t0="$(date +%s%3N 2>/dev/null || date +%s000)"
  # Build command
  cmd=( "$OPENSSL_BIN" s_client -connect "${ip}:${port}" -brief -quiet -showcerts )
  # If SNI provided (after comma), set -servername
  if [ -n "${sni:-}" ]; then cmd+=( -servername "$sni" ); fi

  # Do not wait for stdin; quit after handshake
  cmd+=( -ign_eof )                 # close after handshake
  # execute with timeout and capture
  if timeout "$TIMEOUT_SECS" "${cmd[@]}" < /dev/null >"$file" 2>&1; then
    status="OK"
  else
    rc=$?
    if [ $rc -eq 124 ]; then status="TIMEOUT"; else status="FAIL($rc)"; fi
  fi
  t1="$(date +%s%3N 2>/dev/null || date +%s000)"
  rt_ms=$(( t1 - t0 ))

  # Parse tls version & cipher if present
  tls_ver="$(grep -m1 -Eo 'TLSv[0-9\.]+' "$file" || true)"
  cipher="$(grep -m1 -Eo 'Cipher is [^ ]+' "$file" | awk '{print $3}' || true)"

  echo "${ip}:${port}${sni:+,$sni},${status},${rt_ms},${tls_ver},${cipher}" >> "$SUMMARY"
  echo "[$status] ${ip}:${port}${sni:+ ($sni)} in ${rt_ms}ms -> $file"
}

# Read targets (skip blanks/comments)
while IFS= read -r line; do
  line="${line%"${line##*[![:space:]]}"}"; line="${line#"${line%%[![:space:]]*}"}"
  [[ -z "$line" || "$line" =~ ^# ]] && continue
  joblim
  run_one "$line" &
done < "$TARGETS_FILE"

wait
echo
echo "Done. Summary: $SUMMARY"