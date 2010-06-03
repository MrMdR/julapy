#include "ccextractor.h"

void params_dump(void) 
{
    // Display parsed parameters
    printf ("Input: ");
    for (int i=0;i<num_input_files;i++)
        printf ("%s%s",inputfile[i],i==(num_input_files-1)?"":",");

    printf ("\n");
    printf ("[Raw Mode: %s] ", rawmode ? "DVD" : "Broadcast");
    printf ("[Extract: %d] ", extract);
    printf ("[Stream mode: ");
    switch (auto_stream)
    {
        case SM_ELEMENTARY_OR_NOT_FOUND:
            printf ("Elementary");
            break;
        case SM_TRANSPORT:
            printf ("Transport");
            break;
        case SM_PROGRAM:
            printf ("Program");
            break;
        case SM_ASF:
            printf ("DVR-MS");
            break;
        case SM_MCPOODLESRAW:
            printf ("McPoodle's raw");
            break;
        case SM_AUTODETECT:
            printf ("Autodetect");
            break;
        default:
            fatal (EXIT_BUG_BUG, "BUG: Unknown stream mode.\n");
            break;
    }
    printf ("]\n");
    printf ("[Use MythTV code: ");
    switch (auto_myth)
    {
        case 0:
            printf ("Disabled");
            break;
        case 1:
            printf ("Forced - Overrides stream mode setting");
            break;
        case 2:
            printf ("Auto");
            break;
    }
    printf ("]\n");

    printf ("[Timing mode: %s] ", use_gop_as_pts ? "GOP": "default");
    printf ("[Debug: %s] ", debug_verbose ? "Yes": "No");
    printf ("[Buffer input: %s]\n", buffer_input ? "Yes": "No");
    printf ("[Print CC decoder traces: %s]\n", debug_608 ? "Yes": "No");
    printf ("[Target format: %s] ",extension);    
    printf ("[Encoding: ");
    switch (encoding)
    {
        case ENC_UNICODE:
            printf ("Unicode");
            break;
        case ENC_UTF_8:
            printf ("UTF-8");
            break;
        case ENC_LATIN_1:
            printf ("Latin-1");
            break;
    }
    printf ("] ");
    printf ("[Delay: %lld] ",subs_delay);    

    printf ("[Trim lines: %s]\n",trim_subs?"Yes":"No");
    printf ("[Add font color data: %s] ", nofontcolor? "No" : "Yes");
    printf ("[Convert case: ");
    if (sentence_cap_file!=NULL)
        printf ("Yes, using %s", sentence_cap_file);
    else
    {
        printf ("%s",sentence_cap?"Yes, but only built-in words":"No");
    }
    printf ("]");
    printf (" [Video-edit join: %s]", binary_concat?"No":"Yes");
    printf ("\n[Extraction start time: ");
    if (extraction_start.set==0)
        printf ("not set (from start)");
    else
        printf ("%02d:%02d:%02d", extraction_start.hh,
        extraction_start.mm,extraction_start.ss);
    printf ("]\n");
    printf ("[Extraction end time: ");
    if (extraction_end.set==0)
        printf ("not set (to end)");
    else
        printf ("%02d:%02d:%02d", extraction_end.hh,extraction_end.mm,
        extraction_end.ss);
    printf ("]\n");
    printf ("[Live stream: ");
    if (live_stream==0)
        printf ("No");
    else
    {
        if (live_stream<1)
            printf ("Yes, no timeout");
        else
            printf ("Yes, timeout: %d seconds",live_stream);
    }
    printf ("] [Clock frequency: %d]\n",MPEG_CLOCK_FREQ);
    printf ("Start credits text: [%s]\n", start_credits_text?start_credits_text:"None");
    if (start_credits_text)
    {
        printf ("Start credits time: Insert between [%ld] and [%ld] seconds\n",
            (long) (startcreditsnotbefore.time_in_ms/1000), 
            (long) (startcreditsnotafter.time_in_ms/1000)
            );
        printf ("                    Display for at least [%ld] and at most [%ld] seconds\n",
            (long) (startcreditsforatleast.time_in_ms/1000), 
            (long) (startcreditsforatmost.time_in_ms/1000)
            );
    }
    if (end_credits_text)
    {
        printf ("End credits text: [%s]\n", end_credits_text?end_credits_text:"None");
        printf ("                    Display for at least [%ld] and at most [%ld] seconds\n",
            (long) (endcreditsforatleast.time_in_ms/1000), 
            (long) (endcreditsforatmost.time_in_ms/1000)
            );
    }

}
