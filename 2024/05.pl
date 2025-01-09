#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my %rules;
while (<>) {
	chomp;
	last if /^\s*$/;
	$rules{$_} = 1;
}

my $ans1 = 0;
my $ans2 = 0;
while (<>) {
	chomp;
	my @seq = split ',';
	my $ans = f1(@seq);
	if ($ans > 0) {
		$ans1 += $ans;
	} else {
		while ($ans < 0) {
			@seq[-$ans, -$ans-1] = @seq[-$ans-1, -$ans];
			$ans = f1(@seq);
		}
		$ans2 += $ans;
	}
}
print "$ans1 $ans2\n";

sub f1 {
	my $n = @_;
	for (my $i = 0; $i < $n - 1; $i++) {
		for (my $j = $i + 1; $j < $n; $j++) {
			return -$j if $rules{$_[$j]."|".$_[$i]};
		}
	}
	return $_[$n/2];
}
