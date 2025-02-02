#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use Memoize;
memoize('match');

my $ans1 = 0;
my $ans2 = 0;

while (<>) {
	my ($pat, $rle) = split;
	my @rle = split(',', $rle);
	$ans1 += match('?', $pat, @rle);
	$ans2 += match('?', join('?', ($pat) x 5), (@rle) x 5);
}

print "$ans1 $ans2\n";

sub match {
	no warnings 'recursion';

	my $prev = shift;
	my $pat = shift;

	if (length($pat) == 0) {
		return @_ == 0 || @_ == 1 && $_[0] == 0;
	}

	my $n = @_ - 1;
	$n += $_ for (@_);
	return 0 if ($n > length($pat));

	my $ans = 0;
	my $c = substr($pat, 0, 1);
	if ($c eq '.') {
		if ($prev eq '#') {
			return 0 if ($_[0] != 0);
			shift @_;
		}
		$ans = match($c, substr($pat, 1), @_);
	} elsif ($c eq '#') {
		return 0 if (@_ == 0 || $_[0] == 0);
		$ans = match($c, substr($pat, 1), $_[0] - 1, @_[1 .. $#_]);
	} elsif ($c eq '?') {
		substr($pat, 0, 1) = '.';
		$ans = match($prev, $pat, @_);
		substr($pat, 0, 1) = '#';
		$ans += match($prev, $pat, @_);
	} else {
		die;
	}
	$ans;
}
