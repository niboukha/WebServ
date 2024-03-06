/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:45:22 by shicham           #+#    #+#             */
/*   Updated: 2024/03/06 09:30:33 by shicham          ###   ########.fr       */
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
    fd_set  readFds, writeFds, tmpReadFds, tmpWriteFds;
    int     readyFds, maxFds, acceptedClient, r, s;
    char    buff[2048];

    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    for (size_t i = 0; i < servers.size(); i++)
       servers[i].createAndBindSocket(readFds);
    maxFds =  servers[servers.size() - 1].getMasterSocket();
    while (1)
    {
        tmpReadFds = readFds;
        tmpWriteFds = writeFds;
        readyFds = select(maxFds + 1, &tmpReadFds, &tmpWriteFds, NULL, NULL);
        if (readyFds > 0)
        {
            for (size_t i = 0; i < servers.size(); i++)
            {
                // std::cout << "------> here " << FD_ISSET(servers[i].getMasterSocket(), &tmpReadFds) << std::endl;
                if (FD_ISSET(servers[i].getMasterSocket(), &tmpReadFds))
                {
                    acceptedClient = servers[i].acceptNewConnection(readFds, writeFds);
                    // std::cout << "===> " << acceptedClient << " | " << FD_ISSET(acceptedClient, &readFds) << " | " << FD_ISSET(acceptedClient, &writeFds) << std::endl;
                    (acceptedClient > maxFds and acceptedClient <= 1024) ? maxFds = acceptedClient: false;
                    clients.push_back(Client(servers, acceptedClient));
                    // std::cout << ">>>>>>> here" << clients.begin()->getFd() << std::endl;
                    
                }
            }
            for (std::list<Client>::iterator i = clients.begin(); i != clients.end(); i++)
            {
                if (FD_ISSET(i->getFd(), &tmpReadFds) and i->getStage() < RESHEADER)
                {
                        // std::cout << "====-> " << i->getFd() << std::endl;

                    r = read(i->getFd(), buff, 2048);
                     if (r <= 0)
                    {
                        perror("holla : ");
                        exit(1);
                    }
                    if (r > 0)
                    {
                        i->setReqBuff(i->getReqBuff() + std::string(buff, r));
                        i->serve();
                    }
                }
                if (FD_ISSET(i->getFd(), &tmpWriteFds) and i->getStage() >= RESHEADER)
                {
                    i->serve();
                    if ( i->getSendBuff().size() > 0 and i->getStage() < RESEND)
                    {
                        // std::cout << "-----> " << i->getFd() << std::endl;
                        // std::cout << "===> send buff : " << i->getSendBuff().c_str() << "||" <<std::endl;
                        s = send(i->getFd(), i->getSendBuff().c_str(), i->getSendBuff().size(), 0);
                        std::cout << "=====> " << i->getSendBuff().c_str() << std::endl;
                        if (s == -1)
                        {
                            perror ("send : ");
                            FD_CLR(i->getFd(), &readFds);
                            FD_CLR(i->getFd(), &writeFds);
                            close(i->getFd());
                            i = clients.erase(i);
                            continue;
                        }
                        i->setSendBuff("");
                    }
                }
                    if (i->getStage() == RESEND)
                    {
                        // std::cout << "========> here the end ========= " << std::endl;
                        FD_CLR(i->getFd(),&readFds);
                        FD_CLR(i->getFd(), &writeFds);
                        close(i->getFd());
                        i = clients.erase(i);
                    }
                
            }
        }
    }
}

