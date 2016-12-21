#pragma once
#ifndef RUNEIO_CONNECTION_STATE_MESSAGE_HPP
#define RUNEIO_CONNECTION_STATE_MESSAGE_HPP

#include <memory>
#include <string>
#include "server/connection.hpp"
#include "network_message.hpp"

namespace runeio {

class conn_state_message : public message {
public:
	enum state {
		M_CS_OPEN,
		M_CS_CLOSE
	};

	conn_state_message(uint32_t conn_id, state s)
		: message(type::M_WS_CONN_STATE), m_conn_id(conn_id), m_state(s) {}

	uint32_t get_conn_id() { return m_conn_id; }
	state get_state() { return m_state; }

private:
	uint32_t m_conn_id;
	state m_state;
};

} // namespace runeio

#endif //RUNEIO_CONNECTION_STATE_MESSAGE_HPP