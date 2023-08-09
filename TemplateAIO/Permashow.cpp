#include "Permashow.hpp"
#include "Others.h"
#include "Helpers.h"
#include <array>
#include <sstream>

TreeEntry* permashow_enabled;
TreeEntry* permashow_fontsize;

TreeEntry* permashow_pos_x;
TreeEntry* permashow_pos_y;

TreeEntry* background_color;
TreeEntry* border_color;
TreeEntry* title_color;
TreeEntry* title_background_color;
TreeEntry* text_color;
TreeTab*   elements;


constexpr std::int32_t padding_x = 14;
constexpr std::int32_t padding_y = 6;

bool permashow_clicked = false;
bool permashow_update  = false;

permashow permashow::instance;

permashow& get_permashow()
{
	return permashow::instance;
}

void permashow_element_status(TreeEntry* element)
{
	const auto it = std::find_if(permashow::instance.permashow_elements.begin(), permashow::instance.permashow_elements.end(), [element](const permashow_element& perma_element)
	{
		return perma_element.assigned_menu_element->display_name() == element->display_name();
	});

	if (it == permashow::instance.permashow_elements.end()) return;

	it->enabled = element->get_bool();

	permashow_update = true;
}

void permashow_menu_element_change(TreeEntry* element)
{
	auto it = std::find_if(permashow::instance.permashow_elements.begin(), permashow::instance.permashow_elements.end(), [element](const permashow_element& perma_element)
	{
		return perma_element.assigned_menu_element == element;
	});

	if (it == permashow::instance.permashow_elements.end()) return;

	if (it->assigned_menu_element->get_int() > 0)
	{
		switch (it->assigned_menu_element->get_int())
		{
			case 1:
				it->keybind = "MB1";
				break;
			case 2:
				it->keybind = "MB2";
				break;
			case 4:
				it->keybind = "MMB";
				break;
			case 5:
				it->keybind = "MB4";
				break;
			case 6:
				it->keybind = "MB5";
				break;
			case 16:
				it->keybind = "SHIFT";
				break;
			case 17:
				it->keybind = "CTRL";
				break;
			case 18:
				it->keybind = "ALT";
				break;
			case 20:
				it->keybind = "Caps";
				break;
			case 33:
				it->keybind = "PG UP";
				break;
			case 34:
				it->keybind = "PG DOWN";
				break;
			case 35:
				it->keybind = "END";
				break;
			case 36:
				it->keybind = "HOME";
				break;
			case 45:
				it->keybind = "INS";
				break;
			case 46:
				it->keybind = "DEL";
				break;
			case 96:
				it->keybind = "Num 0";
				break;
			case 97:
				it->keybind = "Num 1";
				break;
			case 98:
				it->keybind = "Num 2";
				break;
			case 99:
				it->keybind = "Num 3";
				break;
			case 100:
				it->keybind = "Num 4";
				break;
			case 101:
				it->keybind = "Num 5";
				break;
			case 102:
				it->keybind = "Num 6";
				break;
			case 103:
				it->keybind = "Num 7";
				break;
			case 104:
				it->keybind = "Num 8";
				break;
			case 105:
				it->keybind = "Num 9";
				break;
			case 112:
				it->keybind = "F1";
				break;
			case 113:
				it->keybind = "F2";
				break;
			case 114:
				it->keybind = "F3";
				break;
			case 115:
				it->keybind = "F4";
				break;
			case 116:
				it->keybind = "F5";
				break;
			case 117:
				it->keybind = "F6";
				break;
			case 118:
				it->keybind = "F7";
				break;
			case 119:
				it->keybind = "F8";
				break;
			case 120:
				it->keybind = "F9";
				break;
			case 121:
				it->keybind = "F10";
				break;
			case 122:
				it->keybind = "F11";
				break;
			case 123:
				it->keybind = "F12";
				break;
			case 109:
				it->keybind = "Numpad - ";
				break;
			case 106:
				it->keybind = "Numpad *";
				break;
			case 110:
				it->keybind = "Numpad ,";
				break;
			case 111:
				it->keybind = "Numpad /";
				break;
			case 107:
				it->keybind = "Numpad +";
				break;
			case 144:
				it->keybind = "Numlock";
				break;
			default:
				it->keybind = static_cast<char>(it->assigned_menu_element->get_int());
				break;
		}
	}
	else
	{
		it->keybind = "";
	}


	switch (element->element_type())
	{
		case TreeEntryType::Checkbox:
		case TreeEntryType::Hotkey:
			it->value = element->get_bool() ? "ON" : "OFF";
			it->value_color = element->get_bool() ? vector(0, 255, 0) : vector(255, 0, 0);
			break;
	}

	permashow_update = true;
}

void permashow_font_change(TreeEntry*)
{
	permashow_update = true;
}

