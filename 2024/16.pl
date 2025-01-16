#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my @grid = map {chomp; [split //] } <>;

my @s;
my @e;
my %graph;

my @dir = ([1, 0], [0, 1], [-1, 0], [0, -1]);
my @dim = (scalar(@{$grid[0]}), scalar(@grid));
for (my $j = 1; $j < $dim[1] - 1; $j++) {
	for (my $i = 1; $i < $dim[0] - 1; $i++) {
		my $c = $grid[$j][$i];
		next if $c eq '#';

		if ($c eq 'S')    { @s = ($i, $j) }
		elsif ($c eq 'E') { @e = ($i, $j) }

		for (my $d = 0; $d < @dir; $d++) {
			my $i1 = $i + $dir[$d][0];
			my $j1 = $j + $dir[$d][1];
			next if $grid[$j1][$i1] eq '#';

			for (my $d0 = 0; $d0 < @dir; $d0++) {
				my $v = name($i, $j, @{$dir[$d0]});
				my $u = name($i1, $j1, @{$dir[$d]});
				$graph{$v}{$u} = ($d == $d0 ? 1 : 1001);
			}
		}
	}
}

my ($prev, $end) = dijkstra(\%graph, name(@s, 1, 0), name(@e));

walk1($end);

my %visited;
walk2($end);
#p();

print "$ans1 $ans2\n";

sub name { "@_" }

sub p {
	local $" = '';
	print "@$_\n" for (@grid);
}

sub walk1 {
	no warnings 'recursion';

	my $v = shift;
	return if !$prev->{$v};

	my $u = $prev->{$v}[0];
	$ans1 += $graph{$u}{$v};
	walk1($u);
}

sub walk2 {
	no warnings 'recursion';

	my $v = shift;
	return if $visited{$v};
	$visited{$v} = 1;

	my ($i, $j) = split ' ', $v;
	if ($grid[$j][$i] ne 'O') {
		$grid[$j][$i] = 'O';
		$ans2++;
	}

	if ($prev->{$v}) {
		walk2($_) for (@{$prev->{$v}});
	}
}

sub pq_push {
	my ($p, $q, $d, $v) = @_;
	$p->{$v} = $d;
	my ($i, $j) = (0, $#$q);
	while ($i <= $j) {
		my $k = int($i + ($j - $i) / 2);
		if ($p->{$q->[$k]} < $d) { $i = $k + 1 }
		else { $j = $k - 1 }
	}
	splice @$q, $i, 0, $v;
}

sub dijkstra {
	my ($g, $s, $e) = @_;
	my %prev;
	my %p;  # priority (dist)
	my @q;	# queue
	pq_push(\%p, \@q, 0, $s);
	while (1) {
		my $v = shift @q;
		if (!defined($v) || $v =~ /^$e /) {
			$e = $v;
			last;
		}
		while (my ($u, $d) = each %{$g->{$v}}) {
			$d += $p{$v};
			my $pu = $p{$u};
			if (!defined($pu) || $d < $pu) {
				$prev{$u} = [$v];
				pq_push(\%p, \@q, $d, $u);
			} elsif ($d == $pu) {
				push @{$prev{$u}}, $v;
				pq_push(\%p, \@q, $d, $u);
			}
		}
	}
	(\%prev, $e);
}
