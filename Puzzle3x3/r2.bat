echo off
rem puzzle2 -tbegin 8 0 3 6 4 7 2 5 1 -tend > puzzle2.trace
rem this should be the one with no solution
puzzle2 -tbegin 8 0 6 3 4 7 2 5 1 -tend > puzzle2.trace
rem good
rem puzzle2 -tbegin 2 5 4 0 7 6 3 1 8 -tend  > puzzle2.trace
rem write puzzle2.trace
echo "See puzzle2.trace"
pause prompt