#include "conn_manager.hpp"

conn_manager::conn_manager()
	: m_next_conn_id(0)
{
}

void conn_manager::add_connection(connection_hdl hdl)
{
	connection_ptr c_ptr = std::make_shared<connection>(hdl, m_next_conn_id++);

	m_connections[hdl] = c_ptr;
	add(c_ptr);
}

void conn_manager::remove_connection(connection_hdl hdl)
{
	remove(m_connections[hdl]->conn_id());
	m_connections.erase(hdl);
}

connection_ptr & conn_manager::operator[](connection_hdl && hdl)
{
	return m_connections[hdl];
}

connection_ptr & conn_manager::operator[](const connection_hdl & hdl)
{
	return m_connections[hdl];
}
