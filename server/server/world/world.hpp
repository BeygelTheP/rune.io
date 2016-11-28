#pragma once
#ifndef RUNEIO_WORLD_HPP
#define RUNEIO_WORLD_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/high_resolution_timer.hpp>
#include <boost/thread/thread.hpp>
#include "channel/channel.hpp"

class world
{
public:
	world();
	~world();

	void run();
	void join_all();

	void set_server_channel(channel *);

	channel * get_channel(void);

private:
	static const unsigned int m_num_threads = 1;

	boost::asio::io_service m_io_service;
	boost::thread_group m_threads;
	channel m_channel;

	std::shared_ptr<boost::asio::high_resolution_timer> m_tick_timer_ptr;

	channel * m_server_channel;

	void m_set_tick_timer();
	void m_on_tick_timer(const boost::system::error_code&);

	void on_queue_submit();
	void on_queue_dispatch();
	void on_channel_message(message_ptr);
};

#endif //RUNEIO_WORLD_HPP