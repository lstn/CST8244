/*
 * calc_client.c
 *
 *  Created on: Feb 14 2020
 *      Author: Lucas Estienne <esti0011@algonquinlive.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <float.h>
#include <stdint.h>

#include "../../calc_server/src/calc_message.h"

void show_usage(void);

int main(int argc, char* argv[]) {

	client_send_t msg;
	server_response_t resp;
	pid_t spid;
	int conn;

	if(argc != 5 ){
		fprintf(stderr, "ERROR: You must provide all required arguments.\n");
		show_usage();
		exit(EXIT_FAILURE);
	}

	spid = atoi(argv[1]);
	msg.left_hand = atoi(argv[2]);
	msg.operator = argv[3][0];
	msg.right_hand = atoi(argv[4]);

	// connect to calc_server
	conn = ConnectAttach(ND_LOCAL_NODE, spid, 1, _NTO_SIDE_CHANNEL, 0);
	if(conn == -1) {
		fprintf (stderr, "ERROR: Could not connect to the calc_server. Did you specify the right PID?\n");
		exit (EXIT_FAILURE);
	}

	// send message
	if (MsgSend(conn, &msg, sizeof(msg), &resp, sizeof(resp)) == -1) {
		fprintf(stderr, "ERROR: Could not send message.\n");
		exit(EXIT_FAILURE);
	}

	// validate we have a response
	if (sizeof(resp) == 0) {
		fprintf(stderr, "ERROR: Got a zero-length response from the server.\n");
		exit(EXIT_FAILURE);
	}

	// did we get SRVR_OK?
	if (resp.statusCode == SRVR_OK) {
		printf("The server has calculated the result of %d %c %d as %.2f \n", msg.left_hand, msg.operator, msg.right_hand, resp.answer);
	} else {
		printf("ERROR: Message from server: %s", resp.errorMsg);
	}


	// disconnect from calc_server
	ConnectDetach(conn);
	return EXIT_SUCCESS;
}

void show_usage(void) {
	printf("   USAGE:\n");
	printf("./calc_client <server-pid> <left-operand> <operator> <right-operand>\n");
	printf("The <operator> argument MUST be one of ‘+’, ‘-’, ‘x’, and ‘/’.\n");
}
