#!/usr/bin/env perl

use strict;
use warnings;
use integer;

$/ = '';
chop(my $map = <>);
my $seq = <>;
$seq =~ s/\n//g;

my $nc = index($map, "\n");
$nc++;
#$map =~ s/\n//g;
my $nr= length($map) / $nc;

my $pos = index $map, '@';
#substr($map, $pos, 1) = '.';

my $map2 = $map;
$map2 =~ s/#/##/g;
$map2 =~ s/\./../g;
$map2 =~ s/@/@./g;
$map2 =~ s/O/[]/g;

my $ans1 = 0;
my $ans2 = 0;

for (my $c = 0; $c < length($seq); $c++) {
	f(substr($seq, $c, 1));
}
$ans1 = score('O');

$map = $map2;
$nc = ($nc - 1) * 2 + 1;
$pos = index $map, '@';
for (my $c = 0; $c < length($seq); $c++) {
	my %m = ('<' => -1, '>' => 1, '^' => -$nc, v => $nc);
	my $v = substr($seq, $c, 1);
	if ($v eq '<' || $v eq '>') {
		mh($m{$v});
	} else {
		mv($m{$v});
	}
}
$ans2 = score('[');

print "$ans1 $ans2\n";

sub f {
	my $c = shift;
	my %m = ('<' => -1, '>' => 1, '^' => -$nc, v => $nc);
	my $dir = $m{$c};
	die "'$c'" unless defined $dir;
	my $p = $pos;
	my $n = substr($map, $p, 1);
	my $last;
	while (1) {
		$last = $n;
		$p += $dir;
		$n = substr($map, $p, 1);
		if ($n eq '.') {
			substr($map, $pos, 1) = '.';
			$pos += $dir;
			substr($map, $pos, 1) = '@';
			substr($map, $p, 1) = $last;
			last;
		} elsif ($n eq '#') {
			last;
		}
	}
}

sub score {
	my $c = shift;
	my $sum = 0;
	while ($map =~ /\Q$c\E/g) {
		my $pos = $-[0];
		$sum += 100*($pos / $nc) + ($pos % $nc);
	}
	$sum;
}

sub mh {
	my $dir = shift;
	my $p = $pos;
	while (1) {
		$p += $dir;
		my $n = substr($map, $p, 1);
		if ($n eq '.') {
			substr($map, $pos, 1) = '.';
			$pos += $dir;
			substr($map, $pos, 1) = '@';
			for (; $p != $pos; $p -= $dir) {
				substr($map, $p, 1) = $dir > 0 ? ']' : '[';
				$p -= $dir;
				substr($map, $p, 1) = $dir < 0 ? ']' : '[';
			}
			last;
		} elsif ($n eq '#') {
			last;
		}
		$p += $dir;
	}
}

sub mv {
	my ($dir) = @_;
	my %stack;
	if (g($dir, $pos + $dir, \%stack)) {
		while (my ($k, $v) = each %stack) {
			if (!exists($stack{$k - $dir})) {
				substr($map, $k, 1) = '.';
			}
			substr($map, $k + $dir, 1) = $v;
		}
		substr($map, $pos, 1) = '.';
		$pos += $dir;
		substr($map, $pos, 1) = '@';
	}
}

sub g {
	my ($dir, $pos, $stack) = @_;
	my $n = substr($map, $pos, 1);
	if ($n eq '#') {
		return 0;
	}
	if ($n eq '.') {
		return 1;
	}
	if (exists $stack->{$pos}) {
		return 1;
	}
	$stack->{$pos} = $n;
	if (!g($dir, $pos + $dir, $stack)) {
		return 0;
	}
	if ($n eq '[') {
		return 0 if !g($dir, $pos + 1, $stack);
	} else {
		return 0 if !g($dir, $pos - 1, $stack);
	}
	return 1;
}
