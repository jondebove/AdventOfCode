#!/usr/bin/env perl

use strict;
use warnings;
use integer;

sub mv {
	my ($dst, $src) = @_;
	push @$dst, @$src;
	@$src = ();
}

sub min {
	my $inc = shift;
	my $ans = $_[0];
	for (my $i = $inc; $i < @_; $i += $inc) {
		$ans = $_[$i] if $_[$i] < $ans;
	}
	$ans;
}

my $ans1 = 0;
my $ans2 = 0;

$_ = <>;
my @dst1 = /\d+/g;
my @src1;

my @dst2 = @dst1;
my @src2;

<>;  # skip empty line
while (<>) {
	if (/ map:/) {
		mv(\@src1, \@dst1);
		mv(\@src2, \@dst2);
	} elsif (/^\s*$/) {
		mv(\@dst1, \@src1);
		mv(\@dst2, \@src2);
	} elsif (/(\d+) (\d+) (\d+)/) {
		my $d = $1;
		my $b = $2;
		my $e = $b + $3;
		for (my $i = 0; $i < @src1; $i++) {
			if ($b <= $src1[$i] < $e) {
				push @dst1, $src1[$i] + $d - $b;
				splice @src1, $i--, 1;
			}
		}
		for (my $i = 0; $i < @src2; $i += 2) {
			my $beg = $src2[$i];
			my $end = $beg + $src2[$i + 1];
			if ($end <= $b || $beg >= $e) {
				# next
			} elsif ($beg >= $b && $end <= $e) {
				push @dst2, $beg + $d - $b, $end - $beg;
				splice @src2, $i, 2;
				$i -= 2;
			} elsif ($beg >= $b && $end > $e) {
				push @dst2, $beg + $d - $b, $e - $beg;
				splice @src2, $i, 2, $e, $end - $e;
			} elsif ($beg < $b && $end <= $e) {
				push @dst2, $d, $end - $b;
				splice @src2, $i, 2, $beg, $b - $beg;
			} else {
				push @dst2, $d, $e - $b;
				splice @src2, $i, 2, $beg, $b - $beg, $e, $end - $e;
				$i += 2;
			}
		}
	}
}
mv(\@dst2, \@src1);
mv(\@dst2, \@src2);

$ans1 = min(1, @dst1);
$ans2 = min(2, @dst2);

print "$ans1 $ans2\n";
