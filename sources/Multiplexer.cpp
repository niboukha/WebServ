/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:45:22 by shicham           #+#    #+#             */
/*   Updated: 2024/03/02 09:04:20 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Multiplexer.hpp"

Multiplexer::Multiplexer(ConfigFile& config) : configFile(config)
{
}

Multiplexer::~Multiplexer()
{
}

const   std::vector<int>&    Multiplexer::getMasterSockets() const
{
    return masterSockets;
}

const   ConfigFile&    Multiplexer::getConfigFile() const
{
    return configFile;
}

void    Multiplexer::bindSocket(int fdServer, std::string add, std::string port)
{
    sockaddr_in serverAdd;
    int     opt;
    
    (void)add;//to check mn b3ed 
    opt = 1;
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = INADDR_ANY;
    // serverAdd.sin_addr.s_addr = gettaddrinfo(add.c_str(), );
    serverAdd.sin_port = htons(atoi(port.c_str()));
    if (setsockopt(fdServer, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        throw strerror(errno);
    if (bind(fdServer, (const sockaddr *)&serverAdd, sizeof(serverAdd)) < 0)
        throw strerror(errno);
    if (listen(fdServer, 5) < 0)
        throw strerror(errno);
}

void    Multiplexer::createSockets()
{
   std::vector<Server> servers;
   int                  sockFd;

    servers = configFile.getServers();
    for(size_t i = 0; i < servers.size(); i++)
    {
        sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockFd < 0)
            throw strerror(errno);
        masterSockets.push_back(sockFd);
        bindSocket(masterSockets[i], servers[i].getServerData().find("host")->second, 
            servers[i].getServerData().find("port")->second);
    }
}

int   Multiplexer::acceptNewConnection(int masterSocket)
{
    sockaddr_in clientAdd;
    socklen_t   clientAddLen;
    int         acceptedClient;

    clientAddLen = sizeof(clientAdd);
    if((acceptedClient  = accept(masterSocket, (struct sockaddr*)&clientAdd, &clientAddLen)) < 0)
        throw strerror(errno);
     return acceptedClient;   
}

void Multiplexer::findClient(int fd, std::list<std::pair<int, Client> >::iterator& it)
{
    if (clients.size())
    {
        it = clients.begin();
        for (; it != clients.end(); it++)
        { 
            if (it->first == fd)
                return ;
        }
        it =  clients.end();
    }
}
void    Multiplexer::multiplexing()
{
    fd_set  readFds, writeFds, tmpReadFds, tmpWriteFds;
    int     maxFds, readyFds, newClient, b , s;
    char    buff[1024];
    int     i = 0;
    std::list<std::pair<int, Client> >::iterator  it;
    // std::string buffSend;
    // (void)b;
    
    createSockets();
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    for (size_t i = 0; i < masterSockets.size(); i++)
    {
        FD_SET(masterSockets[i], &readFds);
        maxFds = masterSockets[i];
    }

    while (1)
    {
        signal(SIGPIPE, SIG_IGN);
        tmpReadFds = readFds;
        tmpWriteFds = writeFds;
        readyFds = select(maxFds + 1, &tmpReadFds, &tmpWriteFds, NULL, NULL);
        if (readyFds > 0)
        {
            for (int fd = 0; fd < maxFds + 1; fd++)
            {
               if (std::find(masterSockets.begin(), masterSockets.end(), fd) != masterSockets.end()
                    and FD_ISSET(fd, &tmpReadFds))
                {
                    newClient =  acceptNewConnection(fd);
                    if (newClient > maxFds and newClient <= 1024)
                        maxFds = newClient;
                    // std::cout << maxFds << " " << newClient << "\n";
                    FD_SET(newClient, &readFds);
                    FD_SET(newClient, &writeFds);
                    clients.push_back(std::make_pair(newClient, Client(configFile)));
                    it = clients.begin();
                    i++;
                }
                else
                {
                    findClient(fd, it);
                    if (FD_ISSET(fd, &tmpReadFds) and it != clients.end() and it->second.getStage() < RESHEADER)
                    {
                        b = read(fd, buff, 1024);
                        if (b <= 0)
                        {
                            perror("holla : ");
                            exit(1);
                        }
                        if (b > 0)
                        {
                            it->second.setReqBuff(it->second.getReqBuff() + std::string(buff, b)) ;
                            it->second.serve();
                        }
                    }
                    if (FD_ISSET(fd, &tmpWriteFds) and  it != clients.end() and it->second.getStage() >= RESHEADER)
                    {
                        it->second.serve();
                        if ( it->second.getSendBuff().size() > 0 and it->second.getStage() < RESEND)
                        {
                            // std::cout << "before send : " << std::endl;

                            // std::cout << "-----+--> size() " << it->second.getSendBuff().size() << " " << fd << std::endl;
                            s = send(fd, it->second.getSendBuff().c_str(), it->second.getSendBuff().size(), 0);
                            // std::cout << s << "\n";
                            if (s == -1)
                            {
                                // std::cout << "it failed \n";
                                FD_CLR(fd, &readFds);
                                FD_CLR(fd, &writeFds);
                                it = clients.erase(it);
                                close(fd);
                                // perror(NULL);
                                continue;
                            }
                            it->second.setSendBuff("");
                            // std::cout << "after send " << s << std::endl;
                        }
                        if (it->second.getStage() == RESEND)
                        {
                            // std::cout << "--------> here --------> " << std::endl; 
                            FD_CLR(fd, &readFds);
                            FD_CLR(fd, &writeFds);
                            it = clients.erase(it);
                            close(fd);
                        }
                    }
                }
            }   
        }
    }
}