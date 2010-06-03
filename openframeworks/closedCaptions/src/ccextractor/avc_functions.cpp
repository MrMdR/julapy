#include "ccextractor.h"

// Work in progress. Disable H.264 code for now.
#define TRY_H264 0

// local functions
static unsigned char *remove_03emu(unsigned char *from, unsigned char *to);
static void sei_rbsp (unsigned char *seibuf, unsigned char *seiend);
static unsigned char *sei_message (unsigned char *seibuf, unsigned char *seiend);
static void user_data_registered_itu_t_t35 (unsigned char *userbuf, unsigned char *userend);


// Process inbuf bytes in buffer holding and AVC (H.264) video stream.
// The number of processed bytes is returned.
LLONG process_avc (unsigned char *avcbuf, LLONG avcbuflen)
{
    unsigned char *bpos = avcbuf;
    unsigned char *NALstart;
    unsigned char *NALstop;

    if( !TRY_H264 )
        fatal(EXIT_BUG_BUG,
              "H.264 transport streams are work in progress ...");

    // At least 5 bytes are needed for a NAL unit
    if(avcbuflen <= 5)
    {
        fatal(EXIT_BUG_BUG,
              "NAL unit need at last 5 bytes ...");
    } 

    // Warning there should be only leading zeros, nothing else 
    if( !(bpos[0]==0x00 && bpos[1]==0x00) )
    {
        fatal(EXIT_BUG_BUG,
              "Broken AVC stream - no 0x0000 ...");
    } 
    bpos = bpos+2;

    set_fts(); // FIXME - check this!!!

    int firstloop=1; // Check for valid start code at buffer start

    // Loop over NAL units
    do
    {
        int zeropad=0; // Count leading zeros

        // Find next NALstart
        while (bpos < avcbuf + avcbuflen)
        {
            if(*bpos == 0x01)
            {
                // OK, found a start code
                break;
            }
            else if(firstloop && *bpos != 0x00)
            {
                // Not 0x00 or 0x01
                fatal(EXIT_BUG_BUG,
                      "Broken AVC stream - no 0x00 ...");
            }
            bpos++;
            zeropad++;
        }
        firstloop=0;
        if (bpos >= avcbuf + avcbuflen)
        {
            // No new start sequence
            break;
        }
        NALstart = bpos+1;

        // Find next start code or buffer end
        long restlen;
        do
        {
            // Search for next 000000 or 000001
            bpos++;
            restlen = avcbuf - bpos + avcbuflen - 2; // leave room for two more bytes

            // Find the next zero
            if (restlen > 0)
            {
                bpos = (unsigned char *) memchr (bpos, 0x00, restlen);

                if(!bpos)
                {
                    // No 0x00 till the end of the buffer
                    NALstop = avcbuf + avcbuflen;
                    bpos = NALstop;
                    break;
                }

                if(bpos[1]==0x00 && (bpos[2]|0x01)==0x01)
                {
                    // Found new start code
                    NALstop = bpos;
                    bpos = bpos + 2; // Move after the two leading 0x00
                    break;
                }
            }
            else
            {
                NALstop = avcbuf + avcbuflen;
                bpos = NALstop;
                break;
            }
        }
        while(restlen); // Should never be true - loop is exited via break

        if(*NALstart & 0x80)
        {
            dump(NALstart-4,10);
            fatal(EXIT_BUG_BUG,
                  "Broken AVC stream - forbidden_zero_bit not zero ...");
        }

        unsigned nal_ref_idc = *NALstart >> 5;
        unsigned nal_unit_type = *NALstart & 0x1F;

        if (debug_verbose)
            printf("NAL unit type: %d length %d  zeros: %d  ref_idc: %d\n",
                   nal_unit_type,  NALstop-NALstart-1, zeropad, nal_ref_idc);

        if ( nal_unit_type == 6 )
        {
            // Found SEI (used for subtitles)
            NALstop = remove_03emu(NALstart+1, NALstop);
            sei_rbsp(NALstart+1, NALstop);
        }
    }
    while(bpos < avcbuf + avcbuflen - 2); // bpos points to 0x01 plus at least two bytes

    return avcbuflen;
}


// Remove 0x000003 emulation sequences.
unsigned char *remove_03emu(unsigned char *from, unsigned char *to)
{
    int offset=0;
    for(unsigned char *stmp = from; stmp < to; stmp++)
    {
        if(stmp+3 < to // one byte follows 0x000003
           && stmp[0]==0x00
           && stmp[1]==0x00
           && stmp[2]==0x03)
        {
            stmp[0-offset]=stmp[0];
            stmp[1-offset]=stmp[1];
            stmp += 3;
            offset++;
        }
        stmp[0-offset] = stmp[0];
    }
    if (offset)
    {
        to -= offset;
        if (debug_verbose)
            printf("Found/removed %d 0x000003 sequences\n", offset);
    }
    return to;
}


