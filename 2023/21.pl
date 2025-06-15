#!/usr/bin/env perl

use strict;
use warnings;
use integer;

## example
#my $discard = 4;
#my $n1 = 6;
#my $n2 = 5000;

#input
my $discard = 0;
my $n1 = 64;
my $n2 = 26501365;

my $ans1 = 0;
my $ans2 = 0;

my $grid = do { local $/; <> };
my $ncol = index($grid, "\n") + 1;
my $nrow = length($grid) / $ncol;

my $s = index($grid, 'S');
my $pos = {join($;, $s / $ncol, $s % $ncol) => 1};
my @count = (1);
for (1 .. $nrow * ($discard + 3)) {
	#warn "$_\n";
	$pos = round($pos);
	$count[$_] = scalar(keys %$pos);
}

$ans1 = $count[$n1];

# Use Lagrange polynomials to compute y = a0 + a1.x + a2.x^2
my $x = $n2;
my $offset = $x % $nrow;
my @x = map { $offset + $nrow * $_ } ($discard .. ($discard + 2));
my @y = map { $count[$_] } @x;

for (0 .. 2) {
	no integer;
	$ans2 += $y[$_] * (
		($x - $x[($_+1)%3])/($x[$_] - $x[($_+1)%3])*
		($x - $x[($_+2)%3])/($x[$_] - $x[($_+2)%3]));
}

print "$ans1 $ans2\n";

sub round {
	my ($src) = @_;
	my %dst;
	for my $pos (keys %$src) {
		my ($r0, $c0) = split $;, $pos;
		for my $d ([1, 0], [-1, 0], [0, 1], [0, -1]) {
			my $r1 = $r0 + $d->[0];
			my $c1 = $c0 + $d->[1];
			my $r2 = $r1 % $nrow;
			my $c2 = $c1 % ($ncol - 1);
			$r2 += $nrow     if $r2 < 0;
			$c2 += $ncol - 1 if $c2 < 0;
			if (substr($grid, $r2 * $ncol + $c2, 1) ne '#') {
				$dst{$r1, $c1}++;
			}
		}
	}
	\%dst;
}
