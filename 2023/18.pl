#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my @path1;
my @path2;

my @pos1 = (0, 0);
my @pos2 = (0, 0);
push @path1, [@pos1];
push @path2, [@pos2];
while (<>) {
	if (/([RDLU]) (\d+) \(#([0-9a-f]{5})([0-3])\)/) {
		if    ($1 eq 'R') { $pos1[0] += $2 }
		elsif ($1 eq 'D') { $pos1[1] += $2 }
		elsif ($1 eq 'L') { $pos1[0] -= $2 }
		elsif ($1 eq 'U') { $pos1[1] -= $2 }
		push @path1, [@pos1];
		if    ($4 eq '0') { $pos2[0] += hex($3) }
		elsif ($4 eq '1') { $pos2[1] += hex($3) }
		elsif ($4 eq '2') { $pos2[0] -= hex($3) }
		elsif ($4 eq '3') { $pos2[1] -= hex($3) }
		push @path2, [@pos2];
	} else { die }
}

# Pick's theorem
$ans1 = area(\@path1) + perimeter(\@path1) / 2 + 1;
$ans2 = area(\@path2) + perimeter(\@path2) / 2 + 1;

print "$ans1 $ans2\n";

sub perimeter {
	my ($p) = @_;
	my $ans = 0;
	for (my $i = 0; $i < $#$p; $i++) {
		$ans += (abs($p->[$i][0] - $p->[$i+1][0]) +
			abs($p->[$i][1] - $p->[$i+1][1]));
	}
	$ans;
}

sub area {
	my ($p) = @_;
	my $ans = 0;
	for (my $i = 0; $i < $#$p; $i++) {
		$ans += $p->[$i][0] * $p->[$i+1][1] - $p->[$i][1] * $p->[$i+1][0];
	}
	abs($ans) / 2;
}
