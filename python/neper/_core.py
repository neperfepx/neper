# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

"""Running the neper program from Python."""

import codecs
import os
import re
import shlex
import shutil
import signal
import subprocess
import sys
import time
from pathlib import Path

from . import _io

__all__ = [
    "NeperError",
    "Result",
    "T",
    "M",
    "S",
    "V",
    "run",
    "module",
    "set_defaults",
    "get_defaults",
    "executable",
    "version",
    "data_dir",
]

_DEFAULTS = {
    "executable": None,  # neper executable (None: $NEPER_EXECUTABLE, then PATH)
    "rcfile": None,  # initialization file (None: Neper's default, "none": no file)
    "threads": None,  # OpenMP threads (None: $OMP_NUM_THREADS or all threads)
    "verbose": False,  # print Neper's output while it runs
    "check": True,  # raise NeperError if Neper fails
}


class NeperError(RuntimeError):
    """Neper failed (error message, unknown option, missing file, etc.).

    Attributes: ``message`` (Neper's error message, without the ``Error :``
    header), ``returncode``, ``cmd`` (the command line) and ``output``
    (Neper's whole output)."""

    def __init__(self, message, returncode=None, cmd=None, output=""):
        super().__init__(message)
        self.message = message
        self.returncode = returncode
        self.cmd = cmd
        self.output = output

    def __str__(self):
        text = self.message
        if self.cmd:
            text += "\n(command: " + shlex.join(self.cmd) + ")"
        return text


class Result:
    """Result of a Neper run.

    Attributes:

    - ``cmd``: command line (list of strings);
    - ``cwd``: working directory (:class:`pathlib.Path`);
    - ``returncode``: exit status of Neper;
    - ``output``: Neper's output (standard output and standard error);
    - ``files``: files written by Neper, in the order they were written
      (list of :class:`pathlib.Path`, absolute);
    - ``elapsed``: elapsed time, in seconds.
    """

    def __init__(self, cmd, cwd, returncode, output, elapsed):
        self.cmd = cmd
        self.cwd = Path(cwd)
        self.returncode = returncode
        self.output = output
        self.elapsed = elapsed
        self.files = [self.cwd / f for f in _written_files(output)]

    @property
    def ok(self):
        """True if Neper succeeded."""
        return self.returncode == 0

    def file(self, ext=None):
        """Return the (first) written file of extension *ext* (e.g. ``"tess"``
        or ``".msh"``), or the first written file if *ext* is None (which is
        the main output, e.g. the image of module -V, before the scale
        image)."""
        files = self.files
        if ext is not None:
            ext = "." + ext.lstrip(".")
            files = [f for f in files if f.name.endswith(ext)]
        if not files:
            raise LookupError(f"no {ext} file written" if ext else "no file written")
        return files[0]

    def read(self, ext=None):
        """Read the (first) written file of extension *ext* (or the first written
        file), using :func:`neper.read` (e.g. ``T(n=10).read("tess")`` returns
        a :class:`neper.Tess`)."""
        return _io.read(self.file(ext))

    def stat(self, entity):
        """Return the statistics of *entity* (e.g. ``"cell"`` for option
        ``-statcell``, written to a .stcell file), as a dictionary key →
        values, as in ``T(n=10, statcell="vol,x").stat("cell")["vol"]``.
        The keys are those of the option, and each must give a single value
        (otherwise, read the file with :func:`neper.read_stat`)."""
        keys = None
        for i, arg in enumerate(self.cmd[:-1]):
            if arg == "-stat" + entity:
                keys = self.cmd[i + 1]
        if keys is None:
            raise LookupError(f"option -stat{entity} not found in the command line")
        return _io.read_stat(self.file("st" + entity), keys)

    def _repr_png_(self):
        # display of the (first) PNG image in Jupyter
        pngs = [f for f in self.files if f.suffix == ".png" and f.is_file()]
        return pngs[0].read_bytes() if pngs else None

    def __repr__(self):
        files = ", ".join(f.name for f in self.files)
        return (f"<neper.Result {shlex.join(self.cmd[1:])!r}: "
                f"returncode={self.returncode}, files=[{files}]>")


def set_defaults(**kwargs):
    """Set the default values of the run parameters, among ``executable``,
    ``rcfile``, ``threads``, ``verbose`` and ``check`` (see :func:`run`)."""
    for key, val in kwargs.items():
        if key not in _DEFAULTS:
            raise TypeError(f"unknown parameter `{key}' (expected one of "
                            + ", ".join(_DEFAULTS) + ")")
        _DEFAULTS[key] = val


def get_defaults():
    """Return the default values of the run parameters (a copy)."""
    return dict(_DEFAULTS)


def executable():
    """Return the path of the neper executable.

    It is, successively, the value set with ``set_defaults(executable=...)``,
    environment variable ``NEPER_EXECUTABLE``, or ``neper`` in the PATH."""
    return _find_executable()


