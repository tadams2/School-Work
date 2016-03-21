my $dir = '/afs/umbc.edu/users/c/d/cdowd1/pub/shakespeare/';
my %count;
my $forsooth;

foreach my $file (glob("$dir/*.txt"))
{
    if (-f $file and -r $file)
    {

        open my $thisFile, "<", $file or die "cant open file";
        my $title = <$thisFile>;
        print $title;

    }
}
