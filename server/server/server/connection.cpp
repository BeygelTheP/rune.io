#include "connection.hpp"

namespace runeio {

connection::connection(websocketpp::connection_hdl conn_hdl, uint32_t id)
	: m_conn_hdl(conn_hdl), m_conn_id(id)
{
}

websocketpp::connection_hdl connection::conn_hdl()
{
	return m_conn_hdl;
}

uint32_t connection::conn_id() {
	return m_conn_id;
}

} // namespace runeio