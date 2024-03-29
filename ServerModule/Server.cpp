#include "Server.hpp"

int Server::_push_message(int client_socket, std::string message, std::string status)
{
    Message msg;

    msg.client_socket = client_socket;
    msg.message = message;
    msg.status_out = status;
    this->_messages.push_back(msg);
    return 1;
}


int Server::check_read(int client_socket)
{
    if (FD_ISSET(client_socket, &_readfds))
        return 1;
    return 0;
}

int Server::check_write(int client_socket)
{
    if (FD_ISSET(client_socket, &_writefds))
        return 1;
    return 0;
}

int Server::check_except(int client_socket)
{
    if (FD_ISSET(client_socket, &_exceptfds))
        return 1;
    return 0;
}


int     Server::update()
{
    memset(&_readfds, 0, sizeof(_readfds));
    memset(&_writefds, 0, sizeof(_writefds));
    memset(&_exceptfds, 0, sizeof(_exceptfds));
    FD_ZERO(&_readfds);
    FD_SET(this->_socket, &_readfds);
    int max_sd = this->_socket;
    for (std::list<int>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        int sd = *it;
        if (sd > 0)
            FD_SET(sd, &_readfds);
        if (sd > max_sd)
            max_sd = sd;
    }
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int activity = select(max_sd + 1, &_readfds, NULL, NULL, &tv);
    if (activity < 0)
        {std::cout << "Error: select failed" << std::endl; exit(1);};
    return activity;
}



Server::Server(Server const & src)
{
    *this = src;
}

void    Server::_add_client(int client_socket)
{
    this->_clients.push_back(client_socket);
}

void    Server::_remove_client(int client_socket)
{
    std::list<int> updated_list;

    for (std::list<int>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if (*it != client_socket)
            updated_list.push_back(*it);
    }
    this->_clients = updated_list;
}

std::string Server::_log_time()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "[%d/%m/%Y %H:%M:%S]", timeinfo);
    std::string str(buffer);
    return str;
}

char    *Server::recive_message(int client_socket)
{
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, 1024);
    if (valread == -1)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            return NULL;
        }
        else
        {
            std::cout << _log_time() << " Error: recv failed" << std::endl;
            _push_message(client_socket, "", "0000");
            _remove_client(client_socket);
            return NULL;
        }
    }
    if (valread == 0)
    {
        std::cout << _log_time() << " Client disconnected" << std::endl;
        _remove_client(client_socket);
        _push_message(client_socket, "", "0000");
        close(client_socket);
        return NULL;
    }
    return strdup(buffer);
}

int Server::send_message(int client_socket, std::string message)
{
    int valsend = send(client_socket, message.c_str(), message.length(), 0);
    if (valsend < 0)
    {
        std::cout << _log_time() << " Error: send failed" << std::endl;
        _push_message(client_socket, "", "0000");
        _remove_client(client_socket);
        return 0;
    }
    return valsend;
}

int Server::itereate_clients()
{
    static int check = 0;
    static std::list<int>::iterator it = this->_clients.begin();
    if (check == 1)
    {
        check = 0;
        return -1;
    }
    if (it == this->_clients.end())
    {
        it = this->_clients.begin();
        check = 1;
        return *it;
    }
    int client_socket = *it;
    it++;
    return client_socket;
}


int Server::accept_connection()
{
    int new_socket = accept(this->_socket, NULL, NULL);
    if (new_socket < 0)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            return -2;
        }
        else
        {
            std::cout << "Error: accept failed" << std::endl;
            exit(1);
        }
    }
    if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cout << "Error: fcntl failed" << std::endl;
        exit(1);
    }
    _add_client(new_socket);
    std::cout << this->_log_time() << " New connection accepted" << std::endl;
    return new_socket;
}


int Server::get_port() const
{
    return this->_port;
}

int &Server::get_socket() const
{
    return (int &)this->_socket;
}


Server::Server(int port, int listen_q_size) : _port(port)
{
    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cout << "Error: fcntl failed" << std::endl;
        exit(1);
    }
    if (this->_socket == -1)
    {
        std::cout << "Error: socket creation failed" << std::endl;
        exit(1);
    }
    int opt = 1;
    if (int a = setsockopt(this->_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cout << "Error: setsockopt failed" << std::endl;
        std::cout << a << std::endl;
        exit(1);
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(this->_port);
    address.sin_addr.s_addr = INADDR_ANY;
    if (bind(this->_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "Error: bind failed" << std::endl;
        exit(1);
    }
    if (listen(this->_socket, listen_q_size) < 0)
    {
        std::cout << "Error: listen failed" << std::endl;
        exit(1);
    }
    std::cout << this->_log_time() << " Server is listening on port " << this->_port << std::endl;
}




Server::~Server()
{
    for (std::list<int>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        close(*it);
    }
    close(this->_socket);
    std::cout << this->_log_time() << " Server is closed" << std::endl;
}


Server &Server::operator=(Server const & rhs)
{
    if (this != &rhs)
    {
        this->_port = rhs._port;
        this->_socket = rhs._socket;
    }
    return *this;
}

static std::vector<std::string> spliter(std::string str, char delim)
{
    std::vector<std::string> v;
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] == delim)
        {
            v.push_back(str.substr(0, i));
            str = str.substr(i + 1, str.length() - i - 1);
            i = 0;
        }
    }
    return v;
}

void Server::update_messages()
{
    int client_socket;
    Message msg;

    this->_messages.clear();
    while ((client_socket = itereate_clients()) != -1)
    {
        if (this->check_read(client_socket) == 0)
        {
            continue;
        }
        char *msg_s = recive_message(client_socket);
        if (!msg_s)
            continue;
        std::string message(msg_s);
            free(msg_s);
        if (message.size())
        {
            std::vector<std::string> v = spliter(message, '\n');
            for (int i = 0; i < (int)v.size(); i++)
            {
                msg.message = v[i];
                msg.client_socket = client_socket;
                this->_messages.push_back(msg);
            }
       
        }
    }
    
}

std::list<Message> Server::get_messages()
{
    return this->_messages;
}


int Server::_command_handler(Message msg)
{
    if (msg.status_out == "0000")
    {
        std::cout << _log_time() << " Client " << msg.client_socket << " disconnected" << std::endl;
        _remove_client(msg.client_socket);
        close(msg.client_socket);
        return 1;
    }
    return 0;
}

void Server::send_messages(std::list<Message> messages)
{

    for (std::list<Message>::iterator it = messages.begin(); it != messages.end(); it++)
    {
        _command_handler(*it);
        if (it->message != "")
            send_message(it->client_socket, it->message);
    }
    if (messages.size() > 0)
        std::cout << _log_time() << " Messages sent" << std::endl;
}