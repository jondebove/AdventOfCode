#!/usr/bin/env perl

use strict;
use warnings;
#use integer;

my $ans1 = 0;
my $ans2 = 0;

my @stone = map { [/(-?\d+)/g] } <>;

my $min = 200000000000000; # 7
my $max = 400000000000000; # 27

for (my $i = 0; $i < @stone; $i++) {
	for (my $j = $i + 1; $j < @stone; $j++) {
		$ans1 += f1($stone[$i], $stone[$j]);
	}
}

print "$ans1 $ans2\n";

sub f1 {
	my ($s1, $s2) = @_;

	my @A = ([$s1->[3], -$s2->[3]], [$s1->[4], -$s2->[4]]);
	my @B = ($s2->[0] - $s1->[0], $s2->[1] - $s1->[1]);

	my $det = $A[0][0] * $A[1][1] - $A[1][0] * $A[0][1];
	return 0 if $det == 0; # parallel trajectories

	my @X = (($A[1][1] * $B[0] - $A[0][1] * $B[1]) / $det,
		 ($A[0][0] * $B[1] - $A[1][0] * $B[0]) / $det);
	return 0 if $X[0] < 0 || $X[1] < 0; # cross in the past

	my $x = $s1->[0] + $s1->[3] * $X[0];
	my $y = $s1->[1] + $s1->[4] * $X[0];
	$x >= $min && $x <= $max && $y >= $min && $y <= $max ? 1 : 0;
}
