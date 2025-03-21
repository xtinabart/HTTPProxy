#include "mongoose.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Christina Bartsch
 * February 25, 2025
 * CS 455: HttpProxy in C
 */

static const char *TARGET_HOST;
static const char *TARGET_PORT;

static void forward(struct mg_connection *c, int ev, void *ev_data, void *user_data) {
    	if (ev == MG_EV_HTTP_MSG) {
        	struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        	mg_http_reply(user_data, 200, NULL, "%.*s", (int)hm->message.len, hm->message.ptr); //create response
        	c->is_closing = 1; //close target server connection
        	((struct mg_connection*)user_data)->is_closing = 1; //close client connection

    	} else if (ev == MG_EV_ERROR) {
        	LOG(LL_ERROR, ("Forward error: %s", (char *)ev_data));
        	mg_http_reply(user_data, 500, NULL, "Server Error");
        	((struct mg_connection*)user_data)->is_closing = 1;
        	c->is_closing = 1;
    	}
}

static void ev_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
	(void) fn_data; //not used in func but needed for mongoose listen
    	if (ev == MG_EV_HTTP_MSG) { //http message?
        	struct mg_http_message *hm = (struct mg_http_message *)ev_data; //put event data into http msg structure

        	if (strncmp(hm->uri.ptr, "/forward/", 9) == 0) { //find forward in URI
            		struct mg_str path; //construct path after forward
			path.ptr = hm->uri.ptr + 9;
			path.len = hm->uri.len - 9;
            		char target_url[2048];
			//create url, forward message
            		snprintf(target_url, sizeof(target_url), "http://%s:%s/%.*s", TARGET_HOST, TARGET_PORT, (int)path.len, path.ptr);
            		mg_http_connect(c->mgr, target_url, forward, c); //call forward method

        	} else {
            		mg_http_reply(c, 404, NULL, "Not Found"); //404 error
        	}
    	} else if (ev == MG_EV_ERROR) { //event errror
		LOG(LL_ERROR, ("Proxy error: %s", (char *)ev_data)); //log error
        	c->is_closing = 1;
    	}
}

int main(int argc, char *argv[]) {
    	struct mg_mgr mgr; //mongoose event manager
    	int exitcode = EXIT_FAILURE;

    	if (argc != 4) {
        	fprintf(stderr, "usage: %s <listen_port> <target_host> <target_port>\n", argv[0]);
        	return EXIT_FAILURE;
    	}

    	const char *listen_port = argv[1];
    	TARGET_HOST = argv[2];
    	TARGET_PORT = argv[3];

    	mg_mgr_init(&mgr); //intialize manager
    	mg_log_set("3"); //debug level logging for events/errors

	char listen_url[32];
	snprintf(listen_url, sizeof(listen_url), "http://0.0.0.0:%s", listen_port);
    	if (mg_http_listen(&mgr, listen_url, ev_handler, NULL) == NULL) { //call event handler, listen on port
        	fprintf(stderr, "Cannot listen on port %s\n", listen_port);
        	goto done;
    	}

    	for (;;) { //event loop
        	mg_mgr_poll(&mgr, 1000);
    	}

	exitcode = EXIT_SUCCESS;

done:
    	mg_mgr_free(&mgr);
    	return exitcode;
}
