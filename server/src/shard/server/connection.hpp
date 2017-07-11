#pragma once
#ifndef RUNEIO_CONNECTION_HPP
#define RUNEIO_CONNECTION_HPP

#include <websocketpp/connection.hpp>
#include <string>
#include <memory>

namespace runeio {

// Connection class for pointing on all related services
class connection {
public:
	connection(websocketpp::connection_hdl, uint32_t id);

	uint32_t conn_id();
	websocketpp::connection_hdl conn_hdl();

private:
	// Connection network handler
	websocketpp::connection_hdl m_conn_hdl;

	// Internal for current server run connection id (logic: dumb +1 for each new connection)
	uint32_t m_conn_id;
};

typedef std::shared_ptr<connection> connection_ptr;

} // namespace runeio

#endif //RUNEIO_CONNECTION_HPP