std::stringstream get_keybind_format(permashow_element element)
{
	std::stringstream s1;
	if (!element.keybind.empty())
	{
		s1 << "[" << element.keybind << "] ";
	}
	if (element.assigned_menu_element)
	{
		s1 << element.assigned_menu_element->display_name().c_str();
	}
	else
	{
		s1 << element.name.c_str();
	}
	return s1;
}

void permashow_on_draw()
{
	if (permashow_update)
	{
		permashow::instance.update();
		permashow_update = false;
	}

	//Update position
	//

	const point2 p = game_input->get_game_cursor_pos();
	if (keyboard_state->is_pressed(keyboard_game::mouse1))
	{
		if (!permashow_clicked)
		{
			const auto rect = p - point2{permashow_pos_x->get_int(), permashow_pos_y->get_int()};
			if (rect.x >= 0.f && rect.y >= 0.f && rect.x <= permashow::instance.box_size.x && rect.y <= permashow::instance.box_size.y)
			{
				permashow::instance.drag_offset = rect;
				permashow_clicked               = true;
			}
		}
	}
	else
	{
		permashow_clicked = false;
	}

	if (permashow_clicked)
	{
		permashow_pos_x->set_int(p.x - permashow::instance.drag_offset.x);
		permashow_pos_y->set_int(p.y - permashow::instance.drag_offset.y);
	}

	//Draw permashow
	//
	const auto offset_from_top = vector(0, 5);
	const auto font_size       = permashow_fontsize->get_int();
	const auto position        = vector(permashow_pos_x->get_int(), permashow_pos_y->get_int());
	const auto box_size        = vector(permashow::instance.box_size.x, permashow::instance.box_size.y);
	const auto text_color_argb = helpers::convert_to_argb(text_color->get_color());

	draw_manager->add_filled_rect(position + vector{0, permashow::instance.title_box_size.y}, position + box_size + offset_from_top * 2, background_color->get_color());
	draw_manager->add_filled_rect(position, position + permashow::instance.title_box_size, title_background_color->get_color());
	draw_manager->add_line_on_screen(position + vector{0, permashow::instance.title_box_size.y}, position + permashow::instance.title_box_size, border_color->get_color());
	draw_manager->add_rect(position, position + box_size + offset_from_top * 2, border_color->get_color());
	draw_manager->add_text_on_screen(position + (permashow::instance.title_box_size / 2.f) - (permashow::instance.title_size / 2.f) + vector(0, 2), title_color->get_color(), font_size + 3, "%s",
	                                 permashow::instance.title.c_str());

	for (auto& element : permashow::instance.permashow_elements)
	{
		if (!element.enabled) continue;

		std::stringstream s1 = get_keybind_format(element);
		draw_manager->add_text_on_screen(position + element.name_pos + offset_from_top, text_color->get_color(), font_size, "%s", s1.str().c_str());

		draw_manager->add_text_on_screen(position + element.value_pos + offset_from_top,
		                                 MAKE_COLOR(static_cast<int>(element.value_color.x), static_cast<int>(element.value_color.y), static_cast<int>(element.value_color.z), text_color_argb.a),
		                                 font_size, "%s", element.value.c_str());
	}
}

void permashow::update()
{
	auto font_size = permashow_fontsize->get_int();

	title_size = draw_manager->calc_text_size(font_size + 3, "%s", title.c_str());

	std::int32_t max_name_width  = title_size.x;
	std::int32_t max_value_width = 0;
	std::int32_t max_text_height = title_size.y;

	for (const auto& element : permashow_elements)
	{
		if (!element.enabled) continue;

		std::stringstream s1 = get_keybind_format(element);

		const auto name_size  = draw_manager->calc_text_size(font_size, "%s", s1.str().c_str());
		const auto value_size = draw_manager->calc_text_size(font_size, "%s", "OFF");

		if (name_size.x > max_name_width) max_name_width = name_size.x;
		if (value_size.x > max_value_width) max_value_width = value_size.x;
		if (name_size.y > max_text_height) max_text_height = name_size.y;
		if (value_size.y > max_text_height) max_text_height = value_size.y;
	}

	auto element_height    = max_text_height + padding_y;
	auto current_element_y = element_height;

	box_size.x = max_name_width + max_value_width + (padding_x * 2);
	for (auto& element : permashow_elements)
	{
		if (!element.enabled) continue;

		std::stringstream s1 = get_keybind_format(element);

		auto name_size  = draw_manager->calc_text_size(font_size, s1.str().c_str());
		auto value_size = draw_manager->calc_text_size(font_size, "%s", element.value.c_str());

		element.name_pos      = {padding_x / 2, current_element_y + (element_height / 2) - (name_size.y / 2)};
		element.seperator_pos = {static_cast<float>((padding_x / 2) + max_name_width + padding_x), current_element_y + 6.f};
		element.value_pos     = {static_cast<float>(padding_x / 2 + max_name_width + (padding_x)), current_element_y + (element_height / 2) - (value_size.y / 2)};

		current_element_y += element_height;
	}

	title_box_size   = vector(box_size.x, element_height);
	seperator_height = element_height - 12;
	box_size.y       = current_element_y;
}

