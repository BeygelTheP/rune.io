#pragma once
#ifndef RUNEIO_CONN_GROUP_MESSAGE_HPP
#define RUNEIO_CONN_GROUP_MESSAGE_HPP

#include <memory>
#include <string>
#include "server/conn_group.hpp"
#include "network_message.hpp"

namespace runeio {

class conn_group_message : public network_message {
public:
	conn_group_message(conn_group_wptr conn_group_ptr, std::shared_ptr<std::string> str)
		: network_message(str), m_conn_group_ptr(conn_group_ptr)
	{
		set_type(type::M_WS_GROUPCAST);
	}

	conn_group_message(conn_group_wptr conn_group_ptr, const std::string & str)
		: conn_group_message(conn_group_ptr, std::make_shared<std::string>(str)) {}

	conn_group_message(conn_group_wptr conn_group_ptr, const char * str)
		: conn_group_message(conn_group_ptr, std::string(str)) {}

    conn_group_wptr get_conn_group() { return m_conn_group_ptr; }

private:
	conn_group_wptr m_conn_group_ptr;
};

} // namespace runeio

#endif //RUNEIO_CONN_GROUP_MESSAGE_HPP
