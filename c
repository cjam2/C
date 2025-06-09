/* REXX */
address TSO
rc = sockinit()
say "RC from SOCKINIT:" rc
call sockterm
