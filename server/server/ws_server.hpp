#pragma once

#include <boost/thread.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "conn_manager.hpp"

class ws_server {
public:
	typedef websocketpp::connection_hdl connection_hdl;
	typedef websocketpp::server<websocketpp::config::asio> server;

	ws_server();

	void run(std::string docroot, uint16_t port);
	void join_all();

	void on_http(connection_hdl hdl);
	void on_open(connection_hdl hdl);
	void on_close(connection_hdl hdl);

private:
	static const unsigned int m_num_threads = 1;
	boost::asio::io_service m_io_service;
	boost::thread_group m_threads;

	server m_endpoint;
	conn_manager m_conn_manager;
	
	std::string m_docroot;
};