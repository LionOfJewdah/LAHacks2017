CUBE=`echo $1 | sed "s/O/P/g" | awk '{
  for (i=0; ++i <= length($0);) 
    printf "%s%c", substr($0, i, 1), 32
  }'`
rubcu -a 3 -c -e $CUBE |grep Solution|awk -v FS=":" '{print $2;}'|sed "s/ //g" 
