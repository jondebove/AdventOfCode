#!/usr/bin/env perl

use strict;
use warnings;
use integer;

sub is_safe {
	my @r = @_;
	my $safe = 1;
	if (@r > 1) {
		my $diff = $r[1] - $r[0];
		my $inf = 1;
		my $sup = 3;
		if ($diff < 0) {
			$inf = -3;
			$sup = -1;
		}
		$safe = $inf <= $diff <= $sup;
		for (my $i = 2; $safe && $i < @r; $i++) {
			$diff = $r[$i] - $r[$i - 1];
			$safe = $inf <= $diff <= $sup;
		}
	}
	return $safe;
}

my $ans1 = 0;
my $ans2 = 0;
while (<>) {
	my @rec = split;
	my $safe = is_safe(@rec);
	$ans1++ if $safe;

	for (my $i = 0; !$safe && $i < @rec; $i++) {
		@rec[0, $i] = @rec[$i, 0];
		$safe = is_safe(@rec[1 .. $#rec]);
	}
	$ans2++ if $safe;
}
print "$ans1 $ans2\n";
