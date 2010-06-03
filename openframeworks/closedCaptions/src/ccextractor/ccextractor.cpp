/* CCExtractor, cfsmp3 at gmail
Credits: See CHANGES.TXT
License: GPL 2.0
*/
#include <stdio.h>
#include "ccextractor.h"
#include <sys/types.h>
#include <sys/stat.h>


extern unsigned char *filebuffer;
extern int bytesinbuffer; // Number of bytes we actually have on buffer

// PTS timing related stuff
LLONG min_pts, max_pts, sync_pts;
LLONG fts_now; // Time stamp of current file (w/ fts_offset, w/o fts_global)
LLONG fts_offset; // Time before first sync_pts
LLONG fts_fc_offset; // Time before first GOP
LLONG fts_max; // Remember the maximum fts that we saw in current file
LLONG fts_global=0; // Duration of previous files (-ve mode), see c1global
// Count 608 (per field) and 708 blocks since last set_fts() call
int cb_field1, cb_field2, cb_708;
int saw_caption_block;

int pts_set; //0 = No, 1 = received, 2 = min_pts set

unsigned pts_big_change;

int MPEG_CLOCK_FREQ = 90000; // This "constant" is part of the standard

// Stuff common to both loops
unsigned char *buffer = NULL;
LLONG past; /* Position in file, if in sync same as ftell()  */
unsigned char *pesheaderbuf = NULL;
LLONG inputsize;
LLONG total_inputsize=0, total_past=0; // Only in binary concat mode

int last_reported_progress;
int processed_enough; // If 1, we have enough lines, time, etc. 

int live_stream=0; /* -1 -> Not a complete file but a live stream, without timeout
                       0 -> A regular file 
                      >0 -> Live stream with a timeout of this value in seconds */
 
// Small buffer to help us with the initial sync
unsigned char startbytes[STARTBYTESLENGTH]; 
unsigned int startbytes_pos;
int startbytes_avail;

/* Stats */
int stat_numuserheaders;
int stat_dvdccheaders;
int stat_scte20ccheaders;
int stat_replay5000headers;
int stat_replay4000headers;
int stat_dishheaders;
int stat_hdtv;
int stat_divicom;
unsigned total_frames_count;
unsigned total_pulldownfields;
unsigned total_pulldownframes;
int cc_stats[4];
int false_pict_header;
int resets_708=0;

/* GOP-based timing */
struct gop_time_code gop_time, first_gop_time, printed_gop;
int saw_gop_header=0;
int frames_since_last_gop=0;
LLONG fts_at_gop_start=0;

int max_gop_length=0; // (Maximum) length of a group of pictures
int last_gop_length=0; // Length of the previous group of pictures
int frames_since_ref_time=0;

int gop_rollover=0;

/* Detect gaps in caption stream - only used for dvr-ms/NTSC. */
int CaptionGap=0;

/* Parameters */
int buffer_output = 0; // Buffer output? (doesn't help much)
#ifdef _WIN32
int buffer_input = 1; // In Windows buffering seems to help
#else
int buffer_input = 0; // In linux, not so much.
#endif
int ff_cleanup = 1; // Verify end of CC block marker?
stream_mode_enum stream_mode = SM_ELEMENTARY_OR_NOT_FOUND; // Data parse mode: 0=elementary, 1=transport, 2=program stream, 3=ASF container
stream_mode_enum auto_stream = SM_AUTODETECT;
int use_gop_as_pts = 0; // Use GOP instead of PTS timing
int fix_padding = 0; // Replace 0000 with 8080 in HDTV (needed for some cards)
int rawmode = 0; // Broadcast or DVD
// See -d from http://www.geocities.com/mcpoodle43/SCC_TOOLS/DOCS/SCC_TOOLS.HTML#CCExtract
int extract = 1; // Extract 1st, 2nd or both fields
int cc_channel = 1; // Channel we want to dump in srt mode
int debug_verbose = 0; // Show lots of debugging output
int debug_608=0; // Show CC-608 decoder debug? 
int debug_708=0; // Show CC-708 decoder debug? 
int debug_time=0; // Show GOP and PTS timing information
int debug_vides=0; // Show video stream related information
int debug_parse=0; // Show information related to parsing the container
int debug_cbraw=0; // Caption blocks with FTS timing
int nosync=0; // Disable syncing
int fullbin=0; // Disable pruning of padding cc blocks
LLONG subs_delay=0; // ms to delay (or advance) subs
int trim_subs=0; // "    Remove spaces at sides?    "
struct boundary_time extraction_start, extraction_end; // Segment we actually process
struct boundary_time startcreditsnotbefore, startcreditsnotafter; // Where to insert start credits, if possible
struct boundary_time startcreditsforatleast, startcreditsforatmost; // How long to display them?
struct boundary_time endcreditsforatleast, endcreditsforatmost;
int startcredits_displayed=0, end_credits_displayed=0;
LLONG last_displayed_subs_ms=0; // When did the last subs end?
LLONG screens_to_process=-1; // How many screenfuls we want?
char *basefilename=NULL; // Input filename without the extension
char **inputfile=NULL; // List of files to process
const char *extension; // Output extension
int current_file=-1; // If current_file!=1, we are processing *inputfile[current_file]
int direct_rollup=0; // Write roll-up captions directly instead of line by line?
int num_input_files=0; // How many?

