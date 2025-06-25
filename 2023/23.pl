#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my $grid = do { local $/; <> };
my $ncol = index($grid, "\n") + 1;
my $nrow = length($grid) / $ncol;

my %seen;
$ans1 = dfs(1);

print "$ans1 $ans2\n";

sub dfs {
	no warnings 'recursion';
	my ($pos, $p2) = @_;

	my $dist = scalar(keys %seen);
	return $dist if $pos / $ncol == $nrow - 1;

	my $ans = 0;
	$seen{$pos} = 1;

	my @dir = (1, -1, $ncol, -$ncol);
	if (!$p2) {
		my $c = substr($grid, $pos, 1);
		if    ($c eq '>') { @dir = (     1) }
		elsif ($c eq '<') { @dir = (    -1) }
		elsif ($c eq 'v') { @dir = ( $ncol) }
		elsif ($c eq '^') { @dir = (-$ncol) }
	}

	for my $d (@dir) {
		my $p = $pos + $d;
		next if $seen{$p} || $p < 0 || $p >= length($grid);
		my $c = substr($grid, $p, 1);
		next if $c eq '#' || $c eq "\n";

		my $ans1 = dfs($p, $p2);
		$ans = $ans > $ans1 ? $ans : $ans1;
	}

	delete $seen{$pos};
	return $ans;
}
