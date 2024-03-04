/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:45:22 by shicham           #+#    #+#             */
/*   Updated: 2024/03/04 17:53:41 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Multiplexer.hpp"

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

void    Multiplexer::multiplexing()
{
    fd_set  readsFds, writeFds, tmpReadFds, tmpWriteFds;
    int     readyFds, maxFds, acceptedClient, r, s;
    char    buff[1024];

    FD_ZERO(&readsFds);
    FD_ZERO(&writeFds);
    for (size_t i = 0; i < servers.size(); i++)
       servers[i].createAndBindSocket(readsFds);
    maxFds =  servers[servers.size() - 1].getMasterSocket();
    while (1)
    {
        tmpReadFds = readsFds;
        tmpWriteFds = writeFds;
        readyFds = select(maxFds + 1, &tmpReadFds, &tmpWriteFds, NULL, NULL);
        if (readyFds > 0)
        {
            for (size_t i = 0; i < servers.size(); i++)
            {
                // std::cout << "------> here " << FD_ISSET(servers[i].getMasterSocket(), &tmpReadFds) << std::endl;
                if (FD_ISSET(servers[i].getMasterSocket(), &tmpReadFds))
                {
                    acceptedClient = servers[i].acceptNewConnection(readsFds, writeFds);
                    (acceptedClient > maxFds and acceptedClient <= 1024) ? maxFds = acceptedClient: false;
                    clients.push_back(Client(servers, acceptedClient));
                    // std::cout << ">>>>>>> here" << clients.begin()->getFd() << std::endl;
                    
                }
            }
            for (std::list<Client>::iterator i = clients.begin(); i != clients.end(); i++)
            {
                if (FD_ISSET(i->getFd(), &tmpReadFds) and i->getStage() < RESHEADER)
                {
                    r = read(i->getFd(), buff, 1024);
                    if (r > 0)
                    {
                        i->setReqBuff(i->getReqBuff() + buff);
                        i->serve();
                    }
                }
                if (FD_ISSET(i->getFd(), &tmpWriteFds) and i->getStage() >= RESHEADER)
                {
                    i->serve();
                    // std::cout << "===> send buff : " << i->getSendBuff().c_str() << std::endl;
                    s = send(i->getFd(), i->getSendBuff().c_str(), i->getSendBuff().size(), 0);
                    if (i->getStage() == RESEND)
                    {
                        // std::cout << "========> here the end ========= " << std::endl;
                        FD_CLR(i->getFd(),&readsFds);
                        FD_CLR(i->getFd(), &writeFds);
                        i = clients.erase(i);
                        close(i->getFd());
                    }
                }
            }
        }
    }
}

