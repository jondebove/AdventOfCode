#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my $grid = do { local $/; <> };
my $ncol = index($grid, "\n") + 1;
my $nrow = length($grid) / $ncol;
my @dir = (1, $ncol, -1, -$ncol);

my $s = 0;
my $e = length($grid) - 2;

my $graph = make_graph(1, 3);
my ($prev, $end) = dijkstra($graph, name($s, 1), name($e));
$ans1 = walk($end);

$graph = make_graph(4, 10);
($prev, $end) = dijkstra($graph, name($s, 1), name($e));
$ans2 = walk($end);

#print $grid;

print "$ans1 $ans2\n";

sub name { "@_" }

sub make_graph {
	my ($min, $max) = @_;
	my %graph;
	for (my $pos = 0; $pos < length($grid); $pos++) {
		next if substr $grid, $pos, 1 eq "\n";
	
		for (my $d = 0; $d < @dir; $d++) {
			my $v = name($pos, $d);
			for my $d1 (($d+1)%4, ($d+3)%4) {
				my $pos1 = $pos;
				my $c1 = 0;
				for (my $n = 0; $n < $max; $n++) {
					$pos1 += $dir[$d1];
					last if $pos1 < 0 || $pos1 >= length($grid);
					my $c = substr $grid, $pos1, 1;
					last if $c eq "\n";
					$c1 += $c;
					$graph{$v}{name($pos1, $d1)} = $c1 if $n >= $min - 1;
				}
			}
		}
	}
	\%graph;
}

sub walk {
	my $v = shift;
	my $ans = 0;
	for (my $u = $prev->{$v}; $u; $u = $prev->{$u}) {
		#my ($pos, $dir) = split ' ', $v;
		#substr($grid, $pos, 1) = ('>', 'v', '<', '^')[$dir];

		$ans += $graph->{$u}{$v};
		$v = $u;
	}
	$ans;
}

sub pq_new {
	{queue => [], seen => {}};
}

sub pq_push {
	my ($pq, $v, $p) = @_;
	my ($i, $j) = (0, $#{$pq->{queue}} - 1);
	while ($i <= $j) {
		my $k = int($i + ($j - $i) / 2);
		$k-- if ($k & 1);
		if ($pq->{queue}[$k + 1] < $p) { $i = $k + 2 }
		else { $j = $k - 2 }
	}
	splice @{$pq->{queue}}, $i, 0, $v, $p;
}

sub pq_pop {
	my ($pq) = @_;
	my ($v, $p) = splice @{$pq->{queue}}, 0, 2;
	while ($pq->{seen}{$v}) {
		($v, $p) = splice @{$pq->{queue}}, 0, 2;
	}
	$pq->{seen}{$v} = 1;
	$v;
}

sub dijkstra {
	my ($g, $s, $e) = @_;
	my %prev;
	my %dist = ($s => 0);
	my $pq = pq_new();
	pq_push($pq, $s, 0);
	while (1) {
		my $v = pq_pop($pq);
		if (!defined($v) || $v =~ /^$e /) {
			$e = $v;
			last;
		}
		my $dv = $dist{$v};
		while (my ($u, $d) = each %{$g->{$v}}) {
			$d += $dv;
			my $du = $dist{$u};
			if (!defined($du) || $d < $du) {
			#	$prev{$u} = [$v];
				$prev{$u} = $v;
				$dist{$u} = $d;
				pq_push($pq, $u, $d);
			#} elsif ($d == $pu) {
			#	push @{$prev{$u}}, $v;
			#	pq_push($pq, $u, $d);
			}
		}
	}
	(\%prev, $e);
}
