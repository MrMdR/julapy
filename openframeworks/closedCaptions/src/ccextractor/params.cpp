#include "ccextractor.h"

int inputfile_capacity=0; 
int spell_builtin_added=0; // so we don't do it twice

const char *DEF_VAL_STARTCREDITSNOTBEFORE="0";
const char *DEF_VAL_STARTCREDITSNOTAFTER="5:00"; // To catch the theme after the teaser in TV shows
const char *DEF_VAL_STARTCREDITSFORATLEAST="2";
const char *DEF_VAL_STARTCREDITSFORATMOST="5";
const char *DEF_VAL_ENDCREDITSFORATLEAST="2";
const char *DEF_VAL_ENDCREDITSFORATMOST="5";

// Some basic English words, so user-defined doesn't have to
// include the common stuff
const char *spell_builtin[]=
{
    "I", "I'd",	"I've",	"I'd", "I'll",
    "January","February","March","April", // May skipped intentionally
    "June","July","August","September","October","November",
    "December","Monday","Tuesday","Wednesday","Thursday",
    "Friday","Saturday","Sunday","Halloween","United States",
    "Spain","France","Italy","England",
    NULL
}; 

int stringztoms (const char *s, boundary_time *bt)
{
    unsigned ss=0, mm=0, hh=0;
    int value=-1;
    int colons=0;
    const char *c=s;
    while (*c)
    {
        if (*c==':')
        {
            if (value==-1) // : at the start, or ::, etc
                return -1;
            colons++;
            if (colons>2) // Max 2, for HH:MM:SS
                return -1;
            hh=mm;
            mm=ss;
            ss=value;
            value=-1;			
        }
        else
        {
            if (!isdigit (*c)) // Only : or digits, so error
                return -1;
            if (value==-1)
                value=*c-'0';
            else
                value=value*10+*c-'0';
        }
        c++;
    }
    hh=mm;
    mm=ss;
    ss=value;
    if (mm>59 || ss>59)
        return -1;
    bt->set=1;
    bt->hh=hh;
    bt->mm=mm;
    bt->ss=ss;	
    LLONG secs=(hh*3600+mm*60+ss);
    bt->time_in_ms=secs*1000;
    return 0;
}


int add_word (const char *word)
{
    char *new_lower;
    char *new_correct;
    if (spell_words==spell_capacity)
    {
        // Time to grow
        spell_capacity+=50;
        spell_lower=(char **) realloc (spell_lower, sizeof (char *) * 
            spell_capacity);
        spell_correct=(char **) realloc (spell_correct, sizeof (char *) * 
            spell_capacity);		
    }
    size_t len=strlen (word);
    new_lower = (char *) malloc (len+1);
    new_correct = (char *) malloc (len+1);
    if (spell_lower==NULL || spell_correct==NULL ||
        new_lower==NULL || new_correct==NULL)
    {        
        return -1;
    }
    strcpy (new_correct, word);
    for (size_t i=0; i<len; i++)
    {
        char c=new_correct[i];
        c=tolower (c); // TO-DO: Add Spanish characters
        new_lower[i]=c;
    }
    new_lower[len]=0;
    spell_lower[spell_words]=new_lower;
    spell_correct[spell_words]=new_correct;
    spell_words++;
    return 0;
}


int add_built_in_words()
{
    if (!spell_builtin_added)
    {
        int i=0;
        while (spell_builtin[i]!=NULL)
        {
            if (add_word(spell_builtin[i]))
                return -1;
            i++;
        }
        spell_builtin_added=1;
    }
    return 0;
}


int process_cap_file (char *filename)
{
    FILE *fi = fopen (filename,"rt");
    if (fi==NULL)
    {
        printf ("\rUnable to open capitalization file: %s\n", filename);
        return -1;
    }
    char line[35]; // For screen width (32)+CRLF+0
    int num=0;
    while (fgets (line,35,fi))
    {
        num++;
        if (line[0]=='#') // Comment
            continue;
        char *c=line+strlen (line)-1;
        while (c>=line && (*c==0xd || *c==0xa))
        {
            *c=0;
            c--;
        }
        if (strlen (line)>32)
        {
            printf ("Word in line %d too long, max = 32 characters.\n",num);
            fclose (fi);
            return -1;
        }
        if (strlen (line)>0)
        {
            if (add_word (line))
                return -1;
        }
    }
    fclose (fi);
    return 0;
}