def _find_executable():
    exe = _DEFAULTS["executable"] or os.environ.get("NEPER_EXECUTABLE")
    if exe:
        exe = os.path.expanduser(str(exe))
        if os.path.isdir(exe):
            exe = os.path.join(exe, "neper")
        if not (os.path.isfile(exe) and os.access(exe, os.X_OK)):
            raise FileNotFoundError(f"neper executable `{exe}' not found")
        return os.path.abspath(exe)
    exe = shutil.which("neper")
    if exe is None:
        raise FileNotFoundError(
            "neper executable not found: install Neper (https://neper.info) "
            "or set NEPER_EXECUTABLE or neper.set_defaults(executable=...)")
    return exe


def version():
    """Return the version of Neper (string)."""
    return _simple("--version")


def data_dir():
    """Return Neper's data directory (string)."""
    return _simple("--data")


def _simple(arg):
    exe = _find_executable()
    proc = subprocess.run([exe, arg], capture_output=True, text=True)
    if proc.returncode:
        raise NeperError((proc.stderr or proc.stdout).strip(), proc.returncode,
                         [exe, arg], proc.stdout + proc.stderr)
    return proc.stdout.strip()


def T(*args, **options):
    """Run module -T (tessellation), as in ``T(n=100, id=1, o="poly")``.

    See :func:`module` for the syntax and :func:`run` for the run parameters
    (``cwd``, ``threads``, etc.)."""
    return module("-T", *args, **options)


def M(*args, **options):
    """Run module -M (meshing), as in ``M("poly.tess", elttype="hex")``.

    See :func:`module` for the syntax and :func:`run` for the run parameters
    (``cwd``, ``threads``, etc.)."""
    return module("-M", *args, **options)


def S(*args, **options):
    """Run module -S (simulation results), as in ``S("sim", resnode="disp")``.

    See :func:`module` for the syntax and :func:`run` for the run parameters
    (``cwd``, ``threads``, etc.)."""
    return module("-S", *args, **options)


def V(*args, **options):
    """Run module -V (visualization), as in
    ``V("poly.tess", datacellcol="id", print="img")``.

    See :func:`module` for the syntax and :func:`run` for the run parameters
    (``cwd``, ``threads``, etc.)."""
    return module("-V", *args, **options)


_RUN_PARAMS = ("cwd", "threads", "rcfile", "verbose", "check", "env", "executable")


def module(name, /, *args, **options):
    """Run Neper module *name* (``"-T"``, ``"-M"``, ``"-S"`` or ``"-V"``).

    The arguments are passed to Neper in order, positional arguments first:

    - a string (or path) is passed as is: an input file or directory, or a
      raw argument (e.g. ``"-loop"``);
    - a list or tuple is passed as a comma-separated argument (e.g.
      ``["n10.tess", "n10.msh"]`` gives ``n10.tess,n10.msh``), and a 2D array
      (or list of lists) is written to a data file, passed as
      ``file(<path>)`` (see :func:`neper.datafile`);
    - a dictionary is passed as options, in order (which allows to repeat
      an option, as needed by module -V).

    Keyword arguments are passed as options: ``name=value`` gives
    ``-name value``.  A trailing underscore is removed from the name (to
    allow Python keywords, e.g. ``in_``).  A value of None omits the option,
    True and False give ``1`` and ``0``, a list or tuple gives a
    comma-separated argument (e.g. ``format=["tess", "vtk"]`` gives
    ``-format tess,vtk``), and a 2D array (or list of lists) gives
    ``file(<path>)`` (e.g. ``ori=oris``, with ``oris`` of shape ``(n, 3)``).

    Keyword arguments ``cwd``, ``threads``, ``rcfile``, ``verbose``,
    ``check``, ``env`` and ``executable`` are run parameters, not options
    (see :func:`run`).  Return a :class:`Result`."""
    params = {k: options.pop(k) for k in _RUN_PARAMS if k in options}
    argv = [name]
    for arg in args:
        if isinstance(arg, dict):
            argv += _options(arg)
        else:
            argv.append(_value(arg))
    argv += _options(options)
    return run(argv, **params)


