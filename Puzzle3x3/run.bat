echo off
rem puzzle2 0 3 8 5 1 4 6 2 7 90 10000 3 2 1 0 > puzzle2.trace
rem puzzle2 0 3 9 5 1 4 6 2 7 90 10000 3 2 1 0 > puzzle2.trace
rem puzzle2 > puzzle2.trace
rem main 8 6 7 2 5 4 3 0 1 50 10000 > puzzle2.trace
rem puzzle2 6 4 7 8 5 0 3 2 1 40 10000 > puzzle2.trace
rem good
rem puzzle2 6 4 7 8 5 0 3 2 1 44 30000 > puzzle2.trace
rem good
rem puzzle2 6 4 7 8 5 0 3 2 1 35 30000 > puzzle2.trace
rem good, 31 steps
rem puzzle2 6 4 7 8 5 0 3 2 1 33 30000 > puzzle2.trace
rem puzzle2 -tbegin 6 4 7 8 5 0 3 2 1 -tend -depth 32 -moves 30000 > puzzle2.trace
rem puzzle2 -tbegin 6 0 4 2 5 3 1 7 8 -tend -depth 31 -moves 20000 > puzzle2.trace
puzzle3x3 -tbegin 4 8 1 0 6 3 2 7 5 -tend -depth 40 -moves 50000 > puzzle2.trace
notepad puzzle2.trace
pause prompt
