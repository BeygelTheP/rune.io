#pragma once
#ifndef RUNEIO_WSSERVER_HPP
#define RUNEIO_WSSERVER_HPP

#include <boost/thread.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "channel/channel.hpp"
#include "conn_manager.hpp"

namespace runeio {

class ws_server {
public:
	typedef websocketpp::connection_hdl connection_hdl;

	ws_server();

	void run(uint16_t port);
	void join_all();

	channel * get_channel(void);
	void set_listen_channel(channel *);

private:
	typedef websocketpp::server<websocketpp::config::asio> server;

	static const unsigned int m_num_threads = 1;
	boost::asio::io_service m_io_service;
	boost::thread_group m_threads;

	server m_endpoint;
	conn_manager m_conn_manager;
	channel m_channel;

	channel * m_listen_channel;

	// WebSocket events handlers
	void on_open(connection_hdl);
	void on_close(connection_hdl);
	void on_message(connection_hdl, server::message_ptr);

	void send(const conn_manager::id_iterator &, std::shared_ptr<std::string>);
	void send(const conn_manager::hdl_iterator &, std::shared_ptr<std::string>);

	// Channel handlers
	void on_channel_message(message_ptr);
};

} // namespace runeio

#endif //RUNEIO_WSSERVER_HPP