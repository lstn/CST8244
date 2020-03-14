
#ifndef DES_H_
#define DES_H_

struct person {
	int id;
	int weight;
	int state;
} typedef person_t;

struct controller_response {
	person_t person;
	int statusCode;
	char errorMsg[128];
} typedef controller_response_t;

typedef void *(*StateFunc)();

#define NUM_STATES 15
typedef enum
{
	ST_START = 0,
	ST_READY = 1,
	ST_LEFT_SCAN = 2,
	ST_RIGHT_SCAN = 3,
	ST_WEIGHT_SCALE = 4,
	ST_LEFT_OPEN = 5,
	ST_RIGHT_OPEN = 6,
	ST_LEFT_CLOSED = 7,
	ST_RIGHT_CLOSED = 8,
	ST_GUARD_RIGHT_LOCK = 9,
	ST_GUARD_RIGHT_UNLOCK = 10,
	ST_GUARD_LEFT_LOCK = 11,
	ST_GUARD_LEFT_UNLOCK = 12,
	ST_EXIT = 13,
	ST_STOP = 14
} State;

#define NUM_INPUTS 12
typedef enum
{
	IN_LEFT_SCAN = 0,
	IN_RIGHT_SCAN = 1,
	IN_WEIGHT_SCALE = 2,
	IN_LEFT_OPEN = 3,
	IN_RIGHT_OPEN = 4,
	IN_LEFT_CLOSED = 5,
	IN_RIGHT_CLOSED = 6,
	IN_GUARD_RIGHT_LOCK = 7,
	IN_GUARD_RIGHT_UNLOCK = 8,
	IN_GUARD_LEFT_LOCK = 9,
	IN_GUARD_LEFT_UNLOCK = 10,
	IN_EXIT = 11
} Input;

#define NUM_OUTPUTS 14
typedef enum
{
	OUT_START = 0,
	OUT_READY = 1,
	OUT_SCAN = 2,
	OUT_WEIGHT_SCALE = 3,
	OUT_LEFT_OPEN = 4,
	OUT_RIGHT_OPEN = 5,
	OUT_LEFT_CLOSED = 6,
	OUT_RIGHT_CLOSED = 7,
	OUT_GUARD_RIGHT_LOCK = 8,
	OUT_GUARD_RIGHT_UNLOCK = 9,
	OUT_GUARD_LEFT_LOCK = 10,
	OUT_GUARD_LEFT_UNLOCK = 11,
	OUT_EXIT = 12,
	OUT_STOP = 13
} Output;

const char *outMessage[NUM_OUTPUTS] = {
	"The controller is running as PID: ",
	"Waiting for Person... ",
	"Person scanned ID. ID =",
	"Person weighed. Weight =",
	"Person opened left door.",
	"Person opened right door.",
	"Person closed left door. ",
	"Person closed right door. ",
	"Right door locked by Guard. ",
	"Right door unlocked by Guard. ",
	"Left door locked by Guard. ",
	"Left door unlocked by Guard. ",
	"Exit display."
	"Exiting controller."
};

const char *inMessage[NUM_INPUTS] = {
	"ls",
	"rs",
	"ws",
	"lo",
	"ro",
	"lc",
	"rc",
	"grl",
	"gru",
	"gll",
	"glu",
	"exit"
};
#endif /* DES_H_ */
