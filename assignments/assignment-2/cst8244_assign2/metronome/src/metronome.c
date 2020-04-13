#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <math.h>
#include <sys/types.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#define PULSE_CODE _PULSE_CODE_MINAVAIL
#define PAUSE_CODE (_PULSE_CODE_MINAVAIL + 1)
#define QUIT_CODE (_PULSE_CODE_MINAVAIL + 2)
#define ATTACH_POINT "metronome"

void show_usage(void);
void *metronome(void *context);
int io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra);

struct DataTableRow
{
	int tst;
	int tsb;
	int intervals;
	char pattern[16];
};

struct ThreadParams {
	int bpm;
	int tst;
	int tsb;
};

struct DataTableRow t[] = {
		{2, 4, 4, "|1&2&"},
		{3, 4, 6, "|1&2&3&"},
		{4, 4, 8, "|1&2&3&4&"},
		{5, 4, 10, "|1&2&3&4-5-"},
		{3, 8, 6, "|1-2-3-"},
		{6, 8, 6, "|1&a2&a"},
		{9, 8, 9, "|1&a2&a3&a"},
		{12, 8, 12, "|1&a2&a3&a4&a"}
};

typedef union {
	struct _pulse pulse;
	char msg[255];
} my_message_t;


char data[255];
int server_coid, bpm, tsb, tst;
name_attach_t *attach;


