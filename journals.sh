#!/bin/bash

nb=`grep \\*\\* applications.rst | grep "^-" | awk -F '\\*\\*' '{print $2}' | wc -l | awk '{print $1}'`

grep \\*\\* applications.rst                    \
  | grep "^-"                                   \
  | awk -F '\\*\\*' '{print $2}'                \
  | sort | uniq -c | sort -rnk1 > tmp
awk 'FNR==NR{total+=$1;next} {id+=1; sum+=$1; printf "%2d %2d %2d %5.1f%% %5.1f%% ", id, $1, sum, $1/total*100, sum/total*100}
     {for (i=2; i<NF; i++) printf $i " "; print $NF}' tmp tmp

rm tmp

exit 0
