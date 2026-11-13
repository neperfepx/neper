# Sphinx extension: write, next to each simulation directory <name>.sim of the tutorials, its
# archive <name>.sim.tgz, offered for download by the tutorials (:download:).  The archives are
# not stored in the repository; they are rewritten when older than their directory.

import os
import tarfile


def _newest_mtime(path):
    mtime = os.path.getmtime(path)
    for root, dirs, files in os.walk(path):
        for name in dirs + files:
            mtime = max(mtime, os.path.getmtime(os.path.join(root, name)))
    return mtime


def write_sim_archives(app):
    tutorials = os.path.join(app.srcdir, 'tutorials')
    if not os.path.isdir(tutorials):
        tutorials = os.path.join(app.srcdir, 'doc', 'tutorials')
    for root, dirs, files in os.walk(tutorials):
        for name in dirs:
            if not name.endswith('.sim'):
                continue
            sim = os.path.join(root, name)
            archive = sim + '.tgz'
            if os.path.exists(archive) and os.path.getmtime(archive) >= _newest_mtime(sim):
                continue
            with tarfile.open(archive, 'w:gz') as tar:
                tar.add(sim, arcname=name)
        dirs[:] = [d for d in dirs if not d.endswith('.sim')]


def setup(app):
    app.connect('builder-inited', write_sim_archives)
    return {'parallel_read_safe': True, 'parallel_write_safe': True}
