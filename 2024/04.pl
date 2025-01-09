#!/usr/bin/env perl

use strict;
use warnings;
use integer;

# read grid
chomp(my @lines = <>);
my $nr = @lines;
my $nc = length $lines[0];

# pad grid with '*'
unshift @lines, ('*' x $nc);
push @lines, ('*' x $nc);
@lines = map { '*' . $_ . '*' } @lines;
my $grid = join '', @lines;
$nr += 2;
$nc += 2;

sub f1 {
	my ($word, $pos, $stride) = @_;
	my $len = length $word;
	for (my $k = 0; $k < $len; $k++) {
		if (substr($word, $k, 1) ne substr($grid, $pos, 1)) {
			return 0;
		}
		$pos += $stride;
	}
	return 1;
}
sub f2 {
	my $word = shift;
	f1($word, @_) + f1(scalar(reverse($word)), @_);
}

my $ans1 = 0;
my $ans2 = 0;
for (my $i = 1; $i < $nr - 1; $i++) {
	for (my $j = 1; $j < $nc - 1; $j++) {
		my $pos = $j + $nc * $i;
		$ans1 += f2('XMAS', $pos, 1);
		$ans1 += f2('XMAS', $pos, $nc - 1);
		$ans1 += f2('XMAS', $pos, $nc);
		$ans1 += f2('XMAS', $pos, $nc + 1);
		if (f2('MAS', $pos, $nc+1) && f2('MAS', $pos+2, $nc-1)) {
			$ans2++;
		}
	}
}
print "$ans1 $ans2\n";
