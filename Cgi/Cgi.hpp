/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:38:41 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/08 09:30:00 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "../Response/Response.hpp"

class	Response;

class	Cgi
{
	private:
		Response							&response;

		std::map<std::string, std::string>	requestEnv;
		char								**env;

	public:

		Cgi (Response &res);
		~Cgi();

		void	linkReqEnv();

		void	fillEnvirement( );
};

#endif
