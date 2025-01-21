#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use List::Util qw(sum0);

sub blink {
	my %stones;
	while (my ($k, $v) = each %{$_[0]}) {
		if ($k == 0) {
			$stones{1} += $v;
			next;
		}
		my $d = 1;
		for (my $n = $k / 10; $n; $n /= 10) {
			$d++;
		}
		if ($d & 1) {
			$stones{$k * 2024} += $v;
			next;
		}
		my $q = 10;
		for ($d -= 2; $d; $d -= 2) {
			$q *= 10;
		}
		$stones{$k / $q} += $v;
		$stones{$k % $q} += $v;
	}
	\%stones;
}

my $stones = {};
$stones->{$_}++ for (split ' ', <>);

for (my $i = 25; $i; $i--) {
	$stones = blink($stones);
}
my $ans1 = sum0(values %$stones);

for (my $i = 50; $i; $i--) {
	$stones = blink($stones);
}
my $ans2 = sum0(values %$stones);

print "$ans1 $ans2\n";
