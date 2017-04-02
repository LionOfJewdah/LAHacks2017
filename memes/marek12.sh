CUBE=`echo $1 | sed "s/O/P/g"`
RES=`./rubcli localhost 5026 $CUBE | sed "s/ //g"`
t2 $RES
