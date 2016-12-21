#pragma once
#ifndef RUNEIO_WORLD_HPP
#define RUNEIO_WORLD_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/high_resolution_timer.hpp>
#include <boost/thread/thread.hpp>
#include "channel/channel.hpp"
#include "character.hpp"

namespace runeio {
	
class world
{
public:
	world();

	void run();
	void join_all();
	
	channel * get_channel(void);
	void set_server_channel(channel *);

private:
	static const unsigned int m_num_threads = 1;
	static const int m_tickdelay = 50;

	boost::asio::io_service m_io_service;
	boost::thread_group m_threads;
	channel m_channel;
	channel * m_server_channel;

	std::shared_ptr<boost::asio::high_resolution_timer> m_tick_timer_ptr;

	std::map<int, character_ptr> m_characters;

	void m_set_tick_timer();
	void m_on_tick_timer(const boost::system::error_code&);

	void on_channel_message(message_ptr);
};

} // namespace runeio

#endif //RUNEIO_WORLD_HPP