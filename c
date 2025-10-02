@echo off
setlocal EnableDelayedExpansion

:: ================== CONFIG =====================
set "PLINK=plink.exe"
set "USER=myuser"
set "PASS=mypassword"

:: File containing list of servers (one per line)
set "SERVERFILE=servers.txt"

:: Output file
set "OUTFILE=output.txt"

:: ======= Customize your Linux commands here =====
:: Separate multiple commands with " && "
set "CMDS=uname -a && whoami && uptime"
:: ================================================
if exist "%OUTFILE%" del "%OUTFILE%"

:: Loop over each server in servers.txt
for /f "usebackq tokens=* delims=" %%S in ("%SERVERFILE%") do (
  if not "%%S"=="" (
    echo ======================================================= >> "%OUTFILE%"
    echo Running on server: %%S >> "%OUTFILE%"
    echo ======================================================= >> "%OUTFILE%"

    "%PLINK%" -ssh -pw "%PASS%" -batch -noagent %USER%@%%S "%CMDS%" >> "%OUTFILE%" 2>&1

    echo. >> "%OUTFILE%"
  )
)

echo Done. Output saved to %OUTFILE%
endlocal
pause