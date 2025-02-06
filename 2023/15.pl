#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my @box = map { [] } (1 .. 256);
for (split ',', <>) {
	chomp;
	$ans1 += hash($_);

	if (/([a-z]+)(-|=(\d))/) {
		my $b = $box[hash($1)];
		my $i;
		for ($i = 0; $i < @$b; $i++) {
			last if ($b->[$i][0] eq $1);
		}
		if ($2 eq '-') {
			splice(@$b, $i, 1);
		} else {
			$b->[$i] = [$1, $3];
		}
	} else { die }
}
for (my $b = 0; $b < @box; $b++) {
	for (my $i = 0; $i < @{$box[$b]}; $i++) {
		$ans2 += ($b + 1) * ($i + 1) * $box[$b][$i][1];
	}
}

print "$ans1 $ans2\n";

sub hash {
	my $ans = 0;
	for (my $i = 0; $i < length($_[0]); $i++) {
		$ans += ord(substr($_[0], $i, 1));
		$ans = (($ans << 4) + $ans) & 0xff;
	}
	$ans;
}
