#include "conn_group.hpp"

namespace runeio {

void conn_group::add(connection_ptr conn_ptr)
{
	m_connections[conn_ptr->conn_id()] = conn_ptr;
}

void conn_group::remove(uint32_t id)
{
	m_connections.erase(id);
}

connection_ptr & conn_group::operator[](uint32_t key)
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

} // namespace runeio