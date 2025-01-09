#!/usr/bin/env perl

use strict;
use warnings;
use integer;

# Read
my @id1;
my @id2;
while (<>) {
	my @F = split;
	push @id1, $F[0];
	push @id2, $F[1];
}

# Part 1
my $ans1 = 0;
@id1 = sort { $a <=> $b } @id1;
@id2 = sort { $a <=> $b } @id2;
$ans1 += abs($id1[$_] - $id2[$_]) for (0 .. $#id1);

# Part 2
my %count;
my $ans2 = 0;
$count{$_}++ for (@id2);
$ans2 += $_ * ($count{$_} || 0) for (@id1);

print "$ans1 $ans2\n";
