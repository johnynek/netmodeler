#!/usr/bin/perl
#
# converts a two column pdf to a two column cdf
#
#count the total first:
while(<>)
{
  if(/^#/) { next; } #this is a comment line
  if(/(\d+)\s+(\d+)/) {
    $total += $2;
    $pdf{$1} = $2;
    push @keys, $1;
  }
}

foreach $key (@keys) {
  print "$key\t" . ($total - $sum) . "\n";
  $sum += $pdf{$key};
}
