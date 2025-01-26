#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my %max = (red => 12, green => 13, blue => 14);
while (<>) {
	my $ok = 1;
	my %pow = (red => 0, green => 0, blue => 0);
	while (/ (\d+) (red|green|blue)/g) {
		$ok = 0 if $1 > $max{$2};
		$pow{$2} = $1 if $1 > $pow{$2};
	}
	$ans1 += $1 if $ok && /Game (\d+):/;
	$ans2 += $pow{red} * $pow{green} * $pow{blue};
}

print "$ans1 $ans2\n";
