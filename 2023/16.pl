#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my $grid = do { local $/; <> };
my $ncol = index($grid, "\n") + 1;

my %seen;
dfs(-1, 3);
$ans1 = keys %seen;

$ans2 = $ans1;
for (my $pos = $ncol - 1; $pos < length($grid); $pos += $ncol) {
	%seen = ();
	dfs($pos, 3);
	$ans2 = keys %seen if (keys %seen > $ans2);
}
for (my $pos = -$ncol; $pos < 0; $pos += 1) {
	%seen = ();
	dfs($pos, 2);
	$ans2 = keys %seen if (keys %seen > $ans2);
}
for (my $pos = $ncol - 1; $pos < length($grid); $pos += $ncol) {
	%seen = ();
	dfs($pos, 1);
	$ans2 = keys %seen if (keys %seen > $ans2);
}
for (my $pos = length($grid); $pos < length($grid)+$ncol; $pos += 1) {
	%seen = ();
	dfs($pos, 0);
	$ans2 = keys %seen if (keys %seen > $ans2);
}

print "$ans1 $ans2\n";

sub dfs {
	no warnings 'recursion';
	my ($pos, $dir) = @_;

	$pos += (-$ncol, -1, $ncol, 1)[$dir];

	# check if inside grid
	return if ($pos < 0 || $pos >= length($grid));
	my $op = substr($grid, $pos, 1);
	return if ($op eq "\n");

	# check for cycle
	return if (exists $seen{$pos} && ($seen{$pos} & (1 << $dir)));
	$seen{$pos} |= (1 << $dir);

	if ($op eq '.') {
		dfs($pos, $dir);
	} elsif ($op eq '/') {
		dfs($pos, (3, 2, 1, 0)[$dir]);
	} elsif ($op eq '\\') {
		dfs($pos, (1, 0, 3, 2)[$dir]);
	} elsif ($op eq '-') {
		if ($dir & 1) {
			dfs($pos, $dir);
		} else {
			dfs($pos, 1);
			dfs($pos, 3);
		}
	} elsif ($op eq '|') {
		if ($dir & 1) {
			dfs($pos, 0);
			dfs($pos, 2);
		} else {
			dfs($pos, $dir);
		}
	}
}