def run(args, cwd=None, threads=None, rcfile=None, verbose=None, check=None,
        env=None, executable=None):
    """Run neper with command-line arguments *args* (list of strings, or
    string split as by a shell), as in ``run("-T -n 10 -id 1")``.

    Run parameters (None means the default, see :func:`set_defaults`):

    - ``cwd``: working directory (default: the current directory);
    - ``threads``: number of OpenMP threads (default: ``$OMP_NUM_THREADS``,
      or all threads);
    - ``rcfile``: initialization file (default: :file:`$HOME/.neperrc`);
      ``"none"`` disables it;
    - ``verbose``: print Neper's output while it runs (default: False);
    - ``check``: raise :class:`NeperError` if Neper fails (default: True);
    - ``env``: environment variables to add to (or, if a value is None, remove
      from) the environment of Neper;
    - ``executable``: neper executable (default: see :func:`executable`).

    Return a :class:`Result`.  Neper's error messages (written before it
    aborts) are reported as :class:`NeperError` exceptions; interrupting
    Python (Ctrl+C) interrupts Neper."""
    if isinstance(args, str):
        args = shlex.split(args)
    args = [_value(a) for a in args]
    if args and args[0] == "neper":
        args = args[1:]
    threads = _DEFAULTS["threads"] if threads is None else threads
    rcfile = _DEFAULTS["rcfile"] if rcfile is None else rcfile
    verbose = _DEFAULTS["verbose"] if verbose is None else verbose
    check = _DEFAULTS["check"] if check is None else check

    if executable is None:
        exe = _find_executable()
    else:
        exe = os.path.abspath(os.path.expanduser(str(executable)))

    cmd = [exe]
    if rcfile is not None and not (args and args[0] == "--rcfile"):
        cmd += ["--rcfile", _value(rcfile)]
    cmd += args

    cwd = os.path.abspath(os.path.expanduser(str(cwd))) if cwd else os.getcwd()

    penv = dict(os.environ)
    if threads is not None:
        penv["OMP_NUM_THREADS"] = str(int(threads))
    for key, val in (env or {}).items():
        if val is None:
            penv.pop(key, None)
        else:
            penv[key] = str(val)

    beg = time.monotonic()
    returncode, output = _execute(cmd, cwd, penv, verbose)
    result = Result(cmd, cwd, returncode, output, time.monotonic() - beg)

    if check and returncode != 0:
        raise NeperError(_error_message(output, returncode), returncode, cmd,
                         output)
    return result


def _options(options):
    argv = []
    for key, val in options.items():
        if val is None:
            continue
        key = str(key)
        if key.endswith("_"):
            key = key[:-1]
        argv += ["-" + key.lstrip("-"), _value(val)]
    return argv


def _value(val):
    if isinstance(val, _io.DataFile):
        return str(val)  # file(<path>)
    if hasattr(val, "tolist") and not isinstance(val, (int, float, str)):
        val = val.tolist()  # NumPy scalar or array
    if isinstance(val, bool):
        return "1" if val else "0"
    if isinstance(val, (list, tuple)):
        if any(isinstance(v, (list, tuple)) or getattr(v, "ndim", 0) for v in val):
            return str(_io.datafile(val))  # 2D: data file
        return ",".join(_value(v) for v in val)
    if isinstance(val, os.PathLike):
        return os.fspath(val)
    if isinstance(val, str):
        return val
    return str(val)


def _preexec():
    # own process group, so that Ctrl+C reaches Neper only through Python,
    # which forwards it once (see _execute)
    os.setpgid(0, 0)
    # Neper aborts on error, which would otherwise write a core file
    try:
        import resource

        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    except (ImportError, ValueError, OSError):
        pass


def _execute(cmd, cwd, env, verbose):
    proc = subprocess.Popen(cmd, cwd=cwd, env=env, stdin=subprocess.DEVNULL,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            preexec_fn=_preexec)
    chunks = []
    decoder = codecs.getincrementaldecoder("utf-8")(errors="replace")
    interrupts = 0
    while True:
        try:
            data = os.read(proc.stdout.fileno(), 65536)
            if not data:
                break
            chunks.append(data)
            if verbose:
                sys.stdout.write(decoder.decode(data))
                sys.stdout.flush()
        except KeyboardInterrupt:
            # first Ctrl+C: interrupt Neper (which may then finish its current
            # step, e.g. a -morpho optimization, and continue), second: kill
            interrupts += 1
            if interrupts == 1:
                proc.send_signal(signal.SIGINT)
            else:
                proc.kill()
                proc.wait()
                proc.stdout.close()
                raise
    proc.stdout.close()
    proc.wait()
    # Neper may have handled the interruption and completed (as with
    # -morpho, whose optimization stops on Ctrl+C)
    if interrupts and proc.returncode != 0:
        raise KeyboardInterrupt
    return proc.returncode, b"".join(chunks).decode("utf-8", errors="replace")


_WROTE = re.compile(r"\[o\] Wrote file `(.*?)'")


def _written_files(output):
    files = []
    for name in _WROTE.findall(output):
        if name not in files:
            files.append(name)
    return files


def _error_message(output, returncode, program="neper"):
    lines = output.splitlines()
    messages = []
    for i, line in enumerate(lines):
        if line.startswith("Error  : "):
            message = [line[9:].strip()]
            for cont in lines[i + 1:]:
                if cont.startswith("         ") and cont.strip():
                    message.append(cont.strip())
                else:
                    break
            messages.append(re.sub(r"^-\s+", "", " ".join(message)))
    if messages:
        return " ".join(messages)
    if returncode < 0:
        try:
            name = signal.Signals(-returncode).name
        except ValueError:
            name = f"signal {-returncode}"
        text = f"{program} terminated by {name}"
        if returncode == -signal.SIGABRT and program == "neper":
            # Neper versions before the fix of 2026-11 lose the error messages
            # printed just before aborting when the output is redirected
            text += (" (Neper error, whose message may have been lost: run the "
                     "command in a terminal to see it)")
        return text
    tail = [l.strip() for l in lines if l.strip() and not l.startswith("====")]
    return tail[-1] if tail else f"{program} exited with status {returncode}"
