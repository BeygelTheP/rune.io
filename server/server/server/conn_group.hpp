#pragma once
#ifndef RUNEIO_CONN_GROUP_HPP
#define RUNEIO_CONN_GROUP_HPP

#include <map>
#include "connection.hpp"

class conn_group
{
public:
	typedef std::map<int, connection_ptr>::iterator iterator;

	void add(connection_ptr);
	void remove(int);

	connection_ptr & operator[](int &&);
	connection_ptr & operator[](const int &);

	iterator begin();
	iterator end();

private:
	std::map<int, connection_ptr> m_connections;
};

typedef std::shared_ptr<conn_group> conn_group_ptr;


#endif //RUNEIO_CONN_GROUP_HPP
