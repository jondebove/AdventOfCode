#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

sub all_equal {
	for (my $i = 1; $i < @_; $i++) {
		return 0 if $_[$i] != $_[0];
	}
	1;
}

while (<>) {
	my @F = split;
	my $sgn = 1;
	while (!all_equal(@F)) {
		$ans2 += $F[0] * $sgn;
		for (my $i = 0; $i < $#F; $i++) {
			$F[$i] = $F[$i + 1] - $F[$i];
		}
		$ans1 += pop @F;
		$sgn = -$sgn;
	}
	$ans1 += $F[-1];
	$ans2 += $F[0] * $sgn;
}

print "$ans1 $ans2\n";
