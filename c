

for /f "usebackq tokens=*" %%S in ("%SERVERS_FILE%") do (
    set "SERVER=%%S"
    if not "!SERVER!"=="" (
        echo -------------------------------
        echo Connecting to: !SERVER!
        echo Trying with -batch first...

        :: Try non-interactive
        "%PLINK%" -ssh -batch -pw "%PASS%" %USER%!@!SERVER! "%REMOTE_COMMAND%"
        if !ERRORLEVEL! EQU 0 (
            echo [SUCCESS] Batch worked fine for !SERVER!
        ) else (
            echo [FAILED] Batch mode failed for !SERVER!
            echo Switching to manual mode so you can accept key or login...
            "%PLINK%" -ssh -pw "%PASS%" %USER%!@!SERVER! "%REMOTE_COMMAND%"
        )
        echo.
    )
)

echo ========================================
echo Completed for all servers.
echo ========================================
endlocal