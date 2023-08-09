#pragma once
#include "../plugin_sdk/plugin_sdk.hpp"

struct permashow_element
{
	TreeEntry* assigned_menu_element = nullptr;

	bool        enabled;
	std::string name;
	std::string value;
	vector      value_color;
	std::string keybind;
	vector      name_pos;
	vector      value_pos;
	vector      seperator_pos;
};

class permashow
{
public:
	static permashow   instance;
	static std::string aio_name;
public:
	bool                           initialized = false;
	std::string                    title;
	vector                         title_box_size;
	vector                         title_size;
	float                          seperator_height{};
	point2                         box_size;
	point2                         drag_offset;
	std::vector<permashow_element> permashow_elements;
	void                           add_element(const std::string& name, TreeEntry*);
	void                           update();
	void                           init(TreeTab* other_menu);
	void                           destroy();
};

permashow& get_permashow();
