#include "channel.hpp"

namespace runeio {

channel::channel(boost::asio::io_service * service, handler handler)
{
	m_io_service = service;
	m_handler = handler;
}

void channel::submit(message_ptr m_ptr)
{
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		push(m_ptr);
	}

	m_io_service->post(boost::bind(&channel::dispatch_one, this));
}

message_ptr channel::pop_message()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	message_ptr m_ptr = front();
	pop();

	return m_ptr;
}

void channel::dispatch_one()
{
	m_handler(pop_message());
}

void channel::dispatch_queue()
{
	while (!empty()) {
		dispatch_one();
	}
}

} // namespace runeio