int nofontcolor=0; // 1 = don't put <font color> tags 
output_format write_format=OF_SRT; // 0=Raw, 1=srt, 2=SMI
encoding_type encoding = ENC_LATIN_1;
int auto_myth = 2; // Use myth-tv mpeg code? 0=no, 1=yes, 2=auto
int sentence_cap =0 ; // FIX CASE? = Fix case?
char *sentence_cap_file=NULL; // Extra words file?
int binary_concat=1; // Disabled by -ve or --videoedited
int norollup=0; // If 1, write one line at a time
int gui_mode_reports=0; // If 1, output in stderr progress updates so the GUI can grab them
int no_progress_bar=0; // If 1, suppress the output of the progress to stdout
char *output_filename=NULL;
char *clean_filename=NULL;

// Case arrays
char **spell_lower=NULL;
char **spell_correct=NULL;
int spell_words=0;
int spell_capacity=0;

/* Credit stuff */
char *start_credits_text=NULL;
char *end_credits_text=NULL;

// RCWT header (11 bytes):
//byte(s)   value   description (All values below are hex numbers, not
//                  actual numbers or values
//0-2       CCCCED  magic number, for Closed Caption CC Extractor Data
//3         CC      Creating program.  Legal values: CC = CC Extractor
//4-5       0050    Program version number
//6-7       0001    File format version
//8-10      000000  Padding, required  :-)
const unsigned char rcwt_header[11]={0xCC, 0xCC, 0xED, 0xCC, 0x00, 0x50, 0, 1, 0, 0, 0};

struct s_write wbout1, wbout2; // Output structures

const unsigned char BROADCAST_HEADER[]={0xff, 0xff, 0xff, 0xff};
const unsigned char LITTLE_ENDIAN_BOM[]={0xff, 0xfe};

const unsigned char DVD_HEADER[8]={0x00,0x00,0x01,0xb2,0x43,0x43,0x01,0xf8};
const unsigned char lc1[1]={0x8a};
const unsigned char lc2[1]={0x8f};
const unsigned char lc3[2]={0x16,0xfe};
const unsigned char lc4[2]={0x1e,0xfe};
const unsigned char lc5[1]={0xff};
const unsigned char lc6[1]={0xfe};

/* For XMLRPC connection with opensubtitles.org - we build the content
body here */
char *xmlrpc_content_body=NULL;
long xmlrpc_content_length=0;
long xmlrpc_content_capacity=0;
const char *xmlrpc_url="http://test.opensubtitles.org/xml-rpc";

/* File handles */
FILE *clean;
int infd=-1; // descriptor number to input. Set to -1 to indicate no file is open.

typedef enum
{
    NTSC_CC_f1         = 0,
    NTSC_CC_f2         = 1,
    DTVCC_PACKET_DATA  = 2,
    DTVCC_PACKET_START = 3,
};

const double framerates_values[16]=
{
    0,
    24000.0/1001, /* 23.976 */
    24.0,
    25.0,
    30000.0/1001, /* 29.97 */
    30.0,
    50.0,
    60000.0/1001, /* 59.94 */
    60.0,
    0,
    0,
    0,
    0,
    0
};

