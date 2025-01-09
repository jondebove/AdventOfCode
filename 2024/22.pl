#!/usr/bin/env perl

use strict;
use warnings;
use integer;

sub n {
	my ($num) = @_;
	$num = (($num <<  6) ^ $num) & 16777215;
	$num = (($num >>  5) ^ $num) & 16777215;
	$num = (($num << 11) ^ $num) & 16777215;
}

my $ans1 = 0;
my $ans2 = 0;

my %all;
while (<>) {
	my $num = $_;
	my @seq;
	my $n = 2000;
	$seq[$n] = $num % 10;
	while ($n--) {
		$num = n($num);
		$seq[$n] = $num % 10;
	}
	$ans1 += $num;

	my %seq4;
	for ($n = 2000-4; $n >= 0; $n--) {
		my $s = join(',',
			$seq[$n+3] - $seq[$n+4],
			$seq[$n+2] - $seq[$n+3],
			$seq[$n+1] - $seq[$n+2],
			$seq[$n+0] - $seq[$n+1]);
		if (!exists $seq4{$s}) {
			$seq4{$s} = $seq[$n];
		}
	}
	while (my ($k, $v) = each %seq4) {
		$all{$k} += $v;
	}
}

for (values %all) {
	$ans2 = $_ if $ans2 < $_;
}

print "$ans1 $ans2\n";
