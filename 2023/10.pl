#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my $grid = do { local $/; <> };
my $ncol = index($grid, "\n") + 1;
my $pos = index($grid, "S");
my @delta = (1, $ncol, -1, -$ncol);

for (my $dir = 0; $dir < 4; $dir++) {
	my $path = f($pos, $dir);
	if (defined $path) {
		$ans1 = keys(%$path) / 2;
		$ans2 = area($path, $pos, $pos) - $ans1 + 1;
		last;
	}
}

print "$ans1 $ans2\n";

sub area {
	my ($path, $start, $end) = @_;
	my $ans = 0;
	my $pos = $end;
	my @p = ($pos % $ncol, $pos / $ncol);
	do {
		$pos = $path->{$pos};
		my @q = ($pos % $ncol, $pos / $ncol);
		$ans += $p[0] * $q[1] - $p[1] * $q[0];
		@p = @q;
	} while ($pos != $start);
	abs($ans) / 2;
}

sub f {
	my ($pos, $dir) = @_;

	my %path = ();
	while (1) {
		my $prev = $pos;
		$pos += $delta[$dir];
		return if $pos < 0 || $pos > length($grid);

		my $tile = substr $grid, $pos, 1;
		return if $tile eq '.' || $tile eq "\n";

		$path{$pos} = $prev;
		return \%path if $tile eq 'S';

		if ($dir == 0) {
			if ($tile eq '-') { $dir = 0 }
			elsif ($tile eq '7') { $dir = 1 }
			elsif ($tile eq 'J') { $dir = 3 }
			else { return }
		} elsif ($dir == 1) {
			if ($tile eq '|') { $dir = 1 }
			elsif ($tile eq 'J') { $dir = 2 }
			elsif ($tile eq 'L') { $dir = 0 }
			else { return }
		} elsif ($dir == 2) {
			if ($tile eq '-') { $dir = 2 }
			elsif ($tile eq 'L') { $dir = 3 }
			elsif ($tile eq 'F') { $dir = 1 }
			else { return }
		} elsif ($dir == 3) {
			if ($tile eq '|') { $dir = 3 }
			elsif ($tile eq 'F') { $dir = 0 }
			elsif ($tile eq '7') { $dir = 2 }
			else { return }
		}
	}
}
