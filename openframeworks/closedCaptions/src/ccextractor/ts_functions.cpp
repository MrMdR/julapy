#include "ccextractor.h"

unsigned char tspacket[188]; // Current packet

struct ts_payload
{
    unsigned char *start; // Payload start
    unsigned length;      // Payload length
    unsigned pesstart;    // PES or PSI start
    unsigned pid;         // Stream PID
    int counter;          // continuity counter
};

struct ts_payload payload;

unsigned char *capbuf = (unsigned char*)malloc(20000);
long capbufsize = 20000;
long capbuflen = 0; // Bytes read in capbuf

unsigned TS_program_number = 0; // Identifier for current program
unsigned pmtpid = 0; // PID for Program Map Table
unsigned cappid = 0; // PID for stream that holds caption information
unsigned cap_stream_type; // Stream type for cappid

// Descriptions for ts stream_type
const char *desc[256];

void init_ts_constants(void)
{
    desc[UNKNOWNSTREAM] = "Unknown";
    desc[VIDEO_MPEG1] = "MPEG-1 video";
    desc[VIDEO_MPEG2] = "MPEG-2 video";
    desc[AUDIO_MPEG1] = "MPEG-1 audio";
    desc[AUDIO_MPEG2] = "MPEG-2 audio";
    desc[AUDIO_AAC] =   "AAC audio";
    desc[VIDEO_MPEG4] = "MPEG-4 video";
    desc[VIDEO_H264] =  "H.264 video";

    desc[AUDIO_AC3] =   "AC3 audio";
    desc[AUDIO_DTS] =   "DTS audio";
    desc[AUDIO_HDMV_DTS]="HDMV audio";
}


// Return 1 for sucessfully read ts packet
int ts_readpacket(void)
{
    buffered_read(tspacket,188);
    past+=result;
    if (result!=188)
    {
        if (result>0)
            printf("Premature end of file!\n");
        end_of_file=1;
        return 0;
    }

    int printtsprob = 1;
    while (tspacket[0]!=0x47)
    {
        if (printtsprob)
        {
            printf ("\nProblem: No TS header mark. Received bytes:\n");
            dump (tspacket,4);

            printf ("Skip forward to the next TS header mark.\n");
            printtsprob = 0;
        }

        unsigned char *tstemp;
        // The amount of bytes read into tspacket
        int tslen = 188;

        // Check for 0x47 in the remaining bytes of tspacket
        tstemp = (unsigned char *) memchr (tspacket+1, 0x47, tslen-1);
        if (tstemp != NULL )
        {
            // Found it
            int atpos = tstemp-tspacket;

            memmove (tspacket,tstemp,(size_t)(tslen-atpos));
            buffered_read(tspacket+(tslen-atpos),atpos);
            past+=result;
            if (result!=atpos) 
            {
                printf("Premature end of file!\n");
                end_of_file=1;
                return 0;
            }
        }
        else
        {
            // Read the next 188 bytes.
            buffered_read(tspacket,tslen);
            past+=result;
            if (result!=tslen) 
            {
                printf("Premature end of file!\n");
                end_of_file=1;
                return 0;
            }
        }
    }

    unsigned char *payload_start = tspacket + 4;
    unsigned payload_length = 188 - 4;

    unsigned transport_error_indicator = (tspacket[1]&0x80)>>7;
    unsigned payload_start_indicator = (tspacket[1]&0x40)>>6;
    // unsigned transport_priority = (tspacket[1]&0x20)>>5;
    unsigned pid = (((tspacket[1] & 0x1F) << 8) | tspacket[2]) & 0x1FFF;
    // unsigned transport_scrambling_control = (tspacket[3]&0xC0)>>6;
    unsigned adaptation_field_control = (tspacket[3]&0x30)>>4;
    unsigned ccounter = tspacket[3] & 0xF;

    if (transport_error_indicator)
    {
        printf ("Warning: Defective TS packet:\n");
        dump (tspacket, 188);
    }

    unsigned adaptation_field_length = 0;
    if ( adaptation_field_control & 2 )
    {
        adaptation_field_length = tspacket[4];

        payload_start = payload_start + adaptation_field_length + 1;
        payload_length = tspacket+188-payload_start;
    }

    if (debug_parse)
        printf ("TS pid: %d  PES start: %d  counter: %u  payload length: %u  adapt length: %d\n",
                pid, payload_start_indicator, ccounter, payload_length,
                int(adaptation_field_length));

    // Catch bad packages with adaptation_field_length > 184 and
    // the unsigned nature of payload_length leading to huge numbers.
    if (payload_length > 184)
    {
        // This renders the package invalid
        payload_length = 0;
        if (debug_parse)
        {
            printf ("  Reject package - set length to zero.\n");
        }
    }

    // Save data in global struct
    payload.start = payload_start;
    payload.length = payload_length;
    payload.pesstart = payload_start_indicator;
    payload.pid = pid;
    payload.counter = ccounter;

    if (payload_length == 0)
    {
        if (debug_parse)
            printf ("  No payload in package.\n");
    }

    // Store packet information
    return 1;
}


