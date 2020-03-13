#include <stdio.h>
#include <stdlib.h>

void show_usage(void);

int main(int argc, char* argv[]) {

	pid_t cpid;
	int conn;
	person_t msg;
	controller_response_t resp;
	char event[128];

	if(argc != 2 ){
		fprintf(stderr, "ERROR: You must provide all required arguments.\n");
		show_usage();
		exit(EXIT_FAILURE);
	}

	cpid = atoi(argv[1]);

	// connect to des_controller
	conn = ConnectAttach(ND_LOCAL_NODE, cpid, 1, _NTO_SIDE_CHANNEL, 0);
	if(conn == -1) {
		fprintf (stderr, "ERROR: Could not connect to the des_controller. Did you specify the right PID?\n");
		exit (EXIT_FAILURE);
	}

	while(1) {
		printf("Enter the event type (ls = left scan, rs = right scan, ws = weight scale, \n"
					"lo = left open, ro = right open, lc = left closed, rc = right closed, \n"
					"gru = guard right unlock, grl = guard right lock, gll = guard left lock, \n"
					"glu = guard left unlock, exit = exit programs) \n");

		scanf("%s", event);
		// validate we have a response
		if (sizeof(resp) == 0) {
			fprintf(stderr, "ERROR: Got a zero-length response from the server.\n");
			exit(EXIT_FAILURE);
		}

		if ((strcmp(event, inMessage[IN_LEFT_SCAN]) == 0)
				|| (strcmp(event, inMessage[IN_RIGHT_SCAN]) == 0)) {

		} else if (strcmp(event, inMessage[IN_WEIGHT_SCALE]) == 0) {

		} // all the other elif shit

		// send message
		if (MsgSend(conn, &msg, sizeof(msg), &resp, sizeof(resp)) == -1) {
			fprintf(stderr, "ERROR: Could not send message.\n");
			exit(EXIT_FAILURE);
		}


		// did we get something other than EOK?
		if (resp.statusCode != EOK) {
			printf("ERROR: Message from server: %s", resp.errorMsg);
		}
	}

	// disconnect from calc_server
	ConnectDetach(conn);
	return EXIT_SUCCESS;
}

void show_usage(void) {
	printf("   USAGE:\n");
	printf("./des_inputs <controller-pid>\n");
}
