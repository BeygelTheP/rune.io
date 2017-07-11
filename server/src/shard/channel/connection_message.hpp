#pragma once
#ifndef RUNEIO_CONNECTION_MESSAGE_HPP
#define RUNEIO_CONNECTION_MESSAGE_HPP

#include <memory>
#include <string>
#include "server/connection.hpp"
#include "network_message.hpp"

namespace runeio {

class connection_message : public network_message {
public:
	connection_message(uint32_t conn_id, std::shared_ptr<std::string> str)
		: network_message(str), m_conn_id(conn_id)
	{
		set_type(type::M_WS_CONN);
	}

	connection_message(uint32_t conn_id, const std::string & str)
		: connection_message(conn_id, std::make_shared<std::string>(str)) {}

	connection_message(uint32_t conn_id, const char * str)
		: connection_message(conn_id, std::string(str)) {}

    uint32_t get_conn_id() { return m_conn_id; }

private:
	uint32_t m_conn_id;
};

} // namespace runeio

#endif //RUNEIO_CONNECTION_MESSAGE_HPP
