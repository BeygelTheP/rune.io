#pragma once
#ifndef RUNEIO_NETWORK_MESSAGE_HPP
#define RUNEIO_NETWORK_MESSAGE_HPP

#include <memory>
#include <string>
#include "message.hpp"

class network_message : public message {
public:
	network_message(std::shared_ptr<std::string>);
	network_message(const std::string &);
	network_message(const char *);

	std::shared_ptr<std::string> get_data(void) { return m_data_ptr; };

private:
	std::shared_ptr<std::string> m_data_ptr;
};

#endif //RUNEIO_NETWORK_MESSAGE_HPP