#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use List::Util qw(reduce zip);

my $ans1 = 0;
my $ans2 = 0;

my @grid;
while (<>) {
	chomp;
	if (/^$/) {
		$ans1 += 100 * f(0, @grid) + f(0, zip @grid);
		$ans2 += 100 * f(1, @grid) + f(1, zip @grid);
		@grid = ();
	} else {
		push @grid, [split //];
	}
}
$ans1 += 100 * f(0, @grid) + f(0, zip @grid);
$ans2 += 100 * f(1, @grid) + f(1, zip @grid);

print "$ans1 $ans2\n";

sub diff {
	reduce { $a + ($b->[0] ne $b->[1]) } 0, zip(@_);
}

sub f {
	my $want = shift;
	for (my $i = 0; $i < $#_; $i++) {
		my $got = 0;
		my $n = ($i + 1 < $#_ - $i ? $i + 1 : $#_ - $i);
		for (my $j = 0; $j < $n; $j++) {
			$got += diff($_[$i-$j], $_[$i+$j+1]);
			last if ($got > $want);
		}
		if ($got == $want) {
			return $i + 1;
		}
	}
	return 0;
}
