#pragma once
#include "../plugin_sdk/plugin_sdk.hpp"

class others
{
public:
	static others instance;
public:
	bool       initialized = false;
	void       init();
	void       destroy();
	TreeEntry* thicc              = nullptr;
	TreeEntry* color_q            = nullptr;
	TreeEntry* color_w            = nullptr;
	TreeEntry* color_e            = nullptr;
	TreeEntry* color_r            = nullptr;
	TreeEntry* color_q2           = nullptr;
	TreeEntry* color_w2           = nullptr;
	TreeEntry* color_e2           = nullptr;
	TreeEntry* color_r2           = nullptr;
	TreeEntry* inside_glow_size   = nullptr;
	TreeEntry* inside_glow_power  = nullptr;
	TreeEntry* outside_glow_size  = nullptr;
	TreeEntry* outside_glow_power = nullptr;
	TreeEntry* color_damage_unk   = nullptr;
	TreeEntry* color_damage_k     = nullptr;
	TreeEntry* enable_gradient    = nullptr;
};

others& misc();
