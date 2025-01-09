#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $ans1 = 0;
my $ans2 = 0;

my $input = do { local $/; <> };

$ans1 += $1*$2 while $input =~ /mul\((\d{1,3}),(\d{1,3})\)/g;

$input =~ s/don't\(\).*?do\(\)//sg;
$input =~ s/don't\(\).*//;
$ans2 += $1*$2 while $input =~ /mul\((\d{1,3}),(\d{1,3})\)/g;

print "$ans1 $ans2\n";
