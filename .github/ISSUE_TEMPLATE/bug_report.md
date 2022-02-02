---
name: Bug report
about: Create a report to help us improve
title: ''
labels: ''
assignees: ''

---

**Describe the bug**

Provide a clear and concise description of what the bug is.

**To Reproduce**

Provide a *minimal working example* that can be used to reproduce the bug, that is, *complete* but *excluding unnecessary options*, *large inputs* and *excessive computation times*.  The example should include the program commands and other necessary input, and the terminal output.

*Formatting is important*.  An example should be written as *plain text* (no screenshot, etc.) and provided between triple backticks (Markdown formatting).  Input or output text files that are too large to be provided in-text can be attached (GitHub requires a .txt extension).  Images can be used if necessary.

To preview your message, use "Preview".  To edit your message after it is published, use "Edit".

Here is an example of properly formatted example:

```
$ neper -T -n 10

========================    N   e   p   e   r    =======================
Info   : A software package for polycrystal generation and meshing.
Info   : Version 4.2.0
Info   : Built with: gsl|muparser|opengjk|openmp|nlopt|libscotch (full)
Info   : Running on 8 threads.
Info   : <https://neper.info>
Info   : Copyright (C) 2003-2021, and GNU GPL'd, by Romain Quey.
Info   : Loading initialization file `/home/rquey/.neperrc'...
Info   : ---------------------------------------------------------------
Info   : MODULE  -T loaded with arguments:
Info   : [ini file] -tesrformat ascii
Info   : [com line] -n 10
Info   : ---------------------------------------------------------------
Info   : Reading input data...
Info   : Creating domain...
Info   : Creating tessellation...
Info   :   - Setting seeds... 
Info   :   - Generating crystal orientations...
Info   :   - Running tessellation...
Info   : Writing results...
Info   :     [o] Writing file `n10-id1.tess'...
Info   :     [o] Wrote file `n10-id1.tess'.
Info   : Elapsed time: 0.012 secs.
========================================================================
```
