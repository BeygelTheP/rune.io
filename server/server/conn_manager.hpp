#pragma once

#include <set>

#include "connection.hpp"

class conn_manager {
public:
	typedef websocketpp::connection_hdl connection_hdl;
	typedef std::map<connection_hdl, connection, std::owner_less<connection_hdl>> con_list;

	conn_manager();

	connection & add_connection(connection_hdl);
	void remove_connection(connection_hdl);

private:
	con_list m_connections;

	int m_next_conn_id;
};