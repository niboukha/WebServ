/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:45:22 by shicham           #+#    #+#             */
/*   Updated: 2024/03/12 15:35:50 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Multiplexer.hpp"

Multiplexer::Multiplexer() 
{
}

Multiplexer::~Multiplexer()
{
}


void    Multiplexer::setServers(const std::vector<Server>& servers)
{
    this->servers = servers;
}

void     Multiplexer::readReq(Client& cl)
{
    char buff[2048];
    int r;

    r = read(cl.getFd(), buff, 2048);
    if (r <= 0)
    {
        perror("holla : ");
        exit(1);
    }
    
    if (r > 0)
    {
        cl.setReqBuff(cl.getReqBuff() + std::string(buff, r));
        cl.serve();
    }
}

void    Multiplexer::sendRes(Client& cl)
{
    int s;

    cl.serve();
    if (cl.getSendBuff().size() > 0 and cl.getStage() < RESEND)
    {
        s = send(cl.getFd(), cl.getSendBuff().c_str(), cl.getSendBuff().size(), 0);
        std::cout << cl.getSendBuff().size() << " " << s << "\n";
        if (s == -1)
        {
            perror ("send : ");//!!!
            cl.setStage(RESEND);
        }
        cl.setSendBuff("");
    }
}

void Multiplexer::clear(fd_set& read, fd_set& write, Client& client)
{
    FD_CLR(client.getFd(),&read);
    FD_CLR(client.getFd(), &write);
    close(client.getFd());
}

void    Multiplexer::reqServers(Server& serv, std::vector<Server>& servs)
{
    std::string host, port;

    host = serv.getServerData().find("host")->second;
    port = serv.getServerData().find("port")->second;
    for (size_t i = 0; i < servers.size(); i++)
    {
        if (!host.compare(servers[i].getServerData().find("host")->second)
            and !port.compare(servers[i].getServerData().find("port")->second))
            servs.push_back(servers[i]);
    } 
}

void    Multiplexer::multiplexing()
{
    fd_set  readFds, writeFds, tmpReadFds, tmpWriteFds;
    int     readyFds, maxFds, acceptedClient ;
    std::vector<Server> servs;
    struct timeval tv;
    
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    for (size_t i = 0; i < servers.size(); i++)
       servers[i].createAndBindSocket(readFds);
    maxFds =  servers[servers.size() - 1].getMasterSocket();
    signal(SIGPIPE, SIG_IGN);
    std::cout << GREEN << "Server is running..." << RESET << std::endl;
    while (1)
    {
        tmpReadFds = readFds;
        tmpWriteFds = writeFds;
        readyFds = select(maxFds + 1, &tmpReadFds, &tmpWriteFds, NULL, NULL);
        if (readyFds > 0)
        {
            for (size_t i = 0; i < servers.size(); i++)
            {
                if (FD_ISSET(servers[i].getMasterSocket(), &tmpReadFds))
                {
                    acceptedClient = servers[i].acceptNewConnection(readFds, writeFds);
                    fcntl(acceptedClient, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
                    reqServers(servers[i], servs);
                    (acceptedClient > maxFds and acceptedClient <= 1024) ? maxFds = acceptedClient: false;
                    clients.push_back(Client(servs, acceptedClient));
                }
            }
            for (std::list<Client>::iterator i = clients.begin(); i !=  clients.end(); i++)
            {
                if (FD_ISSET(i->getFd(), &tmpReadFds) and i->getStage() < RESHEADER)
                    readReq(*i);
                if (FD_ISSET(i->getFd(), &tmpWriteFds) and i->getStage() >= RESHEADER)
                    sendRes(*i);
                if (i->getStage() == RESEND)
                {
                    
                    clear(readFds, writeFds, *i);
                    i = clients.erase(i);
                    i--;
                } 
            }
        }
    }
    
}

