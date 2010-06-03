/* This file contains functions that report the user of the GUI of
relevant events. */

#include "ccextractor.h"

/* Print current progress. For percentaje, -1 -> streaming mode */
void activity_progress (int percentaje, int cur_min, int cur_sec)
{
    if (!no_progress_bar)
    {
        if (percentaje==-1)
            printf ("\rStreaming |  %02d:%02d", cur_min, cur_sec);
        else
            printf ("\r%3d%%  |  %02d:%02d",percentaje, cur_min, cur_sec);
    }
    fflush (stdout);
    if (gui_mode_reports)
    {
        fprintf (stderr, "###PROGRESS#%d#%d#%d\n", percentaje, cur_min, cur_sec);
        fflush (stderr);
    }
}

void activity_input_file_open (const char *filename)
{
    if (gui_mode_reports)
    {
        fprintf (stderr, "###INPUTFILEOPEN#%s\n", filename);
        fflush (stderr);        
    }
}

void  activity_video_info (int hor_size,int vert_size, 
    const char *aspect_ratio, const char *framerate)
{
    if (gui_mode_reports)
    {
        fprintf (stderr, "###VIDEOINFO#%u#%u#%s#%s\n",
            hor_size,vert_size, aspect_ratio, framerate);
    fflush (stderr);             
    }
}
                            

void activity_message (const char *fmt, ...)
{
    if (gui_mode_reports)
    {
        va_list args;        
        fprintf (stderr, "###MESSAGE#");
        va_start(args, fmt);
        fprintf(stderr, fmt, args);
        fprintf(stderr, "\n");
        va_end(args);
        fflush (stderr);        
    }
}

void activity_input_file_closed (void)
{
    if (gui_mode_reports)
    {
        fprintf (stderr, "###INPUTFILECLOSED\n");
        fflush (stderr);        
    }
}

void activity_report_version (void)
{
    if (gui_mode_reports)
    {
        fprintf (stderr, "###VERSION#CCExtractor#%s\n",VERSION);
        fflush (stderr);
    }
}

void activity_header (void)
{
    printf ("CCExtractor %s, Carlos Fernandez Sanz, Volker Quetschke.\n", VERSION);
    printf ("--------------------------------------------------------------------------\n");
}

