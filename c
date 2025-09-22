rem === Build the remote command to run the script ===
set "REMOTE_CMD=bash -lc ""TS=$(date +%%Y%%m%%d_%%H%%M%%S); \
OUT='%REMOTE_DIR%/%SERVER%_'$TS'.log; \
chmod +x '%REMOTE_DIR%/%REMOTE_SH_NAME%'; \
'%REMOTE_DIR%/%REMOTE_SH_NAME%' > $OUT 2>&1; \
echo $OUT"""