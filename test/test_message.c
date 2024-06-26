/* libcmime - A C mime library
 * Copyright (C) 2013 SpaceNet AG and Axel Steiner <ast@treibsand.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/cmime_address.h"
#include "../src/cmime_message.h"
#include "../src/cmime_header.h"
#include "../src/cmime_list.h"
#include "../src/cmime_part.h"
#include "../src/cmime_string.h"

#include "test_data.h"

char test_files[54][10] = {
    "m0001.txt","m0002.txt","m0003.txt","m0004.txt","m0005.txt",
    "m0006.txt","m0007.txt","m0008.txt","m0009.txt","m0010.txt",
    "m0011.txt","m0012.txt","m0013.txt","m0014.txt","m0015.txt",
    "m0016.txt","m0017.txt","m0018.txt","m1001.txt","m1002.txt",
    "m1003.txt","m1004.txt","m1005.txt","m1006.txt","m1007.txt",
    "m1008.txt","m1009.txt","m1010.txt","m1011.txt","m1012.txt",
    "m1013.txt","m1014.txt","m1015.txt","m1016.txt","m2001.txt",
    "m2002.txt","m2003.txt","m2004.txt","m2005.txt","m2006.txt",
    "m2007.txt","m2008.txt","m2009.txt","m2010.txt","m2011.txt",
    "m2012.txt","m2013.txt","m2014.txt","m2015.txt","m2016.txt",
    "m3001.txt","m3002.txt","m3003.txt","m3004.txt"
};



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      mime_test_message_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
	CMimeMessage_T *msg = cmime_message_new();
    char *s = NULL;
    char *s2 = NULL;
    char *msg_string = NULL;
    char *fname = NULL;
    CMimeHeader_T *h = NULL;
    CMimeList_T *recipient_list = NULL;
    CMimeListElem_T *elem;
    CMimeAddress_T *ca = NULL;
    int retval = 0;
    FILE *fp = NULL;
    FILE *fp2 = NULL;
    long size = 0;
    int i = 0;

    if (argc == 2) {
        /* use message path from command line argument */
        fname = strdup(argv[1]);
        i = cmime_message_from_file(&msg,fname,1);
          
        if (i == 0) {
        
            printf("Message summary:\n=========================================\n");
            s = cmime_message_get_sender_string(msg);
            s2 = cmime_string_strip(s);
            printf("Sender: [%s]\n",s2);
            free(s);
            printf("Recipients (%d):\n",msg->recipients->size);
            elem = cmime_list_head(msg->recipients);
            while(elem != NULL) {
                ca = (CMimeAddress_T *)cmime_list_data(elem);
                s = cmime_address_to_string(ca);
                s2 = cmime_string_strip(s);
                printf("- [%s]\n",s2);
                free(s);
                elem = elem->next;
            }
            printf("Mime Version: [%s]\n",cmime_message_get_mime_version(msg));
            printf("Content ID: [%s]\n",cmime_message_get_content_id(msg));
            printf("Content Type: [%s]\n",cmime_message_get_content_type(msg));
            printf("Number of message headers: [%d]\n",msg->headers->size);
            printf("Number of mime parts: [%d]\n",cmime_message_part_count(msg));
            printf("Message boundary: [%s]\n",cmime_message_get_boundary(msg));
            printf("=========================================\n\n");

            msg_string = cmime_message_to_string(msg);
            printf("%s",msg_string);
            free(msg_string);
        } else {
            printf("failed to parse file [%s] [%d]\n",fname,i);
        } 
        free(fname);
        cmime_message_free(msg);
    } else {
        /* loop test_files */
        cmime_message_set_sender(msg,addr_string1);
        s = cmime_message_get_sender_string(msg);
        assert(strcmp(s,addr_string1)==0);
        free(s);
        
        // check if sender will be overwritten
        cmime_message_set_sender(msg,addr_string2);
        s = cmime_message_get_sender_string(msg);
        assert(strcmp(s,addr_string2)==0);
        free(s);
        
        // check mime sender header
        cmime_message_set_sender_encode(msg,addr_string3);
        s = cmime_message_get_sender_string(msg);
        assert(strcmp(s,addr_string3_full_encoded)==0);
        free(s);

        // set message id
        cmime_message_set_message_id(msg,message_id_string);
        s = cmime_message_get_message_id(msg);
        assert(strcmp(s,message_id_string_out)==0);
        
        cmime_message_set_subject(msg, "A subject  "); // IMPORTANT: with whitespaces!
        s = cmime_message_get_subject(msg);
        assert(strcmp(s, "A subject") == 0);
        
        cmime_message_set_subject_encode(msg,mime_header_string1);
        s = cmime_message_get_subject(msg);
        assert(strcmp(s,mime_header_string1_b64_encoded)==0);

        // set a test header
        if (cmime_message_set_header(msg, header_string1)!=0) 
            return(-1);
        h = cmime_message_get_header(msg,header_string1_key);
        assert(strcmp(cmime_header_get_value(h,0),header_string1_value)==0);
        
        // overwrite header
        if (cmime_message_set_header(msg, header_string1_2)!=0)
            return(-1);
            
        h = cmime_message_get_header(msg,header_string1_key);
        assert(strcmp(cmime_header_get_value(h,0),header_string2_value)==0);

        // check with lower case key
        h = cmime_message_get_header(msg,header_string1_key_lower);
        assert(strcmp(cmime_header_get_value(h,0),header_string2_value)==0);

        // check recipients
        if (cmime_message_add_recipient(msg,addr_string1,CMIME_ADDRESS_TYPE_TO)!=0)
            return(-1);
        
        if (cmime_message_add_recipient(msg,addr_string2,CMIME_ADDRESS_TYPE_CC)!=0)
            return(-1);
            
        if (cmime_message_add_recipient_encode(msg,addr_string3,CMIME_ADDRESS_TYPE_TO)!=0)
            return(-1);

        recipient_list = cmime_message_get_recipients(msg);
        assert(recipient_list);

        elem = cmime_list_tail(recipient_list);
        if (elem != NULL) {
            s = cmime_address_to_string((CMimeAddress_T *)cmime_list_data(elem));
            assert(strcmp(s,addr_string3_full_encoded)==0);
            free(s);
        }

        elem = cmime_list_head(recipient_list);
        while(elem != NULL) {
            s = cmime_address_to_string((CMimeAddress_T *)cmime_list_data(elem));
            assert(s);
            free(s);
            elem = elem->next;
        }
        
        // checking date
        if (cmime_message_set_date_now(msg)!=0)
            return(-1);
            
        s = cmime_message_get_date(msg);
        assert(s);
                
        // check boundary stuff
        s = cmime_message_generate_boundary();
        cmime_message_set_boundary(msg,s);
        s2 = cmime_message_get_boundary(msg);
        assert(strcmp(s,s2)==0);
        
        free(s);
        cmime_message_free(msg);


        for (i=0; i < 54; i++) {
            printf("Checking sample message [%s]...", test_files[i]);
            
            msg = cmime_message_new();
            asprintf(&fname,"%s/%s",SAMPLES_DIR,test_files[i]);
            retval = cmime_message_from_file(&msg,fname,0);
            if (retval != 0)
                return retval;
                
            msg_string = cmime_message_to_string(msg);
        
            if ((fp = fopen(fname, "rb")) == NULL) 
                return(-1);
            free(fname);    

            if (fseek(fp, 0, SEEK_END)!=0)
                return(-1);
            
            size = ftell(fp);
            rewind(fp); 
            s = (char*) calloc(sizeof(char), size + sizeof(char));
            fread(s, size, 1, fp);
            if(ferror(fp))
                return(-1);
            
            fclose(fp);
            asprintf(&s2,"out_%s",test_files[i]);
            fp2 = fopen(s2,"wb");
            fwrite(msg_string,strlen(msg_string),1,fp2);
            fclose(fp2);
            free(s2);
            assert(strcmp(msg_string,s)==0);
            free(s);
            free(msg_string);
            cmime_message_free(msg);
            printf("passed!\n");
        }
    }
    return(0);
}
