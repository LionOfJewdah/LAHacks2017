echo off
rem puzzle2 -tbegin 6 4 7 8 5 0 3 2 1 -tend > puzzle2.trace
rem puzzle2 -tbegin 6 0 4 2 5 3 1 7 8 -tend > puzzle2.trace
rem puzzle2 -tbegin 6 4 7 8 5 0 3 2 1 -tend > puzzle2.trace
puzzle3x3 -tbegin 8 6 7 2 5 4 3 0 1 -tend -depth 34 -moves 80000 > puzzle2.trace
notepad puzzle2.trace
pause prompt
