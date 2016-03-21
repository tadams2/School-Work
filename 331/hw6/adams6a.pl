#! /usr/bin/perl
my $url = $ARGV[0];

use LWP::Simple;

my $content = get $url;
die "couldn't get $url" unless defined $content;

@urls = $content =~ /\shref="?([^\s>"]+)/gi ;

foreach $url(@urls)
{
    print ($url);
    print("\n");
}
