#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "conn_manager.hpp"

class ws_server {
public:
	typedef websocketpp::connection_hdl connection_hdl;
	typedef websocketpp::server<websocketpp::config::asio> server;

	ws_server();

	void run(std::string docroot, uint16_t port);

	void on_http(connection_hdl hdl);
	void on_open(connection_hdl hdl);
	void on_close(connection_hdl hdl);

private:
	server m_endpoint;
	conn_manager m_conn_manager;
	server::timer_ptr m_timer;
	
	std::string m_docroot;
};