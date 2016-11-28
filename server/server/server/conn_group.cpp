#include "conn_group.hpp"

void conn_group::add(connection_ptr conn_ptr)
{
	m_connections[conn_ptr->conn_id()] = conn_ptr;
	m_connections.begin();
}

void conn_group::remove(int id)
{
	m_connections.erase(id);
}

connection_ptr & conn_group::operator[](int && key)
{
	return m_connections[key];
}

connection_ptr & conn_group::operator[](const int & key)
{
	return m_connections[key];
}

conn_group::iterator conn_group::begin()
{
	return m_connections.begin();
}

conn_group::iterator conn_group::end()
{
	return m_connections.end();
}
