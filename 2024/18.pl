#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my @dim = (71, 71);
my %grid;
my @s = (0, 0);
my @e = ($dim[0] - 1, $dim[1] - 1);

my @bytes = map {chomp; [split ','] } <>;
my $lo = 0;
my $hi = @bytes;

my $ans1 = 0;
for ($lo = 0; $lo < 1024; $lo++) {
	$grid{$bytes[$lo][0], $bytes[$lo][1]} = '#';
}

my $prev = bfs(\%grid, \@dim, \@s, \@e);
for (my @pos = @e; $pos[0] != $s[0] || $pos[1] != $s[1];
	@pos = @{$prev->{$pos[0], $pos[1]}}) {
	$ans1++;
}

my %grid0 = %grid;
my $mi = $lo + ($hi - $lo) / 2;
while ($hi != $lo + 1) {
	%grid = %grid0;
	for (my $k = 1024; $k <= $mi; $k++) {
		$grid{$bytes[$k][0], $bytes[$k][1]} = '#';
	}

	my $p = bfs(\%grid, \@dim, \@s, \@e);
	if (exists $p->{$e[0], $e[1]}) {
		$lo = $mi;
	} else {
		$hi = $mi;
	}
	$mi = $lo + ($hi - $lo) / 2;
}
my $ans2 = join ',', @{$bytes[$hi]};

print "$ans1 $ans2\n";

sub bfs {
	my ($grid, $dim, $s, $e) = @_;
	my %grid = %$grid;
	my %prev;
	my @q;
	push @q, $s->[0], $s->[1];
	$grid{$s->[0], $s->[1]} = '.';

	while (@q) {
		my @p = splice @q, 0, 2;
		last if $p[0] == $e->[0] && $p[1] == $e->[1];
		my @dir = (1, 0, 0, 1, -1, 0, 0, -1);
		for (my $i = 0; $i < @dir; $i += 2) {
			my @pp = ($p[0] + $dir[$i], $p[1] + $dir[$i+1]);
			next if exists($grid{$pp[0], $pp[1]}) ||
				$pp[0] < 0 || $pp[0] >= $dim->[0] ||
				$pp[1] < 0 || $pp[1] >= $dim->[1];

			push @q, $pp[0], $pp[1];
			$grid{$pp[0], $pp[1]} = '.';
			$prev{$pp[0], $pp[1]} = [@p];
		}
	}
	\%prev;
}

sub p {
	for (my $j = 0; $j < $dim[1]; $j++) {
		for (my $i = 0; $i < $dim[0]; $i++) {
			print exists($grid{$i,$j}) ? $grid{$i,$j} : '.';
		}
		print "\n";
	}
}
