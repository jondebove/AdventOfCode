#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use lib qw(./lib ../lib);
use Combin qw(comb);

my $ans1 = 0;
my $ans2 = '';

my %conn;
while (<>) {
	chomp;
	my ($c1, $c2) = split '-';
	$conn{$c1}{$c2}++;
	$conn{$c2}{$c1}++;
}

my %seen1;
my $max = [];
while (my ($k, $v) = each %conn) {
	if (substr($k, 0, 1) eq 't') {
		comb(sub {
			my $c = join(',', sort($k, @_));
			if (exists $seen1{$c}) {
				return 1;
			}
			$seen1{$c} = 1;

			if (exists $conn{$_[0]}{$_[1]}) {
				$ans1++;
			}
			1;
		}, 2, keys %$v);
	}

	my $m = dfs([keys %$v], 0, [$k], $max);
	$max = $m if @$m > @$max;
}
$ans2 = join(',', sort(@$max));

print "$ans1 $ans2\n";

sub dfs {
	my ($cs, $i, $lan, $max) = @_;

	for (; $i < @$cs; $i++) {
		my $c = $cs->[$i];
		my @lan = @$lan;

		my $ok = 1;
		for (@lan) {
			if (!exists $conn{$c}{$_}) {
				$ok = 0;
				last;
			}
		}
		push @lan, $c if $ok;
		if (@$max < @lan) {
			$max = \@lan;
		}

		$max = dfs($cs, $i + 1, \@lan, $max);
	}
	return $max;
}
