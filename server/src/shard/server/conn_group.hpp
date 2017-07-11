#pragma once
#ifndef RUNEIO_CONN_GROUP_HPP
#define RUNEIO_CONN_GROUP_HPP

#include <map>
#include "connection.hpp"

namespace runeio {

class conn_group
{
public:
	typedef std::map<uint32_t, connection_ptr>::iterator iterator;

	void add(connection_ptr);
	void remove(uint32_t);

	connection_ptr & operator[](uint32_t);

	iterator begin();
	iterator end();

private:
	std::map<uint32_t, connection_ptr> m_connections;
};

typedef std::shared_ptr<conn_group> conn_group_ptr;
typedef std::weak_ptr<conn_group> conn_group_wptr;

} // namespace runeio

#endif //RUNEIO_CONN_GROUP_HPP
