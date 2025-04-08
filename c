#!/bin/bash

DURATION=$((24 * 60 * 60))   # 24 hours
INTERVAL=60                  # check every 60 seconds
APP_KEYWORD="your_app_keyword"
ALERT_EMAIL="you@example.com"
LOG_FILE="memory_oom_log_$(date +%Y%m%d_%H%M%S).log"
OOM_LOG_FILE="oom_kills.log"

echo "Monitoring started at $(date)" | tee -a "$LOG_FILE"

START_TIME=$(date +%s)

while [ $(($(date +%s) - START_TIME)) -lt $DURATION ]; do
    TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')
    
    echo "[$TIMESTAMP] Memory Stats:" | tee -a "$LOG_FILE"
    free -m | tee -a "$LOG_FILE"
    
    # Monitor app presence
    if ! pgrep -f "$APP_KEYWORD" > /dev/null; then
        echo "[$TIMESTAMP] ALERT: Application '$APP_KEYWORD' is not running!" | tee -a "$LOG_FILE"
        echo "Subject: ALERT - App '$APP_KEYWORD' stopped running on $(hostname)
        
Time: $TIMESTAMP
App '$APP_KEYWORD' is NOT running on $(hostname).
        
Check memory usage or logs for issues." | mail -s "App '$APP_KEYWORD' Down" "$ALERT_EMAIL"
    fi

    # Check for new OOM kills
    NEW_OOM=$(dmesg --ctime | grep -i "killed process" | tail -n 1)
    if [ -n "$NEW_OOM" ]; then
        echo "[$TIMESTAMP] OOM Kill Detected: $NEW_OOM" | tee -a "$LOG_FILE"
        echo "$NEW_OOM" >> "$OOM_LOG_FILE"

        echo "Subject: ALERT - OOM Kill Detected on $(hostname)

Time: $TIMESTAMP
OOM Kill Message: $NEW_OOM

Check memory usage or restart necessary apps." | mail -s "OOM Kill on $(hostname)" "$ALERT_EMAIL"
    fi

    echo "---------------------------------------------" | tee -a "$LOG_FILE"
    sleep $INTERVAL
done

echo "Monitoring ended at $(date)" | tee -a "$LOG_FILE"