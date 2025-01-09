#!/usr/bin/env perl

use strict;
use warnings;
use integer;

# registers
chomp(my $A = (split(': ', scalar(<>)))[1]);
chomp(my $B = (split(': ', scalar(<>)))[1]);
chomp(my $C = (split(': ', scalar(<>)))[1]);
scalar <>;
# program
chomp(my $P = (split(': ', scalar(<>)))[1]);

my @P = split ',', $P;

my $ans1 = run($A, $B, $C);
my $ans2 = 0;
for (my $i = $#P; $i >= 0; $i--) {
	$ans2 <<= 3;
	while (run($ans2, $B, $C) ne join(',', @P[$i..$#P])) {
		$ans2++;
	}
}
print "$ans1 $ans2\n";

sub run {
	my ($A, $B, $C) = @_;
	my $pos = 0;
	my @out;
	my $combo = sub { (0, 1, 2, 3, $A, $B, $C)[$_[0]] };
	my @instruction = (
		sub { $A = $A >> $combo->($_[0]) },		# adv
		sub { $B = $B ^ $_[0] },			# bxl
		sub { $B = $combo->($_[0]) & 0x7 },		# bst
		sub { $pos = $A ? $_[0] - 2 : $pos - 1},	# jnz
		sub { $B = $B ^ $C },				# bxc
		sub { push @out, $combo->($_[0]) & 0x07 },	# out
		sub { $B = $A >> $combo->($_[0]) },		# bdv
		sub { $C = $A >> $combo->($_[0]) },		# cdv
	);

	while ($pos < @P) {
		$instruction[$P[$pos]]($P[$pos+1] || 0);
		$pos += 2;
	}
	join ',', @out;
}
