#pragma once
#ifndef RUNEIO_CONN_MANAGER_HPP
#define RUNEIO_CONN_MANAGER_HPP

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <mutex>
#include "connection.hpp"

namespace runeio {

class conn_manager {
	typedef websocketpp::connection_hdl conn_hdl;

	struct id {};
	struct hdl {};

	typedef boost::multi_index_container<
		connection_ptr,
		boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<id>,
				boost::multi_index::mem_fun<connection, uint32_t, &connection::conn_id>
			>,
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<hdl>,
				boost::multi_index::mem_fun<connection, conn_hdl, &connection::conn_hdl>,
				std::owner_less<conn_hdl>
			>
		>
	> connections_map;

public:
	typedef connections_map::index<id>::type::iterator id_iterator;
	typedef connections_map::index<hdl>::type::iterator hdl_iterator;
	
	const connection_ptr& insert(conn_hdl);
	void erase(conn_hdl);

	id_iterator find(const uint32_t&);
	id_iterator begin_id();
	id_iterator end_id();
	
	hdl_iterator find(const conn_hdl&);
	hdl_iterator end_hdl();
	
	bool empty() const;
	std::size_t size() const;

private:
	connections_map m_connections;

	static uint32_t m_next_conn_id;
	static std::mutex m_mutex;
};

} // namespace runeio

#endif //RUNEIO_CONN_MANAGER_HPP