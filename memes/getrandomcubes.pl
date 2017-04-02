#!/usr/bin/perl -w
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


print "Random Rubik Cubes Generator, version 3.0.1 (C) Marek Karcz 2015.\n";
print "All rights reserved.\n";
print "Usage:\n";
print "   $0 [num_tests [scramble_steps]]\n";
print "Where:\n";
print "   num_tests      - number of cubes to generate (default: 100)\n";
print "   scramble_steps - steps to scramble cube (0 or default: random)\n";
print "\nWait 5 seconds to proceed or precc CTRL-C to stop...\n";
sleep 5;

my $input = "";
my @lines;
my $maxsteps = shift || 100;
my $tests = $maxsteps;
my $scramble = shift || 0;
my $scrsteps = 40;
my $cubedef;

while ($tests > 0) {

    if ($scramble == 0) {
       $scrsteps = 1 + int(rand(99));
	} else {
	   $scrsteps = $scramble;
	}
    my $cmd = "rubik -a 3 -s $scrsteps -b -c -C";
    $input = `$cmd`;
    @lines = split /\n/, $input;

    foreach (@lines) {
	   if (/Color notation/) {
	      my @values = split(':');
		  $cubedef = $values[1];
	   }
    }
    $cubedef =~ s/ //g;
    $cubedef =~ s/P/O/g;
    print "$cubedef\n";
    $tests = $tests - 1;
    if ($scramble != 0) {
       sleep 1
    }
}

