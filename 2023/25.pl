#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my %G;
while (<>) {
	chomp;
	s/://;
	my ($n, @ns) = split;
	for (@ns) {
		$G{$n}{$_} = 1;
		$G{$_}{$n} = 1;
	}
}

my ($g, $p1, $p2);
do {
	$g = krager(%G);
	$ans2++;
	($p1, $p2) = keys %$g;
} while ($g->{$p1}{$p2} != 3);

$ans1 = count($p1) * count($p2);

print "$ans1 $ans2\n";

sub count {
	scalar($_[0] =~ y/,//) + 1;
}

sub rget {
	my @k = keys %{$_[0]};
	$k[int(rand(@k))];
}

sub krager {
	my (%g) = @_;

	# clone
	while (my ($k1, $v1) = each %g) {
		$g{$k1} = { %$v1 };
	}

	# contract
	while (keys %g > 2) {
		my $u = rget(\%g);
		my $v = rget($g{$u});
		my $uv = $u . ',' . $v;
		my $eu = delete $g{$u};
		delete $eu->{$v};
		for (keys %$eu) {
			delete $g{$_}{$u};
			$g{$uv}{$_} += $eu->{$_};
			$g{$_}{$uv} += $eu->{$_};
		}
		my $ev = delete $g{$v};
		delete $ev->{$u};
		for (keys %$ev) {
			delete $g{$_}{$v};
			$g{$uv}{$_} += $ev->{$_};
			$g{$_}{$uv} += $ev->{$_};
		}
	}

	\%g;
}
