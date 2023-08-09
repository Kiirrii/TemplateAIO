#pragma once
#include "../plugin_sdk/plugin_sdk.hpp"
#include "Others.h"
#include <string>
#include <sstream>

#define MENU_KEY "TemplateAIO"
#define AIO_NAME "Template AIO [Kiri]"

namespace helpers
{
	struct color
	{
		unsigned int a;
		unsigned int r;
		unsigned int g;
		unsigned int b;
	};

	extern int        count_minions(const vector& position, float range, const std::string& team);
	extern bool       is_valid_minion(const game_object_script& obj);
	extern void       draw_damage(const game_object_script& enemy, float damage);
	extern void       draw_range(const vector& center, unsigned int color, unsigned int color2, float radius, float line_width, const glow_data& glow);
	extern int        count_enemies_after_delay(const vector& position, float range, float delay, bool ignore_inv);
	extern bool       is_winding_up_attack();
	extern color      convert_to_argb(uint32_t color);
	extern hit_chance get_hitchance(int hc);
	extern glow_data  get_glow();
}
