#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use List::Util qw(sum0);

# read grid
chomp(my @lines = <>);
my $nr = @lines;
my $nc = length $lines[0];

# pad grid with '*'
my $pad = '*';
unshift @lines, ($pad x $nc);
push @lines, ($pad x $nc);
@lines = map { $pad . $_ . $pad } @lines;
my $grid = join '', @lines;
$nr += length($pad) * 2;
$nc += length($pad) * 2;

my $ans1 = 0;
my $ans2 = 0;
for (my $pos = 0; $pos < $nr*$nc; $pos++) {
	if (substr($grid, $pos, 1) eq '0') {
		my %counts;
		f($pos, \%counts);
		$ans1 += keys %counts;
		$ans2 += sum0(values %counts);
	}
}
print "$ans1 $ans2\n";

sub f {
	my ($pos, $counts) = @_;
	my $val = substr($grid, $pos, 1);
	if ($val eq '9') {
		$counts->{$pos}++;
		return;
	}
	$val++;
	for (-$nc, -1, 1, $nc) {
		f($pos + $_, $counts) if substr($grid, $pos + $_, 1) eq $val;
	}
}
