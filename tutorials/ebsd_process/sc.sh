#!/bin/bash

neper -T -n 2 -for tesr -domain "square(0.004,0.003)" -tesrsize 4:3 -dim 2 -orispread "normal(2)" -o n2

# remove cell section
beg=`grep -n "^ \*\*" n2.tesr | grep cell | awk -F: '{print $1}'`
end=`grep -n "^ \*\*" n2.tesr | grep -A 1 cell | tail -1 | awk -F: '{print $1-1}'`
sed -i "${beg},${end}d" n2.tesr

# remove data section
beg=`grep -n "^ \*\*" n2.tesr | grep "\<data\>" | awk -F: '{print $1}'`
end=`grep -n "^ \*\*" n2.tesr | grep -A 1 "\<data\>" | tail -1 | awk -F: '{print $1-1}'`
sed -i "${beg},${end}d" n2.tesr

# n2b
sed -e '$ d' n2.tesr               > n2b.tesr
echo " **oridef"                   >> n2b.tesr
echo "   ascii"                   >> n2b.tesr
echo "   1 1 1 1 1 1 1 1 0 1 1 1" >> n2b.tesr
echo "***end"                     >> n2b.tesr
sed -i "19s/.*/   0.000000000000    0.000000000000    0.000000000000/g" n2b.tesr

# n2c

neper -T -n 2 -for tesr -domain "square(0.004,0.003)" -tesrsize 4:3 -dim 2 -orispread "normal(2)" -o n2c

sed -i '$ d' n2c.tesr
echo " **oridef"                  >> n2c.tesr
echo "   ascii"                   >> n2c.tesr
echo "   1 1 1 1 1 1 1 1 0 1 1 1" >> n2c.tesr
echo "***end"                     >> n2c.tesr

# remove ori and orispread sections
beg=`grep -n "^  \*" n2c.tesr | grep "ori\>" | awk -F: '{print $1}'`
end=`grep -n "^ .\*" n2c.tesr | grep -A 1 "orispread\>" | tail -1 | awk -F: '{print $1-1}'`
sed -i "${beg},${end}d" n2c.tesr

sed -i "28s/.*/   0.000000000000    0.000000000000    0.000000000000/g" n2c.tesr

exit 0
