#pragma once
#ifndef RUNEIO_CONNECTION_MESSAGE_HPP
#define RUNEIO_CONNECTION_MESSAGE_HPP

#include <memory>
#include <string>
#include "server/connection.hpp"
#include "network_message.hpp"

class connection_message : public network_message {
public:
	connection_message(connection_wptr, std::shared_ptr<std::string>);
	connection_message(connection_wptr, const std::string &);
	connection_message(connection_wptr, const char *);

	connection_wptr get_conn() { return m_conn_ptr; };

private:
	connection_wptr m_conn_ptr;
};

#endif //RUNEIO_CONNECTION_MESSAGE_HPP