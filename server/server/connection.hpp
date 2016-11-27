#pragma once

#include <websocketpp/connection.hpp>
#include <string>
#include <memory>

// Connection class for pointing on all related services
class connection {
public:

private:
	// WebSocket connection
	websocketpp::connection_hdl m_wscon_hdl;

	// Internal for current server run connection id (logic: dumb +1 for each new connection)
	int m_conn_id;

	// Global session id that allows save related services state
	std::string m_session_id;

	// Part of future services: player nickname
	// 
	std::string m_nickname;
};

typedef std::shared_ptr<connection> connection_ptr;