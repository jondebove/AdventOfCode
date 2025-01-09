#!/usr/bin/env perl

use strict;
use warnings;
use integer;

chomp(my @twls = split(', ', scalar(<>)));

my $ans1 = 0;
my $ans2 = 0;
while (<>) {
	chomp;
	next if /^$/;

	my %m = ($_ => 1);
	my $count = dfs($_, '', \%m);
	$ans1 += !!$count;
	$ans2 += $count;
}
print "$ans1 $ans2\n";

sub dfs {
	my ($d, $s, $m) = @_;

	if (!exists $m->{$s}) {
		$m->{$s} = 0;
		for my $t (@twls) {
			if (substr($d, length($s), length($t)) eq $t) {
				$m->{$s} += dfs($d, $s . $t, $m);
			}
		}
	}
	return $m->{$s};
}
