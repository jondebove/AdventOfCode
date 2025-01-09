#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = '';

my @ks;
my @ls;

while (<>) {
	chomp;
	if ($_ eq '#####') {
		my @k = (0, 0, 0, 0, 0);
		for (my $i = 5; $i; $i--) {
			my $row = <>;
			for (my $j = 0; $j < 5; $j++) {
				if (substr($row, $j, 1) eq '#') {
					$k[$j]++;
				}
			}
		}
		<>;
		push @ks, \@k;
	} elsif ($_ eq '.....') {
		my @l = (5, 5, 5, 5, 5);
		for (my $i = 5; $i; $i--) {
			my $row = <>;
			for (my $j = 0; $j < 5; $j++) {
				if (substr($row, $j, 1) eq '.') {
					$l[$j]--;
				}
			}
		}
		<>;
		push @ls, \@l;
	}
}

for my $k (@ks) {
	for my $l (@ls) {
		my $ok = 1;
		for (my $j = 0; $j < 5; $j++) {
			if ($k->[$j] + $l->[$j] > 5) {
				$ok = 0;
				last;
			}
		}
		$ans1 += $ok;
	}
}

print "$ans1 $ans2\n";
