/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:45:22 by shicham           #+#    #+#             */
/*   Updated: 2024/03/01 21:59:58 by shicham          ###   ########.fr       */
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
    std::list<std::pair<int, Client> >::iterator i = clients.begin();
    for (; i != clients.end(); i++)
    { 
        if (i->first == fd)
        {
             it = i;
            break;
        }
    }
    it =  clients.end();
}
void    Multiplexer::multiplexing()
{
    fd_set  readFds, writeFds, tmpReadFds, tmpWriteFds;
    int     maxFds, readyFds, newClient, b , s;
    char    buff[1024];
    int     i = 0;
    // std::string buffSend;
    // (void)b;
    
    createSockets();
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    for (size_t i = 0; i < masterSockets.size(); i++)
    {
        // std::cout << acceptNewConnection(masterSockets[i]) << std::endl;
        FD_SET(masterSockets[i], &readFds);
        maxFds = masterSockets[i];
    }

    while (1)
    {
        tmpReadFds = readFds;
        tmpWriteFds = writeFds;
        readyFds = select(maxFds + 1, &tmpReadFds, &tmpWriteFds, NULL, NULL);
        if (readyFds > 0)
        {
            for (int fd = 0; fd < maxFds + 1; fd++)
            {
                // std::cout << "-------> start | " << fd << std::endl;
               if (std::find(masterSockets.begin(), masterSockets.end(), fd) != masterSockets.end()
                    and FD_ISSET(fd, &tmpReadFds))
                {
                    newClient =  acceptNewConnection(fd);
                    if (newClient > maxFds and newClient <= 1024)
                        maxFds = newClient;
                    FD_SET(newClient, &readFds);
                    FD_SET(newClient, &writeFds);
                    // Request        req;//to ckeck mn b3ed !!!
                    // Client          client(req);
                    std::cout << "----> here " << std::endl;
                    req.push_back(Request());
                    req[i].SetConfigFile(configFile);
                    // clients[newClient] = Client(req);
                    std::cout << "=======> keleb" << std::endl;
                    // clients.insert(std::make_pair(newClient, Client(req[i])));
                    clients.push_back(std::make_pair(newClient, Client(req[i])));
                    i++;
                }
                else
                {
                    std::list<std::pair<int, Client> >::iterator  it;
                    findClient(fd, it);
                    if (FD_ISSET(fd, &readFds) and it != clients.end() and it->second.getStage() < RESHEADER)
                    {
                        b = read(fd, buff, 1024);
                        if (b > 0)
                        {
                            it->second.setReqBuff(it->second.getReqBuff() + std::string(buff, b)) ;
                            it->second.serve();
                        }
                        
                    }
                    if (FD_ISSET(fd, &writeFds) and  it != clients.end() and it->second.getStage() >= RESHEADER)
                    {
                        it->second.serve();
                        // std::cout << " ===> send bufff : " <<  it->second.getSendBuff().size()<< std::endl;
                        s = send(fd, it->second.getSendBuff().c_str(), it->second.getSendBuff().size(), 0);
                        
                        if (it->second.getStage() == RESEND)
                        {
                            FD_CLR(fd, &readFds);
                            FD_CLR(fd, &writeFds);
                            close(fd);
                        }
                            std::cout << "in RESENDDDDDDDD\n";
                    }
                }
            }
            
        }
    }
}