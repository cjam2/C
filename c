- name: Check if today is the second Monday (Toronto)
  env:
    TZ: America/Toronto   # set your local tz
  shell: bash
  run: |
    dow=$(date +%u)                  # 1..7 (Mon..Sun)
    dom=$((10#$(date +%d)))          # 1..31 (strip leading zero)
    if [ "$dow" -eq 1 ] && [ "$dom" -ge 8 ] && [ "$dom" -le 14 ]; then
      echo "IS_SECOND_MONDAY=true" >> "$GITHUB_ENV"
    else
      echo "IS_SECOND_MONDAY=false" >> "$GITHUB_ENV"
    fi



- name: Run job only on second Monday
  if: env.IS_SECOND_MONDAY == 'true'
  run: echo "Doing the thing…"

