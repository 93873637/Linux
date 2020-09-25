/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2019, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * simple HTTP POST using the easy interface
 * </DESC>
 */
#include <stdio.h>
#include <curl/curl.h>

//#define HTTPS_URL    "http://postit.example.com/moo.cgi"
//#define HTTPS_POSTFIELDS  "name=daniel&project=curl"
//#define HTTPS_URL "https://www.baidu.com"
//#define HTTPS_URL "https://appr.tc"
#define HTTPS_URL "https://appr.tc/join/2125115795"
#define HTTPS_POSTFIELDS ""

int main(void)
{
    printf("http-post test: url=\"%s\", fields=\"%s\"\n", HTTPS_URL, HTTPS_POSTFIELDS);

    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl)
    {
        /**
         * First set the URL that is about to receive our POST. This URL can
         * just as well be a https:// URL if that is what should receive the data. 
         **/
        curl_easy_setopt(curl, CURLOPT_URL, HTTPS_URL);
        //curl_easy_setopt(curl, CURLOPT_POST, 1L);

        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, HTTPS_POSTFIELDS);

        // CURLcode ret;
        // struct curl_slist *list;
        // list = curl_slist_append(list, "Content-Length: 0");
        // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "Content-Length:0");

        /* verify peer, 0-skip */
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        /* verify host, 0-skip */
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}
