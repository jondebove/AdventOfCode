#!/usr/bin/env perl

use strict;
use warnings;
use integer;

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

my @regions;
for (my $pos = $nc; $pos < ($nr-1)*$nc; $pos++) {
	my $val = substr($grid, $pos, 1);
	next if $val eq $pad;
	my $reg = { val => $val };
	f($pos, $reg);
	push @regions, $reg;
	substr($grid, $_, 1) = $pad for keys(%{$reg->{pos}});
}
for my $r (@regions) {
	my @n = values %{$r->{pos}};
	$ans1 += $r->{area} * $r->{perim};
	$ans2 += $r->{area} * $r->{corners};
}
print "$ans1 $ans2\n";

sub f {
	no warnings 'recursion';

	my ($pos, $reg) = @_;
	if (substr($grid, $pos, 1) ne $reg->{val}) {
		$reg->{perim}++;
		return 0;
	}
	return 1 if exists $reg->{pos}{$pos};
	$reg->{pos}{$pos} = 1;

	$reg->{area}++;
	my @side;
	my $i = 0;
	for (-$nc, -1, 1, $nc) {
		$side[$i++] = f($pos + $_, $reg);
	}

	$reg->{corners} += (
		# convex or concave
		((!$side[0] && !$side[1]) || ($side[0] && $side[1] &&
				substr($grid, $pos-$nc-1, 1) ne $reg->{val})) +
		((!$side[0] && !$side[2]) || ($side[0] && $side[2] &&
				substr($grid, $pos-$nc+1, 1) ne $reg->{val})) +
		((!$side[3] && !$side[2]) || ($side[3] && $side[2] &&
				substr($grid, $pos+$nc+1, 1) ne $reg->{val})) +
		((!$side[3] && !$side[1]) || ($side[3] && $side[1] &&
				substr($grid, $pos+$nc-1, 1) ne $reg->{val})));
	return 1;
}
