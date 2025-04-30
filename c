#!/bin/bash

PROCESS_NAME="Abd_0000"
LOG_DIR="$HOME/memory_logs"
mkdir -p "$LOG_DIR"
LOG_FILE="$LOG_DIR/${PROCESS_NAME}_usage_$(date +%Y%m%d_%H%M%S).log"

echo "Monitoring $PROCESS_NAME started at $(date)" >> "$LOG_FILE"

while true
do
    TIMESTAMP=$(date)

    PID=$(pgrep -f "$PROCESS_NAME" | head -n 1)

    if [ -z "$PID" ]; then
        echo "[$TIMESTAMP] Process $PROCESS_NAME not running" >> "$LOG_FILE"
    else
        STATS=$(ps -p "$PID" -o pid,cmd,%mem,%cpu,rss --no-headers)
        RSS_MB=$(echo "$STATS" | awk '{print $5/1024}')  # Convert KB to MB

        echo "[$TIMESTAMP] $STATS" >> "$LOG_FILE"

        # Alert if memory exceeds 1500MB
        if (( $(echo "$RSS_MB > 1500" | bc -l) )); then
            echo "[$TIMESTAMP] ALERT: Memory usage exceeded 1500MB: ${RSS_MB}MB" >> "$LOG_FILE"
        fi
    fi

    sleep 60
done