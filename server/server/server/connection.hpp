#pragma once
#ifndef RUNEIO_CONNECTION_HPP
#define RUNEIO_CONNECTION_HPP

#include <websocketpp/connection.hpp>
#include <string>
#include <memory>

// Connection class for pointing on all related services
class connection {
public:
	connection(int id = -1);

	int get_conn_id() const;

	void set_session_id(std::string id);
	std::string get_session_id() const;

	void set_nickname(std::string nickname);
	std::string get_nickname() const;

private:
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