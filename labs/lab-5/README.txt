# Lucas Estienne's CST8244 Lab 4 - Threads
## Status
The calc_client builds successfully, includes calc_message with relative path,
processes all the required command line arguments, calls ConnectAttach() with the right args,
sends a client_send_t object to the server, processes a server_response_t object response from
the server, the client uses server response codes (constants), it detaches from the channel and
exits with success.
The calc_server builds successfully, imports calc_message, creates the channel, displays its PID,
starts a while true loop in which it receives a message from the client, calculates an answer based
on the operator, it detects error conditions such as divide by 0 and invalid operand,
it sends back a server_response_t object, it uses server response codes from the header file,
and formats errorMsg with sprintf().
On break of the while look it destroys the channel and exits with success.
## Known Issues
None.
## Expected Grade
I expect the full 5 points for the client and the full 10 points for the server.