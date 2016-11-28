#pragma once
#ifndef RUNEIO_CHANNEL_HPP
#define RUNEIO_CHANNEL_HPP

#include <queue>
#include <mutex>
#include "message.hpp"

class channel : public std::queue<message_ptr> {
public:
	typedef std::function<void(message_ptr)> dispatch_handler;
	typedef std::function<void(void)> submit_handler;

	void submit(message_ptr);
	message_ptr dispatch_one();
	void dispatch_queue(dispatch_handler);
	
	void bind_submit_handler(submit_handler);

private:
	std::mutex m_mutex;
	submit_handler m_submit_handler;
};

#endif //RUNEIO_CHANNEL_HPP