int isanumber (char *s)
{
    while (*s)
    {
        if (!isdigit (*s))
            return 0;
        s++;
    }
    return 1;
}

int parsedelay (char *par)
{
    int sign=0; 
    char *c=par;
    while (*c)
    {		
        if (*c=='-' || *c=='+')
        {
            if (c!=par) // Sign only at the beginning
                return 1; 
            if (*c=='-')
                sign=1;
        }
        else
        {
            if (!isdigit (*c))
                return 1;
            subs_delay=subs_delay*10 + (*c-'0');			
        }
        c++;
    }
    if (sign)
        subs_delay=-subs_delay;
    return 0;
}

int append_file_to_queue (char *filename)
{
    char *c=(char *) malloc (strlen (filename)+1);    
    if (c==NULL)
        return -1;           
    strcpy (c,filename);
    if (inputfile_capacity<=num_input_files)
    {
        inputfile_capacity+=10;
        inputfile=(char **) realloc (inputfile,sizeof (char *) * inputfile_capacity);
        if (inputfile==NULL)
            return -1;
    }
    inputfile[num_input_files]=c;
    num_input_files++;            
    return 0;
}

int add_file_sequence (char *filename)
{
    int m,n;
    n=strlen (filename)-1;
    // Look for the last digit in filename
    while (n>=0 && !isdigit (filename[n]))
        n--;
    if (n==-1) // None. No expansion needed    
        return append_file_to_queue(filename);
    m=n;
    while (m>=0 && isdigit (filename[m]))
        m--;
    m++;
    // Here: Significant digits go from filename[m] to filename[n]
    char *num=(char *) malloc (n-m+2);
    strncpy (num,filename+m, n-m+1);
    num[n-m+1]=0;
    int i = atoi (num);
    char *temp=(char *) malloc (n-m+3); // For overflows
    // printf ("Expanding %d to %d, initial value=%d\n",m,n,i);
    for (;;)
    {
        FILE *f=fopen (filename,"r");
        if (f==NULL) // Doesn't exist or we can't read it. We're done
            break;
        fclose (f);
        if (append_file_to_queue (filename)) // Memory panic
            return -1;
        i++;
        sprintf (temp,"%d",i);
        if (strlen (temp)>strlen (num)) // From 999 to 1000, etc.
            break;
        strncpy (filename+m+(strlen (num)-strlen (temp)),temp,strlen (temp));
        memset (filename+m,'0',strlen (num)-strlen (temp));        
    }
    return 0;
}

void set_output_format (const char *format)
{
    while (*format=='-')
        format++;
    if (strcmp (format,"srt")==0)
        write_format=OF_SRT;
    else if (strcmp (format,"sami")==0 || strcmp (format,"smi")==0) 
        write_format=OF_SAMI;
    else if (strcmp (format,"transcript")==0 || strcmp (format,"txt")==0)
        write_format=OF_TRANSCRIPT;
    else if (strcmp (format,"raw")==0)
        write_format=OF_RAW;
    else if (strcmp (format,"bin")==0)
        write_format=OF_RCWT;
    else if (strcmp (format,"dvdraw")==0)
    {
        write_format=OF_RAW;
        rawmode=1;
    }
    else
        fatal (EXIT_MALFORMED_PARAMETER, "Unknown output file format: %s\n", format);
}

void set_input_format (const char *format)
{
    while (*format=='-')
        format++;
    if (strcmp (format,"es")==0) // Does this actually do anything?
        auto_stream = SM_ELEMENTARY_OR_NOT_FOUND;
    else if (strcmp (format,"ts")==0)
        auto_stream = SM_TRANSPORT;
    else if (strcmp (format,"ps")==0 || strcmp (format,"nots")==0)
        auto_stream = SM_PROGRAM;
    else if (strcmp (format,"asf")==0 || strcmp (format,"dvr-ms")==0)
        auto_stream = SM_ASF;
    else if (strcmp (format,"raw")==0)
        auto_stream = SM_MCPOODLESRAW;
    else if (strcmp (format,"bin")==0)
        auto_stream = SM_RCWT;
    else
        fatal (EXIT_MALFORMED_PARAMETER, "Unknown input file format: %s\n", format);
}