void permashow::add_element(const std::string& name, TreeEntry* element)
{
	auto entry = elements->add_checkbox(element->config_key(), name, true);
	entry->add_property_change_callback(permashow_element_status);

	element->set_display_name(name);
	permashow_elements.push_back({element, entry->get_bool()});

	element->add_property_change_callback(permashow_menu_element_change);
	permashow_menu_element_change(element);
}

void permashow_preset_one(TreeEntry* entry)
{
	float bg_color[4]{14.f / 255.f, 22.f / 255.f, 23.f / 255.f, 200.f / 255.f};
	float t_color[4]{59.f / 255.f, 138.f / 255.f, 131.f / 255.f, 200.f / 255.f};
	float br_color[4]{124.f / 255.f, 106.f / 255.f, 73.f / 255.f, 200.f / 255.f};
	float tl_color[4]{143.f / 255.f, 122.f / 255.f, 72.f / 255.f, 200.f / 255.f};
	float tl_bg_color[4]{19.f / 255.f, 30.f / 255.f, 32.f / 255.f, 200.f / 255.f};

	background_color->set_color(bg_color);
	text_color->set_color(t_color);
	border_color->set_color(br_color);
	title_color->set_color(tl_color);
	title_background_color->set_color(tl_bg_color);
}

void permashow_preset_two(TreeEntry* entry)
{
	float bg_color[4]{0.f / 255.f, 0.f / 255.f, 0.f / 255.f, 100.f / 255.f};
	float t_color[4]{255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 200.f / 255.f};
	float br_color[4]{255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 100.f / 255.f};
	float tl_color[4]{255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 220.f / 255.f};
	float tl_bg_color[4]{0.f / 255.f, 0.f / 255.f, 0.f / 255.f, 120.f / 255.f};

	background_color->set_color(bg_color);
	text_color->set_color(t_color);
	border_color->set_color(br_color);
	title_color->set_color(tl_color);
	title_background_color->set_color(tl_bg_color);
}

void permashow::init(TreeTab* others_menu)
{
	initialized = true;

	const auto permashow = others_menu->add_tab("permashow", "Permashow");
	{
		permashow_pos_x = permashow->add_slider("posx", "Position X", static_cast<int>(renderer->screen_width() * 0.72f), 0, renderer->screen_width() - 100.f);
		permashow_pos_y = permashow->add_slider("posy", "Position Y", static_cast<int>(renderer->screen_height() * 0.74f), 0, renderer->screen_height() - 100.f);

		permashow_fontsize = permashow->add_slider("font", "Font size", 14, 12, 25);
		permashow_fontsize->add_property_change_callback(permashow_font_change);
		elements = permashow->add_tab("elements", "Permashow Elements");

		const auto colors = permashow->add_tab("colors", "Colors");
		{
			background_color       = colors->add_colorpick("background_color", "Background", {14.f / 255.f, 22.f / 255.f, 23.f / 255.f, 200.f / 255.f});
			text_color             = colors->add_colorpick("text_color", "Color", {59.f / 255.f, 138.f / 255.f, 131.f / 255.f, 200.f / 255.f});
			border_color           = colors->add_colorpick("border_color", "Border", {124.f / 255.f, 106.f / 255.f, 73.f / 255.f, 200.f / 255.f});
			title_color            = colors->add_colorpick("title_color", "Title", {143.f / 255.f, 122.f / 255.f, 72.f / 255.f, 200.f / 255.f});
			title_background_color = colors->add_colorpick("title_background_color", "Title Background", {19.f / 255.f, 30.f / 255.f, 32.f / 255.f, 200.f / 255.f});
		}

		// Presets
		permashow->add_separator("empty2", "");
		const auto permashow_preset_1 = permashow->add_button("permashow_preset_1", "Permashow Preset [1]");
		const auto permashow_preset_2 = permashow->add_button("permashow_preset_2", "Permashow Preset [2]");

		// Callbacks
		permashow_preset_1->add_property_change_callback(permashow_preset_one);
		permashow_preset_2->add_property_change_callback(permashow_preset_two);
	}

	instance.title = AIO_NAME;
	event_handler<events::on_draw>::add_callback(permashow_on_draw);
}

void permashow::destroy()
{
	event_handler<events::on_draw>::remove_handler(permashow_on_draw);
}
