$language = "Python"

$interface = "1.0"

""" SecureCRT Multi-Server Command Runner (no sudo)

What this script does

Lets you define server groups and commands in one place.

Runs commands across many servers (in separate tabs), skipping any command that starts with "sudo".

Lets you choose at run time which groups/hosts to target.

Supports three "areas" of commands:

1. commands_for_all: run on every selected server.


2. commands_per_group: extra commands per chosen group.


3. one_off_commands: run once on a single designated server ("one area").



Uses existing SecureCRT sessions by name (recommended). Optionally, quick-connect via username/password.


How to use

1. Put this file in SecureCRT's Scripts folder.


2. Adjust the CONFIG section below for your environment.


3. In SecureCRT: Script ➜ Run, pick this script.



Notes

No sudo: commands beginning with "sudo" are ignored for safety.

Works best when each host has a saved SecureCRT session named exactly as the host string. E.g., a session "srv-01" under Sessions/.

If you prefer quick-connect, set USE_SAVED_SESSIONS = False and fill USERNAME (and optionally PASSWORD or use prompt at runtime). """


import re import time from datetime import datetime

==========================

====== CONFIG HERE =======

==========================

Use saved SecureCRT sessions by name (True) or quick-connect (False)

USE_SAVED_SESSIONS = True USERNAME = ""   # used only if USE_SAVED_SESSIONS is False PASSWORD = None  # set to None to be prompted at runtime (safer) PORT = 22

Timeout when waiting for shell prompt after connect (seconds)

CONNECT_TIMEOUT = 20

How to detect the remote prompt; tweak as needed for your fleet.

PROMPT_REGEX = r"[$#>]\s*$"  # generic shell prompt ending

---- Define server groups ----