const char *framerates_types[16]=
{
    "00 - forbidden",
    "01 - 23.976",
    "02 - 24",
    "03 - 25",
    "04 - 29.97",
    "05 - 30",
    "06 - 50",
    "07 - 59.94",
    "08 - 60",
    "09 - reserved",
    "10 - reserved",
    "11 - reserved",
    "12 - reserved",
    "13 - reserved",
    "14 - reserved",
    "15 - reserved"
};

const char *aspect_ratio_types[16]=
{
    "00 - forbidden",
    "01 - 1:1",
    "02 - 4:3",
    "03 - 16:9",
    "04 - 2.21:1",
    "05 - reserved",
    "06 - reserved",
    "07 - reserved",
    "08 - reserved",
    "09 - reserved",
    "10 - reserved",
    "11 - reserved",
    "12 - reserved",
    "13 - reserved",
    "14 - reserved",
    "15 - reserved"
};


const char *pict_types[8]=
{
    "00 - ilegal (0)",
    "01 - I",
    "02 - P",
    "03 - B",
    "04 - ilegal (D)",
    "05 - ilegal (5)",
    "06 - ilegal (6)",
    "07 - ilegal (7)"
};

const char *cc_types[4] =
{
    "NTSC line 21 field 1 closed captions",
    "NTSC line 21 field 2 closed captions",
    "DTVCC Channel Packet Data",
    "DTVCC Channel Packet Start"
};



void xmlrpc_append (char *data, long length)
{
    if (xmlrpc_content_length+length>xmlrpc_content_capacity)
    {
        int extra_length=length>XMLRPC_CHUNK_SIZE?length:XMLRPC_CHUNK_SIZE;
        xmlrpc_content_capacity+=extra_length;
        xmlrpc_content_body=(char *) realloc (xmlrpc_content_body,xmlrpc_content_capacity);
        if (xmlrpc_content_body==NULL) // PANIC: Not enough memory
        {
            fatal (EXIT_NOT_ENOUGH_MEMORY, "Fatal: Not enough memory at xmlrpc_append, bailing out!\n");            
        }
    }
    memcpy (xmlrpc_content_body+xmlrpc_content_length,data,length);
    xmlrpc_content_length+=length;
}



void dump (unsigned char *start, int l)
{
    for (int x=0; x<l; x=x+16)
    {
        printf ("%03d | ",x);
        for (int j=0; j<16; j++)
        {
            if (x+j<l)
                printf ("%02X ",start[x+j]);
            else
                printf ("   ");
        }
        printf (" | ");
        for (int j=0; j<16; j++)
        {
            if (x+j<=l && start[x+j]>=' ')
                printf ("%c",start[x+j]);
            else
                printf (" ");
        }
        printf ("\n");
    }
}




void init_boundary_time (boundary_time *bt)
{
    bt->hh=0;
    bt->mm=0;
    bt->ss=0;
    bt->set=0;
    bt->time_in_ms=0;
}


void sleep_secs (int secs)
{
#ifdef _WIN32
	Sleep(secs * 1000);
#else
	sleep(secs);
#endif
}


