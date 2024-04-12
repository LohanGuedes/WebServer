#include "Listener.hpp"

Listener::Listener(std::string const &host, std::string const &port)
    : APollable(Listener::getEpollEventStruct()), host(host),
      port(std::atoi(port.c_str())),
      hostPortHash(Listener::hashStr(host + port)) {

    const int          val = 1;
    struct sockaddr_in conf;

    // get default configs for the listening socket
    if (this->resolveAddr(&conf) == false) {
        throw SocketCreateException();
    }
    // create the socket
    if ((this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK,
                            0)) == -1) {
        throw SocketCreateException();
    }
    // disable port reserving
    if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) ==
        -1) {
        throw SocketCreateException();
    }

    // bind the socket to an address/port
    if (bind(this->_fd, (struct sockaddr *)&conf, sizeof(struct sockaddr_in)) ==
        -1) {
        throw SocketCreateException();
    }
}

Listener::~Listener(void) {
    if (this->_fd != -1)
        close(this->_fd);
    return;
}

void Listener::handlePoll(epoll_event_bitflag bitflag) {
    if (bitflag & EPOLLIN) {
        this->handlePollin();
    }
    if (bitflag & EPOLLOUT) {
        std::cout << "EPOLLOUT hit" << std::endl;
    }
    return;
}

void Listener::listen(void) const {
    // start listening
    if (::listen(this->_fd, SOMAXCONN) == -1) {
        throw SocketListenException();
    }
    return;
}

void Listener::handlePollin(void) {
    RunTime *const runTime = RunTime::getInstance();
    const Client  *newClient;
    const int      fd =
        accept(*this->fd_ptr, NULL,
               NULL); // fill these null pointers to get info on the peer socket
    if (fd < 0) {
        Logger::log(LOG_ERROR, "Failure on Listener handlePollin in accept()");
        return;
    }
    newClient = new Client(fd);
    runTime->addToEpoll(newClient);
    runTime->clientPool.push_back(newClient);

    return;
}

unsigned long Listener::hashStr(std::string const &str) {
    unsigned long hash = 5318; // magic number;
    char          c;

    for (size_t i = 0; i < str.length(); i++) {
        c = str[i];
        hash = ((hash << 5) + hash) + c; // isso é: hash * 33 + c
    }
    return hash;
}

bool Listener::resolveAddr(struct sockaddr_in *result) const throw() {
    char             port[10] = {0};
    struct addrinfo *it;
    struct addrinfo *head;
    struct addrinfo  hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    snprintf(port, sizeof(port), "%d", this->port); // port number itoa
    if (getaddrinfo(this->host.c_str(), port, &hints, &head) != 0) {
        return (false);
    }
    for (it = head; it != NULL; it = it->ai_next) {
        if (it->ai_addrlen == sizeof(struct sockaddr_in) &&
            it->ai_family == AF_INET && it->ai_socktype == SOCK_STREAM) {
            *result = *reinterpret_cast<struct sockaddr_in *>(it->ai_addr);
            freeaddrinfo(head);
            return (true);
        }
    }
    freeaddrinfo(head);
    return (false);
}

struct epoll_event Listener::getEpollEventStruct(void) const throw() {
    return ((struct epoll_event){.events = EPOLLIN | EPOLLET,
                                 .data = {.ptr = (void *)this}});
}
