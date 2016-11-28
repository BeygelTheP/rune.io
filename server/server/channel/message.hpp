#pragma once
#ifndef RUNEIO_MESSAGE_HPP
#define RUNEIO_MESSAGE_HPP

#include <memory>

class message {
public:
	enum type {
		M_WS_CONN,
		M_WS_NETWORK,
		M_WS_GROUPCAST
	};

	message(type t) : m_type(t) {};
	virtual ~message() {};

	type get_type() const { return m_type; };

protected:
	void set_type(type t) { m_type = t; };

private:
	type m_type;
};

typedef std::shared_ptr<message> message_ptr;

#endif //RUNEIO_MESSAGE_HPP