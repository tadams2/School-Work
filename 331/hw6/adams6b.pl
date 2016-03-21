#! usr/bin/perl

use LWP::Simple;
use Compress::Zlib;

my $url1 = $ARGV[0];
my $url2 = $ARGV[1];



my $content1 = get $url1;
my $content2 = get $url2;

#NCD(x,y) = (C(x.y) - min{C(x),C(y)}) / max{C(x),C(y)}
my $compress1 = compress $content1;
my $compress2 = compress $content2;
my $compress3 = compress ($content1.$content2);

my $compLength1 = length($compress1);
my $compLength2 = length($compress2);
my $compLength3 = length($compress3);


if($compLength1 < $compLength2)
{
    $min = $compLength1;
    $max = $compLength2;
}
else
{
    $min = $compLength2;
    $max = $compLength1;
}


my $NCD = ($compLength3 - $min) / $max;

print("the compression of $url1 and $url2 is \n");
print("$NCD\n");