// Process SEI payload in AVC data. This function combines sei_rbsp()
// and rbsp_trailing_bits().
void sei_rbsp (unsigned char *seibuf, unsigned char *seiend)
{
    unsigned char *tbuf = seibuf;
    while(tbuf < seiend - 1) // Use -1 because of trailing marker
    {
        tbuf = sei_message(tbuf, seiend - 1);
    }
    if(tbuf == seiend - 1 )
    {
        if(*tbuf != 0x80)
            printf("Strange rbsp_trailing_bits value: %02X\n",*tbuf);
    }
    else
        fatal(EXIT_BUG_BUG,
              "Unexpected SEI unit length ...");

    //exit(1);
}


// This combines sei_message() and sei_payload().
unsigned char *sei_message (unsigned char *seibuf, unsigned char *seiend)
{
    // FIXME: check for 0xFF
    int payloadType = *seibuf; seibuf++;

    // FIXME: check for 0xFF
    int payloadSize = *seibuf;seibuf++;

    unsigned char *paystart = seibuf;
    seibuf+=payloadSize;
    if(seibuf > seiend)
        fatal(EXIT_BUG_BUG,
              "SEI payload too long ...");

    printf("PT: %d size: %d\n", payloadType, payloadSize);

    // Ignore all except user_data_registered_itu_t_t35() payload
    if(payloadType == 4)
        user_data_registered_itu_t_t35(paystart, paystart+payloadSize);

    return seibuf;
}


void user_data_registered_itu_t_t35 (unsigned char *userbuf, unsigned char *userend)
{
    unsigned char *tbuf = userbuf;
    int itu_t_t35_country_code = *((uint8_t*)tbuf);
    tbuf++;
    int itu_t_35_provider_code = *tbuf * 255 + *(tbuf+1);
    tbuf+=2;

    // ANSI/SCTE 128 2008:
    // itu_t_t35_country_code == 0xB5
    // itu_t_35_provider_code == 0x0031
    // see spec for details - no example -> no support

    // Example files (sample.ts, ...):
    // itu_t_t35_country_code == 0xB5
    // itu_t_35_provider_code == 0x002F
    // user_data_type_code == 0x03 (cc_data)
    // user_data_len == next byte (length after this byte up to (incl) marker.)
    // cc_data struct (CEA-708)
    // marker == 0xFF

    if(itu_t_t35_country_code != 0xB5)
    {
        printf("Not a supported user data SEI\n");
        printf("  itu_t_35_country_code: %02x\n", itu_t_t35_country_code);
        return;
    }

    if(itu_t_35_provider_code == 0x0031)
        fatal(EXIT_BUG_BUG,
              "Captions according to ANSI/SCTE 128. Unsupported, please provide example.");

    if(itu_t_35_provider_code == 0x002F)
    {
        int user_data_type_code = *((uint8_t*)tbuf);
        if(user_data_type_code != 0x03)
        {
            printf("Not supported  user_data_type_code: %02x\n",
                   user_data_type_code);
            return;
        }
        tbuf++;
        int user_data_len = *((uint8_t*)tbuf);
        tbuf++;

        unsigned char cc_count = *tbuf & 0x1F;
        unsigned char process_cc_data_flag = (*tbuf & 0x40) >> 6;
        if (!process_cc_data_flag)
            return;
        unsigned char *cc_data = tbuf+2;

        if (cc_count*3+3 != user_data_len)
            fatal(EXIT_BUG_BUG,
                  "Syntax problem: user_data_len != cc_count*3+3.");

        // Enough room for CC captions?
        if (cc_data+cc_count*3 >= userend)
            fatal(EXIT_BUG_BUG,
                  "Syntax problem: Too many caption blocks.");
        if (cc_data[cc_count*3]!=0xFF)
            fatal(EXIT_BUG_BUG,
                  "Syntax problem: Final 0xFF marker missing.");


        // Not working yet. Needs HD sorting ... frame type + sequence num
        for (int j=0; j<cc_count*3; j=j+3)
        {
            do_cb(cc_data+j);
        }
        dump(tbuf,user_data_len-1);
        exit(1);
    }
    else
    {
        printf("Not a supported user data SEI\n");
        printf("  itu_t_35_provider_code: %04x\n", itu_t_35_provider_code);
        return;
    }
}
