package Arithm;

use strict;
use warnings;
use integer;

sub _gcd {
	my ($x, $y) = @_;
	($x, $y) = ($y, $x % $y) while ($y != 0);
	$x;
}

sub gcd {
	my $ans = $_[0];
	for (my $i = 1; $i < @_; $i++) {
		$ans = _gcd($ans, $_[$i]);
	}
	$ans;
}

sub lcm {
	my $ans = $_[0];
	for (my $i = 1; $i < @_; $i++) {
		$ans = ($ans / _gcd($ans, $_[$i])) * $_[$i];
	}
	$ans;
}

1;
