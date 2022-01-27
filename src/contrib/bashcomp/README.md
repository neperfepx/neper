# Neper bash completion support

## Overview

This script is to add bash completion feature to the [Neper software package](https://rquey.github.io/neper). 
It is used to auto complete or suggest arguments, files, or directories by partially typing in neper.

Specifically:
  
  * neper <tab>  would show -T, -M, -S, -V and all -- options;
  * neper -T <tab> would show all available -T options;
  * neper -M <tab> would show all possible inputs (.tess and .msh files), directories and available -M options;
  * neper -S <tab> would show all possible inputs (sim directories), files,  and available -S options;
  * neper -V <tab> would show all possible inputs (.tess and .msh files, sim directories) and available -V options.
  * neper -T -argument<tab> would "autocomplete" to the full arguments, or list suggested full argumments.
    e.g. neper -T -reg<tab> would "autocomplete" to -regularization
  * Where multiple files(or directories) are allowed as input, separate the file/directory names with comma ','
    and auto completion will still work.
