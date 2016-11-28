#include "connection.hpp"

connection::connection(websocketpp::connection_hdl conn_hdl, int id)
	: m_conn_hdl(conn_hdl), m_conn_id(id)
{
	m_nickname = "";
	m_session_id = "";
}

websocketpp::connection_hdl connection::conn_hdl()
{
	return m_conn_hdl;
}

int connection::conn_id() const {
	return m_conn_id;
}

void connection::session_id(std::string id) {
	m_session_id = id;
}

std::string connection::session_id() const {
	return m_session_id;
}

void connection::nickname(std::string nickname) {
	m_nickname = nickname;
}

std::string connection::nickname() const {
	return m_nickname;
}