void usage (void)
{
    printf ("Originally based on McPoodle's tools. Check his page for lots of information\n");
    printf ("on closed captions technical details.\n");
    printf ("(http://www.geocities.com/mcpoodle43/SCC_TOOLS/DOCS/SCC_TOOLS.HTML)\n\n");
    printf ("This tool home page:\n");
    printf ("http://ccextractor.sourceforge.net\n");
    printf ("  Extracts closed captions from MPEG files.\n");
    printf ("    (DVB, .TS, ReplayTV 4000 and 5000, dvr-ms, bttv, Tivo and Dish Network\n");
    printf ("      are known to work).\n\n");
    printf ("  Syntax:\n");
    printf ("  ccextractor [options] inputfile1 [inputfile2...] [-o outputfilename]\n");
    printf ("               [-o1 outputfilename1] [-o2 outputfilename2]\n\n");
    printf ("File name related options:\n");
    printf ("            inputfile: file(s) to process\n");
    printf ("    -o outputfilename: Use -o parameters to define output filename if you don't\n");
    printf ("                       like the default ones (same as infile plus _1 or _2 when\n");
    printf ("                       needed and .raw or .srt extension).\n");
    printf ("                           -o or -o1 -> Name of the first (maybe only) output\n");
    printf ("                                        file.\n");
    printf ("                           -o2       -> Name of the second output file, when\n");
    printf ("                                        it applies.\n");
    printf ("         -cf filename: Write 'clean' data to a file. Cleans means the ES\n");
    printf ("                       without TS or PES headers.\n\n");
    printf ("You can pass as many input files as you need. They will be processed in order.\n");
    printf ("If a file name is suffixed by +, ccextractor will try to follow a numerical\n");
    printf ("sequence. For example, DVD001.VOB+ means DVD001.VOB, DVD002.VOB and so on\n");
    printf ("until there are no more files.\n");
    printf ("Output will be one single file (either raw or srt). Use this if you made your\n");
    printf ("recording in several cuts (to skip commercials for example) but you want one\n");
    printf ("subtitle file with contiguous timing.\n\n");
    printf ("Options that affect what will be processed:\n");
    printf ("          -1, -2, -12: Output Field 1 data, Field 2 data, or both\n");
    printf ("                       (DEFAULT is -1)\n");
    printf ("                 -cc2: When in srt/sami mode, process captions in channel 2\n");
    printf ("                       instead channel 1.\n\n");
    printf ("In general, if you want English subtitles you don't need to use these options\n");
    printf ("as they are broadcast in field 1, channel 1. If you want the second language\n");
    printf ("(usually Spanish) you may need to try -2, or -cc2, or both.\n\n");
    printf ("Input formats:\n");
    printf ("       With the exception of McPoodle's raw format, which is just the closed\n");
    printf ("       caption data with no other info, CCExtractor can usually detect the\n");
    printf ("       input format correctly. To force a specific format:\n\n");
    printf ("                  -in=format\n\n");
    printf ("       where format is one of these:\n");
    printf ("                       ts   -> For Transport Streams.\n");
    printf ("                       ps   -> For Program Streams.\n");
    printf ("                       es   -> For Elementary Streams.\n");
    printf ("                       asf  -> ASF container (such as DVR-MS).\n");
    printf ("                       bin  -> CCExtractor's own binary format.\n\n");
    printf ("                       raw  -> For McPoodle's raw files.\n\n");
    printf ("       -ts, -ps, -es and -asf (or --dvr-ms) can be used as shorts.\n\n");
    printf ("Output formats:\n\n");
    printf ("                 -out=format\n\n");
    printf ("       where format is one of these:\n");
    printf ("                       srt    -> SubRip (default, so not actually needed).\n");
    printf ("                       sami   -> MS Synchronized Accesible Media Interface.\n");
    printf ("                       bin    -> CC data in CCExtractor's own binary format.\n");
    printf ("                       raw    -> CC data in McPoodle's Broadcast format.\n");
    printf ("                       dvdraw -> CC data in McPoodle's DVD format.\n");
    printf ("                       txt    -> Transcript (no time codes, no roll-up\n");
    printf ("                                 captions, just the plain transcription.\n\n");
     
    printf ("Options that affect how input files will be processed.\n");
   
    printf ("        -gt --goptime: Use GOP for timing instead of PTS. This only applies\n");
    printf ("                       to Program or Transport Streams with MPEG2 data and\n");
    printf ("                       overrides the default PTS timing.\n");
    printf ("                       GOP timing is always used for Elementary Streams.\n");
    printf ("     -fp --fixpadding: Fix padding - some cards (or providers, or whatever)\n");
    printf ("                       seem to send 0000 as CC padding instead of 8080. If you\n");
    printf ("                       get bad timing, this might solve it.\n");
    printf ("               -90090: Use 90090 (instead of 90000) as MPEG clock frequency.\n");
    printf ("                       (reported to be needed at least by Panasonic DMR-ES15\n");
    printf ("                       DVD Recorder)\n");
    printf ("    -ve --videoedited: By default, ccextractor will process input files in\n");
    printf ("                       sequence as if they were all one large file (i.e.\n");
    printf ("                       split by a generic, non video-aware tool. If you\n");
    printf ("                       are processing video hat was split with a editing\n");
    printf ("                       tool, use -ve so ccextractor doesn't try to rebuild\n");
    printf ("                       the original timing.\n");
    printf ("   -s --stream [secs]: Consider the file as a continuous stream that is\n");
    printf ("                       growing as ccextractor processes it, so don't try\n");
    printf ("                       to figure out its size and don't terminate processing\n");
    printf ("                       when reaching the current end (i.e. wait for more\n");
    printf ("                       data to arrive). If the optional parameter secs is\n");
    printf ("                       present, it means the number of seconds without any\n");
    printf ("                       new data after which ccextractor should exit. Use\n");
    printf ("                       this parameter if you want to process a live stream\n");
    printf ("                       but not kill ccextractor externally.\n");
    printf ("                       Note: If -s is used then only one input file is\n");
    printf ("                       allowed.\n");
    printf ("                -myth: Force MythTV code branch.\n");
    printf ("              -nomyth: Disable MythTV code branch.\n");
    printf ("The MythTV branch is needed for analog captures where the closed caption\n");
    printf ("data is stored in the VBI, such as those with bttv cards (Hauppage 250 for\n");
    printf ("example). This is is detected automatically so you don't need to worry\n");
    printf ("about this unless autodetection doesn't work for you.\n");    
    printf ("\n");
    printf ("Options that affect what kind of output will be produced:\n");
    printf ("             -unicode: Encode subtitles in Unicode instead of Latin-1\n");
    printf ("                -utf8: Encode subtitles in UTF-8 instead of Latin-1\n");    
    printf ("  -nofc --nofontcolor: For .srt/.sami, don't add font color tags.\n");
    printf ("                -trim: Trim lines.\n");
    printf ("   -dc --defaultcolor: Select a different default color (instead of\n");
    printf ("                       white). This causes all output in .srt/.smi\n");
    printf ("                       files to have a font tag, which makes the files\n");
    printf ("                       larger. Add the color you want in RGB, such as\n");
    printf ("                       -dc #FF0000 for red.\n");
    printf ("    -sc --sentencecap: Sentence capitalization. Use if you hate\n");
    printf ("                       ALL CAPS in subtitles.\n");
    printf ("  --capfile -caf file: Add the contents of 'file' to the list of words\n");
    printf ("                       that must be capitalized. For example, if file\n");
    printf ("                       is a plain text file that contains\n\n");
    printf ("                       Tony\n");
    printf ("                       Alan\n\n");
    printf ("                       Whenever those words are found they will be written\n");
    printf ("                       exactly as they appear in the file.\n");
    printf ("                       Use one line per word. Lines starting with # are\n");
    printf ("                       considered comments and discarded.\n\n");
    printf ("Options that affect how ccextractor reads and writes (buffering):\n");
    printf ("    -bi --bufferinput: Forces input buffering.\n");    
    printf (" -nobi -nobufferinput: Disables input buffering.\n\n");    
    printf ("Note: -bo is only used when writing raw files, not .srt or .sami\n\n");
    printf ("Options that affect the built-in closed caption decoder:\n");
    printf ("                 -dru: Direct Roll-Up. When in roll-up mode, write character by\n");
    printf ("                       character instead of line by line. Note that this\n");
    printf ("                       produces (much) larger files.\n");
    printf ("     -noru --norollup: If you hate the repeated lines caused by the roll-up\n");
    printf ("                       emulation, you can have ccextractor write only one\n");
    printf ("                       line at a time, getting rid of these repeated lines.\n\n");
    printf ("Options that affect timing:\n");
    printf ("            -delay ms: For srt/sami, add this number of milliseconds to\n");
    printf ("                       all times. For example, -delay 400 makes subtitles\n");
    printf ("                       appear 400ms late. You can also use negative numbers\n");
    printf ("                       to make subs appear early.\n");
    printf ("Notes on times: -startat and -endat times are used first, then -delay.\n");
    printf ("So if you use -srt -startat 3:00 -endat 5:00 -delay 120000, ccextractor will\n");
    printf ("generate a .srt file, with only data from 3:00 to 5:00 in the input file(s)\n");
    printf ("and then add that (huge) delay, which would make the final file start at\n");
    printf ("5:00 and end at 7:00.\n\n");
    printf ("Options that affect what segment of the input file(s) to process:\n");
    printf ("        -startat time: Only write caption information that starts after the\n");
    printf ("                       given time.\n");
    printf ("                       Time can be seconds, MM:SS or HH:MM:SS.\n");
    printf ("                       For example, -startat 3:00 means 'start writing from\n");
    printf ("                       minute 3.\n");
    printf ("          -endat time: Stop processing after the given time (same format as\n");
    printf ("                       -startat).\n");
    printf ("                       The -startat and -endat options are honored in all\n");
    printf ("                       output formats.  In all formats with timing information\n");
    printf ("                       the times are unchanged.\n");
    printf ("-scr --screenfuls num: Write 'num' screenfuls and terminate processing.\n\n");
    printf ("Adding start and end credits:\n");
    printf ("  CCExtractor can _try_ to add a custom message (for credits for example) at\n");
    printf ("  the start and end of the file, looking for a window where there are no\n");
    printf ("  captions. If there is no such window, then no text will be added.\n");
    printf ("  The start window must be between the times given and must have enough time\n");
    printf ("  to display the message for at least the specified time.\n");
    printf ("        --startcreditstext txt: Write this text as start credits. If there are\n");
    printf ("                                several lines, separate them with the\n");
    printf ("                                characters \\n, for example Line1\\nLine 2.\n");
    printf ("  --startcreditsnotbefore time: Don't display the start credits before this\n");
    printf ("                                time (S, or MM:SS). Default: %s\n", DEF_VAL_STARTCREDITSNOTBEFORE);
    printf ("   --startcreditsnotafter time: Don't display the start credits after this\n");
    printf ("                                time (S, or MM:SS). Default: %s\n", DEF_VAL_STARTCREDITSNOTAFTER);
    printf (" --startcreditsforatleast time: Start credits need to be displayed for at least\n");
    printf ("                                this time (S, or MM:SS). Default: %s\n", DEF_VAL_STARTCREDITSFORATLEAST);
    printf ("  --startcreditsforatmost time: Start credits should be displayed for at most\n");
    printf ("                                this time (S, or MM:SS). Default: %s\n", DEF_VAL_STARTCREDITSFORATMOST);
    printf ("          --endcreditstext txt: Write this text as end credits. If there are\n");
    printf ("                                several lines, separate them with the\n");
    printf ("                                characters \\n, for example Line1\\nLine 2.\n");
    printf ("   --endcreditsforatleast time: End credits need to be displayed for at least\n");
    printf ("                                this time (S, or MM:SS). Default: %s\n", DEF_VAL_ENDCREDITSFORATLEAST);
    printf ("    --endcreditsforatmost time: End credits should be displayed for at most\n");
    printf ("                                this time (S, or MM:SS). Default: %s\n", DEF_VAL_ENDCREDITSFORATMOST);
    printf ("\n");
    printf ("Options that affect debug data:\n");
    printf ("               -debug: Show lots of debugging output.\n");
    printf ("                 -608: Print debug traces from the EIA-608 decoder.\n");
    printf ("                       If you need to submit a bug report, please send\n");
    printf ("                       the output from this option.\n");
    printf ("                 -708: Print debug information from the (currently\n");
    printf ("                       in development and useless) EIA-708 (DTV) decoder.\n");
    printf ("              -goppts: Enable lots of time stamp output.\n");
    printf ("               -vides: Print debug info about the analysed elementary\n");
    printf ("                       video stream.\n");
    printf ("               -cbraw: Print debug trace with the raw 608/708 data with\n");
    printf ("                       time stamps.\n");
    printf ("              -nosync: Disable the syncing code.  Only useful for debugging\n");
    printf ("                       purposes.\n");
    printf ("             -fullbin: Disable the removal of trailing padding blocks\n");
    printf ("                       when exporting to bin format.  Only useful for\n");
    printf ("                       for debugging purposes.\n");
    printf ("          -parsedebug: Print debug info about the parsed container\n");
    printf ("                       file. (Only for TS/ASF files at the moment.)\n\n");
    printf ("Communication with other programs and console output:\n");
    printf ("   --gui_mode_reports: Report progress and interesting events to stderr\n");
    printf ("                       in a easy to parse format. This is intended to be\n");
    printf ("                       used by other programs. See docs directory for.\n");
    printf ("                       details.\n");
    printf ("    --no_progress_bar: Suppress the output of the progress bar\n");
    printf ("\n");
}

