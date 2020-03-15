# Lucas Estienne's CST8244 Assignment 1 - State Machines
## Status
All parts of the project build successfully without warnings, the controller meets all requirements in the rubric, so does the inputs
and display, all parts clean up their channels and connections when they exit, they all display PID, have usage messages, process args, etc.

## Known Issues
When exiting, the display experiences a core dump memfault when it `returns EXIT_SUCCESS` or `exit(EXIT_SUCCESS)`,
I couldn't quite figure that one out. gdb didn't really shed any light on this. Apart from that it's still a "graceful exit"
in that it destroys the channel before exiting, etc.

## Expected Grade
I expect the full marks for the assignment minus the late penalty.
I honestly have had to work non-stop to prepare in compliance our business continuity plan at work (we're essential infrastructure) since
we're switching to fully remote next week and lots of things needed to get done. It's not an excuse but frankly I haven't had time to worry
about Algonquin with everything that's been going on wrt this virus thing. The penalty is fine, it's fair game.