#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use lib '.';
use Comb;

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
	Comb::comb(sub {
		my $i = $_[0][0]*2 - $_[1][0];
		my $j = $_[0][1]*2 - $_[1][1];
		if (0 <= $i < $nr && 0 <= $j < $nc) {
			$antinodes{$i . "," . $j}++;
		}
		$i = $_[1][0]*2 - $_[0][0];
		$j = $_[1][1]*2 - $_[0][1];
		if (0 <= $i < $nr && 0 <= $j < $nc) {
			$antinodes{$i . "," . $j}++;
		}
		1;
	}, 2, @$v);
}
my $ans1 = (keys %antinodes);

%antinodes = ();
while (my ($k, $v) = each %antennas) {
	Comb::comb(sub {
		my $i = $_[0][0];
		my $j = $_[0][1];
		while (0 <= $i < $nr && 0 <= $j < $nc) {
			$antinodes{$i . "," . $j}++;
			$i += $_[0][0] - $_[1][0];
			$j += $_[0][1] - $_[1][1];
		}
		$i = $_[0][0];
		$j = $_[0][1];
		while (0 <= $i < $nr && 0 <= $j < $nc) {
			$antinodes{$i . "," . $j}++;
			$i -= $_[0][0] - $_[1][0];
			$j -= $_[0][1] - $_[1][1];
		}
		1;
	}, 2, @$v);
}
my $ans2 = (keys %antinodes);

print "$ans1 $ans2\n";
