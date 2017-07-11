#include <iostream>
#include "proto/character.pb.h"
#include "character.hpp"

namespace runeio {

character::character(int id)
	: m_input_buffer(32)
{
	m_conn_id = id;

	m_x = 0;
	m_y = 0;
	m_vx = 0;
	m_vy = 0;
	m_angle = 0.0;
	m_vangle = 0.0;

	m_up_key = false;
	m_down_key = false;
	m_right_key = false;
	m_left_key = false;
	m_target_angle = 0.0;

	m_last_parsed_input_id = -1;
}

void character::process_input()
{
	std::shared_ptr<input> in;
	while (m_input_buffer.size() > 0) {
		typedef input_type t;

		in = m_input_buffer.front();

		switch (in->type)
		{
		case t::KEY:
			m_up_key = in->controls & runeio::proto::character::user_input_codes_base::KEY_UP;
			m_down_key = in->controls & runeio::proto::character::user_input_codes_base::KEY_DOWN;
			m_right_key = in->controls & runeio::proto::character::user_input_codes_base::KEY_RIGHT;
			m_left_key = in->controls & runeio::proto::character::user_input_codes_base::KEY_LEFT;
			break;
		case t::MOUSE:
			m_target_angle = in->target_view_angle;
			break;
		}

		m_last_parsed_input_id = in->id;
		m_input_buffer.pop_front();
	}
}

void character::move()
{
	// setting velocity
	m_vy = 0;
	if (m_up_key)
		m_vy -= 5;
	if (m_down_key)
		m_vy += 5;

	m_vx = 0;
	if (m_right_key)
		m_vx += 5;
	if (m_left_key)
		m_vx -= 5;

	// moving
	m_x += m_vx;
	m_y += m_vy;

	// view angle
	m_angle = m_target_angle;
}

void character::parse_net_input(const proto::character::user_input * net)
{
	typedef proto::character::user_input::SharedCase t;

	if (net->shared_case() == t::SHARED_NOT_SET)
		return;

	auto i_ptr = std::make_shared<input>();
	i_ptr->id = net->input_id();

	switch (net->shared_case()) {
	case t::kInput:
		i_ptr->type = input_type::KEY;
		i_ptr->controls = net->input();
		break;
	case t::kTargetViewAngle:
		i_ptr->type = input_type::MOUSE;
		i_ptr->target_view_angle = net->target_view_angle();
		break;
	}

	m_input_buffer.push_back(i_ptr);
}

void character::fill_net_public(proto::character::public_char * net) const
{
	auto position = net->mutable_position();
	auto point = position->mutable_point();
	auto velocity = position->mutable_velocity();

	position->set_view_angle(m_angle);
	point->set_x(m_x);
	point->set_y(m_y);
	velocity->set_x(m_vx);
	velocity->set_y(m_vy);
}

void character::fill_net_private(proto::character::private_char * net) const
{
	net->set_last_parsed_input_id(m_last_parsed_input_id);
}

} // namespace runeio