#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my $grid = do { local $/ = undef; <> };
my $len = index($grid, "\n") + 1;
my %stars;

while ($grid =~ /\d+/g) {
	my $val = $&;
	my $beg = $-[0];
	my $end = $+[0];
	if (find_symbols($val, $beg - 1, $beg) |
		find_symbols($val, $end, $end + 1) |
		find_symbols($val, $beg - $len - 1, $end - $len + 1) |
		find_symbols($val, $beg + $len - 1, $end + $len + 1)) {
		$ans1 += $val;
	}
}

for (values %stars) {
	$ans2 += $_->[0] * $_->[1] if (@$_ == 2);
}

print "$ans1 $ans2\n";

sub find_symbols {
	my ($val, $beg, $end) = @_;
	my $ok = 0;
	for (my $p = $beg; $p != $end; $p++) {
		next if $p < 0 || $p > length($grid);
		my $c = substr($grid, $p, 1);
		if ($c =~ /\D/ && $c ne '.' && $c ne "\n") {
			push(@{$stars{$p}}, $val) if ($c eq '*');
			$ok = 1;
		}
	}
	$ok;
}
