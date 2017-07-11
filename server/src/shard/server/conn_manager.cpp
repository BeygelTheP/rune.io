#include "conn_manager.hpp"

namespace runeio {

uint32_t conn_manager::m_next_conn_id = 1;
std::mutex conn_manager::m_mutex;

const connection_ptr& conn_manager::insert(conn_hdl hdl)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	connection_ptr c_ptr = std::make_shared<connection>(hdl, m_next_conn_id++);

	auto ret = m_connections.insert(c_ptr).first;

	return *ret;
}

void conn_manager::erase(conn_hdl h)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_connections.get<hdl>().erase(h);
}

conn_manager::id_iterator conn_manager::find(const uint32_t & i)
{
	return m_connections.get<id>().find(i);
}

conn_manager::id_iterator conn_manager::begin_id()
{
	return m_connections.get<id>().begin();
}

conn_manager::id_iterator conn_manager::end_id()
{
	return m_connections.get<id>().end();
}

conn_manager::hdl_iterator conn_manager::find(const conn_hdl & h)
{
	return m_connections.get<hdl>().find(h);
}

conn_manager::hdl_iterator conn_manager::end_hdl()
{
	return m_connections.get<hdl>().end();
}

bool conn_manager::empty() const
{
	return m_connections.empty();
}

std::size_t conn_manager::size() const
{
	return m_connections.size();
}

} // namespace runeio