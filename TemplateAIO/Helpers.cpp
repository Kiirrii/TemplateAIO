#include "Helpers.h"
#include <string>

namespace helpers
{
	void draw_damage(const game_object_script& enemy, const float damage)
	{
		vector4 bar_pos;
		vector4 hp_pos;
		enemy->get_health_bar_position(bar_pos, hp_pos);

		const float hp_after_dmg = enemy->get_real_health() - damage;
		const float x1           = hp_pos.x + ((enemy->get_real_health() / enemy->get_max_health()) * hp_pos.z);
		const float x2           = hp_pos.x + (((hp_after_dmg > 0 ? hp_after_dmg : 0) / enemy->get_max_health()) * hp_pos.z);
		const float center_y     = hp_pos.y + hp_pos.w / 2;

		auto color = misc().color_damage_k->get_color();
		if (hp_after_dmg > 0) color = misc().color_damage_unk->get_color();

		draw_manager->add_line_on_screen(vector(x1, center_y), vector(x2, center_y), color, hp_pos.w);
	}

	void draw_range(const vector& center, const unsigned int color, const unsigned int color2, const float radius, const float line_width, const glow_data& glow)
	{
		if (misc().enable_gradient->get_bool())
		{
			draw_manager->add_circle_with_glow_gradient(center, color, color2, radius, line_width, glow);
		}
		else
		{
			draw_manager->add_circle_with_glow(center, color, radius, line_width, glow);
		}
	}

	bool is_winding_up_attack()
	{
		return gametime->get_time() - orbwalker->get_last_aa_time() < 0.1f || (myhero->get_active_spell() != nullptr && myhero->get_active_spell()->is_auto_attacking());
	}

	bool is_valid_minion(const game_object_script& obj)
	{
		if (obj == nullptr)
		{
			return false;
		}

		if (obj->is_ward())
		{
			return false;
		}
		const auto hash = buff_hash_real(obj->get_base_skin_name().c_str());
		if (hash == buff_hash("TeemoMushroom") || hash == buff_hash("JhinTrap") || hash == buff_hash("NidaleeSpear"))
		{
			return false;
		}
		return true;
	}

	int count_enemies_after_delay(const vector& position, const float range, const float delay, const bool ignore_inv)
	{
		int count = 0;
		for (auto&& t : entitylist->get_enemy_heroes())
		{
			if (t == nullptr) continue;
			if (!t->is_targetable()) continue;
			if (!t->is_valid_target(range, position, ignore_inv)) continue;

			auto pred = prediction->get_prediction(t, delay);
			if (pred.get_unit_position().distance(position) > range) continue;
			count++;
		}
		return count;
	}

	int count_minions(const vector& position, const float range, const std::string& team)
	{
		int count = 0;
		if (team == "ally")
		{
			for (auto&& t : entitylist->get_ally_minions())
			{
				if (t != nullptr && t->is_valid() && is_valid_minion(t)) count++;
			}
		}
		if (team == "enemy")
		{
			for (auto&& t : entitylist->get_enemy_minions())
			{
				if (t != nullptr && t->is_valid_target(range, position) && is_valid_minion(t)) count++;
			}
		}
		if (team == "neutral")
		{
			for (auto&& t : entitylist->get_jugnle_mobs_minions())
			{
				if (t != nullptr && t->is_valid_target(range, position) && is_valid_minion(t)) count++;
			}
		}
		if (team == "both")
		{
			for (auto&& t : entitylist->get_enemy_minions())
			{
				if (t != nullptr && t->is_valid_target(range, position) && is_valid_minion(t)) count++;
			}
			for (auto&& t : entitylist->get_jugnle_mobs_minions())
			{
				if (t != nullptr && t->is_valid_target(range, position) && is_valid_minion(t)) count++;
			}
		}
		return count;
	}

	color convert_to_argb(const uint32_t color)
	{
		const unsigned int red   = (color) & 0xff;
		const unsigned int green = ((color) >> 8) & 0xff;
		const unsigned int blue  = ((color) >> 16) & 0xff;
		const unsigned int alpha = ((color) >> 24) & 0xff;
		return {alpha, red, green, blue};
	}

	hit_chance get_hitchance(const int hc)
	{
		switch (hc)
		{
			case 0:
				return hit_chance::low;
			case 1:
				return hit_chance::medium;
			case 2:
				return hit_chance::high;
			case 3:
				return hit_chance::very_high;
		}
		return hit_chance::medium;
	}

	glow_data get_glow()
	{
		return glow_data{
			misc().inside_glow_size->get_int() / 100.f,
			misc().inside_glow_power->get_int() / 100.f,
			misc().outside_glow_size->get_int() / 100.f,
			misc().outside_glow_power->get_int() / 100.f
		};
	}
}
