#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my %grid;
my @s;
my @e;

my @bytes = map {chomp; [split //] } <>;

my @dim = (scalar(@{$bytes[0]}), scalar(@bytes));
for (my $j = 0; $j < $dim[1]; $j++) {
	for (my $i = 0; $i < $dim[0]; $i++) {
		if ($bytes[$j][$i] eq '#') {
			$grid{$i,$j} = '#';
		} elsif ($bytes[$j][$i] eq 'S') {
			@s = ($i, $j);
		} elsif ($bytes[$j][$i] eq 'E') {
			@e = ($i, $j);
		}
	}
}

my $prev = bfs(\%grid, \@dim, \@s, \@e);

my @path;
for (my @pos = @e; $pos[0] != $s[0] || $pos[1] != $s[1];
	@pos = @{$prev->{$pos[0], $pos[1]}}) {
	$grid{$pos[0], $pos[1]} = 'O';
	push @path, [@pos];
}
$grid{$s[0], $s[1]} = 'O';
push @path, \@s;

p(\%grid);

my $ans1 = 0;
my $ans2 = 0;

for (my $i = 0; $i < @path - 102; $i++) {
	for (my $j = $i + 102; $j < @path; $j++) {
		my $dist = abs($path[$i][0]-$path[$j][0])+abs($path[$i][1]-$path[$j][1]);
		if ($dist <= 20 && $j - $i - $dist >= 100) {
			$ans1++ if $dist <= 2;
			$ans2++;
		}
	}
}

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
	my $grid = shift;
	for (my $j = 0; $j < $dim[1]; $j++) {
		for (my $i = 0; $i < $dim[0]; $i++) {
			print exists($grid->{$i,$j}) ? $grid->{$i,$j} : '.';
		}
		print "\n";
	}
}
