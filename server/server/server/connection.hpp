#pragma once
#ifndef RUNEIO_CONNECTION_HPP
#define RUNEIO_CONNECTION_HPP

#include <websocketpp/connection.hpp>
#include <string>
#include <memory>

// Connection class for pointing on all related services
class connection {
public:
	connection(websocketpp::connection_hdl, int id = -1);

	int conn_id() const;
	websocketpp::connection_hdl conn_hdl();

	void session_id(std::string id);
	std::string session_id() const;

	void nickname(std::string nickname);
	std::string nickname() const;

private:
	// Connection network handler
	websocketpp::connection_hdl m_conn_hdl;

	// Internal for current server run connection id (logic: dumb +1 for each new connection)
	int m_conn_id;

	// Global session id that allows save related services state
	std::string m_session_id;

	// Part of future services: player nickname
	// 
	std::string m_nickname;
};

typedef std::shared_ptr<connection> connection_ptr;
typedef std::weak_ptr<connection> connection_wptr;

#endif //RUNEIO_CONNECTION_HPP