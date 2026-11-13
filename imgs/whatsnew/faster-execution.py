# Execution times of 5.0.0 and the current version, relative to 5.0.0
# (linear scale), for -T -n 1000 -morpho gg, -M and -V, on 12 threads
steps = ["Tessellation (-T)", "Meshing (-M)", "Visualization (-V)"]
old = [190, 81, 4.8]; new = [70, 2.3, 3.8]; fac = ["3×", "35×", "1.3×"]
# time labels, rounded (the times depend on the computer)
lab_old = ["3 min", "80 s", "5 s"]; lab_new = ["1 min", "2 s", "4 s"]
ink, ink2, grid = "#0b0b0b", "#52514e", "#e1e0d9"
cold, cnew = "#eb6834", "#2a78d6"
W, H = 900, 400; x0, x1 = 200, 640  # plot area for 0..100%
def X(f): return x0 + (x1 - x0) * f
o = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" font-family="DejaVu Sans, sans-serif">',
     f'<rect width="{W}" height="{H}" fill="white"/>',
     f'<text x="{x0}" y="30" font-size="16" fill="{ink2}">Time relative to 5.0.0, 1000 grain-growth cells, 12 threads</text>']
top, bot = 50, 330
for f in [0, 0.25, 0.5, 0.75, 1]:
    o.append(f'<line x1="{X(f):.1f}" y1="{top}" x2="{X(f):.1f}" y2="{bot}" stroke="{grid}" stroke-width="1"/>')
    o.append(f'<text x="{X(f):.1f}" y="{bot+22}" font-size="14" fill="{ink2}" text-anchor="middle">{int(f*100)}%</text>')
bh, gap = 30, 2
for i, (s, a, b, fc, la, lb) in enumerate(zip(steps, old, new, fac, lab_old, lab_new)):
    yc = top + 45 + i * 92
    for yy, v, c, lab in [(yc - bh - gap/2, a, cold, la), (yc + gap/2, b, cnew, lb)]:
        w = max(X(v / a) - x0, 6)
        o.append(f'<path d="M{x0},{yy} h{w-4:.1f} a4,4 0 0 1 4,4 v{bh-8} a4,4 0 0 1 -4,4 h{-(w-4):.1f} z" fill="{c}"/>')
        o.append(f'<text x="{x0+w+8:.1f}" y="{yy+bh/2+5}" font-size="14" fill="{ink2}">{lab}</text>')
    o.append(f'<text x="{x0-12}" y="{yc+5}" font-size="16" fill="{ink}" text-anchor="end">{s}</text>')
    o.append(f'<text x="{W-20}" y="{yc+6}" font-size="18" font-weight="bold" fill="{ink}" text-anchor="end">{fc} faster</text>')
ly = H - 18
for k, (lab, c) in enumerate([("5.0.0", cold), ("Now", cnew)]):
    lx = x0 + k * 110
    o.append(f'<rect x="{lx}" y="{ly-12}" width="14" height="14" rx="3" fill="{c}"/>')
    o.append(f'<text x="{lx+22}" y="{ly}" font-size="15" fill="{ink}">{lab}</text>')
o.append('</svg>')
open("faster-execution.svg", "w").write("\n".join(o))