SERVER_GROUPS = { # Example groups — edit to match your environment "web": ["web01", "web02", "web03"], "api": ["api01", "api02"], "db":  ["db01"], }

---- Define commands by area ----

COMMANDS = { "commands_for_all": [ "uname -a", "uptime", "df -h | head -n 5", ], # Per-group extras (only run if that group is selected) "commands_per_group": { "web": [ "grep -E 'worker_processes|worker_connections' /etc/nginx/nginx.conf || true", ], "api": [ "ps -o pid,cmd -C gunicorn || true", ], "db": [ "pg_isready || true", ], }, # Per-host overrides/extras (optional) "commands_per_host": { # "web02": ["echo 'special case for web02'"] }, # Run once in ONE designated place ("one area") # Set ONE_OFF_HOST to choose where these run. "one_off_commands": [ "echo 'Running once only on the one-off host'", "date", ], }

ONE_OFF_HOST = "web01"  # change to your preferred single host for one-off commands

==========================

===== END OF CONFIG ======

==========================

def _msg(text, title="Info"): try: crt.Dialog.MessageBox(str(text), title) except Exception: pass

def _prompt(text, title="Input", default=""): try: return crt.Dialog.Prompt(text, title, default, False) except Exception: return default

def _confirm(text, title="Confirm"): try: return crt.Dialog.MessageBox(text, title, 4) == 6  # IDYES except Exception: return True

def wait_for_prompt(tab, timeout=CONNECT_TIMEOUT): screen = tab.Screen screen.Synchronous = True end_time = time.time() + timeout while time.time() < end_time: text = screen.Get(1, 1, screen.Rows, screen.Columns) if re.search(PROMPT_REGEX, text): return True time.sleep(0.2) return False

def send_command(tab, cmd): if cmd.strip().startswith("sudo"): tab.Session.Log("[SKIPPED sudo]: {}\n".format(cmd)) return screen = tab.Screen screen.Send(cmd + "\r")

def run_commands(tab, commands): for cmd in commands: send_command(tab, cmd) # simple pacing; adjust if your commands are long-running time.sleep(0.2)

def connect_to_host(host): # Returns a tab or None if USE_SAVED_SESSIONS: # Expect a pre-saved session with this exact name try: tab = crt.Session.ConnectInTab(f"/S {host}") tab.Activate() if not wait_for_prompt(tab): _msg(f"Timeout waiting for prompt on {host}", "Timeout") return tab except Exception as e: _msg(f"Failed to open session '{host}': {e}", "Connect Error") return None else: # Quick connect global PASSWORD if not USERNAME: _msg("USERNAME is empty. Set USERNAME or switch USE_SAVED_SESSIONS=True.", "Config Error") return None if PASSWORD is None: PASSWORD = _prompt("Password (will not be saved):", title="Quick Connect Password") cmd = f"/SSH2 /L {USERNAME} /PASSWORD {PASSWORD} /P {PORT} {host}" try: tab = crt.Session.ConnectInTab(cmd) tab.Activate() if not wait_for_prompt(tab): _msg(f"Timeout waiting for prompt on {host}", "Timeout") return tab except Exception as e: _msg(f"Failed quick-connect to '{host}': {e}", "Connect Error") return None

def choose_targets(): # Show available groups and allow comma-separated selection groups_list = ", ".join(sorted(SERVER_GROUPS.keys())) answer = _prompt( f"Enter groups to target (comma-separated). Available: {groups_list}\n" 
"Or enter a single hostname to target just that host.", title="Choose Targets", default=",".join(sorted(SERVER_GROUPS.keys())) ) answer = (answer or "").strip() hosts = [] if not answer: return hosts parts = [p.strip() for p in answer.split(',') if p.strip()] for p in parts: if p in SERVER_GROUPS: hosts.extend(SERVER_GROUPS[p]) else: # treat as a literal hostname/session name hosts.append(p) # de-dup preserving order seen = set() unique_hosts = [] for h in hosts: if h not in seen: seen.add(h) unique_hosts.append(h) return unique_hosts, parts  # also return raw selectors

def main(): ts = datetime.now().strftime("%Y-%m-%d_%H-%M-%S") crt.Screen.Synchronous = True

targets, selectors = choose_targets()
if not targets:
    _msg("No targets chosen. Exiting.")
    return

dry = _confirm("Dry-run first? (Shows what would run, opens no sessions)")
if dry:
    lines = ["DRY-RUN plan:\n"]
    lines.append("Targets: " + ", ".join(targets))
    lines.append("\nCommands for ALL:")
    for c in COMMANDS.get("commands_for_all", []):
        if c.strip().startswith("sudo"): c = f"[SKIP sudo] {c}"
        lines.append("  " + c)
    lines.append("\nPer-group additions:")
    for sel in selectors:
        if sel in COMMANDS.get("commands_per_group", {}):
            for c in COMMANDS["commands_per_group"][sel]:
                if c.strip().startswith("sudo"): c = f"[SKIP sudo] {c}"
                lines.append(f"  ({sel}) {c}")
    lines.append("\nPer-host additions:")
    for h in targets:
        for c in COMMANDS.get("commands_per_host", {}).get(h, []):
            if c.strip().startswith("sudo"): c = f"[SKIP sudo] {c}"
            lines.append(f"  ({h}) {c}")
    lines.append("\nOne-off commands host: " + ONE_OFF_HOST)
    for c in COMMANDS.get("one_off_commands", []):
        if c.strip().startswith("sudo"): c = f"[SKIP sudo] {c}"
        lines.append("  " + c)
    _msg("\n".join(lines), title="Dry Run")
    if not _confirm("Proceed with real run now?"):
        return

# Open a log folder per run (use SecureCRT logging if desired)
# You can also enable per-tab log: Session->Log Session in your session defaults.

# --- Run on all targets ---
opened_tabs = {}
for host in targets:
    tab = connect_to_host(host)
    if not tab:
        continue
    opened_tabs[host] = tab
    if not wait_for_prompt(tab):
        continue
    run_commands(tab, COMMANDS.get("commands_for_all", []))
    # per-group extras (if selected by name)
    for sel in selectors:
        for c in COMMANDS.get("commands_per_group", {}).get(sel, []):
            send_command(tab, c)
    # per-host extras
    for c in COMMANDS.get("commands_per_host", {}).get(host, []):
        send_command(tab, c)

# --- Run once in ONE area (single host) ---
if ONE_OFF_HOST:
    if ONE_OFF_HOST in opened_tabs:
        tab = opened_tabs[ONE_OFF_HOST]
    else:
        tab = connect_to_host(ONE_OFF_HOST)
    if tab and wait_for_prompt(tab):
        run_commands(tab, COMMANDS.get("one_off_commands", []))

_msg("Done.")

if name == "main": try: main() except Exception as e: _msg(f"Unexpected error: {e}", "Error")