void parse_parameters (int argc, char *argv[])
{
    // Sensible default values for credits
    stringztoms (DEF_VAL_STARTCREDITSNOTBEFORE, &startcreditsnotbefore);
    stringztoms (DEF_VAL_STARTCREDITSNOTAFTER, &startcreditsnotafter); 
    stringztoms (DEF_VAL_STARTCREDITSFORATLEAST, &startcreditsforatleast);
    stringztoms (DEF_VAL_STARTCREDITSFORATMOST, &startcreditsforatmost);
    stringztoms (DEF_VAL_ENDCREDITSFORATLEAST, &endcreditsforatleast);
    stringztoms (DEF_VAL_ENDCREDITSFORATMOST, &endcreditsforatmost);

    // Parse parameters
    for (int i=1; i<argc; i++)
    {
        if (argv[i][0]!='-')
        {            
            int rc;
            if (argv[i][strlen (argv[i])-1]!='+')
            {
                rc=append_file_to_queue (argv[i]);
            }
            else
            {
                argv[i][strlen (argv[i])-1]=0;
                rc=add_file_sequence (argv[i]);
            }
            if (rc)
            {
                fatal (EXIT_NOT_ENOUGH_MEMORY, "Fatal: Not enough memory.\n");                
            }
        }
        if (strcmp (argv[i],"-bi")==0 ||
            strcmp (argv[i],"--bufferinput")==0)
            buffer_input = 1;
        if (strcmp (argv[i],"-nobi")==0 ||
            strcmp (argv[i],"--nobufferinput")==0)
            buffer_input = 0;
        if (strcmp (argv[i],"-dru")==0)
            direct_rollup = 1;
        if (strcmp (argv[i],"-nofc")==0 ||
            strcmp (argv[i],"--nofontcolor")==0)
            nofontcolor=1;
            
        /* Input file formats */
        if (    strcmp (argv[i],"-es")==0 ||
                strcmp (argv[i],"-ts")==0 ||
                strcmp (argv[i],"-ps")==0 ||
                strcmp (argv[i],"-nots")==0 ||
                strcmp (argv[i],"-asf")==0 ||
                strcmp (argv[i],"--dvr-ms")==0 )                
            set_input_format (argv[i]); 
        if (strncmp (argv[i],"-in=", 4)==0)
            set_input_format (argv[i]+4);
            
        /* Output file formats */        
        if (strcmp (argv[i],"-srt")==0 ||
           strcmp (argv[i],"-sami")==0 || strcmp (argv[i],"-smi")==0 ||
           strcmp (argv[i],"--transcript")==0 || strcmp (argv[i],"-txt")==0)
           set_output_format (argv[i]);
        if (strncmp (argv[i],"-out=", 5)==0)
            set_output_format (argv[i]+5);
        
        /* Credit stuff */
        if ((strcmp (argv[i],"--startcreditstext")==0)
            && i<argc-1)
        {
            start_credits_text=argv[i+1];
            i++;
        }
        if ((strcmp (argv[i],"--startcreditsnotbefore")==0)
            && i<argc-1)
        {
            if (stringztoms (argv[i+1],&startcreditsnotbefore)==-1)
            {
                fatal (EXIT_MALFORMED_PARAMETER, "--startcreditsnotbefore only accepts SS, MM:SS or HH:MM:SS\n");
            }
            i++;
        }
        if ((strcmp (argv[i],"--startcreditsnotafter")==0)
            && i<argc-1)
        {
            if (stringztoms (argv[i+1],&startcreditsnotafter)==-1)
            {
                fatal (EXIT_MALFORMED_PARAMETER, "--startcreditsnotafter only accepts SS, MM:SS or HH:MM:SS\n");
            }
            i++;
        }
        if ((strcmp (argv[i],"--startcreditsforatleast")==0)
            && i<argc-1)
        {
            if (stringztoms (argv[i+1],&startcreditsforatleast)==-1)
            {
                fatal (EXIT_MALFORMED_PARAMETER, "--startcreditsforatleast only accepts SS, MM:SS or HH:MM:SS\n");
            }
            i++;
        }
        if ((strcmp (argv[i],"--startcreditsforatmost")==0)
            && i<argc-1)
        {
            if (stringztoms (argv[i+1],&startcreditsforatmost)==-1)
            {
                fatal (EXIT_MALFORMED_PARAMETER, "--startcreditsforatmost only accepts SS, MM:SS or HH:MM:SS\n");
            }
            i++;
        }
        
        if  ((strcmp (argv[i],"--endcreditstext")==0 )
            && i<argc-1)
        {
            end_credits_text=argv[i+1];
            i++;
        }
        if ((strcmp (argv[i],"--endcreditsforatleast")==0)
            && i<argc-1)
        {
            if (stringztoms (argv[i+1],&endcreditsforatleast)==-1)
            {
                fatal (EXIT_MALFORMED_PARAMETER, "--endcreditsforatleast only accepts SS, MM:SS or HH:MM:SS\n");
            }
            i++;
        }
        if ((strcmp (argv[i],"--endcreditsforatmost")==0)
            && i<argc-1)
        {
            if (stringztoms (argv[i+1],&endcreditsforatmost)==-1)
            {
                fatal (EXIT_MALFORMED_PARAMETER, "--startcreditsforatmost only accepts SS, MM:SS or HH:MM:SS\n");
            }
            i++;
        }

        /* More stuff */
        if (strcmp (argv[i],"-ve")==0 ||
            strcmp (argv[i],"--videoedited")==0)
            binary_concat=0;
        if (strcmp (argv[i],"-12")==0)
            extract = 12;
        if (strcmp (argv[i],"-gt")==0 || 
            strcmp (argv[i],"--goptime")==0)
            use_gop_as_pts = 1;
        if (strcmp (argv[i],"-fp")==0 || 
            strcmp (argv[i],"--fixpadding")==0)
            fix_padding = 1;
        if (strcmp (argv[i],"-90090")==0)
            MPEG_CLOCK_FREQ=90090;
        if (strcmp (argv[i],"-noru")==0 || 
            strcmp (argv[i],"--norollup")==0)
            norollup = 1;
        if (strcmp (argv[i],"-trim")==0)
            trim_subs=1;
        if (strcmp (argv[i],"--gui_mode_reports")==0)
        {
            gui_mode_reports=1;
            no_progress_bar=1;
            // Do it as soon as possible, because it something fails we might not have a chance
            activity_report_version(); 
        }
        if (strcmp (argv[i],"--no_progress_bar")==0)
            no_progress_bar=1;
        if (strcmp (argv[i],"--sentencecap")==0 ||
            strcmp (argv[i],"-sc")==0)
        {
            if (add_built_in_words())
                fatal (EXIT_NOT_ENOUGH_MEMORY, "Not enough memory for word list");
            sentence_cap=1;
        }
        if ((strcmp (argv[i],"--capfile")==0 ||
            strcmp (argv[i],"-caf")==0)
            && i<argc-1)
        {
            if (add_built_in_words())
                fatal (EXIT_NOT_ENOUGH_MEMORY, "Not enough memory for word list");
            if (process_cap_file (argv[i+1])!=0)
                fatal (EXIT_ERROR_IN_CAPITALIZATION_FILE, "There was an error processing the capitalization file.\n");
            sentence_cap=1;
            sentence_cap_file=argv[i+1];
            i++;
        }
        if (strcmp (argv[i],"--stream")==0 ||
            strcmp (argv[i],"-s")==0)
        {
            if (i==argc-1 // Means no following argument 
                || !isanumber (argv[i+1])) // Means is not a number
                live_stream=-1; // Live stream without timeout
            else
            {
                live_stream=atoi (argv[i+1]);
                i++;
            }
        }
        if ((strcmp (argv[i],"--defaultcolor")==0 ||
            strcmp (argv[i],"-dc")==0)
            && i<argc-1)
        {
            if (strlen (argv[i+1])!=7 || argv[i+1][0]!='#')
            {
                fatal (EXIT_MALFORMED_PARAMETER, "--defaultcolor expects a 7 character parameter that starts with #\n");                
            }
            strcpy ((char *) usercolor_rgb,argv[i+1]);
            default_color=COL_USERDEFINED;
            i++;
        }
        if (strcmp (argv[i],"-delay")==0 && i<argc-1)
        {
            if (parsedelay (argv[i+1]))
            {
                fatal (EXIT_MALFORMED_PARAMETER, "-delay only accept integers (such as -300 or 300)\n");                
            }
            i++;
        }
        if ((strcmp (argv[i],"-scr")==0 || 
            strcmp (argv[i],"--screenfuls")==0) && i<argc-1)
        {
            screens_to_process=atoi (argv[i+1]);
            if (screens_to_process<0)
            {
                fatal (EXIT_MALFORMED_PARAMETER, "--screenfuls only accepts positive integers.\n");                
            }
            i++;
        }
        if (strcmp (argv[i],"-startat")==0 && i<argc-1)
        {
            if (stringztoms (argv[i+1],&extraction_start)==-1)
            {
                fatal (EXIT_MALFORMED_PARAMETER, "-startat only accepts SS, MM:SS or HH:MM:SS\n");
            }
            i++;
        }
        if (strcmp (argv[i],"-endat")==0 && i<argc-1)
        {
            if (stringztoms (argv[i+1],&extraction_end)==-1)
            {
                fatal (EXIT_MALFORMED_PARAMETER, "-endat only accepts SS, MM:SS or HH:MM:SS\n");                
            }
            i++;
        }		
        if (strcmp (argv[i],"-1")==0)
            extract = 1;
        if (strcmp (argv[i],"-2")==0)
            extract = 2;
        if (strcmp (argv[i],"-cc2")==0 || strcmp (argv[i],"-CC2")==0)
            cc_channel=2;
        if (strcmp (argv[i],"-debug")==0)
            debug_verbose = 1;
        if (strcmp (argv[i],"-608")==0)
            debug_608 = 1;
        if (strcmp (argv[i],"-708")==0)
            debug_708 = 1;            
        if (strcmp (argv[i],"-goppts")==0) 
            debug_time = 1;
        if (strcmp (argv[i],"-vides")==0)
            debug_vides = 1;
        if (strcmp (argv[i],"-parsedebug")==0)
            debug_parse = 1;
        if (strcmp (argv[i],"-cbraw")==0)
            debug_cbraw = 1;
        if (strcmp (argv[i],"-fullbin")==0)
            fullbin = 1;
        if (strcmp (argv[i],"-nosync")==0)
            nosync = 1;
        if (strstr (argv[i],"-unicode")!=NULL)
            encoding=ENC_UNICODE;
        if (strstr (argv[i],"-utf8")!=NULL)
            encoding=ENC_UTF_8;
        if (strstr (argv[i],"-myth")!=NULL)
            auto_myth=1;
        if (strstr (argv[i],"-nomyth")!=NULL)
            auto_myth=0;
        if (strcmp (argv[i],"-o")==0 && i<argc-1)
        {
            output_filename=argv[i+1];
            i++;
        }
        if (strcmp (argv[i],"-cf")==0 && i<argc-1)
        {
            clean_filename=argv[i+1];
            i++;
        }
        if (strcmp (argv[i],"-o1")==0 && i<argc-1)
        {
            wbout1.filename=argv[i+1];
            i++;
        }
        if (strcmp (argv[i],"-o2")==0 && i<argc-1)
        {
            wbout2.filename=argv[i+1];
            i++;
        }
    }
}

