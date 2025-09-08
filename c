# $language = "python"
# $interface = "1.0"
# SecureCRT: Run the same commands in all connected tabs,
# auto-send Ctrl+C after each, and log outputs.

import os, time, datetime

# ======== CONFIGURATION ========
# Option A: Put your commands right here:
COMMANDS = [
    # examples:
    # 'curl -vv --connect-timeout 3 --max-time 5 telnet://host1:1234',
    # 'curl -vv --connect-timeout 3 --max-time 5 telnet://host2:5678',
]

# Option B (optional): load one command per line from a file.
# If non-empty and the file exists, it overrides COMMANDS above.
COMMANDS_FILE = ""  # e.g., r"C:\temp\my_commands.txt" or "/home/you/my_commands.txt"

# How long to let each command run before we "Ctrl+C" it (seconds)
PER_COMMAND_RUN_SECONDS = 5

# How long to wait after Ctrl+C to let output flush (seconds)
POST_INTERRUPT_WAIT_SECONDS = 1.0

# If True, only run on tabs that are connected (recommended)
ONLY_CONNECTED_TABS = True

# Where to store the log (default: your home folder)
LOG_DIR = os.path.expanduser("~")
LOG_BASENAME = "SecureCRT_multi_cmds_{ts}.log"
# ======== END CONFIG ============
def _timestamp():
    return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

def _load_commands_from_file(path):
    cmds = []
    try:
        with open(path, "r") as f:
            for line in f:
                s = line.strip()
                if s and not s.startswith("#"):
                    cmds.append(s)
    except Exception as e:
        crt.Dialog.MessageBox("Failed to read commands file:\n{}\n\n{}".format(path, e))
    return cmds

def _get_log_path():
    ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    name = LOG_BASENAME.format(ts=ts)
    return os.path.join(LOG_DIR, name)

def _log_header(fh, tab, cmd):
    fh.write("\n" + "="*78 + "\n")
    fh.write("[{}] TAB {}  CMD: {}\n".format(_timestamp(), tab.Index, cmd))
    fh.write("="*78 + "\n")
    fh.flush()

def _capture_screen(tab):
    """
    Grab what's currently visible on the screen buffer.
    (Not perfect, but good for most use-cases.)
    """
    try:
        # Save and restore cursor to avoid moving the user's view.
        tab.Screen.IgnoreEscape = True
        r, c = tab.Screen.CurrentRow, tab.Screen.CurrentColumn
        rows = tab.Screen.Rows
        cols = tab.Screen.Columns
        text = tab.Screen.Get(1, 1, rows, cols)
        tab.Screen.GotoXY(c, r)
        return text
    except:
        return ""

def run_on_tab(tab, commands, fh):
    if ONLY_CONNECTED_TABS and not tab.Session.Connected:
        return

    # Bring this tab to the front so Send() goes here
    crt.Session.SetActiveTab(tab.Index)

    for cmd in commands:
        _log_header(fh, tab, cmd)
        try:
            tab.Screen.Send(cmd + "\r")
        except Exception as e:
            fh.write("[{}] ERROR sending command: {}\n".format(_timestamp(), e))
            fh.flush()
            continue

        # Let the command run for PER_COMMAND_RUN_SECONDS
        end_time = time.time() + PER_COMMAND_RUN_SECONDS
        while time.time() < end_time:
            crt.Sleep(200)  # 0.2s increments to stay responsive

        # Send Ctrl+C to interrupt (like you do manually)
        try:
            tab.Screen.Send("\x03")  # ASCII ETX == Ctrl+C
        except Exception as e:
            fh.write("[{}] ERROR sending Ctrl+C: {}\n".format(_timestamp(), e))

        crt.Sleep(int(POST_INTERRUPT_WAIT_SECONDS * 1000))

        # Capture screen content and append to log
        out = _capture_screen(tab)
        if out:
            fh.write(out)
            if not out.endswith("\n"):
                fh.write("\n")
        fh.flush()

def main():
    # Get commands (file overrides inline list if provided)
    commands = COMMANDS[:]
    if COMMANDS_FILE and os.path.isfile(COMMANDS_FILE):
        loaded = _load_commands_from_file(COMMANDS_FILE)
        if loaded:
            commands = loaded

    if not commands:
        crt.Dialog.MessageBox("No commands found.\n\nAdd to COMMANDS or set COMMANDS_FILE.")
        return

    log_path = _get_log_path()
    try:
        fh = open(log_path, "w")
    except Exception as e:
        crt.Dialog.MessageBox("Cannot open log file:\n{}\n\n{}".format(log_path, e))
        return

    fh.write("[{}] Starting multi-tab run. Log: {}\n".format(_timestamp(), log_path))
    fh.flush()

    tabs = list(crt.GetTabs())
    ran_any = False
    for tab in tabs:
        if ONLY_CONNECTED_TABS and not tab.Session.Connected:
            continue
        ran_any = True
        run_on_tab(tab, commands, fh)

    fh.write("[{}] Done.\n".format(_timestamp()))
    fh.close()

    if not ran_any:
        crt.Dialog.MessageBox("No connected tabs to run on (check ONLY_CONNECTED_TABS).")
    else:
        crt.Dialog.MessageBox("Finished. Log saved to:\n{}".format(log_path))

main()