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

# initial direction and position of the guard
my $pos0 = index $grid, '^';

# change chars to bitmap and save it.
$grid =~ y/.^#*/\x00\x00\x10\x20/;
my $grid0 = $grid;

move_guard();

my $ans1 = 0;
my $ans2 = 0;
my $grid1 = $grid;
while ($grid1 =~ /[\x01-\x0f]/g) {
	$ans1++;
	$grid = $grid0;
	vec($grid, $-[0], 8) = 0x10;
	$ans2 += move_guard();
}
print "$ans1 $ans2\n";

# move the guard until he exits (0) or until it cycles (1)
sub move_guard {
	my @mdir = (-$nc, 1, $nc, -1);
	my $dir = 0;
	my $pos = $pos0;
	while (1) {
		vec($grid, $pos, 8) |= 1<<$dir;
		$pos += $mdir[$dir];

		my $b = vec($grid, $pos, 8);
		if ($b == 0x10) {
			$pos -= $mdir[$dir];
			$dir = ($dir+1) % 4;
		} elsif ($b == 0x20) {
			return 0;
		} elsif ($b & (1<<$dir)) {
			return 1;
		}
	}
}
