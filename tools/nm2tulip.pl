#!/usr/bin/perl
#
# This code converts the standard netmodeler output to dot
#

%nodes = ();
$node = 1;
$edge = 1;
$nodelist = "(nodes ";
$edgeset = "(edges ";
$edgelist = "";

while(<STDIN>) {
  if(/(^[^:]*) : (.*)/) {
     if($nodes{$1}) {
       $nodeid = $nodes{$1};
     }
     else {
       $nodeid = $node++;
       $nodes{$1} = $nodeid;
       $nodelist .= "$nodeid ";
     }
     foreach $second ( split / /, $2 ) {
       if($nodes{$second}) {
         $node2id = $nodes{$second};
       }
       else {
         $node2id = $node++;
         $nodes{$second} = $node2id;
         $nodelist .= "$node2id ";
       }
       $edgelist .= "(edge $edge $nodeid $node2id)\n";
       $edgeset .= "$edge ";
       $edge++;
     }
  }
}
$nodelist .= ")";
$edgeset .= ")";
print $nodelist . "\n" . $edgelist . $edgeset . "\n";
