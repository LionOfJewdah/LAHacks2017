CUBE=`echo $1 | sed "s/O/P/g"`
./rubcli localhost 5026 $CUBE | sed "s/ //g" 
