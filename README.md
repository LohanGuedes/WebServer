## Validation
#### If the current device supports the needed protocols
Before we can start the server we have to validate if the machine that we're running has the capabilities for running the server. To validate these requirements we can use the [*getprotobyname()*](https://man7.org/linux/man-pages/man3/getprotoent.3.html)to query the supported protocols in our host system and validate if we can start the server in this machine. [more on protocols](https://man7.org/linux/man-pages/man5/protocols.5.html)
#### Conf File Validation
Steps to parsing the 'server' directive:
- found a listen? validate it's host:port and add it to a temporary vector of host:port until the end of the server directive.
- parse the remaining 'server' options and create a ServerConfig object, and then loop the temporary vector of host:port and:
	- If the Listener already exists for that pair host:port, add the ServerConfig to the configPool of that existing Listener class. You can check it's existence via the static std::vector<Listener \*> that is accessible from the Listener class)
	- If the Listener doesn't yet exist, create a new Listener and add the ServerConfig to the configPool of that newly created Listener class. 
- if the port number is not specified in a server, we can use the *getaddinfo()* to bind to a random port, such as [specified here](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html):
```C
// The following programs demonstrate the use of getaddrinfo(), gai_strerror(), freeaddrinfo(), and getnameinfo(3).  The programs are an echo server and client for UDP datagrams.
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 500

int main(int argc, char *argv[])
{
    int                      sfd, s;
    char                     buf[BUF_SIZE];
    ssize_t                  nread;
    socklen_t                peer_addrlen;
    struct addrinfo          hints;
    struct addrinfo          *result, *rp;
    struct sockaddr_storage  peer_addr;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	hints.ai_protocol = 0;          /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	s = getaddrinfo(NULL, argv[1], &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		 exit(EXIT_FAILURE);
	}

/* getaddrinfo() returns a list of address structures. Try each address until we successfully bind(2). If socket(2) (or bind(2)) fails, we (close the socket and) try the next address. */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
				 rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;                  /* Success */

		close(sfd);
	}
	freeaddrinfo(result);           /* No longer needed */
	if (rp == NULL) {               /* No address succeeded */
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}
```

## Non Blocking I/O
chapter 5.9: Introduction
chapter 44.9-10: More info

Non blocking I/O means that *read()*, *write()* and other analogous operations will not block in case of: reading from an fd that is empty or writing to an fd that is full. Example: Reading from an empty pipe or writing to a full pipe.
Whenever we have to set an fd that was not returned from *open()* (because *open()* can be called with the O_NONBLOCK flag) as non-blocking we can use the fcntl as follows:

```C
// To enable
int flags;

flags = fcntl(fd, F_GETFL);
flags |= O_NONBLOCK;
fcntl(fd, F_SETFL, flags);

// To disable
flags = fcntl(fd, F_GETFL);
flags &= ~O_NONBLOCK;
fcntl(fd, F_SETFL, flags);

```

#### I/O Multiplexing (*select(), poll() epoll()*)
I/O multiplexing is ideal for applications that have to either:
- Check whenever I/O is possible on a file descriptor without blocking if it isn't
- Check multiple file descriptors to see if I/O is possible in any of them
chapter 63.1 specifies that we have three alternatives whenever we need to do such thing:
- I/O Multiplexing
- Signal-driven I/O
- The epoll() API
##### I/O multiplexing
Chapter 63.1 specifies that
>I/O multiplexing allows a process to simultaneously monitor multiple file descriptors to find out whether I/O is possible on any of them. The select() and poll() system calls perform I/O multiplexing.

##### Signal-driven I/O 
Chapter 63.1 specifies that
> Signal-driven I/O is a technique whereby a process requests that the kernel send
> it a signal when input is available or data can be written on a specified file descriptor. The process can then carry on performing other activities, and is notified when I/O becomes possible via receipt of the signal. When monitoring large numbers of file descriptors, signal-driven I/O provides significantly better performance than select() and poll().

##### The epoll() API
Chapter 63.1 specifies that
> The epoll API is a Linux-specific feature that first appeared in Linux 2.6. Like the I/O multiplexing APIs, the epoll API allows a process to monitor multiple file descriptors to see if I/O is possible on any of them. Like signal-driven I/O, the epoll API provides much better performance when monitoring large numbers of file descriptors.

##### Level-Triggered vs Edge-Triggered Notification
There are two models of readiness notification for a file descriptor:
- Level-Trigger notification: An fd is considered ready if it's possible to perform an I/O without blocking, **regardless of whether the function would actually transfer data**.
- Edge-Triggered notification: There was an I/O activity on the fd since it's last monitoring.

| I/O Model          | Level-Triggered? | Edge-Triggered? |
| ------------------ | :--------------: | :-------------: |
| *select(), poll()* |        *         |                 |
| Signal-Driven I/O  |                  |        *        |
| *epoll()*<br>      |        *         |        *        |

The book specifies that: 
> In other words, because the level-triggered model allows us to repeat the I/O monitoring operation at any time, it is not necessary to perform as much I/O as possible (e.g., read as many bytes as possible) on the file descriptor (or even perform any I/O at all) each time we are notified that a file descriptor is ready.

