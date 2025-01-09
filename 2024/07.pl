#!/usr/bin/env perl

use strict;
use warnings;
use integer;

sub f {
	my ($F, $i, $cal, $cat) = @_;
	if ($i == $#$F) {
		return  $F->[0] == $cal + $F->[$i] ||
			$F->[0] == $cal * $F->[$i] ||
			$cat && $F->[0] == $cal . $F->[$i];
	}
	return 0 if $cal > $F->[0];
	f($F, $i + 1, $cal + $F->[$i], $cat) ||
	f($F, $i + 1, $cal * $F->[$i], $cat) ||
	$cat && f($F, $i + 1, $cal . $F->[$i], $cat);
}

my $ans1 = 0;
my $ans2 = 0;
while (<>) {
	my @F = split;
	chop $F[0];  # remove ':'
	$ans1 += $F[0] if f(\@F, 1, 0, 0);
	$ans2 += $F[0] if f(\@F, 1, 0, 1);
}
print "$ans1 $ans2\n";
