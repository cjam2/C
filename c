/* REXX - Trigger Jenkins via raw socket */
ADDRESS TSO
rc = sockinit()
if rc <> 0 then do
  say "❌ SOCKINIT failed."
  exit 8
end

host = 'jenkins.yourdomain.com'
port = 443   /* or 80 if using HTTP */

call socket 'SOCKET', 'AF_INET', 'SOCK_STREAM', '0'
sock = retval
if sock < 0 then do
  say "❌ SOCKET call failed."
  exit 8
end

call socket 'GETHOSTBYNAME', host
if retval = '' then do
  say "❌ Unknown host"
  exit 8
end

ip = word(retval, 2)

call socket 'CONNECT', sock, 'AF_INET.'port'.'ip
if retval <> 0 then do
  say "❌ CONNECT failed"
  exit 8
end

triggerUrl = "/job/YourJobName/build?token=ABC123"
jenkinsHost = "jenkins.yourdomain.com"

req = "POST "triggerUrl" HTTP/1.1" || '0D0A'x
req = req || "Host: " || jenkinsHost || '0D0A'x
req = req || "User-Agent: zOS REXX" || '0D0A'x
req = req || "Connection: close" || '0D0A0D0A'x

call socket 'SEND', sock, req
if retval < 0 then do
  say "❌ SEND failed"
  exit 8
end

do forever
  call socket 'RECV', sock, 4096
  if retval <= 0 then leave
  say retval
end

call socket 'CLOSE', sock
call sockterm
