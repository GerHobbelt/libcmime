/* libcmime - A C mime library
 * Copyright (C) SpaceNet AG and Werner Detter <werner@aloah-from-hell.de>
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
#include "../src/cmime_qp.h"
#include "../src/cmime_string.h"
#include "test_data.h"

/**
* char *file_src
* char *file_tmp
* char *file_res    file that contains string to to check against
* int mode  1 eq encode, else decode
* int rm_tmp_file   1 eq remove temporary file after processing
*/
void validate_qp(char *file_src, char *file_tmp, char *file_res, int mode, int rm_tmp_file) {
    
    char line[250];
    char lt[]="\r\n";
    FILE *f1 = NULL;
    FILE *f1_out = NULL;
    FILE *f1_out_in = NULL;
    FILE *f2 = NULL;
    char *fname = NULL;
    char *si = NULL; 
    char *so = NULL;
    char *res = NULL;

    asprintf(&fname,"%s/%s",SAMPLES_DIR,file_src);
    f1 = fopen(fname, "r");
    while(fgets(line, 250, f1)) {
        char *clean;
        clean = cmime_string_chomp(line);
        if(mode == 1) {
            si = cmime_qp_encode(clean, lt);
        } else {
            si = cmime_qp_decode_text(clean);
        }
    }
    fclose(f1);
    free(fname);
    // write the encoded string to the temporary output file
    asprintf(&fname,"%s/%s",SAMPLES_DIR,file_tmp);
    f1_out = fopen (fname, "w");
    if (f1_out != NULL) { 
        fprintf(f1_out,"%s",si);
        fclose(f1_out);
    }
    free(si);
    // read it out again
    f1_out_in = fopen(fname, "r");
    while(fgets(line, 250, f1_out_in)) {
        so = cmime_string_chomp(line);
    }
    fclose(f1_out_in);
    if(rm_tmp_file == 1) remove(fname);
    free(fname);
    // now open the file that contains the result string
    asprintf(&fname,"%s/%s",SAMPLES_DIR,file_res);
    f2 = fopen(fname, "r");
    while(fgets(line, 250, f2)) {
        res = cmime_string_chomp(line);
    }
    fclose(f2);
    free(fname);
    // comparison
    assert(strcmp(so,res)==0);
}




#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      mime_test_qp_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
	/* validate encoding with iso input */
    char file1[] = "qp_iso_plain.txt";
    char file1_encoded[] = "qp_iso_encoded.txt";
    char file1_tmp[] = "qp_iso_plain_tmp.txt";

    /* validate encoding with utf-8 input */
    char file2[] = "qp_utf8_plain.txt";
    char file2_encoded[] ="qp_utf8_encoded.txt";
    char file2_tmp[] = "qp_utf8_plain_tmp.txt";

    /* validate decoding with iso input */
    char file3[] = "qp_iso_encoded.txt";
    char file3_decoded[] = "qp_iso_plain.txt";
    char file3_tmp[] = "qp_iso_encoded_tmp.txt";

    /* validate decoding with utf-8 input */
    char file4[] = "qp_utf8_encoded.txt";
    char file4_decoded[] = "qp_utf8_plain.txt";
    char file4_tmp[] = "qp_utf8_encoded_tmp.txt";

    /* really do something */
    validate_qp(file1,file1_tmp,file1_encoded, 1, 1);
    validate_qp(file2,file2_tmp,file2_encoded, 1, 1);
    validate_qp(file3,file3_tmp,file3_decoded, 0, 1);
    validate_qp(file4,file4_tmp,file4_decoded, 0, 1);

    return(0);
}
