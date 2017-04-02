#!perl -w
use strict;
use warnings;
#use Time::HiRes qw( usleep );

# *----------------------------------------------------------------------------
# * Rubik Cube solver.
# * Copyright (C) Marek Karcz 2015. All rights reserved.
# *
# * License agreement between the User and the Author:
# *
# * This software and its source code is free to use and distribute
# * for non-commercial purposes only and as long as following conditions
# * are met:
# *
# * - this copyright notice in its entirety along with
# *   the license agreement and disclaimer notice are included
# *   in the distributed package,
# *
# * - the source code is included in distributed package.
# *
# * User is free to modify the code and distribute the modified code
# * as long as the modifications are properly documented and also
# * included in the distributed package.
# * Following forms of documentation of the changes to the code are
# * accepted:
# *
# * - source code comments,
# *
# * - description of the changes in the form of a readme document
# *   in any commonly used format (text, ms office, open office).
# *
# * Disclaimer:
# * 
# * No warranty is offered in any form. Author can not be held
# * responsible for any damage caused by the software.
# * Use at your own risk.
# *----------------------------------------------------------------------------


print "Testing cube solver, version 2.5 (C) Marek Karcz 2015.\n";
print "All rights reserved.\n";
print "Usage:\n";
print "   $0 [num_tests [scramble_steps]]\n";
print "Where:\n";
print "   num_tests      - number of tests to perform (default: 100)\n";
print "   scramble_steps - steps to scramble cube (0 or default: random)\n";
print "\nWait 5 seconds to proceed or precc CTRL-C to stop...\n";
sleep 5;

my $input = "";
my @lines;
my $issolved = 0;
my $numsteps = 0;
my $steps = "";
my $maxsteps = shift || 100;
my $tests = $maxsteps;
my $scramble = shift || 0;
my $scrsteps = 40;
my $sum = 0;
my $maxsl = 0;
my $shortestsl = 1000;
my $cubedef;
my $avgtime = 0;
my $soltime = 0;

while ($tests > 0) {

    if ($scramble == 0) {
       $scrsteps = 1 + int(rand(99));
	} else {
	   $scrsteps = $scramble;
	}
    my $cmd = "rubik -a 3 -s $scrsteps -b -c -N";
    $input = `$cmd`;
    @lines = split /\n/, $input;

    foreach (@lines) {
       if (/Cube solved/) {
          $issolved = 1;
       }
       if (/Solution/) {
          my @info = split(' ');
          my @values = split(':');
          $numsteps = $info[2];
          $steps = $values[1];
          $steps =~ s/^\s+|\s+$//g;
       }
	   if (/Finished/) {
	      my @values = split(' ');
		  $soltime = $values[2];
		  $avgtime = $avgtime + $soltime;
	   }
	   if (/Input notation/) {
	      my @values = split(':');
		  $cubedef = $values[1];
	   }
    }

    if ($issolved == 1) {
       my $n = $maxsteps + 1 - $tests;
	   print "Test #$n - testing solution...";
	   my $cm2 = "rubik -a 3 -i $cubedef -r $steps -b -c";
	   my $cubinp = `$cm2`;
	   my @inplin = split /\n/, $cubinp;
	   $issolved = 0;
	   foreach (@inplin) {
	      if (/Cube solved/) {
		     $issolved = 1;
			 last;
		  }
	   }
	   if ($issolved == 1) {
	      print "OK.\n";
          print "Test #$n PASSED: $cmd, # of steps: $numsteps, time: $soltime [sec]\n";
	   } else {
	      print "FAILED.\n";
          print "ERROR!!! Cube not solved or program failed!!!\n";
          print "$input\n";
          last;
	   }
       $sum = $sum + $numsteps;
    } else {
       print "ERROR!!! Cube not solved or program failed!!!\n";
       print "$input\n";
       last;
    }

    $tests = $tests - 1;
    if ($numsteps > $maxsl) {
       $maxsl = $numsteps;
    }
    if ($numsteps < $shortestsl) {
       $shortestsl = $numsteps;
    }
}

print "Test finished.\n";
my $stperf = $maxsteps - $tests;
my $avgsl = int($sum / $stperf);
$avgtime = $avgtime / $stperf;
print "Stats:\n";
print "# of tests performed: $stperf\n";
print "The average solution  length: $avgsl\n";
print "The longest solution  length: $maxsl\n";
print "The shortest solution length: $shortestsl\n";
print "Average time: $avgtime seconds.\n";
print "Bye!\n";

