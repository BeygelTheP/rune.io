#pragma once
#ifndef RUNEIO_NETWORK_MESSAGE_HPP
#define RUNEIO_NETWORK_MESSAGE_HPP

#include <memory>
#include <string>
#include "message.hpp"

namespace runeio {

class network_message : public message {
public:
	network_message(std::shared_ptr<std::string> str)
		: message(type::M_WS_NETWORK), m_data_ptr(str) {}

	network_message(const std::string & str)
		: network_message(std::make_shared<std::string>(str)) {}

	network_message(const char * str)
		: network_message(std::string(str)) {}

	std::shared_ptr<std::string> get_data(void) { return m_data_ptr; };

private:
	std::shared_ptr<std::string> m_data_ptr;
};

} // namespace runeio

#endif //RUNEIO_NETWORK_MESSAGE_HPP