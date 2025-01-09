#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use Algorithm::Combinatorics qw(combinations);

my %antennas;
my $nr = 0;
my $nc = 0;
while (<>) {
	chomp;
	$nc = length;
	push(@{$antennas{$1}}, [$nr, $-[0]]) while /(\w)/g;
	$nr++;
}

my %antinodes;
while (my ($k, $v) = each %antennas) {
	my $iter = combinations($v, 2);
	while (my $p = $iter->next) {
		my $i = $p->[0]->[0]*2 - $p->[1]->[0];
		my $j = $p->[0]->[1]*2 - $p->[1]->[1];
		if (0 <= $i < $nr && 0 <= $j < $nc) {
			$antinodes{$i . "," . $j}++;
		}
		$i = $p->[1]->[0]*2 - $p->[0]->[0];
		$j = $p->[1]->[1]*2 - $p->[0]->[1];
		if (0 <= $i < $nr && 0 <= $j < $nc) {
			$antinodes{$i . "," . $j}++;
		}
	}
}
my $ans1 = (keys %antinodes);

%antinodes = ();
while (my ($k, $v) = each %antennas) {
	my $iter = combinations($v, 2);
	while (my $p = $iter->next) {
		my $i = $p->[0]->[0];
		my $j = $p->[0]->[1];
		while (0 <= $i < $nr && 0 <= $j < $nc) {
			$antinodes{$i . "," . $j}++;
			$i += $p->[0]->[0] - $p->[1]->[0];
			$j += $p->[0]->[1] - $p->[1]->[1];
		}
		$i = $p->[0]->[0];
		$j = $p->[0]->[1];
		while (0 <= $i < $nr && 0 <= $j < $nc) {
			$antinodes{$i . "," . $j}++;
			$i -= $p->[0]->[0] - $p->[1]->[0];
			$j -= $p->[0]->[1] - $p->[1]->[1];
		}
	}
}
my $ans2 = (keys %antinodes);

print "$ans1 $ans2\n";
