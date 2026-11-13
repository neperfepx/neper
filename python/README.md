# Neper Python interface

The `neper` Python package runs [Neper](https://neper.info)'s modules from
Python.  Each module is a function, whose arguments are the module's
command-line options:

```python
import neper

neper.T(n=100, id=1, o="poly")              # neper -T -n 100 -id 1 -o poly
neper.M("poly.tess", elttype="hex")         # neper -M poly.tess -elttype hex
neper.V("poly.tess", datacellcol="id", print="img")
```

Neper's files are read into NumPy arrays, and arrays are passed to Neper
as data files:

```python
tess = neper.T(n=100, id=1).read("tess")      # neper.Tess
tess.seed_coo                                 # (100, 3) array
mesh = neper.M("poly.tess").read()            # neper.Mesh
mesh.node_coo, mesh.elts().elt_nodes

neper.T(n=100, ori=oris)                      # oris: (100, 3) array

sim = neper.read("n100.sim")                  # neper.Sim
disp = sim.read("nodes", "disp")              # (steps, nodes, 3) array
sim.write("elsets", "myres", values)          # usable by -S and -V
```

The package runs the `neper` program, which must be installed separately
(see the [installation instructions](https://neper.info/docs)); it is
looked for in `NEPER_EXECUTABLE`, then in the `PATH`.

## Installation

From the Neper source tree:

```console
$ pip install ./python
```

## Documentation

See the "Python Interface" section of the Neper documentation
(`doc/python.rst`) and the docstrings (`help(neper)`).

## Testing

```console
$ NEPER_EXECUTABLE=src/build/neper python -m unittest discover -s python/tests
```
