#pragma once
#ifndef RUNEIO_CONN_MANAGER_HPP
#define RUNEIO_CONN_MANAGER_HPP

#include <map>
#include "connection.hpp"
#include "conn_group.hpp"

class conn_manager : public conn_group {
public:
	typedef websocketpp::connection_hdl connection_hdl;
	typedef std::map<connection_hdl, connection_ptr, std::owner_less<connection_hdl>> conn_list;

	conn_manager();

	void add_connection(connection_hdl);
	void remove_connection(connection_hdl);

	connection_ptr & operator[](connection_hdl &&);
	connection_ptr & operator[](const connection_hdl &);

private:
	conn_list m_connections;

	int m_next_conn_id;
};

#endif //RUNEIO_CONN_MANAGER_HPP