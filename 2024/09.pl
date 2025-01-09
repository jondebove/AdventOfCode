#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my @block;
my @file;
my @space;
{
	chomp(my $diskmap = <>);
	my $id = 0;
	my $pos = 0;
	for (my $i = 0; $i < length $diskmap; ) {
		my $f = substr($diskmap, $i++, 1);
		my $e = substr($diskmap, $i++, 1) || 0;
		push @block, (($id) x $f), ((undef) x $e);
		$file[$id] = { pos => $pos, len => $f };
		$pos += $f;
		$space[$id] = { pos => $pos, len => $e };
		$pos += $e;
		$id++;
	}
}

# part 1
my $ans1 = 0;
{
	# defrag
	my $i = 0;
	my $j = $#block;
	while (1) {
		$i++ while defined $block[$i];
		$j-- until defined $block[$j];
		last if $j <= $i;
		$block[$i] = $block[$j];
		undef $block[$j];
	}
	# checksum
	for ($i = 0; $i < @block; $i++) {
		$ans1 += ($block[$i] || 0) * $i;
	}
}

# part 2
my $ans2 = 0;
for (my $i = $#file; $i; $i--) {
	# defrag
	my $pos = $file[$i]{pos};
	my $len = $file[$i]{len};
	for (my $j = 0; $space[$j]{pos} < $pos; $j++) {
		if ($space[$j]{len} >= $len) {
			$pos = $space[$j]{pos};
			if ($space[$j]{len} > $len) {
				$space[$j]{pos} += $len;
				$space[$j]{len} -= $len;
			} else {
				splice @space, $j, 1;
			}
			last;
		}
	}
	# checksum
	my $sum = 0;
	for (my $j = $len - 1; $j >= 0; $j--) {
		$sum += $pos + $j;
	}
	$ans2 += $sum * $i;
}

print "$ans1 $ans2\n";
