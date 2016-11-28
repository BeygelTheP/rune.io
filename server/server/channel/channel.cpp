#include "channel.hpp"

void channel::submit(message_ptr m_ptr)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	push(m_ptr);
	if (m_submit_handler)
		m_submit_handler();
}

void channel::bind_submit_handler(submit_handler handler)
{
	m_submit_handler = handler;
}

message_ptr channel::dispatch_one()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	message_ptr m_ptr = front();
	pop();

	return m_ptr;
}

void channel::dispatch_queue(dispatch_handler handler)
{
	while (!empty()) {
		handler(dispatch_one());
	}
}