#include <fstream>

#include "ws_server.hpp"

ws_server::ws_server()
{
	// Set up access channels to only log interesting things
	m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
	m_endpoint.set_access_channels(websocketpp::log::alevel::access_core); //access_core);
	m_endpoint.set_access_channels(websocketpp::log::alevel::app); //app);

	// Initialize the Asio transport policy
	m_endpoint.init_asio(&m_io_service);

	using websocketpp::lib::placeholders::_1;
	using websocketpp::lib::bind;

	// Bind the handlers we are using with network
	m_endpoint.set_open_handler(bind(&ws_server::on_open, this, _1));
	m_endpoint.set_close_handler(bind(&ws_server::on_close, this, _1));
	m_endpoint.set_http_handler(bind(&ws_server::on_http, this, _1));

	// Bind the queue submit handler
	m_channel.bind_submit_handler(bind(&ws_server::on_queue_submit, this));
}

void ws_server::run(std::string docroot, uint16_t port)
{
	std::stringstream ss;
	ss << "Running telemetry server on port " << port << " using docroot=" << docroot;
	m_endpoint.get_alog().write(websocketpp::log::alevel::app, ss.str());

	m_docroot = docroot;

	// listen on specified port
	m_endpoint.listen(port);

	// Start the server accept loop
	m_endpoint.start_accept();

	// Start the ASIO io_service run loop on threads
	for (unsigned int i = 0; i < m_num_threads; ++i) {
		m_threads.create_thread(
			[&]()
		{
			try {
				m_endpoint.run();
			}
			catch (websocketpp::exception const & e) {
				std::cout << e.what() << std::endl;
			}
		});
	}
}

void ws_server::join_all()
{
	m_threads.join_all();
}

void ws_server::on_http(connection_hdl hdl)
{
	// Upgrade our connection handle to a full connection_ptr
	server::connection_ptr con = m_endpoint.get_con_from_hdl(hdl);

	std::ifstream file;
	std::string filename = con->get_resource();
	std::string response;

	m_endpoint.get_alog().write(websocketpp::log::alevel::app,
		"http request1: " + filename);

	if (filename == "/") {
		filename = m_docroot + "index.html";
	}
	else {
		filename = m_docroot + filename.substr(1);
	}

	m_endpoint.get_alog().write(websocketpp::log::alevel::app,
		"http request2: " + filename);

	file.open(filename.c_str(), std::ios::in);
	if (!file) {
		// 404 error
		std::stringstream ss;

		ss << "<!doctype html><html><head>"
			<< "<title>Error 404 (Resource not found)</title><body>"
			<< "<h1>Error 404</h1>"
			<< "<p>The requested URL " << filename << " was not found on this server.</p>"
			<< "</body></head></html>";

		con->set_body(ss.str());
		con->set_status(websocketpp::http::status_code::not_found);
		return;
	}

	file.seekg(0, std::ios::end);
	response.reserve((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);

	response.assign((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	con->set_body(response);
	con->set_status(websocketpp::http::status_code::ok);
}

void ws_server::on_open(connection_hdl hdl)
{
	m_conn_manager.add_connection(hdl);
}

void ws_server::on_close(connection_hdl hdl)
{
	m_conn_manager.remove_connection(hdl);
}

void ws_server::on_queue_submit()
{
	using websocketpp::lib::bind;

	m_io_service.post(bind(&ws_server::on_queue_dispatch, this));
}

void ws_server::on_queue_dispatch()
{
	using websocketpp::lib::placeholders::_1;
	using websocketpp::lib::bind;

	m_channel.dispatch_queue(bind(&ws_server::on_channel_message, this, _1));
}

void ws_server::on_channel_message(message_ptr m_ptr)
{
	if (m_ptr->getType() != message::type::M_WS_SERVER)
		m_endpoint.get_alog().write(websocketpp::log::alevel::app, "Not M_WS_SERVER message was dispatched on websocket server");
	else {
		m_endpoint.get_alog().write(websocketpp::log::alevel::app, "M_WS_SERVER message!");
	}
}

channel * ws_server::get_channel(void)
{
	return &m_channel;
}