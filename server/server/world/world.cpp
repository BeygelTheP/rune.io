#include <memory>
#include <chrono>

#include "world.hpp"

world::world()
{
}

world::~world()
{
}

void world::run()
{
	m_set_tick_timer();

	// Start the ASIO io_service run loop on threads
	for (unsigned int i = 0; i < m_num_threads; ++i) {
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

void world::set_server_channel(channel * ch)
{
	m_server_channel = ch;
}

void world::m_set_tick_timer()
{
	m_tick_timer_ptr = std::make_shared<boost::asio::high_resolution_timer>(
		m_io_service,
		std::chrono::milliseconds(50)
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
	m_server_channel->submit(std::make_shared<message>(message::type::M_WS_SERVER));

	m_set_tick_timer();
}

void world::on_queue_submit()
{
	using std::bind;

	m_io_service.post(bind(&world::on_queue_dispatch, this));
}

void world::on_queue_dispatch()
{
	using std::placeholders::_1;
	using std::bind;

	m_channel.dispatch_queue(bind(&world::on_channel_message, this, _1));
}

void world::on_channel_message(message_ptr m_ptr)
{
}

channel * world::get_channel(void)
{
	return &m_channel;
}