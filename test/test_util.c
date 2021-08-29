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

#include "../src/cmime_util.h"

#include "test_data.h"

#define EXPECTED_MIMETYPE "message/rfc822; charset=iso-8859-1"



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      mime_test_util_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
	char *out = NULL;
  char *fname = NULL;

  asprintf(&fname,"%s/%s",SAMPLES_DIR,"m0001.txt");
	
	out = cmime_util_get_mimetype(fname);
	assert((strcmp(EXPECTED_MIMETYPE,out)==0));
	free(out);
	free(fname);

	return(0);
}	