/* Write formatted message to stderr and then exit. */
void fatal(int exit_code, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (gui_mode_reports)
        fprintf(stderr,"###MESSAGE#");
    else
        fprintf(stderr, "\rError: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);    
    exit(exit_code);
}

int main(int argc, char *argv[])
{
    char *c;

    activity_header();

    // Initialize some constants
    init_ts_constants();

    // Prepare write structures
    init_write(&wbout1);
    init_write(&wbout2);

    // Prepare time structures
    init_boundary_time (&extraction_start);
    init_boundary_time (&extraction_end);
    init_boundary_time (&startcreditsnotbefore);
    init_boundary_time (&startcreditsnotafter);
    init_boundary_time (&startcreditsforatleast);
    init_boundary_time (&startcreditsforatmost);
    init_boundary_time (&endcreditsforatleast);
    init_boundary_time (&endcreditsforatmost);

    int show_myth_banner = 0;
    
    parse_parameters (argc,argv);

    if (num_input_files==0)
    {
        usage ();
        fatal (EXIT_NO_INPUT_FILES, "(This help screen was shown because there were no input files)\n");        
    }
    if (num_input_files>1 && live_stream)
    {
        fatal(EXIT_TOO_MANY_INPUT_FILES, "Live stream mode accepts only one input file.\n");        
    }
    if (output_filename!=NULL)
    {
        // Use the given output file name for the field specified by
        // the -1, -2 switch. If -12 is used, the filename is used for
        // field 1.
        if (extract==2)
            wbout2.filename=output_filename;
        else
            wbout1.filename=output_filename;
    }

    switch (write_format)
    {
        case OF_RAW:
            extension = ".raw";
            break;
        case OF_SRT:
            extension = ".srt";
            break;
        case OF_SAMI:
            extension = ".smi";
            break;
        case OF_TRANSCRIPT:
            extension = ".txt";
            break;
        case OF_RCWT:
            extension = ".bin";
            break;
        default:
            fatal (EXIT_BUG_BUG, "write_format doesn't have any legal value, this is a bug.\n");            
    }
    params_dump();

    if (auto_stream==SM_MCPOODLESRAW && write_format==OF_RAW)
    {
        fatal (EXIT_INCOMPATIBLE_PARAMETERS, "-in=raw can only be used if the output is a subtitle file.\n");
    }
    if (auto_stream==SM_RCWT && write_format==OF_RCWT && output_filename==NULL)
    {
        fatal (EXIT_INCOMPATIBLE_PARAMETERS,
               "CCExtractor's binary format can only be used simultaneously for input and\noutput if the output file name is specified given with -o.\n");
    }

    buffer = (unsigned char *) malloc (BUFSIZE);
    subline = (unsigned char *) malloc (SUBLINESIZE);
    pesheaderbuf = (unsigned char *) malloc (188); // Never larger anyway

    basefilename = (char *) malloc (strlen (inputfile[0])+1);

    if (wbout1.filename==NULL)
    {
        wbout1.filename = (char *) malloc (strlen (inputfile[0])+3+strlen (extension)); 
        wbout1.filename[0]=0;
    }
    if (wbout2.filename==NULL)
    {
        wbout2.filename = (char *) malloc (strlen (inputfile[0])+3+strlen (extension));
        wbout2.filename[0]=0;
    }
    if (buffer == NULL || basefilename == NULL || pesheaderbuf==NULL ||
        wbout1.filename == NULL || wbout2.filename == NULL ||
        subline==NULL || init_file_buffer() || general_608_init())
    {
        fatal (EXIT_NOT_ENOUGH_MEMORY, "Not enough memory\n");        
    }

    strcpy (basefilename, inputfile[0]);
    for (c=basefilename+strlen (basefilename)-1; c>basefilename &&
        *c!='.'; c--) {;} // Get last .
    if (*c=='.')
        *c=0;
    /* # DVD format uses one raw file for both fields, while Broadcast requires 2 */
    if (rawmode==1)
    {
        if (wbout1.filename[0]==0)
        {
            strcpy (wbout1.filename,basefilename);
            strcat (wbout1.filename,".raw");
        }
        printf ("Creating %s\n", wbout1.filename);
        wbout1.fh=FOPEN64 (wbout1.filename, "wb");
        if (wbout1.fh==NULL)
        {
            fatal (EXIT_FILE_CREATION_FAILED, "Failed\n");
        }
    }
    else
    {
        if (extract!=2)
        {
            if (wbout1.filename[0]==0)
            {
                strcpy (wbout1.filename,basefilename);
                if (extract==12) // _1 only added if there's two files
                    strcat (wbout1.filename,"_1");
                strcat (wbout1.filename,(const char *) extension);
            }
            printf ("Creating %s\n", wbout1.filename);
            wbout1.fh=FOPEN64 (wbout1.filename, "wb");
            if (wbout1.fh==NULL)
            {
                fatal (EXIT_FILE_CREATION_FAILED, "Failed\n");
            }
            if (write_format==OF_RAW)
                writeraw (BROADCAST_HEADER,sizeof (BROADCAST_HEADER),&wbout1);
            else
            {
                if (encoding==ENC_UNICODE) // Write BOM				
                    writeraw (LITTLE_ENDIAN_BOM, sizeof (LITTLE_ENDIAN_BOM), &wbout1);
                write_subtitle_file_header (&wbout1);
            }

        }
        if (extract == 12) 
            printf (" and \n");
        if (extract!=1)
        {
            if (wbout2.filename[0]==0)
            {
                strcpy (wbout2.filename,basefilename);				
                if (extract==12) // _ only added if there's two files
                    strcat (wbout2.filename,"_2");
                strcat (wbout2.filename,(const char *) extension);
            }
            printf ("Creating %s\n", wbout2.filename);
            wbout2.fh=FOPEN64 (wbout2.filename, "wb");
            if (wbout2.fh==NULL)
            {
                fatal (EXIT_FILE_CREATION_FAILED, "Failed\n");                
            }
            if (write_format==OF_RAW)
                writeraw (BROADCAST_HEADER,sizeof (BROADCAST_HEADER),&wbout2);
            else
            {
                if (encoding==ENC_UNICODE) // Write BOM				
                    writeraw (LITTLE_ENDIAN_BOM, sizeof (LITTLE_ENDIAN_BOM), &wbout1);
                write_subtitle_file_header (&wbout2);
            }
        }
    }
    clean = NULL;
    if (clean_filename!=NULL)
    {
        if ((clean = fopen (clean_filename,"wb"))==NULL)
        {
            fatal (EXIT_FILE_CREATION_FAILED, "Unable to open clean file: %s\n",clean_filename);
        }
    }
    encoded_crlf_length = encode_line (encoded_crlf,(unsigned char *) "\r\n"); 
    encoded_br_length = encode_line (encoded_br, (unsigned char *) "<br>");
    build_parity_table();

    // Initialize HDTV caption buffer
    init_hdcc();
    init_708(); // Init 708 decoders

    time_t start, final;
    time(&start);

    processed_enough=0;
    if (binary_concat)
    {
        total_inputsize=gettotalfilessize();
        if (total_inputsize==-1)
            fatal (EXIT_UNABLE_TO_DETERMINE_FILE_SIZE, "Failed to determine total file size.\n");
    }
    
    while (switch_to_next_file(0) && !processed_enough)
    {
        prepare_for_new_file();

        if (auto_stream == SM_AUTODETECT)
        {
            detect_stream_type();            
            switch (stream_mode)
            {
                case SM_ELEMENTARY_OR_NOT_FOUND:
                    printf ("\rFile seems to be an elementary stream, enabling ES mode\n");
                    break;
                case SM_TRANSPORT:
                    printf ("\rFile seems to be a transport stream, enabling TS mode\n");
                    break;
                case SM_PROGRAM:
                    printf ("\rFile seems to be a program stream, enabling PS mode\n");
                    break;
                case SM_ASF:
                    printf ("\rFile seems to be an ASF, enabling DVR-MS mode\n");
                    break;
                case SM_MCPOODLESRAW:
                    printf ("\rFile seems to be McPoodle raw data\n");
                    break;
                case SM_RCWT:
                    printf ("\rFile seems to be a raw caption with time data\n");
                    break;
                case SM_MYTH:
                case SM_AUTODETECT:
                    fatal(EXIT_BUG_BUG, "Cannot be reached!");
                    break;
            }
        }
        else
        {
            stream_mode=auto_stream;
        }

        /* -----------------------------------------------------------------
        MAIN LOOP
        ----------------------------------------------------------------- */

        // The myth loop autodetect will only be used with ES or PS streams
        switch (auto_myth)
        {
            case 0:
                // Use whatever stream mode says
                break;
            case 1:
                // Force stream mode to myth
                stream_mode=SM_MYTH;
                break;
            case 2:
                // autodetect myth files, but only if it does not conflict with
                // the current stream mode
                switch (stream_mode)
                {
                    case SM_ELEMENTARY_OR_NOT_FOUND:
                    case SM_PROGRAM:
                        if ( detect_myth() )
                        {
                            stream_mode=SM_MYTH;
                        }
                        break;
                    default:
                        // Keep stream_mode
                        break;
                }
                break;                    
        }
                
        switch (stream_mode)
        {
            case SM_ELEMENTARY_OR_NOT_FOUND:
                use_gop_as_pts = 1; // Force GOP timing for ES
            case SM_TRANSPORT:
            case SM_PROGRAM:
            case SM_ASF:
                printf ("\rAnalyzing data in general mode\n");
                general_loop();
                break;
            case SM_MCPOODLESRAW:
                printf ("\rAnalyzing data in McPoodle raw mode\n");
                raw_loop();
                break;
            case SM_RCWT:
                printf ("\rAnalyzing data in CCExtractor's binary format\n");
                rcwt_loop();
                break;
            case SM_MYTH:
                printf ("\rAnalyzing data in MythTV mode\n");
                show_myth_banner = 1;
                myth_loop();
            case SM_AUTODETECT:
                fatal(EXIT_BUG_BUG, "Cannot be reached!");
                break;
        }

        printf("\n");
        if (debug_608) 
        {   
            printf("\nTime stamps after last caption block was written:\n");
            printf("Last time stamps:  PTS: %s (%+2dF)        ",
                   print_mstime( LLONG(sync_pts/(MPEG_CLOCK_FREQ/1000)
                                       +frames_since_ref_time*1000.0/current_fps) ),
                   frames_since_ref_time);
            printf("GOP: %s      \n", print_mstime(gop_time.ms) );

            // Blocks since last PTS/GOP time stamp.
            printf("Calc. difference:  PTS: %s (%+3lldms incl.)  ",
                print_mstime( LLONG((sync_pts-min_pts)/(MPEG_CLOCK_FREQ/1000)
                + fts_offset + frames_since_ref_time*1000.0/current_fps)),
                fts_offset + LLONG(frames_since_ref_time*1000.0/current_fps) );
            printf("GOP: %s (%+3dms incl.)\n",
                print_mstime((LLONG)(gop_time.ms
                -first_gop_time.ms
                +get_fts_max()-fts_at_gop_start)),
                (int)(get_fts_max()-fts_at_gop_start));
            // When padding is active the CC block time should be within
            // 1000/29.97 us of the differences.
            printf("Max. FTS:       %s  (without caption blocks since then)\n",
                print_mstime(get_fts_max()));
        }

        if (stat_hdtv)
        {
            printf ("\rCC type 0: %d (%s)\n", cc_stats[0], cc_types[0]);
            printf ("CC type 1: %d (%s)\n", cc_stats[1], cc_types[1]);
            printf ("CC type 2: %d (%s)\n", cc_stats[2], cc_types[2]);
            printf ("CC type 3: %d (%s)\n", cc_stats[3], cc_types[3]);
        }
        printf ("\nTotal frames time:      %s  (%u frames at %.2ffps)\n",
            print_mstime( (LLONG)(total_frames_count*1000/current_fps) ),
            total_frames_count, current_fps);
        if (total_pulldownframes)
            printf ("incl. pulldown frames:  %s  (%u frames at %.2ffps)\n",
                    print_mstime( (LLONG)(total_pulldownframes*1000/current_fps) ),
                    total_pulldownframes, current_fps);
        if (pts_set >= 1 && min_pts != 0x01FFFFFFFFLL)
        {
            LLONG postsyncms = LLONG(frames_since_last_gop*1000/current_fps);
            printf ("\nMin PTS:                %s\n",
                    print_mstime( min_pts/(MPEG_CLOCK_FREQ/1000) - fts_offset));
            if (pts_big_change)
                printf ("(Reference clock was reset at some point, Min PTS is approximated)\n");
            printf ("Max PTS:                %s\n",
                    print_mstime( sync_pts/(MPEG_CLOCK_FREQ/1000) + postsyncms));

            printf ("Length:                 %s\n",
                    print_mstime( sync_pts/(MPEG_CLOCK_FREQ/1000) + postsyncms
                                  - min_pts/(MPEG_CLOCK_FREQ/1000) + fts_offset ));
        }
        // dvr-ms files have invalid GOPs
        if (gop_time.inited && first_gop_time.inited && stream_mode != SM_ASF)
        {
            printf ("\nInitial GOP time:       %s\n",
                print_mstime(first_gop_time.ms));
            printf ("Final GOP time:         %s%+3dF\n",
                print_mstime(gop_time.ms),
                frames_since_last_gop);
            printf ("Diff. GOP length:       %s%+3dF",
                print_mstime(gop_time.ms - first_gop_time.ms),
                frames_since_last_gop);
            printf ("    (%s)\n",
                print_mstime(gop_time.ms - first_gop_time.ms
                +LLONG((frames_since_last_gop)*1000/29.97)) );
        }

        if (false_pict_header)
            printf ("\nNumber of likely false picture headers (discarded): %d\n",false_pict_header);

        if (stat_numuserheaders)
            printf("\nTotal user data fields: %d\n", stat_numuserheaders);
        if (stat_dvdccheaders)
            printf("DVD-type user data fields: %d\n", stat_dvdccheaders);
        if (stat_scte20ccheaders)
            printf("SCTE-20 type user data fields: %d\n", stat_scte20ccheaders);
        if (stat_replay4000headers)
            printf("ReplayTV 4000 user data fields: %d\n", stat_replay4000headers);
        if (stat_replay5000headers)
            printf("ReplayTV 5000 user data fields: %d\n", stat_replay5000headers);
        if (stat_hdtv)
            printf("HDTV type user data fields: %d\n", stat_hdtv);
        if (stat_dishheaders)
            printf("Dish Network user data fields: %d\n", stat_dishheaders);
        if (stat_divicom)
        {
            printf("CEA608/Divicom user data fields: %d\n", stat_divicom);

            printf("\n\nNOTE! The CEA 608 / Divicom standard encoding for closed\n");
            printf("caption is not well understood!\n\n");
            printf("Please submit samples to the developers.\n\n\n");
        }

        // Add one frame as fts_max marks the beginning of the last frame,
        // but we need the end.
        fts_global += fts_max + LLONG(1000.0/current_fps);
        // Reset counters - This is needed if some captions are still buffered
        // and need to be written after the last file is processed.
        cb_field1 = 0; cb_field2 = 0; cb_708 = 0;
        fts_now = 0;
        fts_max = 0;        
    } // file loop
    close_input_file();
    
    if (clean!=NULL)
        fclose (clean);	

    flushbuffer (&wbout1,false);
    flushbuffer (&wbout2,false);

    prepare_for_new_file (); // To reset counters used by handle_end_of_data()

    if (wbout1.fh!=NULL)
    {
        if (write_format==OF_SAMI || write_format==OF_SRT || write_format==OF_TRANSCRIPT)
        {
            handle_end_of_data (&wbout1);
        }
        else if(write_format==OF_RCWT)
        {
            // Write last header and data
            writercwtdata (NULL);
        }
        if (end_credits_text!=NULL)
            try_to_add_end_credits(&wbout1);
        write_subtitle_file_footer (&wbout1);
    }
    if (wbout2.fh!=NULL)
    {
        if (write_format==OF_SAMI || write_format==OF_SRT || write_format==OF_TRANSCRIPT)
        {
            handle_end_of_data (&wbout2);
        }
        if (end_credits_text!=NULL)
            try_to_add_end_credits(&wbout2);
        write_subtitle_file_footer (&wbout2);
    }
    flushbuffer (&wbout1,true);
    flushbuffer (&wbout2,true);
    time (&final);
    /* printf ("At end, xmlrpc capacity=%ld, length=%ld\n", xmlrpc_content_capacity,
    xmlrpc_content_length); */
    // OPENSUBS CODE WILL GO HERE
    // FILE *test=fopen ("f:\\comparame.txt","wb");
    // fwrite (xmlrpc_content_body,1,xmlrpc_content_length,test);
    // fclose (test);
    long proc_time=(long) (final-start);
    printf ("\rDone, processing time = %ld seconds\n", proc_time);
    if (proc_time>0)
    {
        LLONG ratio=(get_fts_max()/10)/proc_time;
        unsigned s1=(unsigned) (ratio/100);
        unsigned s2=(unsigned) (ratio%100);    
        printf ("Performance (real length/process time) = %u.%02u\n", 
            s1, s2);
    }
    if (debug_708)
        printf ("The 708 decoder was reset [%d] times.\n",resets_708);

    if (processed_enough)
    {
        printf ("\rNote: Processing was cancelled before all data was processed because\n");
        printf ("\rone or more user-defined limits were reached.\n");
    }
    printf ("This is beta software. Report issues to cfsmp3 at gmail...\n");
    if (show_myth_banner)
    {
        printf ("NOTICE: Due to the major rework in 0.49, we needed to change part of the timing\n");
        printf ("code in the MythTV's branch. Please report results to the address above. If\n");
        printf ("something is broken it will be fixed. Thanks\n");        
    }

    return EXIT_OK;
}