// Read ts packets until a complete video PES element can be returned.
// The data is read into capbuf and the function returns the number of
// bytes read.
long ts_readstream(void)
{
    static int prev_ccounter = 0;
    static int prev_packet = 0;
    int gotpes = 0;
    long pespcount=0; // count packets in PES with captions
    long pcount=0; // count all packets until PES is complete
    int saw_pesstart = 0;

    capbuflen = 0;

    do
    {
        pcount++;

        if( !prev_packet )
        {
            // Exit the loop at EOF
            if ( !ts_readpacket() )
                break;
        }
        else
            prev_packet = 0;

        // Skip packets with no payload.  This also fixes the problems
        // with the continuity counter not being incremented in empty
        // packets.
        if ( !payload.length )
        {
            if(debug_verbose)
                printf("Packet (pid %u) skipped - no payload.\n",
                       payload.pid);
            continue;
        }

        // Check for PAT
        if( payload.pid == 0 )
        {
            if (!payload.pesstart)
                // Not the first entry. Ignore it, it should not be here.
                continue;

            unsigned pointer_field = *(payload.start);
            unsigned char *payload_start = payload.start + pointer_field + 1;
            unsigned payload_length = tspacket+188-payload_start;

            unsigned table_id = payload_start[0];
            unsigned section_length = (((payload_start[1] & 0x0F) << 8)
                                       | payload_start[2]);
            unsigned transport_stream_id = ((payload_start[3] << 8)
                                            | payload_start[4]);
            unsigned version_number = (payload_start[5] & 0x3E) >> 1;
            unsigned current_next_indicator = payload_start[5] & 0x01;
            unsigned section_number = payload_start[6];
            unsigned last_section_number = payload_start[7];
            if ( last_section_number > 0 )
            {
                fatal(EXIT_BUG_BUG,
                      "Sorry, long PATs not yet supported!\n");
            }

            if (!current_next_indicator)
                // This table is not active, no need to evaluate
                continue;

            payload_start += 8;
            payload_length = tspacket+188-payload_start;

            unsigned programm_data = section_length - 5 - 4; // prev. bytes and CRC

            if(debug_parse)
            {
                printf("Read PAT packet (id: %u) ts-id: 0x%04x\n",
                       table_id, transport_stream_id);
                printf("  section length: %u  number: %u  last: %u\n",
                       section_length, section_number, last_section_number);
                printf("  version_number: %u  current_next_indicator: %u\n",
                       version_number, current_next_indicator);
            }

            if ( programm_data+4 > payload_length )
            {
                fatal(EXIT_BUG_BUG,
                      "Sorry, PAT to long!\n");
            }

            unsigned ts_prog_num = 0;
            unsigned ts_prog_map_pid = 0;
            if (debug_verbose)
                printf ("\nProgram association section (PAT)\n");
            for( unsigned i=0; i < programm_data; i+=4)
            {
                unsigned program_number = ((payload_start[i] << 8)
                                           | payload_start[i+1]);
                unsigned prog_map_pid = ((payload_start[i+2] << 8)
                                         | payload_start[i+3]) & 0x1FFF;

                if(debug_verbose)
                    printf ("  Program number: %u  -> PMTPID: %u\n",
                            program_number, prog_map_pid);

                if( program_number != 0 )
                {
                    if ( ts_prog_num )
                        // We can only work with "simple" ts files
                        fatal(EXIT_BUG_BUG,
                              "TS files with more than one program are not supported");
                    else
                        ts_prog_num = program_number;
                    ts_prog_map_pid = prog_map_pid;
                }
            }

            // If we found a new PAT reset all TS stream variables
            if( ts_prog_num != TS_program_number )
            {
                TS_program_number = ts_prog_num;
                pmtpid = ts_prog_map_pid;
                cappid = 0; // Reset caption stream pid
                // If we have data flush it
                if( capbuflen > 0 )
                {
                    gotpes = 1;
                    break;
                }
            }
            continue;
        }

        // PID != 0 but no PMT defined yet, ignore the rest of the current
        // package and continue searching.
        if ( !pmtpid )
        {
            if(debug_parse)
                printf("Packet (pid %u) skipped - no PMT pid identified yet.\n",
                       payload.pid);
            continue;
        }

        // Check for PMT
        if( payload.pid == pmtpid )
        {
            if (!payload.pesstart)
                // Not the first entry. Ignore it, it should not be here.
                continue;

            unsigned pointer_field = *(payload.start);
            unsigned char *payload_start = payload.start + pointer_field + 1;
            unsigned payload_length = tspacket+188-payload_start;

            unsigned table_id = payload_start[0];
            unsigned section_length = (((payload_start[1] & 0x0F) << 8)
                                       | payload_start[2]);
            unsigned program_number = ((payload_start[3] << 8)
                                       | payload_start[4]);
            if (program_number != TS_program_number)
            {
                // Only use PMTs with matching program number
                if(debug_parse)
                {
                    printf("Reject this PMT packet (pid: %u) program number: %u\n",
                           pmtpid, program_number);
                }
                continue;
            }

            unsigned version_number = (payload_start[5] & 0x3E) >> 1;
            unsigned current_next_indicator = payload_start[5] & 0x01;
            if (!current_next_indicator)
                // This table is not active, no need to evaluate
                continue;
            unsigned section_number = payload_start[6];
            unsigned last_section_number = payload_start[7];
            if ( last_section_number > 0 )
            {
                printf("Long PMTs are not supported - reject!\n");
                continue;
            }
            unsigned PCR_PID = (((payload_start[8] & 0x1F) << 8)
                                | payload_start[9]);
            unsigned pi_length = (((payload_start[10] & 0x0F) << 8)
                                  | payload_start[11]);

            payload_start += 12 + pi_length;
            payload_length = tspacket+188-payload_start;

            unsigned stream_data = section_length - 9 - pi_length - 4; // prev. bytes and CRC

            if(debug_parse)
            {
                printf("Read PMT packet  (id: %u) program number: %u\n",
                       table_id, program_number);
                printf("  section length: %u  number: %u  last: %u\n",
                       section_length, section_number, last_section_number);
                printf("  version_number: %u  current_next_indicator: %u\n",
                       version_number, current_next_indicator);
                printf("  PCR_PID: %u  data length: %u  payload_length: %u\n",
                       PCR_PID, stream_data, payload_length);
            }

            if ( stream_data+4 > payload_length )
            {
                fatal(EXIT_BUG_BUG,
                      "Sorry, PMT to long!\n");
            }

            unsigned newcappid = 0;
            unsigned newcap_stream_type = 0;
            if (debug_verbose)
                printf ("\nProgram map section (PMT)\n");

            for( unsigned i=0; i < stream_data; i+=5)
            {
                unsigned stream_type = payload_start[i];
                unsigned elementary_PID = (((payload_start[i+1] & 0x1F) << 8)
                                           | payload_start[i+2]);
                unsigned ES_info_length = (((payload_start[i+3] & 0x0F) << 8)
                                           | payload_start[i+4]);

                // For the print command below
                unsigned tmp_stream_type = stream_type;
                switch (stream_type)
                {
                case VIDEO_MPEG2:
                case VIDEO_H264:
                    newcappid = elementary_PID;
                    newcap_stream_type = stream_type;
                    break;
                case VIDEO_MPEG1:
                case AUDIO_MPEG1:
                case AUDIO_MPEG2:
                case AUDIO_AAC:
                case VIDEO_MPEG4:
                case AUDIO_AC3:
                case AUDIO_DTS:
                case AUDIO_HDMV_DTS:
                    break;
                default:
                    tmp_stream_type = UNKNOWNSTREAM;
                    break;
                }
                if(debug_verbose)
                    printf ("  %s stream [0x%02x]  -  PID: %u\n",
                            desc[tmp_stream_type],
                            stream_type, elementary_PID);
                i += ES_info_length;
            }
            if (!newcappid)
            {
                printf("No supported stream with caption data found - reject!\n");
                continue;
            }
            if (newcappid != cappid)
            {
                cappid = newcappid;
                cap_stream_type = newcap_stream_type;
                printf ("Decode captions from %s stream [0x%02x]  -  PID: %u\n",
                        desc[cap_stream_type], cap_stream_type, cappid);
                // If we have data flush it
                if( capbuflen > 0 )
                {
                    gotpes = 1;
                    break;
                }
            }
            continue;
        }

        // No caption stream PID defined yet, continue searching.
        if ( !cappid )
        {
            if(debug_parse)
                printf("Packet (pid %u) skipped - no stream with captions identified yet.\n",
                       payload.pid);
            continue;
        }

        // Check for PID with captions
        if( payload.pid == cappid )
        {   // Now we got a payload

            // Video PES start
            if (payload.pesstart)
            {
                // Pretend the previous was smaller
                prev_ccounter=payload.counter-1;

                saw_pesstart = 1;
            }

	    // Discard packets when no pesstart was found.
            if ( !saw_pesstart )
            {
                if(debug_parse)
                    printf("Packet (pid %u) skipped - Did not see pesstart.\n",
                           payload.pid);
                continue;
            }

            // If the buffer is empty we just started this function
            if (payload.pesstart && capbuflen > 0)
            {
                if( debug_verbose)
                    printf("\nPES finished (%ld bytes/%ld PES packets/%ld total packets)\n",
                           capbuflen, pespcount, pcount);

                // Keep the data in capbuf to be worked on

                prev_packet = 1;
                gotpes = 1;
                break;
            }

            if ( (prev_ccounter==15 ? 0 : prev_ccounter+1) != payload.counter )
            {
                printf("TS continuity counter not incremented prev/curr %u/%u\n",
                       prev_ccounter, payload.counter);
            }
            prev_ccounter = payload.counter;


            pespcount++;
            // copy payload to capbuf
            int newcapbuflen = capbuflen + payload.length;
            if ( newcapbuflen > capbufsize) {
                capbuf = (unsigned char*)realloc(capbuf, newcapbuflen);
                if (!capbuf)
                    fatal(EXIT_NOT_ENOUGH_MEMORY, "Out of memory");
                capbufsize = newcapbuflen;
            }
            memcpy(capbuf+capbuflen, payload.start, payload.length);
            capbuflen = newcapbuflen;
        }
        //else
        //    if(debug_verbose)
        //        printf("Packet (pid %u) skipped - unused.\n",
        //               payload.pid);

        // Nothing suitable found, start over
    }
    while( !gotpes ); // gotpes==1 never arrives here because of the breaks

    return capbuflen;
}


