#!/bin/bash

nb=`grep \\* applications.rst | grep "^-" | awk -F '\\*' '{print $2}' | wc -l | awk '{print $1}'`

grep \\* applications.rst                       \
  | tr '[:upper:]' '[:lower:]'                  \
  | sed -e "s/modell/model/g"                   \
  | sed -e "s/,//g"                             \
  | sed -e "s/://g"                             \
  | grep "^-"                                   \
  | sed -e "s/finite.element/finite-element/g"  \
  | sed -e "s/polycrystalline/polycrystal/g"    \
  | sed -e "s/polycrystals/polycrystal/g"       \
  | awk -F '\\*' '{print $2}' | tr ' ' '\n'     \
  | grep -v "\<of\>"                            \
  | grep -v "\<and\>"                           \
  | grep -v "\<in\>"                            \
  | grep -v "\<the\>"                           \
  | grep -v "\<a\>"                             \
  | grep -v "\<on\>"                            \
  | grep -v "\<for\>"                           \
  | grep -v "\<with\>"                          \
  | grep -v "\<to\>"                            \
  | grep -v "\<an\>"                            \
  | grep -v "\<by\>"                            \
  | grep -v "\<under\>"                         \
  | grep -v "\<at\>"                            \
  | grep -v "\<during\>"                        \
  | grep -v "\<from\>"                          \
  | grep -v "\<using\>"                         \
  | grep -v "\<to\>"                            \
  | grep -v "\<via\>"                           \
  | grep -v "\<its\>"                           \
  | sort # | uniq -c | sort -n
# awk 'FNR==NR{total+=$1;next} {id+=1; sum+=$1; printf "%2d %2d %2d %5.1f%% %5.1f%% ", id, $1, sum, $1/total*100, sum/total*100}
     # {for (i=2; i<NF; i++) printf $i " "; print $NF}' tmp tmp

# rm tmp

exit 0
