#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my $re1 = qr/\d/;
my $re2 = qr/one|two|three|four|five|six|seven|eight|nine|\d/;
my %tr = (
	one => 1,
	two => 2,
	three => 3,
	four => 4,
	five => 5,
	six => 6,
	seven => 7,
	eight => 8,
	nine => 9,
);

while (<>) {
	my $d1;
	my $d2;

	$d1 = $1 if (/($re1)/);
	$d2 = $1 if (/.*($re1)/);
	$ans1 += $d1 . $d2;

	$d1 = $tr{$1} || $1 if (/($re2)/);
	$d2 = $tr{$1} || $1 if (/.*($re2)/);
	$ans2 += $d1 . $d2;
}

print "$ans1 $ans2\n";
