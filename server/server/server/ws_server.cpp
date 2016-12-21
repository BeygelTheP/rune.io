#include <boost/bind.hpp>
#include <fstream>

#include "channel/network_message.hpp"
#include "channel/connection_message.hpp"
#include "channel/conn_group_message.hpp"
#include "channel/conn_state_message.hpp"
#include "ws_server.hpp"

namespace runeio {

ws_server::ws_server()
	: m_channel(&m_io_service, boost::bind(&ws_server::on_channel_message, this, _1))
{
	// Set up access channels to only log interesting things
	m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
	m_endpoint.set_access_channels(websocketpp::log::alevel::access_core); //access_core);
	m_endpoint.set_access_channels(websocketpp::log::alevel::app); //app);
	m_endpoint.set_error_channels(websocketpp::log::elevel::all); //app);
	

	// Initialize the Asio transport policy
	m_endpoint.init_asio(&m_io_service);

	using websocketpp::lib::placeholders::_1;
	using websocketpp::lib::placeholders::_2;
	using websocketpp::lib::bind;

	// Bind the handlers we are using with network
	m_endpoint.set_open_handler(bind(&ws_server::on_open, this, _1));
	m_endpoint.set_close_handler(bind(&ws_server::on_close, this, _1));
	m_endpoint.set_message_handler(bind(&ws_server::on_message, this, _1, _2));
}

void ws_server::run(uint16_t port)
{
	std::stringstream ss;
	ss << "Running server on port " << port;
	m_endpoint.get_alog().write(websocketpp::log::alevel::app, ss.str());

	m_endpoint.listen(port);

	m_endpoint.start_accept();

	// Start the ASIO io_service run loop on threads
	for (unsigned int i = 0; i < m_num_threads; ++i) {
		m_threads.create_thread(
			[&]()
		{
			while (true) {
				try {
					m_endpoint.run();
				}
				catch (websocketpp::exception const & e) {
					std::cout << "ws_server thread uncaught exception: " << e.what() << std::endl;
				}
			}
		});
	}
}

void ws_server::join_all()
{
	m_threads.join_all();
}

void ws_server::on_open(connection_hdl hdl)
{
	auto conn_ptr = m_conn_manager.insert(hdl);
	m_listen_channel->submit(
		std::make_shared<conn_state_message>(conn_ptr->conn_id(), conn_state_message::state::M_CS_OPEN)
	);
}

void ws_server::on_close(connection_hdl hdl)
{
	auto it = m_conn_manager.find(hdl);
	if (it == m_conn_manager.end_hdl())
		return;

	m_listen_channel->submit(
		std::make_shared<conn_state_message>((*it)->conn_id(), conn_state_message::state::M_CS_CLOSE)
	);

	m_conn_manager.erase(hdl);
}

void ws_server::on_message(connection_hdl hdl, server::message_ptr msg)
{
	auto it = m_conn_manager.find(hdl);
	if (it == m_conn_manager.end_hdl())
		return;

	m_listen_channel->submit(
		std::make_shared<connection_message>((*it)->conn_id(), msg->get_payload())
	);
}

void ws_server::send(const conn_manager::id_iterator &it, std::shared_ptr<std::string> payload)
{
	try {
		m_endpoint.send((*it)->conn_hdl(), *payload, websocketpp::frame::opcode::binary);
	}
	catch (std::exception e) {
		std::cout << "ws_server::send exception: " << e.what() << std::endl;
	}
}

void ws_server::send(const conn_manager::hdl_iterator &it, std::shared_ptr<std::string> payload)
{
	try {
		m_endpoint.send((*it)->conn_hdl(), *payload, websocketpp::frame::opcode::binary);
	}
	catch (std::exception e) {
		std::cout << "ws_server::send exception: " << e.what() << std::endl;
	}
}

void ws_server::on_channel_message(message_ptr m_ptr)
{
	typedef message::type t;

	switch (m_ptr->get_type()) {
	case t::M_WS_CONN:
	{
		std::shared_ptr<connection_message> message = std::static_pointer_cast<connection_message>(m_ptr);

		auto it = m_conn_manager.find(message->get_conn_id());
		if (it == m_conn_manager.end_id())
			break;

		send(it, message->get_data());

		break;
	}
	case t::M_WS_GROUPCAST:
	{
		std::shared_ptr<conn_group_message> message = std::static_pointer_cast<conn_group_message>(m_ptr);

		/*auto group_ptr = message->get_conn_group().lock();
		if (!group_ptr)
			break;

		for (auto it = group_ptr->begin(); it != group_ptr->end(); ++it) {
			send(it, message->get_data());
		}*/

		break;
	}
	case t::M_WS_NETWORK:
	{
		std::shared_ptr<network_message> message = std::static_pointer_cast<network_message>(m_ptr);

		for (auto it = m_conn_manager.begin_id(); it != m_conn_manager.end_id(); ++it) {
			send(it, message->get_data());
		}
		break;
	}
	default:
		m_endpoint.get_alog().write(websocketpp::log::alevel::app, "Wrong message type was dispatched on websocket server");
		break;

	}
}

channel * ws_server::get_channel(void)
{
	return &m_channel;
}

void ws_server::set_listen_channel(channel * ch)
{
	m_listen_channel = ch;
}

} // namespace runeio
