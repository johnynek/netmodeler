#!/usr/bin/perl
#
# This code converts the standard netmodeler output to dot
#

$biggest_idx = 0;
%label_map = ();
print "graph g {\n";
print "size=\"16,16\";\n";
print "node [style=filled];\n";
print "edge [len=4];\n";
while(<>) {
  if(/(^[^:]*) : (.*)/) {
     $cleanname = clean_name($1);
     foreach $second ( split / /, $2 ) {
       print $cleanname . " -- " . clean_name($second) . ";\n";
     }
     if(defined $label_map{$cleanname}) {
       $label = $label_map{$cleanname};
     }
     else {
       $label_map{$cleanname} = $biggest_idx;
       $label = $biggest_idx;
       $biggest_idx++;
     }
     print $cleanname . " [label=\"$label\"];\n";
  }
}

print "}\n";

sub clean_name {
  my $name = shift;

  $name =~ tr/[a-zA-Z0-9]/_/c;
  $name =~ s/^([0-9])/n$1/;
  return $name;
}
