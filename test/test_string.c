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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/cmime_string.h"

#include "test_data.h"



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      mime_test_string_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
	CMimeStringList_T *sl;
    
    char *s = NULL;
    char *out = NULL;
    
    sl = cmime_string_list_new();
    
    cmime_string_list_insert(sl,test_string1);
    cmime_string_list_insert(sl,test_string2);
    cmime_string_list_insert(sl,test_string3);
    assert(cmime_string_list_get_count(sl)==3);
        
    s = cmime_string_list_get(sl,0);
    assert(strcmp(s,test_string1)==0);
    
    s = cmime_string_list_get(sl,1);
    assert(strcmp(s,test_string2)==0);
    
    s = cmime_string_list_get(sl,2);
    assert(strcmp(s,test_string3)==0);
    
    cmime_string_list_free(sl);
    
    // check string strip
    s = cmime_string_strip(test_string4);
    assert(strcmp(s,test_string3)==0);
	
    // check mime encoding
    out = cmime_string_encode_to_7bit(mime_header_string1,CMIME_STRING_ENCODING_B64);
    assert(strcmp(out,mime_header_string1_b64_encoded)==0);
    free(out);

    out = cmime_string_encode_to_7bit(mime_header_string1,CMIME_STRING_ENCODING_QP);
    assert(strcmp(out,mime_header_string1_qp_encoded)==0);
    free(out);

    return(0);
}
