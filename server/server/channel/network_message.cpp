#include "network_message.hpp"

network_message::network_message(std::shared_ptr<std::string> d)
	: message(type::M_WS_NETWORK), m_data_ptr(d)
{
}

network_message::network_message(const std::string & str)
	: network_message(std::make_shared<std::string>(str))
{
}

network_message::network_message(const char * ca)
	: network_message(std::string(ca))
{
}