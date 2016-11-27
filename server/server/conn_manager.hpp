#pragma once

#include <set>

#include "connection.hpp"

class conn_manager {
public:
	typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_list;

	con_list m_connections;

private:
};