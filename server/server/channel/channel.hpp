#pragma once
#ifndef RUNEIO_CHANNEL_HPP
#define RUNEIO_CHANNEL_HPP

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <queue>
#include <mutex>
#include "message.hpp"

namespace runeio {

class channel : public std::queue<message_ptr> {
public:
	typedef boost::function<void(message_ptr)> handler;

	channel(boost::asio::io_service *, handler);

	void submit(message_ptr);

private:
	std::mutex m_mutex;
	boost::asio::io_service * m_io_service;
	handler m_handler;

	message_ptr pop_message();

	void dispatch_one();
	void dispatch_queue();
};

} // namespace runeio

#endif //RUNEIO_CHANNEL_HPP