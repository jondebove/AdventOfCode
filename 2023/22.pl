#!/usr/bin/env perl

use strict;
use warnings;
use integer;

use Storable qw(dclone);

my $ans1 = 0;
my $ans2 = 0;

my @bricks;
while (<>) {
	if (/(\d+),(\d+),(\d+)~(\d+),(\d+),(\d+)/) {
		push @bricks, {
			x0 => $1 + 0, y0 => $2 + 0, z0 => $3 + 0,
			x1 => $4 + 0, y1 => $5 + 0, z1 => $6 + 0,
		};
	}
}

my @world;
set(\@world, $_, 1) for (@bricks);

@bricks = sort { $a->{z0} <=> $b->{z0} } @bricks;
for my $b (@bricks) {
	my $dz = down(\@world, $b);
	if ($dz) {
		set(\@world, $b, 0);
		$b->{z0} -= $dz;
		$b->{z1} -= $dz;
		set(\@world, $b, 1);
	}
}

for (my $i = 0; $i < @bricks; $i++) {
	set(\@world, $bricks[$i], 0);
	$ans1++;
	for (my $j = $i + 1; $j < @bricks; $j++) {
		if (down(\@world, $bricks[$j])) {
			$ans1--;
			last;
		}
	}
	set(\@world, $bricks[$i], 1);
}

for (my $i = 0; $i < @bricks; $i++) {
	my $w = dclone \@world;
	set($w, $bricks[$i], 0);
	for (my $j = $i + 1; $j < @bricks; $j++) {
		my $dz = down($w, $bricks[$j]);
		if ($dz) {
			my %b = %{$bricks[$j]};
			set($w, \%b, 0);
			$b{z0} -= $dz;
			$b{z1} -= $dz;
			set($w, \%b, 1);
			$ans2++;
		}
	}
}

print "$ans1 $ans2\n";

sub set {
	my ($w, $b, $v) = @_;
	for (my $z = $b->{z0}; $z <= $b->{z1}; $z++) {
		for (my $y = $b->{y0}; $y <= $b->{y1}; $y++) {
			for (my $x = $b->{x0}; $x <= $b->{x1}; $x++) {
				$w->[$z][$y][$x] = $v;
			}
		}
	}
}

sub down {
	my ($w, $b) = @_;
	my $z;
	for ($z = $b->{z0} - 1; $z; $z--) {
		for (my $y = $b->{y0}; $y <= $b->{y1}; $y++) {
			for (my $x = $b->{x0}; $x <= $b->{x1}; $x++) {
				goto out if $w->[$z][$y][$x];
			}
		}
	}
	out:
	$b->{z0} - $z - 1;
}
