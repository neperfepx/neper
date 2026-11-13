# Terminal image of an orilib session (interactive, then scripted), from
# actual runs of the orilib binary given as argument
# Usage: python3 orilib-terminal.py <orilib> [<version>]  ->  orilib-terminal.svg
# (<version> replaces the version of the binary in the header)

import os, pty, re, select, subprocess, sys
from html import escape

orilib = sys.argv[1]
version = sys.argv[2] if len (sys.argv) > 2 else None
inputs = ["ol_e_q", "0 60 0", "ol_q_q_disori", "0.8 0.6 0 0", "1 0 0 0", "cubic", "quit"]

def read (fd, out):
    while select.select ([fd], [], [], 0.3)[0]:
        try:
            out += os.read (fd, 4096)
        except OSError:
            break
    return out

# interactive session, through a pseudo-terminal so that inputs are echoed
pid, fd = pty.fork ()
if pid == 0:
    os.execv (orilib, ["orilib"])
out = read (fd, b"")
for l in inputs:
    os.write (fd, (l + "\n").encode ())
    out = read (fd, out)
os.waitpid (pid, 0)
session = out.decode ().replace ("\r", "").strip ("\n").split ("\n")
if version:
    session = [re.sub (r"Version .*", "Version " + version, l) for l in session]

# scripted run
script = subprocess.run ([orilib, "ol_e_q"], input="0 30 0\n", capture_output=True,
                         text=True).stdout.rstrip ("\n").split ("\n")

# lines: (prompt, text, kind), kind = cmd (typed), out (printed)
lines = [("$ ", "orilib", "cmd")]
for l in session:
    lines.append (("", l, "cmd" if l.strip () in inputs else "out"))
lines += [("$ ", "echo 0 30 0 | orilib ol_e_q", "cmd")] + [("", l, "out") for l in script]

bg, bar, fg, typed, prompt = "#1e1f22", "#2b2d31", "#c9ccd1", "#ffffff", "#7ec27e"
fs, lh, padx, pady, barh = 15, 20, 18, 14, 30
W = 760
H = barh + 2 * pady + lh * len (lines)
o = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" '
     f'font-family="DejaVu Sans Mono, monospace" font-size="{fs}">',
     f'<rect width="{W}" height="{H}" rx="10" fill="{bg}"/>',
     f'<path d="M0,10 a10,10 0 0 1 10,-10 h{W-20} a10,10 0 0 1 10,10 v{barh-10} h-{W} z" fill="{bar}"/>']
for i, c in enumerate (["#ff5f57", "#febc2e", "#28c840"]):
    o.append (f'<circle cx="{20 + 20 * i}" cy="{barh / 2}" r="6" fill="{c}"/>')
y = barh + pady + fs
for p, t, k in lines:
    x = padx
    if p:
        o.append (f'<text x="{x}" y="{y}" fill="{prompt}" xml:space="preserve">{escape (p)}</text>')
        x += len (p) * fs * 0.602
    col, wt = (typed, "bold") if k == "cmd" else (fg, "normal")
    o.append (f'<text x="{x:.1f}" y="{y}" fill="{col}" font-weight="{wt}" '
              f'xml:space="preserve">{escape (t)}</text>')
    y += lh
o.append ('</svg>')
open ("orilib-terminal.svg", "w").write ("\n".join (o))
