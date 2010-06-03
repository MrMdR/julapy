#include "ccextractor.h"

void write_stringz_as_sami (char *string, struct s_write *wb, LLONG ms_start, LLONG ms_end)
{
    sprintf ((char *) str,"<SYNC start=\"%llu\"><P class=\"UNKNOWNCC\">\r\n",ms_start);
    if (debug_608 && encoding!=ENC_UNICODE)
    {
        printf ("\r%s\n", str);
    }
    enc_buffer_used=encode_line (enc_buffer,(unsigned char *) str);
    fwrite (enc_buffer,enc_buffer_used,1,wb->fh);		
    XMLRPC_APPEND(enc_buffer,enc_buffer_used);
    int len=strlen (string);
    unsigned char *unescaped= (unsigned char *) malloc (len+1); 
    unsigned char *el = (unsigned char *) malloc (len*3+1); // Be generous
    if (el==NULL || unescaped==NULL)
        fatal (EXIT_NOT_ENOUGH_MEMORY, "In write_stringz_as_sami() - not enough memory.\n");
    int pos_r=0;
    int pos_w=0;
    // Scan for \n in the string and replace it with a 0
    while (pos_r<len)
    {
        if (string[pos_r]=='\\' && string[pos_r+1]=='n')
        {
            unescaped[pos_w]=0;
            pos_r+=2;            
        }
        else
        {
            unescaped[pos_w]=string[pos_r];
            pos_r++;
        }
        pos_w++;
    }
    unescaped[pos_w]=0;
    // Now read the unescaped string (now several string'z and write them)    
    unsigned char *begin=unescaped;
    while (begin<unescaped+len)
    {
        unsigned int u = encode_line (el, begin);
        if (debug_608 && encoding!=ENC_UNICODE)
        {
            printf ("\r");
            printf ("%s\n",subline);
        }
        fwrite (el, 1, u, wb->fh);
        XMLRPC_APPEND(el,u);
        fwrite (encoded_br, 1, encoded_br_length,wb->fh);			
        XMLRPC_APPEND(encoded_br, encoded_br_length);

        fwrite (encoded_crlf, 1, encoded_crlf_length,wb->fh);
        XMLRPC_APPEND(encoded_crlf,encoded_crlf_length);
        begin+= strlen ((const char *) begin)+1;
    }

    sprintf ((char *) str,"</P></SYNC>\r\n");
    if (debug_608 && encoding!=ENC_UNICODE)
    {
        printf ("\r%s\n", str);
    }
    enc_buffer_used=encode_line (enc_buffer,(unsigned char *) str);
    fwrite (enc_buffer,enc_buffer_used,1,wb->fh);
    XMLRPC_APPEND(enc_buffer,enc_buffer_used);
    sprintf ((char *) str,"<SYNC start=\"%llu\"><P class=\"UNKNOWNCC\">&nbsp;</P></SYNC>\r\n\r\n",ms_end);
    if (debug_608 && encoding!=ENC_UNICODE)
    {
        printf ("\r%s\n", str);
    }
    enc_buffer_used=encode_line (enc_buffer,(unsigned char *) str);
    fwrite (enc_buffer,enc_buffer_used,1,wb->fh);
    XMLRPC_APPEND(enc_buffer,enc_buffer_used);
}



int write_cc_buffer_as_sami (struct eia608_screen *data, struct s_write *wb)
{
    int wrote_something=0;
    LLONG startms = wb->data608->current_visible_start_ms;

    startms+=subs_delay;
    if (startms<0) // Drop screens that because of subs_delay start too early
        return 0; 

    LLONG endms   = get_fts()+subs_delay;
    endms--; // To prevent overlapping with next line.
    sprintf ((char *) str,"<SYNC start=\"%llu\"><P class=\"UNKNOWNCC\">\r\n",startms);
    if (debug_608 && encoding!=ENC_UNICODE)
    {
        printf ("\r%s\n", str);
    }
    enc_buffer_used=encode_line (enc_buffer,(unsigned char *) str);
    fwrite (enc_buffer,enc_buffer_used,1,wb->fh);		
    XMLRPC_APPEND(enc_buffer,enc_buffer_used);
    for (int i=0;i<15;i++)
    {
        if (data->row_used[i])
        {				
            int length = get_decoder_line_encoded (subline, i, data);
            if (debug_608 && encoding!=ENC_UNICODE)
            {
                printf ("\r");
                printf ("%s\n",subline);
            }
            fwrite (subline, 1, length, wb->fh);
            XMLRPC_APPEND(subline,length);
            wrote_something=1;
            if (i!=14)
            {
                fwrite (encoded_br, 1, encoded_br_length,wb->fh);			
                XMLRPC_APPEND(encoded_br, encoded_br_length);
            }
            fwrite (encoded_crlf, 1, encoded_crlf_length,wb->fh);
            XMLRPC_APPEND(encoded_crlf, encoded_crlf_length);
        }
    }
    sprintf ((char *) str,"</P></SYNC>\r\n");
    if (debug_608 && encoding!=ENC_UNICODE)
    {
        printf ("\r%s\n", str);
    }
    enc_buffer_used=encode_line (enc_buffer,(unsigned char *) str);
    fwrite (enc_buffer,enc_buffer_used,1,wb->fh);
    XMLRPC_APPEND(enc_buffer,enc_buffer_used);
    sprintf ((char *) str,"<SYNC start=\"%llu\"><P class=\"UNKNOWNCC\">&nbsp;</P></SYNC>\r\n\r\n",endms);
    if (debug_608 && encoding!=ENC_UNICODE)
    {
        printf ("\r%s\n", str);
    }
    enc_buffer_used=encode_line (enc_buffer,(unsigned char *) str);
    fwrite (enc_buffer,enc_buffer_used,1,wb->fh);
    XMLRPC_APPEND(enc_buffer,enc_buffer_used);
    return wrote_something;
}
