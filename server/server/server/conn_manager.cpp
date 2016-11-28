#include "conn_manager.hpp"

conn_manager::conn_manager()
	: m_next_conn_id(0)
{
}

void conn_manager::add_connection(connection_hdl hdl)
{
	connection conn(m_next_conn_id++);

	m_connections[hdl] = conn;
}

void conn_manager::remove_connection(connection_hdl hdl)
{
	m_connections.erase(hdl);
}
