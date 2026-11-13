# Python interface: grain rotations of n20.sim (see doc/tutorials/python.rst),
# one color per grain

import os
import tarfile

import matplotlib.pyplot as plt
import neper

neper.set_defaults(rcfile="none")
plt.rcParams.update({"figure.figsize": (6, 3.6), "savefig.dpi": 100})

if not os.path.isdir("n20.sim"):
    with tarfile.open("../../doc/tutorials/orientation_trajectories/n20.sim.tgz") as tar:
        tar.extractall()

neper.S("n20.sim", resnode="disp")
sim = neper.read("n20.sim")

disp = sim.read("nodes", "disp")
top = sim.mesh().nsets["z1"] - 1
strain = disp[:, top, 2].mean(axis=1)

ori = sim.read("elsets", "ori")
rotation = neper.ori_angle(ori, ori[0], sim.orides)

colors = plt.get_cmap("tab20").colors

fig, ax = plt.subplots()
for i in range(rotation.shape[1]):
    ax.plot(100 * strain, rotation[:, i], color=colors[i % len(colors)], lw=1.2)
ax.plot(100 * strain, rotation.mean(axis=1), color="black", lw=2.5, label="average")
ax.set_xlabel("strain (%)")
ax.set_ylabel("grain rotation (°)")
ax.legend()
fig.savefig("python.png", bbox_inches="tight")
