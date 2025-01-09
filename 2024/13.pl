#!/usr/bin/env perl

use strict;
use warnings;

my $ans1 = 0;
my $ans2 = 0;
my %A;
my $offset = 10000000000000;
while (<>) {
	if (/Button ([AB]): X([+-]\d+), Y([+-]\d+)/) {
		my $c = $1 eq 'A' ? 0 : 1;
		$A{0,$c} = $2;
		$A{1,$c} = $3;
	}
	if (/Prize: X=(\d+), Y=(\d+)/) {
		my @b = ($1, $2);
		$ans1 += f(@b);
		$ans2 += f($b[0] + $offset, $b[1] + $offset);
	}
}
print "$ans1 $ans2\n";

sub f {
	my $det = $A{0,0} * $A{1,1} - $A{0,1} * $A{1,0};
	die if $det == 0;

	my $x = ($A{1,1} * $_[0] - $A{0,1} * $_[1]) / $det;
	return 0 if $x !~ /^\d+$/;

	my $y = ($A{0,0} * $_[1] - $A{1,0} * $_[0]) / $det;
	return 0 if $y !~ /^\d+$/;

	return $x * 3 + $y;
}
