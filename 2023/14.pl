#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my $grid = do { local $/; <> };
my $ncol = index($grid, "\n") + 1;
my $nrow = length($grid) / $ncol;

my @rocks;
for (my $idx = index $grid, 'O'; $idx != -1;
	$idx = index($grid, 'O', $idx + 1)) {
	push @rocks, $idx;
}

tilt(0);
$ans1 = load();

my @cache;
tilt($_) for (1 .. 3);
push @cache, { grid => $grid, load => load() };

cycle:
while (1) {
	tilt($_) for (0 .. 3);
	for (my $i = 0; $i < @cache; $i++) {
		if ($cache[$i]{grid} eq $grid) {
			my $idx = $i + (1000000000 - $i - 1) % (@cache - $i);
			$ans2 = $cache[$idx]{load};
			last cycle;
		}
	}
	push @cache, { grid => $grid, load => load() };
}

print "$ans1 $ans2\n";

sub load {
	my $ans = 0;
	$ans += $nrow - $_ / $ncol for (@rocks);
	$ans;
}

sub tilt {
	my ($dir) = @_;
	my $inc;
	if ($dir == 0) {
		$inc = -$ncol;
		@rocks = sort { $a / $ncol <=> $b / $ncol } @rocks;
	} elsif ($dir == 1) {
		$inc = -1;
		@rocks = sort { $a % $ncol <=> $b % $ncol } @rocks;
	} elsif ($dir == 2) {
		$inc = $ncol;
		@rocks = sort { $b / $ncol <=> $a / $ncol } @rocks;
	} elsif ($dir == 3) {
		$inc = 1;
		@rocks = sort { $b % $ncol <=> $a % $ncol } @rocks;
	}
	for my $r (@rocks) {
		substr($grid, $r, 1) = '.';
		while (0 <= $r + $inc < length($grid) &&
			substr($grid, $r + $inc, 1) eq '.') {
			$r += $inc;
		}
		substr($grid, $r, 1) = 'O';
	}
}