int main(int argc, char *argv[]) {

	dispatch_t* dpp;
	resmgr_io_funcs_t io_funcs;
	resmgr_connect_funcs_t connect_funcs;
	iofunc_attr_t ioattr;
	pthread_attr_t attr;
	dispatch_context_t   *ctp;
	int id;
	struct ThreadParams threadParams;

	if(argc != 4 ){
		fprintf(stderr, "ERROR: You must provide all required arguments.\n");
		show_usage();
		exit(EXIT_FAILURE);
	}

	threadParams.bpm = atoi(argv[1]);
	bpm = atoi(argv[1]);
	threadParams.tst = atoi(argv[2]);
	tst = atoi(argv[2]);
	threadParams.tsb = atoi(argv[3]);
	tsb = atoi(argv[3]);

	if ((dpp = dispatch_create()) == NULL) {
		fprintf (stderr, "%s:  Unable to allocate dispatch context.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
		fprintf (stderr, "%s:  name_attach failed.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs, _RESMGR_IO_NFUNCS, &io_funcs);
	connect_funcs.open = io_open;
	io_funcs.read = io_read;
	io_funcs.write = io_write;

	iofunc_attr_init(&ioattr, S_IFCHR | 0666, NULL, NULL);

	if ((id = resmgr_attach(dpp, NULL, "/dev/local/metronome", _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &ioattr)) == -1) {
		fprintf (stderr,"%s:  Unable to attach name.\n", argv [0]);
		exit(EXIT_FAILURE);
	}

	ctp = dispatch_context_alloc(dpp);

	pthread_attr_init(&attr);
	pthread_create(NULL, &attr, &metronome, &threadParams);

	while (1) {
		ctp = dispatch_block(ctp);
		dispatch_handler(ctp);
	}

	pthread_attr_destroy(&attr);
	name_detach(attach, 0);
	name_close(server_coid);


	return EXIT_SUCCESS;
}

void show_usage(void) {
	printf("   USAGE:\n");
	printf("./metronome <beats-per-minute> <time-signature-top> <time-signature-bottom>\n");
}

void *metronome(void *context){
	struct ThreadParams *tp = context;
	struct sigevent event;
	struct itimerspec itime;
	timer_t timer_id;
	int rcvid, current;
	my_message_t msg;
	double bps, pmeasure, pinterval, nano;
	char *tPtr;

	event.sigev_notify = SIGEV_PULSE;
	event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, attach->chid, _NTO_SIDE_CHANNEL, 0);
	event.sigev_priority = SchedGet(0,0,NULL);
	event.sigev_code = PULSE_CODE;

	timer_create(CLOCK_REALTIME, &event, &timer_id);

	for (int i = 0; i < 8; i++) {
		if (t[i].tsb == tp->tsb && t[i].tst == tp->tst) {
			current = i;
			break;
		}
	}


	bps = (double) 60 / tp->bpm;
	pmeasure = bps * 2;
	pinterval = (double) pmeasure / tp->tsb;
	nano = pinterval - (int) pinterval;

	itime.it_value.tv_sec = 1;
	itime.it_value.tv_nsec = 500000000;

	itime.it_interval.tv_sec = pinterval;
	itime.it_interval.tv_nsec = nano * 1e+9;

	timer_settime(timer_id, 0, &itime, NULL);


	tPtr = t[current].pattern;
	while(1) {
		if((rcvid = MsgReceivePulse(attach->chid, &msg, sizeof(msg), NULL)) == -1) {
					fprintf(stderr, "Message receive pulse error\n");
					exit(EXIT_FAILURE);
		} else if(rcvid == 0) {
			switch (msg.pulse.code) {
				case PULSE_CODE:
					printf("%c", *tPtr++);
					break;
				case PAUSE_CODE:
					itime.it_value.tv_sec = msg.pulse.value.sival_int;
					timer_settime(timer_id, 0, &itime, NULL);
					break;
				case QUIT_CODE:
					timer_delete(timer_id);
					exit(EXIT_SUCCESS);
			}
		}
		fflush(stdout);
	}




}

int io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb){
	int nb, current;
	double bps, pmeasure, pinterval, nano;
	if (data == NULL)
		return 0;

	for (int i = 0; i < 8; i++) {
		if (t[i].tsb == tsb && t[i].tst == tst) {
			current = i;
			break;
		}
	}

	bps = (double) 60 / bpm;
	pmeasure = bps * 2;
	pinterval = (double) pmeasure / tsb;
	nano = (pinterval - (int) pinterval) * 1e+9;
	sprintf(data, "[metronome: %d beats/min, time signature: %d/%d, secs-per-interval: %.2f, nanoSecs: %lf]\n", bpm, t[current].tst, t[current].tsb, pinterval, nano);

	nb = strlen(data);
	//test to see if we have already sent the whole message.
	if(ocb->offset == nb)
		return 0;

	//We will return which ever is smaller the size of our data or the size of the buffer
	nb = min(nb, msg->i.nbytes);

	//Set the number of bytes we will return
	_IO_SET_READ_NBYTES(ctp, nb);

	//Copy data into reply buffer.
	SETIOV(ctp->iov, data, nb);

	//update offset into our data used to determine start position for next read.
	ocb->offset += nb;

	//If we are going to send any bytes update the access time for this resource.
	if (nb > 0)
		ocb->attr->flags |= IOFUNC_ATTR_ATIME;

	return (_RESMGR_NPARTS(1));
}

int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb){
	int nb = 0;

	if (msg->i.nbytes == ctp->info.msglen - (ctp->offset + sizeof(*msg))){
		/* have all the data */
		char *buf;
		char *alert_msg;
		int i, small_integer;
		buf = (char *)(msg + 1);

		if (strstr(buf, "pause") != NULL){
			for (i = 0; i < 2; i++){
				alert_msg = strsep(&buf, " ");
			}

			small_integer = atoi(alert_msg);
			if (small_integer >= 1 && small_integer <= 9)
				MsgSendPulse(server_coid, SchedGet(0, 0, NULL), PAUSE_CODE, small_integer);
			else
				printf("\nPause must be between 1 and 9.\n");
		} else if (strstr(buf, "quit") != NULL) {
			MsgSendPulse(server_coid, SchedGet(0, 0, NULL), QUIT_CODE, 0);
		} else {
			printf("\n'%s' is not a valid command\n", strsep(&buf, "\n"));
		}

		nb = msg->i.nbytes;
	}
	_IO_SET_WRITE_NBYTES(ctp, nb);

	if (msg->i.nbytes > 0)
		ocb->attr->flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;

	return (_RESMGR_NPARTS(0));
}

int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra){
	if ((server_coid = name_open(ATTACH_POINT, 0)) == -1) {
		perror("name_open failed.");
		return EXIT_FAILURE;
	}
	return (iofunc_open_default(ctp, msg, handle, extra));
}