// TS specific data grabber
LLONG ts_getmoredata(void)
{
    long payload_read = 0;
    const char *tstr; // Temporary string to describe the stream type

    do
    {
        if( !ts_readstream() )
        {   // If we didn't get data, try again
            printf("empty\n");
            continue;
        }

        // Separate MPEG-2 and H.264 video streams
        if( cap_stream_type == VIDEO_MPEG2)
        {
            bufferdatatype = PES;
            tstr = "MPG";
        }
        else if( cap_stream_type == VIDEO_H264 )
        {
            bufferdatatype = H264;
            tstr = "H.264";
        }
        else
            fatal(EXIT_BUG_BUG,
                  "Not reachable!");

        // We read a video PES

        if (capbuf[0]!=0x00 || capbuf[1]!=0x00 ||
            capbuf[2]!=0x01)
        {
            // ??? Shouldn't happen. Complain and try again.
            printf("Missing PES header!\n");
            dump(capbuf,256);
            continue;
        }
        unsigned stream_id = capbuf[3];

        if( !((stream_id&0xf0)==0xe0) )
        {
            // ??? Shouldn't happen. Complain and try again.
            printf("Not a video PES header!\n");
            continue;
        }

        if (debug_verbose)
            printf("TS payload start video PES id: %d  len: %ld\n",
                   stream_id, capbuflen);

        int pesheaderlen;
        int vpesdatalen = read_video_pes_header(capbuf, &pesheaderlen, capbuflen);

        if (vpesdatalen < 0)
        {   // Seems to be a broken PES
            end_of_file=1;
            break;
        }

        unsigned char *databuf = capbuf + pesheaderlen;
        long databuflen = capbuflen - pesheaderlen;

        // If the package length is unknown vpesdatalen is zero.
        // If we know he package length, use it to quit
        if (debug_verbose)
        {
            printf("Read PES-%s (databuffer %ld/PES data %d) ",
                   tstr, databuflen, vpesdatalen);
            // We got the whole PES in buffer
            if( vpesdatalen && (databuflen >= vpesdatalen) )
                printf(" - complete");
            printf("\n");
        }

        if (databuflen > BUFSIZE - inbuf)
        {
            fatal(EXIT_BUG_BUG,
                  "PES data packet (%ld) larger than remaining buffer (%lld).\n"
                  "Please send bug report!",
                   databuflen, BUFSIZE - inbuf);
        }

        memcpy(buffer+inbuf, databuf, databuflen);

        payload_read = databuflen;
        inbuf += databuflen;

        break;
    }
    while ( !end_of_file );

    return payload_read;
}
