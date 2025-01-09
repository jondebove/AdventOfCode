#!/usr/bin/env perl

use strict;
use warnings;
use integer;
use List::Util qw(product);

my @dim = (101, 103);
my @robots;
while (<>) {
	if (/p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+)/) {
		push @robots, [$1, $2, $3, $4];
	}
}

# part 1
my $i = 100;
f($i);
my $ans1 = sf();

# part 2
my $ans2 = $i;
my $sfmin = $ans1;
for ($i++; $i <= $dim[0] * $dim[1]; $i++) {
	f(1);
	my $sf = sf();
	if ($sf < $sfmin) {
		$sfmin = $sf;
		$ans2 = $i;
		#p();
	}
}

print "$ans1 $ans2\n";

sub f {
	my $n = shift;
	for my $r (@robots) {
		$r->[0] += $r->[2] * $n;
		$r->[0] %= $dim[0];
		$r->[0] += $dim[0] if $r->[0] < 0;
		$r->[1] += $r->[3] * $n;
		$r->[1] %= $dim[1];
		$r->[1] += $dim[1] if $r->[1] < 0;
	}
}

sub sf {
	my @c = (0, 0, 0, 0);
	for my $r (@robots) {
		next if $r->[0] == $dim[0] / 2 || $r->[1] == $dim[1] / 2;
		my $x = $r->[0] / (($dim[0] + 1) / 2);
		my $y = $r->[1] / (($dim[1] + 1) / 2);
		$c[$x + 2 * $y]++;
	}
	product(@c);
}

sub p {
	my $grid = (('.' x $dim[0]) . "\n") x $dim[1];
	for my $r (@robots) {
		my $pos = $r->[0] + ($dim[0] + 1) * $r->[1];
		substr($grid, $pos, 1) = 'X';
	}
	print $grid;
}
