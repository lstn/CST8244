# Lucas Estienne's CST8244 Lab 4 - Threads
## Status
For the thread-factory, the project builds successfully, no warnings. It registers
SIGUSR1, prompts for the number of threads, creates them, creates a named semaphore,
calls sem_open, loops until SIGUSR1 is received, and sem_close()s + exits when receiving it.
For the thread waker, the project builds successfully, no warnings. It sem_open()s the named
semaphore, reads the number of threads to wake, wakes them, sends sem_post() on the named semaphore,
and the program sem_close()s the semaphore and exits when the input is 0.
## Known Issues
None.
## Expected Grade
I expect the full 5 points for the thread waker and the full 10 for the thread-factory.