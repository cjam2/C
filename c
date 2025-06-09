//CHKTOOLS JOB (ACCT),'CHECK JAVA/PYTHON',
//             CLASS=A,MSGCLASS=H,NOTIFY=&SYSUID
//STEP1   EXEC PGM=BPXBATCH,
//             PARM='PGM /bin/sh'
//STDOUT  DD SYSOUT=*
//STDERR  DD SYSOUT=*
//SYSIN   DD *
which java
which python
which python3
/*
//
