#include "connection_message.hpp"

connection_message::connection_message(connection_wptr conn_ptr, std::shared_ptr<std::string> d)
	:  network_message(d), m_conn_ptr(conn_ptr)
{
	set_type(type::M_WS_CONN);
}

connection_message::connection_message(connection_wptr conn_ptr, const std::string & str)
	: connection_message(conn_ptr, std::make_shared<std::string>(str))
{
}

connection_message::connection_message(connection_wptr conn_ptr, const char * ca)
	: connection_message(conn_ptr, std::string(ca))
{
}