#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6667

std::string toServer(std::string msg)
{
    std::string prefix = "BOT 123456789 ANO ";

    return prefix + msg + "\n";
}

void fillMessages(std::vector<std::string> &messages)
{
    messages.push_back("Did you know? The human brain can generate 12,000 to 80,000 thoughts per minute.");
    messages.push_back("There are around 5,000 visible stars in the night sky that can be seen with the naked eye.");
    messages.push_back("The longest river in the world is the Amazon River, which is 6,400 km long.");
    messages.push_back("The cheetah is the fastest land animal, capable of running at speeds up to 120 km per hour.");
    messages.push_back("Koalas sleep around 20 hours a day and are actually mammals, not bears.");
    messages.push_back("The deepest point on Earth is the Mariana Trench, with a depth of 11,034 meters.");
    messages.push_back("60% of human DNA is identical to a banana's DNA.");
    messages.push_back("The largest desert in the world is the cold desert of Antarctica.");
    messages.push_back("The brightest star in the night sky is Sirius, which is 25 times brighter than the Sun.");
    messages.push_back("The brain of a sea squirt extends down the length of its spinal cord.");
}


int main()
{

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    connect(client_socket, (sockaddr *)&server_addr, sizeof(server_addr));

    std::vector<std::string> messages;
    fillMessages(messages);
    while (true)
    {
        std::string msg = messages[rand() % messages.size()];
        msg = toServer(msg);
        send(client_socket, msg.c_str(), msg.size(), 0);

        sleep(0);
    }

    close(client_socket);

    return 0;
}