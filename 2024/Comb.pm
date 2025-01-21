package Comb;

use strict;
use warnings;

use Exporter 'import';
BEGIN {
	our @EXPORT_OK = qw(comb combr perm permr);
}

sub comb {
	my $yield = shift;
	my $k = shift;
	die if $k < 0 || $k > @_;
	_comb([], $yield, $k, @_); 
}

sub combr {
	my $yield = shift;
	my $k = shift;
	die if $k < 0;
	_combr([], $yield, $k, @_); 
}

sub perm {
	my $yield = shift;
	my $k = shift;
	die if $k < 0 || $k > @_;
	_perm([], $yield, $k, @_); 
}

sub permr {
	my $yield = shift;
	my $k = shift;
	die if $k < 0;
	_permr([], $yield, $k, @_); 
}

sub _comb {
	my $res = shift;
	my $yield = shift;
	my $k = shift;
	if ($k == 0) {
		return $yield->(@$res);
	}
	for (my $i = @_; $i >= $k; $i--) {
		if (!_comb([@$res, shift], $yield, $k - 1, @_)) {
			return 0;
		}
	}
	1;
}

sub _combr {
	my $res = shift;
	my $yield = shift;
	my $k = shift;
	if ($k == 0) {
		return $yield->(@$res);
	}
	for (my $i = @_; $i; $i--) {
		if (!_combr([@$res, $_[0]], $yield, $k - 1, @_)) {
			return 0;
		}
		shift;
	}
	1;
}

sub _perm {
	my $res = shift;
	my $yield = shift;
	my $k = shift;
	if ($k == 0) {
		return $yield->(@$res);
	}
	for (my $i = @_; $i; $i--) {
		my $a = shift;
		if (!_perm([@$res, $a], $yield, $k - 1, @_)) {
			return 0;
		}
		push @_, $a;
	}
	1;
}

sub _permr {
	my $res = shift;
	my $yield = shift;
	my $k = shift;
	if ($k == 0) {
		return $yield->(@$res);
	}
	for (my $i = @_; $i; $i--) {
		if (!_permr([@$res, $_[0]], $yield, $k - 1, @_)) {
			return 0;
		}
		push @_, shift;
	}
	1;
}

1;
