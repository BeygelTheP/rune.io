#include "connection.hpp"

connection::connection(int id)
	: m_conn_id(id)
{
	m_nickname = "";
	m_session_id = "";
}

int connection::get_conn_id() const {
	return m_conn_id;
}

void connection::set_session_id(std::string id) {
	m_session_id = id;
}

std::string connection::get_session_id() const {
	return m_session_id;
}

void connection::set_nickname(std::string nickname) {
	m_nickname = nickname;
}

std::string connection::get_nickname() const {
	return m_nickname;
}