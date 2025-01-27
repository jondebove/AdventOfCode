#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 1;
my $ans2 = 0;

$_ = <>;
my @time = /\d+/g;
$_ = <>;
my @dist = /\d+/g;

for (my $i = 0; $i < @time; $i++) {
	$ans1 *= solve($time[$i], $dist[$i]);
}
$ans2 = solve(join('', @time), join('', @dist));

print "$ans1 $ans2\n";

sub solve {
	# w^2 - t*w + d < 0
	no integer;
	my ($time, $dist) = @_;
	$dist += 1e-9;
	my $det = sqrt($time * $time - $dist * 4);
	my $min = int(($time - $det) / 2 + 1);
	my $max = int(($time + $det) / 2);
	$max - $min + 1
}
