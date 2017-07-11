#include <boost/bind.hpp>
#include <memory>
#include <chrono>

#include <proto/core.pb.h>
#include <proto/user.pb.h>
#include <proto/character.pb.h>

#include "channel/connection_message.hpp"
#include "channel/conn_state_message.hpp"
#include "world.hpp"

namespace {

const unsigned int g_num_threads = 1;
const int g_tickdelay = 50;

} // namespace

namespace runeio {

world::world()
	: m_channel(&m_io_service, boost::bind(&world::on_channel_message, this, _1))
{
}

void world::run()
{
	m_set_tick_timer();

	// Start the ASIO io_service run loop on threads
    for (unsigned int i = 0; i < g_num_threads; ++i) {
		m_threads.create_thread(
			[&]()
		{
			try {
				m_io_service.run();
			}
			catch (std::exception const & e) {
				std::cout << e.what() << std::endl;
			}
		});
	}
}

void world::join_all()
{
	m_threads.join_all();
}

void world::m_set_tick_timer()
{
	m_tick_timer_ptr = std::make_shared<boost::asio::high_resolution_timer>(
		m_io_service,
        std::chrono::milliseconds(g_tickdelay)
	);

	m_tick_timer_ptr->async_wait(
		boost::bind(
			&world::m_on_tick_timer,
			this,
			boost::placeholders::_1
		)
	);
}

void world::m_on_tick_timer(const boost::system::error_code & e)
{
	m_set_tick_timer();

	// process input
	for (auto it = m_characters.begin(); it != m_characters.end(); ++it)
		it->second->process_input();

	// moving chars
	for (auto it = m_characters.begin(); it != m_characters.end(); ++it)
		it->second->move();

	// send game state to players
	auto net_message = proto::message::default_instance();
	auto net_gamestate = net_message.mutable_game_state();
	auto ch_map = net_gamestate->mutable_pub_chars();

	for (auto it = m_characters.begin(); it != m_characters.end(); ++it)
		it->second->fill_net_public(&(*ch_map)[it->first]);

	for (auto it = m_characters.begin(); it != m_characters.end(); ++it) {
		it->second->fill_net_private(net_gamestate->mutable_private_char());

		m_server_channel->submit(std::make_shared<connection_message>(it->first, net_message.SerializeAsString()));
	}
}

void world::on_channel_message(message_ptr m_ptr)
{
	typedef message::type t;

	switch (m_ptr->get_type()) {
	case t::M_WS_CONN:
	{
		typedef proto::message::SharedCase sc;
		std::shared_ptr<connection_message> message = std::static_pointer_cast<connection_message>(m_ptr);

		auto net_message = proto::message::default_instance();
		net_message.ParseFromString(*(message->get_data()));
		
		if (net_message.shared_case() != sc::kUserInput)
			break;

		auto p_it = m_characters.find(message->get_conn_id());

		if (p_it == m_characters.end())
			break;

		p_it->second->parse_net_input(&net_message.user_input());

		break;
	}
	case t::M_WS_CONN_STATE:
	{
		typedef conn_state_message::state s;

		std::shared_ptr<conn_state_message> message = std::static_pointer_cast<conn_state_message>(m_ptr);

		if (message->get_state() == s::M_CS_OPEN) {
			m_characters[message->get_conn_id()] = std::make_shared<character>(message->get_conn_id());

			auto net_message = proto::message::default_instance();
			
			auto net_info = net_message.mutable_info();
            net_info->set_tickdelay(g_tickdelay);
			m_server_channel->submit(std::make_shared<connection_message>(message->get_conn_id(), net_message.SerializeAsString()));

			auto net_user = net_message.mutable_user();
			net_user->set_id(message->get_conn_id());
			m_server_channel->submit(std::make_shared<connection_message>(message->get_conn_id(), net_message.SerializeAsString()));
		}
		else if (message->get_state() == s::M_CS_CLOSE)
			m_characters.erase(message->get_conn_id());

		break;
	}
	default:
		break;
	}
}

channel * world::get_channel(void)
{
	return &m_channel;
}

void world::set_server_channel(channel * ch)
{
	m_server_channel = ch;
}

} // namespace runeio
