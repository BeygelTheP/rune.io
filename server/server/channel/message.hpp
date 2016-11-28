#pragma once
#ifndef RUNEIO_MESSAGE_HPP
#define RUNEIO_MESSAGE_HPP

#include <memory>

class message {
public:
	enum type {
		M_WS_USER,
		M_WS_SERVER
	};

	message(type t);
	virtual ~message();

	type getType() const { return m_type; };

private:
	type m_type;
};

typedef std::shared_ptr<message> message_ptr;

#endif //RUNEIO_MESSAGE_HPP