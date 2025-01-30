#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my $card1 = '23456789TJQKA';
my $card2 = 'J23456789TQKA';
my $n = length($card1);
my $n5 = $n**5;

my @data;
while (<>) {
	my ($hand, $bid) = split;
	my %count;
	my $score1 = 0;
	my $score2 = 0;
	for (my $i = 0; $i < 5; $i++) {
		my $c = substr $hand, $i, 1;
		$count{$c}++;
		$score1 = $score1 * $n + index($card1, $c);
		$score2 = $score2 * $n + index($card2, $c);
	}

	$score1 += score(\%count);

	my $njk = delete($count{J}) || 0;
	my $max = '';
	for (keys %count) {
		$max = ($count{$max} || 0) >= $count{$_} ? $max : $_;
	}
	$count{$max} += $njk;

	$score2 += score(\%count);

	push @data, {
		bid => $bid,
		score1 => $score1,
		score2 => $score2,
	};
}

$ans1 = win('score1', @data);
$ans2 = win('score2', @data);

print "$ans1 $ans2\n";

sub score {
	my $count = shift;
	my $score = 0;
	for (values %$count) {
		if ($_ == 5) {
			$score += $n5 * 6;
		} elsif ($_ == 4) {
			$score += $n5 * 5;
		} elsif ($_ == 3) {
			$score += $n5 * 3;
		} elsif ($_ == 2) {
			$score += $n5 * 1;
		}
	}
	$score;
}

sub win {
	my $score = shift;
	@_ = sort { $a->{$score} <=> $b->{$score} } @_;
	my $ans = 0;
	for (my $i = 0; $i < @_; $i++) {
		$ans += ($i + 1) * $_[$i]{bid};
	}
	$ans;
}
