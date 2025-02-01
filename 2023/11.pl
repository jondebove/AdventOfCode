#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use lib qw(./lib ../lib);
use Combin qw(comb);

my $ans1 = 0;
my $ans2 = 0;

my @glx;
my $nx = 0;
my $ny = 0;
while (<>) {
	while (/#/g) {
		push @glx, { y => $ny, x => $-[0] };
	}
	$nx = length($_) - 1;
	$ny++;
}

sub bsearch (&@) {
	my $code = shift;
	my $lo = 0;
	my $hi = @_;
	while ($lo < $hi) {
		my $mi = $lo + int(($hi - $lo) / 2);
		$_ = $_[$mi];
		my $cmp = $code->();
		if ($cmp >= 0) { $hi = $mi }
		elsif ($cmp < 0) { $lo = $mi + 1 }
		#else { return $mi }
	}
	return $lo; #-1;
}

sub expand1d {
	my ($glx, $x, $nx, $dx, $sort) = @_;
	@$glx = sort { $a->{$x} <=> $b->{$x} } @$glx if $sort;
	for (my $xx = 0; $xx < $nx; $xx++) {
		my $i = bsearch { $_->{$x} <=> $xx } @$glx;
		next if ($glx->[$i]{$x} == $xx);
		$glx->[$i++]{$x} += $dx while ($i < @$glx);
		$nx += $dx;
		$xx += $dx;
	}
	$nx;
}

sub calc {
	my $ans = 0;
	comb(sub { $ans += abs($_[1]{x} - $_[0]{x}) +
			abs($_[1]{y} - $_[0]{y}); 1 }, 2, @_);
	$ans;
}

# part 1
$ny = expand1d(\@glx, 'y', $ny, 1, 0);
$nx = expand1d(\@glx, 'x', $nx, 1, 1);
$ans1 = calc(@glx);

# part 2
$nx = expand1d(\@glx, 'x', $nx, 1000000/2-1, 0);
$ny = expand1d(\@glx, 'y', $ny, 1000000/2-1, 1);
$ans2 = calc(@glx);

print "$ans1 $ans2\n";
