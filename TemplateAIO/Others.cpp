#include "Others.h"
#include "Helpers.h"
#include "Permashow.hpp"
#include <array>
#include <sstream>

TreeTab* othersMenu = nullptr;

others others::instance;

others& misc()
{
	if (!others::instance.initialized) others::instance.init();

	return others::instance;
}

#pragma region presest
void glow_preset_three(TreeEntry* entry)
{
	misc().inside_glow_size->set_int(0);
	misc().inside_glow_power->set_int(0);
	misc().outside_glow_size->set_int(70);
	misc().outside_glow_power->set_int(70);
}

void glow_preset_two(TreeEntry* entry)
{
	misc().inside_glow_size->set_int(10);
	misc().inside_glow_power->set_int(75);
	misc().outside_glow_size->set_int(0);
	misc().outside_glow_power->set_int(0);
}

void glow_preset_one(TreeEntry* entry)
{
	misc().inside_glow_size->set_int(85);
	misc().inside_glow_power->set_int(29);
	misc().outside_glow_size->set_int(85);
	misc().outside_glow_power->set_int(29);
}

void gradient_update(TreeEntry* entry)
{
	misc().color_q2->is_hidden() = !entry->get_bool();
	misc().color_w2->is_hidden() = !entry->get_bool();
	misc().color_e2->is_hidden() = !entry->get_bool();
	misc().color_r2->is_hidden() = !entry->get_bool();
}
#pragma endregion

void others::init()
{
	initialized = true;

	othersMenu = menu->create_tab("others", AIO_NAME " - Others");
	permashow::instance.init(othersMenu);
	auto colorsMenu = othersMenu->add_tab("colors", "Colors");
	{
		float color_q_c[]     = {0.98f, 1.f, 0.69f, 0.8f};
		float color_q2_c[]    = {0.98f, 1.f, 0.69f, 0.8f};
		float color_w_c[]     = {0.33f, 0.59f, 0.72f, 0.8f};
		float color_w2_c[]    = {0.33f, 0.59f, 0.72f, 0.8f};
		float color_e_c[]     = {0.42f, 0.76f, 0.63f, 0.8f};
		float color_e2_c[]    = {0.42f, 0.76f, 0.63f, 0.8f};
		float color_r_c[]     = {1.f, 0.67f, 0.67f, 0.8f};
		float color_r2_c[]    = {1.f, 0.67f, 0.67f, 0.8f};
		float color_dmg_k[]   = {0.f, 1.f, 0.f, 0.5f};
		float color_dmg_unk[] = {1.f, 0.75f, 0.78f, 0.5f};

		colorsMenu->add_separator("ranges", "-- Ranges --");
		thicc           = colorsMenu->add_slider("thicc", "Line Thickness", 1, 1, 10);
		enable_gradient = colorsMenu->add_checkbox("enable_gradient", "Enable Gradient", false);

		colorsMenu->add_separator("empty", "");
		color_q  = colorsMenu->add_colorpick("color_q", "Q Color", color_q_c);
		color_q2 = colorsMenu->add_colorpick("color_q2", "Second Q Color", color_q2_c);
		color_w  = colorsMenu->add_colorpick("color_w", "W Color", color_w_c);
		color_w2 = colorsMenu->add_colorpick("color_w2", "Second W Color", color_w2_c);
		color_e  = colorsMenu->add_colorpick("color_e", "E Color", color_e_c);
		color_e2 = colorsMenu->add_colorpick("color_e2", "Second E Color", color_e2_c);
		color_r  = colorsMenu->add_colorpick("color_r", "R Color", color_r_c);
		color_r2 = colorsMenu->add_colorpick("color_r2", "Second R Color", color_r2_c);

		colorsMenu->add_separator("empty2", "");
		inside_glow_size   = colorsMenu->add_slider("inside_glow_size", "Inside glow size", 0, 0, 100);
		inside_glow_power  = colorsMenu->add_slider("inside_glow_power", "Inside glow power", 0, 0, 100);
		outside_glow_size  = colorsMenu->add_slider("outside_glow_size", "Outside glow size", 0, 0, 100);
		outside_glow_power = colorsMenu->add_slider("outisde_glow_power", "Outside glow power", 0, 0, 100);

		// Presets
		const auto glow_preset_1 = colorsMenu->add_button("glow_preset_1", "Glow Preset [1]");
		const auto glow_preset_2 = colorsMenu->add_button("glow_preset_2", "Glow Preset [2]");
		const auto glow_preset_3 = colorsMenu->add_button("glow_preset_3", "Glow Preset [3]");

		colorsMenu->add_separator("damage", "-- Damage drawings --");
		color_damage_unk = colorsMenu->add_colorpick("color_damage_unk", "Unkillable damage color", color_dmg_unk);
		color_damage_k   = colorsMenu->add_colorpick("color_damage_k", "Killable damage color", color_dmg_k);

		// Callbacks
		enable_gradient->add_property_change_callback(gradient_update);
		glow_preset_1->add_property_change_callback(glow_preset_one);
		glow_preset_2->add_property_change_callback(glow_preset_two);
		glow_preset_3->add_property_change_callback(glow_preset_three);
	}

	gradient_update(misc().enable_gradient);
}

void others::destroy()
{
	if (othersMenu) menu->delete_tab(othersMenu);
	permashow::instance.destroy();
}
