#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <list>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/select.h>
# include <ctime>
# include <string.h> 


# define WOULD_BLOCK -2
# define CRASH -1


struct Message
{
    std::string     message;
    int             client_socket;
    std::string     status_out;
};


class   Server
{

    private:
        int                 _port;
        int                 _socket;
        std::list<int>      _clients;
        std::list<Message>  _messages;
        fd_set              _readfds;
        fd_set              _writefds;
        fd_set              _exceptfds;
    
        
    private:
        std::string     _log_time();
        void            _add_client(int client_socket);
        void            _remove_client(int client_socket);
        int             _push_message(int client_socket, std::string message, std::string status);
        int             _command_handler(Message msg);
    public:
                        Server(int port, int listen_queue_size);
                        Server(Server const & src);
                        ~Server();
    
    public:

        int             get_port() const;
        int             &get_socket() const;
        int             send_message(int client_socket, std::string message);
        int             accept_connection();
        int             itereate_clients();
        int             update();
        int             check_read(int client_socket);
        int             check_write(int client_socket);
        int             check_except(int client_socket);
        char            *recive_message(int client_socket);

        void                update_messages();
        void                send_messages(std::list<Message> messages);
        std::list<Message>  get_messages();


    public:
        Server          &operator=(Server const & rhs);
        
};

#endif