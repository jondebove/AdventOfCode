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
my %gate;
my $nz;
while (<>) {
	chomp;
	if (/([xy]\d\d): ([01])/) {
		$wire{$1} = $2;
	} elsif (/(\w+) (AND|OR|XOR) (\w+) -> (\w+)/) {
		my $out = $4;
		$gate{$out} = { op => $2, in => [$1, $3] };
		$nz++ if $out =~ /^(z\d+)$/;
	} else {
		next;
	}
}

sub get {
	my ($w) = @_;

	if (defined $wire{$w}) {
		return $wire{$w};
	}

	my $g = $gate{$w};
	my $in1 = get($g->{in}[0]) + 0;
	my $in2 = get($g->{in}[1]) + 0;
	my $out = $op{$g->{op}}->($in1, $in2);
	$wire{$w} = $out;
}

{
	no warnings qw(portable);
	$ans1 = oct('0b' . join('', map { get(sprintf 'z%02d', $nz - $_) }
			(1 .. $nz)));
}

{
	open my $gv, '>', '24.gv' or die;
	print $gv "digraph {\n";
	sub dot {
		my ($w, $v) = @_;
		if (!defined $v) {
			$v = "__$w";
			print $gv "$v [label=$w, shape=rect];\n";
		}
		print $gv "$w -> $v [label=$w];\n";
		my $g = $gate{$w};
		if (defined($g)) {
			print $gv "$w [label=$g->{op}];\n";
			return if ++$g->{done} > 1;
			dot($g->{in}[0], $w);
			dot($g->{in}[1], $w);
		} else {
			print $gv "$w [shape=rect];\n";
		}
	}
	for (my $i = 0; $i < $nz; $i++) {
		dot(sprintf('z%02d', $i));
	}
	print $gv "}\n";
	close $gv;
	# Solution found by hand...
	$ans2 = join ',', sort qw(z08 mvb rds jss wss z18 bmn z23);
}

print "$ans1 $ans2\n";
