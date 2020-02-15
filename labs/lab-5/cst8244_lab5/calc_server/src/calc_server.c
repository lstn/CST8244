/*
 * calc_server.c
 *
 *  Created on: Feb 14 2020
 *      Author: Lucas Estienne <esti0011@algonquinlive.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <float.h>
#include <limits.h>


#include "calc_message.h"


int main(void) {

	client_send_t msg;
	server_response_t resp;
	int chan, rec;

	// create channel
	chan = ChannelCreate (0);
	if (chan == -1) {
		fprintf (stderr, "ERROR: Could not create channel.\n");
		exit (EXIT_FAILURE);
	}

	// print pid
	printf("CalcServer PID : %d\n", getpid());

	// loop
	while(1) {
		rec = MsgReceive(chan, (void*)&msg, sizeof(msg), NULL);
		resp.statusCode = SRVR_OK;

		switch(msg.operator) {
			case '+':
				// catch overflow/underflow
				if ( ((msg.right_hand > 0) && (msg.left_hand > INT_MAX - msg.right_hand))
						|| ((msg.right_hand < 0) && (msg.left_hand < INT_MIN - msg.right_hand)) ) {
					resp.statusCode = SRVR_OVERFLOW;
					sprintf(resp.errorMsg, "SRVR_OVERFLOW: %d + %d", msg.left_hand, msg.right_hand);
					break;
				}
				resp.answer = msg.left_hand + msg.right_hand;
				break;
			case '-':
				// catch overflow/underflow
				if ( ((msg.right_hand < 0) && (msg.left_hand > INT_MAX + msg.right_hand))
						|| ((msg.right_hand > 0) && (msg.left_hand < INT_MIN + msg.right_hand)) ) {
					resp.statusCode = SRVR_OVERFLOW;
					sprintf(resp.errorMsg, "SRVR_OVERFLOW: %d - %d", msg.left_hand, msg.right_hand);
					break;
				}
				resp.answer = msg.left_hand - msg.right_hand;

				break;
			case '/':
				// catch divide by 0
				if (msg.right_hand == 0){
					resp.statusCode = SRVR_UNDEFINED;
					sprintf(resp.errorMsg, "SRVR_UNDEFINED: %d / %d", msg.left_hand, msg.right_hand);
					break;
				}
				// catch overflow
				if ((msg.right_hand == -1) && (msg.left_hand == INT_MIN)) {
					resp.statusCode = SRVR_OVERFLOW;
					sprintf(resp.errorMsg, "SRVR_OVERFLOW: %d / %d", msg.left_hand, msg.right_hand);
					break;
				}

				resp.answer = (double) msg.left_hand / (double) msg.right_hand;
				break;
			case 'x':
				// catch overflow/underflow
				if ( ((msg.left_hand == -1) && (msg.right_hand == INT_MIN))
						|| ((msg.right_hand == -1) && (msg.left_hand == INT_MIN))
						|| (msg.right_hand != 0 && (msg.left_hand > INT_MAX / msg.right_hand))
						|| (msg.right_hand != 0 && (msg.left_hand < INT_MIN / msg.right_hand)) ) {
					resp.statusCode = SRVR_OVERFLOW;
					sprintf(resp.errorMsg, "SRVR_OVERFLOW: %d * %d", msg.left_hand, msg.right_hand);
					break;
				}
				resp.answer = (double) msg.left_hand * (double) msg.right_hand;
				break;
			default:
				// invalid operator
				resp.statusCode = SRVR_INVALID_OPERATOR;
				sprintf(resp.errorMsg, "SRVR_INVALID_OPERATOR: %c", msg.operator);
		}

		// send answer back
		MsgReply(rec, EOK, &resp, sizeof(resp));
	}

	// destroy channel
	ChannelDestroy(chan);
	return EXIT_SUCCESS;
}
