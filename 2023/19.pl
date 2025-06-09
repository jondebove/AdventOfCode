#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use List::Util qw(sum);

my $ans1 = 0;
my $ans2 = 0;

sub parse_rule {
	my $s = $_[0];
	if ($s =~ /^([xmas])([<>])(\d+):(\w+)$/) {
		my %ops = (
			'<' => sub { $_[0] < $_[1] },
			'>' => sub { $_[0] > $_[1] },
		);
		my %vars = (x => 0, m => 1, a => 2, s => 3);

		my $var = $vars{$1};
		my $op  = $ops{$2};
		my $val = $3;
		my $res = $4;

		sub { $op->($_[$var], $val) ? $res : "" };
	} elsif ($s =~ /^(\w+)$/) {
		my $res = $1;
		sub { $res };
	} else {
		die;
	}
}

my %workflows;
while (<>) {
	last if /^$/;
	my ($name, $rules) = /^(\w+)\{(.*)\}$/;
	$workflows{$name} = [map(parse_rule($_), split(',', $rules))]
}

sub accepted {
	my $in = 'in';
	while (1) {
		my $out = '';
		for my $r (@{$workflows{$in}}) {
			last if ($out = $r->(@_));
		}
		return 1 if $out eq 'A';
		return 0 if $out eq 'R';
		die unless $out;
		$in = $out;
	}
}

while (<>) {
	my @xmas = /^\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\}$/;
	$ans1 += sum(@xmas) if accepted(@xmas);
}

print "$ans1 $ans2\n";
