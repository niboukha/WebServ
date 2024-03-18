/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:45:22 by shicham           #+#    #+#             */
/*   Updated: 2024/03/17 16:04:43 by niboukha         ###   ########.fr       */
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
    time_t  currTime;

    // std::cout << ">>>>>> fd : " << cl.getFd() << std::endl;
    r = read(cl.getFd(), buff, 2048);

    if (r <= 0)
    {
        // if (errno == EINTR)
        std::cout << "=====> yeeees " << r << " " << errno << std::endl;
        perror("holla : ");
        exit(1);
    }
    if (r > 0)
    {
        currTime = time(0);
        cl.setLastRead(currTime);
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

void    Multiplexer:: dropClient(Client& cl)
{
    std::string s;
    time_t  currTime;

    cl.getRequest().setMethod("GET");
    cl.getRequest().setProtocolVersion("HTTP/1.1");
    if (cl.getResponse().getCgiStage() == WAITCGI)
    {
        s = "504 Gateway Timeout";
        cl.getResponse().setStatusCodeMsg(s);
        cl.getResponse().setPath( cl.getResponse().pathErrorPage("504"));
    }
    else
    {
        s = "408 Request Timeout";
        cl.getResponse().setStatusCodeMsg(s);
        cl.getResponse().setPath( cl.getResponse().pathErrorPage("408"));
    }
    currTime = time(0);
    cl.setLastRead(currTime);
    cl.getResponse().setCgiStage(ERRORCGI);
    cl.setStage(RESHEADER);
}

void    Multiplexer::removeCgiFiles( Client& cl )
{
    std::string pathI;
    std::string pathO;
    
    pathO = cl.getResponse().getPathOutput();
    pathI = cl.getResponse().getPathInput();
    std::remove(pathI.c_str());
	std::remove(pathO.c_str());
}

void    Multiplexer::multiplexing()
{
    fd_set  readFds, writeFds, tmpReadFds, tmpWriteFds;
    int     readyFds, maxFds, acceptedClient ;
    std::vector<Server> servs;
   
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
                    fcntl(acceptedClient, F_SETFL, O_NONBLOCK, FD_CLOEXEC);//update/////////////////////////////////////////////////
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
                if ((time(0) - i->getLastRead()) >= 10)
                    dropClient(*i);
                if (i->getStage() == RESEND)
                {
                    // std::cout << " CLIENT WAS DIE !!" << std::endl;
                    clear(readFds, writeFds, *i);
                    removeCgiFiles(*i);
                    i = clients.erase(i);
                    i--;
                } 
            }
        }
    }
    
}

