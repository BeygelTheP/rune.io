#pragma once
#ifndef RUNEIO_CHARACTER_HPP
#define RUNEIO_CHARACTER_HPP

#include <boost/circular_buffer.hpp>
#include "proto/character.pb.h"

namespace runeio {

class character {
public:
	character(int id);

	int get_conn_id() const { return m_conn_id; }

	void process_input();
	void move();

	void parse_net_input(const proto::character::user_input *);

	void fill_net_public(proto::character::public_char *) const;
	void fill_net_private(proto::character::private_char *) const;

private:
	enum input_type {
		KEY,
		MOUSE
	};

	struct input {
		uint32_t id;
		input_type type;
		uint32_t controls;
		float target_view_angle;
	};

	int m_conn_id;

	float m_x;
	float m_y;
	float m_vx;
	float m_vy;
	float m_angle;
	float m_vangle;

	bool m_up_key;
	bool m_down_key;
	bool m_right_key;
	bool m_left_key;
	float m_target_angle;

	boost::circular_buffer<std::shared_ptr<input>> m_input_buffer;

	uint32_t m_last_parsed_input_id;
};

typedef std::shared_ptr<character> character_ptr;

} // namespace runeio

#endif //RUNEIO_CHARACTER_HPP
