my $dir = '/afs/umbc.edu/users/c/d/cdowd1/pub/shakespeare/';
my %count;
my $forsooth;
print "part 1: Titles \n";
foreach my $file (glob("$dir/*.txt"))
{
    if (-f $file and -r $file)
    {
	
	open my $thisFile, "<", $file or die "cant open file";
        my $title = <$thisFile>;
	print $title;
	while (my $line = <$thisFile>)
	{
	    chomp $line;
	    foreach my $word ( split " ", $line)
	    {
		$word =~ split (//, $word);
		my $wordlc = $word;
		$wordlc =~ tr/A-Z/a-z/;
                $wordlc =~ s/[^a-z+]//g;
		if ($wordlc =~ "forsooth")
		   {$forsooth++};
	        $count{$wordlc}++;
	    }
	     
        }	    
    }	
}
print "Part 2: Forsooth\n";
print "the word forsooth was in all of his works $forsooth times\n";
print "part 3: Words that occur more than 5000 times \n";
my $i = 0;
foreach my $word (reverse sort { $count{$a} <=> $count{$b}} keys %count)
{
    if($count{$word} > 5000)
    {
    print "$word\n";
    }
}
print "Part 4: 10 most common words\n";
foreach my $word (reverse sort { $count{$a} <=> $count{$b}} keys %count)
{
    if($i < 10)
    {
        printf "%-15s %s\n", $word, $count{$word};
        $i++;
    }

}
                              

