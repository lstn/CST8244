
#ifndef DES_H_
#define DES_H_

struct person {
	int id;
	int weight;
	int state;
} typedef Person;

struct controller_response {
	Person person;
	char errorMsg[128];
} typedef controller_response_t;

typedef void *(*StateFunc)();

#define NUM_STATES 15
typedef enum
{
	START = 0,
	READY = 1,
	LEFT_SCAN = 2,
	RIGHT_SCAN = 3,
	WEIGHT_SCALE = 4,
	LEFT_OPEN = 5,
	RIGHT_OPEN = 6,
	LEFT_CLOSED = 7,
	RIGHT_CLOSED = 8,
	GUARD_RIGHT_LOCK = 9,
	GUARD_RIGHT_UNLOCK = 10,
	GUARD_LEFT_LOCK = 11,
	GUARD_LEFT_UNLOCK = 12,
	EXIT = 13,
	STOP = 14
} State;

#define NUM_INPUTS 12
typedef enum
{
	LEFT_SCAN = 0,
	RIGHT_SCAN = 1,
	WEIGHT_SCALE = 2,
	LEFT_OPEN = 3,
	RIGHT_OPEN = 4,
	LEFT_CLOSED = 5,
	RIGHT_CLOSED = 6,
	GUARD_RIGHT_LOCK = 7,
	GUARD_RIGHT_UNLOCK = 8,
	GUARD_LEFT_LOCK = 9,
	GUARD_LEFT_UNLOCK = 10,
	EXIT = 11
} Input;

#define NUM_OUTPUTS 14
typedef enum
{
	START = 0,
	READY = 1,
	SCAN = 2,
	WEIGHT_SCALE = 3,
	LEFT_OPEN = 4,
	RIGHT_OPEN = 5,
	LEFT_CLOSED = 6,
	RIGHT_CLOSED = 7,
	GUARD_RIGHT_LOCK = 8,
	GUARD_RIGHT_UNLOCK = 9,
	GUARD_LEFT_LOCK = 10,
	GUARD_LEFT_UNLOCK = 11,
	EXIT = 12,
	STOP = 13
} Output;

const char *outMessage[NUM_OUTPUTS] = {
	"The controller is running as PID: ",
	"Waiting for Person... ",
	"Enter the Person's ID: ",
	"Enter the Person's Weight: ",
	"Person opened left door. ",
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
