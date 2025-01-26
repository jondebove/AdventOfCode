#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my @multiplicity;
while (<>) {
	chomp;
	my ($card, $wins, $nums) = split(/ *[:|] */, $_);
	my %win = map { $_ => 1 } split(/ +/, $wins);
	my $n = 0;
	for (split(' ', $nums)) {
		$n++ if $win{$_};
	}
	$ans1 += 1 << ($n - 1) if $n;

	$multiplicity[$.]++;
	$multiplicity[$. + $_] += $multiplicity[$.] for (1 .. $n);
	$ans2 += $multiplicity[$.];
}

print "$ans1 $ans2\n";
