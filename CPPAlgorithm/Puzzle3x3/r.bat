echo off

rem good, solves in 37 steps, comparing to Wojtek's 23
rem puzzle2 -tbegin 8 0 5 3 4 7 2 6 1 -tend > puzzle2.trace
rem solves in 23 steps with modified heuristic evaluation method.
rem puzzle2 -tbegin 8 0 5 3 4 7 2 6 1 -tend > puzzle2.trace
puzzle2 -tbegin 2 5 4 0 7 6 3 1 8 -tend > puzzle2.trace
rem puzzle2 -tbegin 8 0 6 3 4 7 2 5 1 -tend > puzzle2.trace
rem puzzle2 -tbegin 8 0 6 3 4 7 2 5 1 -tend > puzzle2.trace
rem puzzle2 -tbegin 8 0 3 6 4 7 2 5 1 -tend > puzzle2.trace
echo "See puzzle2.trace"
pause prompt