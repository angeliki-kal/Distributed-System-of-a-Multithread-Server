![alt text](NKUA.jpg)
# Distributed-System-of-a-Multithread-Server

## COMPILATION

Compile each app independently using the provided makefile.
In case master is closed unexpectedly and the pipes have now been freed,
use "make clean" goal to delete them.

## RUN

sample commands to run the apps:
#### ./whoServer -q 33333 -s 44444 -w 2 -b 200

#### ./whoClient -q file -w 2 -sp 33333 -sip localhost

#### ./master -w 2 -b 100 -s localhost -p 44444 -i ./bla


All requirements regarding the communication, networking and threading have been implemented.

Adequate messages are printed to stdout to help understand when each app does.
Ideally each app will be run in a separate terminal/tab. Where these messages can be seen.


## IMPLEMENTATION

## *Master*
The main process spawns N many workers.
These workers will open a socket on a port (set with htons(0);) where they
listen for requests from the whoServer.
Before going into an accept()-loop where they listen for requests from the
server, each worker will inform the whoServer about the port that it choose
to listen on by sending a message with this format #pid$port (i.e #15141$8080).
Also statistic are sent to the server right after this.

One the port and stats are sent, the Worker will proceed to listen for incoming
requests that the whoServer sends using accept(). If no conn is available the
process will block and wait for one. For each request received from the whoServer
the Worker responds back and tries to accept() a new conn.
(since there is no 2nd project to serve the results a mock string is send
i.e: "worker's answer to"+"request_msg")


## *whoServer*
The server has 2 ports it is listening for requests on. A query_port for
requests from the whoClients and a server_port for messages from the Workers.
For both of these, their sockets are put in a struct pollfd so that if any of
them becomes available the Thread is awaken. poll() was prefered over select
because its implementation is more intuative, besides being able to monitor
fd's greater than FD_SETSIZE, which select cannot.
Once a POLLIN event happens on a socket the server accepts the connection,
and start a new session with the worker or client.
Then it tries to fulfill the request. After fulfilling the request it closes
the session (socket) and goes back to polling.

In case we had a POLLIN on the server_port's socket (message from Worker),
we would read the message and check if the first char of the messsage is a #.
If yes then it means that we are getting info about a pid-port pair for a
Worker, so we have to save this data to a map in order to be able to forward
requests from the client to the Worker.
Otherwise, if there is no # at the start of the message, we know that the msg
contains the statistic, which we immediately print.

In case we had a POLLIN on the query_port's socket (request from whoClient),
we'd accept the new session on a new client_session_socket.
Read the message (the message are always read in chunks/buffers).
Forward this message to the adequate Worker (using the worker_pid-port) pairs
collected in the previous step.
Note: if the worker hasn't informed yet about its port or is otherwise unavail
the server will send an "no worker available to answer..." msg to the client.
Leaving the socket open the server will wait for the worker to answer and
when it has the answer it will forward it to the client that made the query.
Both sessions with the worker and the client are closed and the server goes
back to polling.

The main thread of the server will of course do a pthread_join() before exiting.

Note: there is no circular buffer needed for saving the pending connections to
threads, as there is already a buffer implemented by the system. Calling accept()
removes entries/connections from this buffer. Implementing a custom buffer would
cause overheads in the communication.

## *whoClient*
After opening the file a Thread is created for each line in the file,
a maximum of -w NUM_THREADS can be spawned. If this num is less than the num of
lines in the file then the rest of the lines will be ignored.
Note: you can use both localhost and 127.0.0.1 for the IP.

A Thread of the client will connect to the server_ip:server_port that is
specified and send a request to the server.
Then it will wait for an answer on the same socket. Once it has the response
it will print it to stdout and exit.

The main thread does a pthread_join(), waiting for the child-threads,
before exiting.
