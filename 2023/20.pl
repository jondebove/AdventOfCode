#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use List::Util qw(all);

use lib qw(../lib);
use Arithm qw(lcm);

use constant {
	NO => 0,
	LO => 1,
	HI => 2,
};	

my $ans1 = 0;
my $ans2 = 0;

sub flip {
	my ($m, $p) = @_;
	if ($p == LO) {
		($m->{on} = !$m->{on}) ? HI : LO;
	} else {
		NO;
	}
}

sub conj {
	my ($m, $p, $f) = @_;
	$m->{inputs}{$f} = $p;
	(all { $_ == HI } values %{$m->{inputs}}) ? LO : HI;
}

sub deft { LO }

my %modules;
while (<>) {
	my ($type, $name, $receivers) = /^([%&])?(\w+) -> (.*)$/;
	my $m = $modules{$name};
	$m->{on} = 0;
	$m->{receivers} = [split(', ', $receivers)];
	if (!defined($type)) {
		$m->{send} = \&deft;
	} elsif ($1 eq '%') { 
		$m->{send} = \&flip;
	} elsif ($1 eq '&') {
		$m->{send} = \&conj;
	}
	for (@{$m->{receivers}}) {
		$modules{$_}{inputs}{$name} = LO;
		$modules{$_}{send} ||= \&deft;
	}
	$modules{$name} = $m;
}

my @count = (0, 0);
for my $n (1 .. 5000) {
	my @q;
	push @q, 'broadcaster', LO, '';
	while (@q) {
		my $dst = shift @q;
		my $sig = shift @q;
		my $src = shift @q;

		$count[$sig]++ if $n <= 1000;  # for part 1
		$modules{$src}{mod} ||= $n if $sig == HI; # for part 2

		my $m = $modules{$dst};
		next if ($sig = $m->{send}($m, $sig, $src)) == NO;
		$src = $dst;
		for my $d (@{$m->{receivers}}) {
			push @q, $d, $sig, $src;
		}
	}
}

$ans1 = $count[LO] * $count[HI];

# first level is a single inverter
# second level is where it happens
my @inrx = keys %{$modules{rx}{inputs}};
die if @inrx != 1;
@inrx = keys %{$modules{$inrx[0]}{inputs}};

$ans2 = lcm(map { $modules{$_}{mod} } @inrx);

print "$ans1 $ans2\n";
