#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use lib '../lib';
use Arithm;

my $ans1 = 0;
my $ans2 = 0;

chomp(my $cmd = <>);
my %graph;
while (<>) {
	$graph{$1} = {L => $2, R => $3} if /(\w+) = \((\w+), (\w+)\)/;
}

sub f {
	my ($continue, $s) = @_;

	my $i = 0;
	my $ans = 0;
	while ($continue->($s)) {
		$s = $graph{$s}{substr($cmd, $i, 1)};
		$i = ($i + 1) % length($cmd);
		$ans++;
	}
	$ans;
}

$ans1 = f(sub { $_[0] ne 'ZZZ' }, 'AAA');

my @s = grep { substr($_, 2, 1) eq 'A' } keys %graph;
my @n = map { f(sub { substr($_[0], 2, 1) ne 'Z' }, $_) } @s;
$ans2 = Arithm::lcm(@n);

print "$ans1 $ans2\n";
