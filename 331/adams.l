



DIGIT    [0-9]
ALPHA    [a-zA-Z]
ALPHANUM [a-zA-Z0-9]
STRING   [a-zA-Z]
PRETAG   ["<pre>"]
PRECLOSE ["</pre>"]
HTMLTAG  ["<html>"]
HTMLCLOSE ["</html>"]
BOLDTAG  ["<bold>"]
BOLDCLOSE ["</bold>"]
TITLETAG ["<title>"]
TITLECLOSE ["</title>"]
BODYTAG  ["<body>"]
BODYCLOSE ["</body>"]
PTAG     ["<P>"]
PCLOSE   ["</P>"]

%%

{HTMLTAG}       printf("HTML tag: \n");
{HTMLCLOSE}     printf("html Closing tag \n");

{BOLDTAG}       printf("Bold tag \n");
{BOLDCLOSE}     printf("Bold close tag \n");

{TITLETAG}      printf("Title tag \n");
{TITLECLOSE}    printf("Title close tag \n");

{BODYTAG}       printf("Body open tag \n");
{BODYCLOSE}     printf("Body close tag \n");

{PTAG}          printf("P tag \n");
{PCLOSE}        printf("P close tag\n");

"http://www."{ALPHA}+".com" printf("Url: %s \n", yytext);
{ALPHA}+                    printf("word: %s \n", yytext);
[ \t\n]+                  /* skip space, tab and newline */;
.                            printf("unrecognized %s: \n", yytext );

%%

main(argc,argv)
  int argc; char **argv;
  {
    if (argc>1) yyin=fopen(argv[1],"r"); else yyin=stdin;
    yylex();
  }