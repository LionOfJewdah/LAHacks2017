PROGRAM="$1"
INPUT="$2"
OUTPUT="$3"
while read CUBE; do
  /usr/bin/time -p $PROGRAM $CUBE > out.$$ 2> time.$$
  echo $CUBE $(cat out.$$) $(grep real time.$$ | tr -dC '0-9.' ) >> $OUTPUT
  rm out.$$ time.$$
done < $INPUT

