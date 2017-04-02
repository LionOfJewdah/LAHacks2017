
OnExit ()
{
   rm -f mksplit_$$_*
}

trap 'OnExit' EXIT
trap 'exit 2' INT TERM

split -l 5000 -a 2 $1 mksplit_$$_
for F in `ls mksplit_$$_*`
do
   for G in `cat $F|sed "s/ /_/g"`
   do
      H=`echo $G|awk -vFS="_" '{print $1}'`
      SOL=`echo $G|awk -vFS="_" '{print $2}'`
      CUBE=`echo $H | sed "s/O/P/g" | awk '{
         for (i=0; ++i <= length($0);) 
         printf "%s%c", substr($0, i, 1), 32
      }'`
      PASS=`rubcu -a 3 -c -e $CUBE -r $SOL  |grep "Cube solved" 2>/dev/null`
      [[ -z $PASS ]] && { echo "FAILED: $H ($CUBE) -> $SOL"; exit; }
      echo "PASSED: $H -> $SOL"
   done
done
rm mksplit_$$_*
