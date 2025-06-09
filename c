//JIRAJAVA JOB (ACCT),'RUN JIRA JAVA',
//             CLASS=A,MSGCLASS=H,NOTIFY=&SYSUID
//STEP1   EXEC PGM=BPXBATCH,
//             PARM='PGM /usr/lpp/java/J8.0_64/bin/java JiraSubtaskCreator ABC-123 johndoe'
//STDOUT  DD SYSOUT=*
//STDERR  DD SYSOUT=*
//STDENV  DD * 
CLASSPATH=/u/yourid/java
HOME=/tmp
JAVA_HOME=/usr/lpp/java/J8.0_64
PATH=/usr/lpp/java/J8.0_64/bin:/bin
TZ=EST5EDT
/*
//
