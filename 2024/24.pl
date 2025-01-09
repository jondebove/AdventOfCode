#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = '';

my %op = (
	AND => sub { $_[0] & $_[1] },
	OR  => sub { $_[0] | $_[1] },
	XOR => sub { $_[0] ^ $_[1] },
);

my %wire;
my $nz = 0;
while (<>) {
	chomp;
	if (/(\w+): ([01])/) {
		$wire{$1} = { value => $2 };
	} elsif (/(\w+) (AND|OR|XOR) (\w+) -> (\w+)/) {
		my $out = $4;
		$wire{$out} = { gate => { op => $2, in => [$1, $3] } };
		$nz++ if $out =~ /^(z\d+)$/;
	} else {
		next;
	}
}

sub get {
	my ($w) = @_;

	if (defined $wire{$w}{value}) {
		return $wire{$w}{value};
	}

	my $gate = $wire{$w}{gate};
	my $in1 = get($gate->{in}[0]) + 0;
	my $in2 = get($gate->{in}[1]) + 0;
	my $out = $op{$gate->{op}}->($in1, $in2);
	$wire{$w}{value} = $out;
}

{
	no warnings qw(portable);
	$ans1 = oct('0b' . join('', map { get(sprintf 'z%02d', $nz - $_) } (1 .. $nz)));
}

# TODO part 2

print "$ans1 $ans2\n";