And:
> If the program employs a loop to perform as much I/O as possible on the file descriptor, and the descriptor is marked as blocking, then eventually an I/O system call will block when no more I/O is possible. For this reason, each monitored file descriptor is normally placed in nonblocking mode, and after notification of an I/O event, I/O operations are performed repeatedly until the relevant system call (e.g., read() or write()) fails with the error EAGAIN or EWOULDBLOCK.
##### Reasoning for which technique to chose
Check same subject on chapter 63.1 (page 1328)

##### Socket I/O poll notification chart

| Condition or Event                                                     | *select()* | *poll()*                             |
| ---------------------------------------------------------------------- | :--------: | :----------------------------------- |
| Input Available                                                        |     r      | POLLIN                               |
| Output Available                                                       |     w      | POLLOUT                              |
| Incoming connection established on listening socket                    |     r      | POLLIN                               |
| Out-of-band data received (TCP only)                                   |     x      | POLLPRI                              |
| Stream socket peer closed connection or executed shutdown<br>(SHUT_WR) |     rw     | POLLIN<br>\| POLLOUT<br>\| POLLRDHUP |

## Listener, Clients, and all things ✨Classes✨
First things first, why do we need so many god damn classes??
Well, since we'll be doing nonblocking i/o with epoll, we must have a vector of monitorable fds, and check for events. Whenever an event happens on a said position we have to do something, but the type of the object in that said position implies different things.
e.g: A pollin in a Listener socket means we gotta handle a new client, but a pollin on a Client means we gotta process a new request. We also have the CGI, that is a forked process and we have to check when it ends it's processing and get the response from the child process. 
In order to do so we have two options:
1) ✨Polymorphism✨
2) RTTI with <_typeinfo_> header (yikes)
A sidenote: I'm definetly not a fan of inheritance, so we'll be using parametrized polymorphism with interfaces (yipiii).

So our implementation for the Socket, Client and Cgi classes will be as follows.
```C++
class ISocket {
public:
  void handlePolls() = 0;

protected:
  void handlePollin() = 0;
  void handlePollout() = 0;
}

class ListenerSocket: public ISocket { ... } // will be a tcp socket 

class ClientSocket : public ISocket { ... } // will be the fd from accept

class CgiSocket : public ISocket { ... } // will be a unix_socket
```

Unix Sockets:
	Unix sockets are a way of creating a socket that is only internal to the host machine, it doesn't connect to the network. If we need a pair of connected Unix sockets, instead of calling *socket()*, *bind()*, *listen()*, *connect()*, and *accept()*, the socketpair call provides two connected sockets (chapter 57.5).

#### Classes interaction in regards to the Conf file and Runtime.
Ok, we got the monitorable i/o classes all figured out, but we still have an issue because of the 42's project pdf. It states that:
> The first server for a host:port will be the default for this host:port (that means it will answer to all the requests that don’t belong to an other server).

Les't use the following as an example:
```YAML
server {
    listen localhost:4040;
    listen 127.0.0.1:4041;
    server_name lguedes;
}

server {
    listen localhost:4040;
    listen 127.0.0.1:4042;
    server_name gsaiago;
}
```
this creates 3 Listeners:
- localhost:4040 -> lguedes or gsaiago (default being config lguedes)
- localhost:4041 -> lguedes (the only option, since this Listener is only associated with lguedes)
- localhost:4042 -> gsaiago (the only option, since this Listener is only associated with gsaiago)

How does this translate into our Structures?
First, let's translate the file semantics into the classes shown earlier:
- Listener: every **unique** 'listen' directive in the config file.
- ServerConfig: Every config associated with a **unique** 'server_name' directive.
- ClientSocket: 

The relationship between them the file parsing and in the event loop is as follows:
- Each 'listen' directive will create a Listener.
- Each ServerConfig will know it's own server_name via an object property.
- The Listener class will have an internal vector of ServerConfig.
  A new ServerConfig object is added to the ServerConfig vector that exists inside the Listener classes that correspond to every 'listen' directive of that parsed 'server' object.
- Whenever a Listen gets a new Client it passes a pointer to the Listener.vector\<ServerConfig> into the Client. Whenever the Client gets a new request, it parses the request body, and gets the appropriate ServerConfig pointer into the Request class.

In summary, the earlier classes scope will be expanded to
```C++
// Unchanged for now
class ISocket { ... }

// New ServerConfig class 
class ServerConfig {
	public:
		std::string hostPort; // "e.g: localhost:4040"
		std::string serverName; // e.g: gsaiago
	...
};

// Former ListenerSocket
class Listener: public ISocket {
	public:
		std::vector<ServerConfig> serverConfigPool; // self explanatory
		static std::vector<Listener *> listenerPool; // can be used to access all Listener objects to do the checkings.
	...
} // will be a tcp socket 

// Former ClientSocket
class Client : public ISocket {
	public:
		std::vector<ServerConfig> const *configPool; // will point to the Listener serverConfigPool
	...
} // will be the fd from accept

// Request Class
class Request : public ISocket {
	public:
		ServerConfig const &serverConfig; // will receive the appropriate ServerConfig from the Client->configPool
}

// Unchanged for now
class CgiSocket : public ISocket { ... } // will be a unix_socket
```

## References
- The linux Programming interface, chapters:
	- 56, 57, 58, and 59: Sockets
	- 61, 63: recv, send, and polling
- Gguedes (the GOAT)
	
