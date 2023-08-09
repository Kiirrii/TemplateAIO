#include "../plugin_sdk/plugin_sdk.hpp"
#include "Helpers.h"
#include "Permashow.hpp"
#include "Others.h"
#include "Amumu.h"

namespace amumu
{
	void on_update();
	void on_env_draw();
	void on_draw();

	/*****************************************
					VARIABLES
	****************************************/

	script_spell* q        = nullptr;
	script_spell* w        = nullptr;
	script_spell* e        = nullptr;
	script_spell* r        = nullptr;
	TreeTab*      main_tab = nullptr;

	/*****************************************
					MENU STUFF
	****************************************/

	namespace combo
	{
		// Q Settings
		TreeEntry* use_q = nullptr;
		TreeEntry* min_q = nullptr;

		// W Settings
		TreeEntry* use_w  = nullptr;
		TreeEntry* w_mana = nullptr;

		// E Settings
		TreeEntry* use_e = nullptr;

		// R Settings
		TreeEntry* hit_r = nullptr;
	}

	namespace harass
	{
		// Q Settings
		TreeEntry* use_q = nullptr;
		TreeEntry* min_q = nullptr;

		// W Settings
		TreeEntry* use_w  = nullptr;
		TreeEntry* w_mana = nullptr;

		// E Settings
		TreeEntry* use_e = nullptr;
	}

	namespace farm_menu
	{
		TreeEntry* toggle = nullptr;
	}

	namespace lane_clear_menu
	{
		// W Settings
		TreeEntry* use_w  = nullptr;
		TreeEntry* hit_w  = nullptr;
		TreeEntry* w_mana = nullptr;

		// E Settings
		TreeEntry* use_e = nullptr;
		TreeEntry* hit_e = nullptr;
	}

	namespace jungle_clear_menu
	{
		// Q Settings
		TreeEntry* use_q = nullptr;

		// W Settings
		TreeEntry* use_w  = nullptr;
		TreeEntry* w_mana = nullptr;

		// E Settings
		TreeEntry* use_e = nullptr;
	}

	namespace ac
	{
		// E Settings
		TreeEntry* auto_e_kill = nullptr;
	}


	namespace q_pred
	{
		TreeEntry* hc        = nullptr;
		TreeEntry* semi_q_hc = nullptr;
	}

	namespace r_pred
	{
		TreeEntry* range_slider = nullptr;
	}

	namespace draws
	{
		TreeEntry* draw_q = nullptr;
		TreeEntry* draw_w = nullptr;
		TreeEntry* draw_e = nullptr;
		TreeEntry* draw_r = nullptr;
	}

	namespace dmg_draws
	{
		TreeEntry* draw_damage = nullptr;
		TreeEntry* draw_q      = nullptr;
		TreeEntry* draw_w      = nullptr;
		TreeEntry* draw_e      = nullptr;
		TreeEntry* draw_r      = nullptr;
		TreeEntry* w_ticks     = nullptr;
	}

	namespace mainMenu
	{
		TreeEntry* semiq = nullptr;
	}

	/*****************************************
					LOAD/UNLOAD
	****************************************/

	void load()
	{
		// Registering a spells
		q = plugin_sdk->register_spell(spellslot::q, 1100.f);
		w = plugin_sdk->register_spell(spellslot::w, 350.f);
		e = plugin_sdk->register_spell(spellslot::e, 350.f);
		r = plugin_sdk->register_spell(spellslot::r, 550.f);

		q->set_spell_lock(false);
		w->set_spell_lock(false);
		e->set_spell_lock(false);
		r->set_spell_lock(false);

		q->set_skillshot(0.25f, 80.f, 2000.f, {collisionable_objects::minions, collisionable_objects::yasuo_wall}, skillshot_type::skillshot_line);

		// Create menu
		main_tab = menu->create_tab(MENU_KEY + myhero->get_model(), AIO_NAME " - " + myhero->get_model());
		main_tab->set_assigned_texture(myhero->get_square_icon_portrait());

		const auto combo = main_tab->add_tab("combo", "Combo");
		{
			// Q Settings
			combo->add_separator("q_set", "-- Q Settings --");
			combo::use_q = combo->add_checkbox("use_q", "Use Q", true);
			combo::min_q = combo->add_slider("min_q", "Min. Q range", 0, 0, 400);

			// W Settings
			combo->add_separator("W_set", "-- W Settings --");
			combo::use_w  = combo->add_checkbox("use_w", "Use W", true);
			combo::w_mana = combo->add_slider("w_mana", "Min. Mana %", 20, 0, 100);

			// E Settings
			combo->add_separator("e_set", "-- E Settings --");
			combo::use_e = combo->add_checkbox("use_e", "Use E", true);

			// R Settings
			combo->add_separator("r_set", "-- R Settings --");
			combo::hit_r = combo->add_slider("hit_r", "Use R if hits >= X enemies", 2, 0, 5);

			combo::hit_r->set_tooltip("0 - Disabled");
		}

		const auto harass = main_tab->add_tab("harass", "Harass");
		{
			// Q Settings
			harass->add_separator("q_set", "-- Q Settings --");
			harass::use_q = combo->add_checkbox("use_q", "Use Q", true);
			harass::min_q = harass->add_slider("min_q", "Min. Q range", 0, 0, 400);

			// W Settings
			harass->add_separator("w_set", "-- W Settings --");
			harass::use_w  = harass->add_checkbox("use_w", "Use W", true);
			harass::w_mana = harass->add_slider("w_mana", "Min. Mana %", 20, 0, 100);

			// E Settings
			harass->add_separator("e_set", "-- E Settings --");
			harass::use_e = harass->add_checkbox("use_e", "Use E", true);
		}

		const auto farming = main_tab->add_tab("farming", "Farming");
		{
			farm_menu::toggle    = farming->add_hotkey("toggle", "Farm", TreeHotkeyMode::Toggle, 'A', true);
			const auto laneclear = farming->add_tab("Lane_clear", "Lane Clear");
			{
				// W Settings
				laneclear->add_separator("w_set", "-- W Settings --");
				lane_clear_menu::use_w  = laneclear->add_checkbox("use_w", "Use W", true);
				lane_clear_menu::hit_w  = laneclear->add_slider("hit_w", " ^- if Hits X Minions", 3, 1, 6);
				lane_clear_menu::w_mana = laneclear->add_slider("w_mana", "Min. Mana %", 20, 0, 100);

				// E Settings
				laneclear->add_separator("e_set", "-- E Settings --");
				lane_clear_menu::use_e = laneclear->add_checkbox("use_e", "Use E", true);
				lane_clear_menu::hit_e = laneclear->add_slider("hit_e", " ^- if Hits X Minions", 3, 1, 6);
			}

			const auto jungleclear = farming->add_tab("Jungle_clear", "Jungle Clear");
			{
				// Q Settings
				jungleclear->add_separator("q_set", "-- Q Settings --");
				jungle_clear_menu::use_q = jungleclear->add_checkbox("use_q", "Use Q", true);

				// W Settings
				jungleclear->add_separator("w_set", "-- W Settings --");
				jungle_clear_menu::use_w  = jungleclear->add_checkbox("use_w", "Use W", true);
				jungle_clear_menu::w_mana = jungleclear->add_slider("w_mana", "Min. Mana %", 20, 0, 100);

				// E Settings
				jungleclear->add_separator("e_set", "-- E Settings --");
				jungle_clear_menu::use_e = jungleclear->add_checkbox("use_e", "Use E", true);
			}
		}

		const auto automaticSettings = main_tab->add_tab("automatic", "Automatic");
		{
			// E Settings
			automaticSettings->add_separator("e_set", "-- E Settings --");
			ac::auto_e_kill = automaticSettings->add_checkbox("autoekill", "Auto E on Killable", true);
		}


		const auto draw_settings = main_tab->add_tab("draw", "Drawings");
		{
			draw_settings->add_separator("ranges", "-- Ranges --");
			draws::draw_q = draw_settings->add_checkbox("draw_q", "Draw Q Range", true);
			draws::draw_w = draw_settings->add_checkbox("draw_w", "Draw W Range", false);
			draws::draw_e = draw_settings->add_checkbox("draw_e", "Draw E Range", false);
			draws::draw_r = draw_settings->add_checkbox("draw_r", "Draw R Range", false);
			draw_settings->add_separator("other", "-- Other --");
			const auto damage_settings = draw_settings->add_tab("damage", "Damage Drawings");
			{
				dmg_draws::draw_damage = damage_settings->add_checkbox("draw_damage", "Draw Damage", true);
				damage_settings->add_separator("empty", "");
				dmg_draws::draw_q  = damage_settings->add_checkbox("draw_q", "Draw Q Damage", true);
				dmg_draws::draw_w  = damage_settings->add_checkbox("draw_w", "Draw W Damage", true);
				dmg_draws::w_ticks = damage_settings->add_slider("w_ticks", " ^- X Ticks", 3, 0, 6);
				dmg_draws::draw_e  = damage_settings->add_checkbox("draw_e", "Draw E Damage", true);
				dmg_draws::draw_r  = damage_settings->add_checkbox("draw_r", "Draw R Damage", true);
			}
		}

		const auto prediction_settings = main_tab->add_tab("predictions", "Predictions");
		{
			const auto q_pred_set = prediction_settings->add_tab("q_pred", "Q Settings");
			{
				q_pred::hc        = q_pred_set->add_combobox("hc", "Hitchance", {{"Low", nullptr}, {"Medium", nullptr}, {"High", nullptr}, {"Very High", nullptr}}, 3);
				q_pred::semi_q_hc = q_pred_set->add_combobox("semi_q_hc", "Semi Q Hitchance", {{"Low", nullptr}, {"Medium", nullptr}, {"High", nullptr}, {"Very High", nullptr}}, 0);
			}

			const auto r_pred_set = prediction_settings->add_tab("r_pred", "R Settings");
			{
				r_pred::range_slider = r_pred_set->add_slider("range_slider", "Range %", 95, 0, 100);
			}
		}

		main_tab->add_separator("sep", " ~~~~ ");
		mainMenu::semiq = main_tab->add_hotkey("semiq", "Semi-Q", TreeHotkeyMode::Hold, 'J', false);
		main_tab->add_separator("version", "Version: 1.0");

		// Register callbacks
		event_handler<events::on_preupdate>::add_callback(on_update);
		event_handler<events::on_env_draw>::add_callback(on_env_draw);
		event_handler<events::on_draw>::add_callback(on_draw);

		// Init permashow
		misc().init();
		get_permashow().add_element("Farm", farm_menu::toggle);
		get_permashow().add_element("Semi-Q", mainMenu::semiq);
	}

	void unload()
	{
		// Unregister spells
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		// Delete menu
		menu->delete_tab(MENU_KEY + myhero->get_model());

		// Unregister callbacks
		event_handler<events::on_preupdate>::remove_handler(on_update);
		event_handler<events::on_env_draw>::remove_handler(on_env_draw);
		event_handler<events::on_draw>::remove_handler(on_draw);

		// Destroy permashow
		misc().destroy();
	}

	/*****************************************
					DAMAGES			      
	****************************************/

	float q_damages[] = {70.f, 95.f, 120.f, 145.f, 170.f};

	float q_damage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = q_damages[q->level() - 1] + myhero->get_total_ability_power() * 0.85f;

		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float w_damages[] = {0.005f, 0.00625f, 0.0075f, 0.00875f, 0.01f};

	float w_damage(const game_object_script& target, const int tick_count)
	{
		if (w->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = 7.f + (w_damages[w->level() - 1] + (0.0025f * (myhero->get_total_ability_power() / 100.f))) * target->get_max_health();
		const float damage       = damagelib->calculate_damage_on_unit(myhero, target, &input);

		return damage * static_cast<float>(tick_count);
	}

	float e_damages[] = {65.f, 100.f, 135.f, 170.f, 205.f};

	float e_damage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = e_damages[e->level() - 1] + myhero->get_total_ability_power() * 0.5f;

		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float r_damages[] = {200.f, 300.f, 400.f};

	float r_damage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = r_damages[r->level() - 1] + myhero->get_total_ability_power() * 0.8f;

		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	/*****************************************
			CHAMPION SPECIFIC FUNCTIONS
	****************************************/

	game_object_script closest_jungle(const float range, const bool big_only)
	{
		game_object_script closest;
		for (auto& mob : entitylist->get_jugnle_mobs_minions())
		{
			if (mob == nullptr) continue;
			if (!mob->is_valid_target()) continue;
			if (mob->get_distance(myhero) > range) continue;
			if (big_only && mob->get_model().substr(mob->get_model().size() - 4) == "Mini") continue;

			if (closest == nullptr)
			{
				closest = mob;
			}
			if (mob->get_distance(myhero) <= closest->get_distance(myhero))
			{
				closest = mob;
			}
		}
		return closest;
	}

	float get_modified_r_range()
	{
		return r->range() * r_pred::range_slider->get_int() / 100.f;
	}

	bool w_is_active()
	{
		return w->toogle_state() == 2;
	}

	/*****************************************
			COMBO/HARASS SPELL USAGE	      
	****************************************/

	void q_combat(const bool combo)
	{
		const auto use_q = combo ? combo::use_q->get_bool() : harass::use_q->get_bool();
		const auto min_q = combo ? combo::min_q->get_int() : harass::min_q->get_int();

		if (!q->is_ready() || !use_q) return;

		const auto target = target_selector->get_target(q->range(), damage_type::magical);
		if (target == nullptr) return;
		if (target->get_distance(myhero) <= min_q) return;

		auto pred = q->get_prediction(target);
		if (pred.hitchance < helpers::get_hitchance(q_pred::hc->get_int())) return;
		if (evade->is_dangerous_path({myhero->get_position(), pred.get_cast_position()}, 0.f)) return;

		q->cast(pred.get_cast_position());
	}

	void w_combat(const bool combo)
	{
		const auto use_w  = combo ? combo::use_w->get_bool() : harass::use_w->get_bool();
		const auto w_mana = combo ? combo::w_mana->get_int() : harass::w_mana->get_int();

		if (!w->is_ready() || !use_w) return;
		if (w_is_active()) return;
		if (myhero->get_mana_percent() < w_mana) return;
		if (myhero->count_enemies_in_range(w->range()) == 0) return;

		w->cast();
	}

	void w_combat_deactivate(const bool combo)
	{
		const auto use_w  = combo ? combo::use_w->get_bool() : harass::use_w->get_bool();
		const auto w_mana = combo ? combo::w_mana->get_int() : harass::w_mana->get_int();

		if (!w->is_ready() || !use_w) return;
		if (!w_is_active()) return;
		if (myhero->count_enemies_in_range(w->range() * 1.1f) > 0 && myhero->get_mana_percent() > w_mana) return;

		w->cast();
	}

	void e_combat(const bool combo)
	{
		const auto use_e = combo ? combo::use_e->get_bool() : harass::use_e->get_bool();

		if (!e->is_ready() || !use_e) return;
		if (helpers::is_winding_up_attack()) return;
		if (helpers::count_enemies_after_delay(myhero->get_position(), e->range(), 0.25f, true) == 0) return;

		e->cast();
	}

	void r_combat()
	{
		if (!r->is_ready() || combo::hit_r->get_int() == 0) return;
		if (helpers::count_enemies_after_delay(myhero->get_position(), get_modified_r_range(), 0.25f, false) < combo::hit_r->get_int()) return;

		r->cast();
	}


	/*****************************************
			LANE CLEAR SPELL USAGE
	****************************************/

	void w_laneclear()
	{
		if (!lane_clear_menu::use_w->get_bool() || !w->is_ready()) return;

		const int all_minions  = helpers::count_minions(myhero->get_position(), w->range(), "both");
		const int minion_count = helpers::count_minions(myhero->get_position(), w->range(), "enemy");
		const int w_mana       = lane_clear_menu::w_mana->get_int();
		const int hit_w        = lane_clear_menu::hit_w->get_int();

		if (!w_is_active() && w_mana < myhero->get_mana_percent() && minion_count >= hit_w)
		{
			w->cast();
		}
		if (w_is_active() && (w_mana >= myhero->get_mana_percent() || all_minions == 0))
		{
			w->cast();
		}
	}

	void e_laneclear()
	{
		if (!lane_clear_menu::use_e->get_bool() || !e->is_ready()) return;

		const int minion_count = helpers::count_minions(myhero->get_position(), e->range(), "enemy");
		const int hit_e        = lane_clear_menu::hit_e->get_int();

		if (minion_count >= hit_e)
		{
			e->cast();
		}
	}

	/*****************************************
			JUNGLE CLEAR SPELL USAGE
	****************************************/

	void q_jungleclear()
	{
		if (!jungle_clear_menu::use_q->get_bool() || !q->is_ready()) return;

		const game_object_script minion = closest_jungle(q->range(), true);
		if (minion == nullptr) return;

		auto pred = q->get_prediction(minion);
		if (pred.hitchance < hit_chance::low) return;

		q->cast(pred.get_cast_position());
	}

	void w_jungleclear()
	{
		if (!jungle_clear_menu::use_w->get_bool() || !w->is_ready()) return;

		const int all_minions  = helpers::count_minions(myhero->get_position(), w->range(), "both");
		const int minion_count = helpers::count_minions(myhero->get_position(), w->range(), "neutral");
		const int w_mana       = jungle_clear_menu::w_mana->get_int();

		if (!w_is_active() && w_mana < myhero->get_mana_percent() && minion_count > 0)
		{
			w->cast();
		}
		if (w_is_active() && (w_mana >= myhero->get_mana_percent() || all_minions == 0))
		{
			w->cast();
		}
	}

	void e_jungleclear()
	{
		if (!jungle_clear_menu::use_e->get_bool() || !e->is_ready()) return;

		const int minion_count = helpers::count_minions(myhero->get_position(), e->range(), "neutral");
		if (minion_count > 0)
		{
			e->cast();
		}
	}

	/*****************************************
				MISC STUFF
	****************************************/

	void semi_q()
	{
		if (!mainMenu::semiq->get_bool()) return;
		if (!q->is_ready()) return;

		const auto target = target_selector->get_target(q->range(), damage_type::magical);
		if (target == nullptr) return;

		auto pred = q->get_prediction(target);
		if (pred.hitchance < helpers::get_hitchance(q_pred::semi_q_hc->get_int())) return;

		q->cast(pred.get_cast_position());
	}

	/*****************************************
					AUTO SPELLS
	****************************************/

	void auto_e_logic(const game_object_script& enemy)
	{
		if (!e->is_ready()) return;
		if (!enemy->is_valid_target(e->range())) return;

		auto pred = prediction->get_prediction(enemy, 0.25f);
		if (pred.get_unit_position().distance(myhero) > e->range()) return;

		if (ac::auto_e_kill->get_bool() && enemy->get_real_health() <= e_damage(enemy))
		{
			e->cast();
		}
	}

	void auto_loop()
	{
		if (myhero->is_recalling()) return;

		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			auto_e_logic(enemy);
		}
	}

	/*****************************************
					MODES
	****************************************/

	void Combo()
	{
		q_combat(true);
		w_combat(true);
		w_combat_deactivate(true);
		e_combat(true);
		r_combat();
	}


	void Harass()
	{
		q_combat(false);
		w_combat(false);
		w_combat_deactivate(false);
		e_combat(false);
	}

	void Lane_clear()
	{
		if (helpers::is_winding_up_attack()) return;

		w_laneclear();
		e_laneclear();
	}

	void Jungle_clear()
	{
		if (helpers::is_winding_up_attack()) return;

		q_jungleclear();
		w_jungleclear();
		e_jungleclear();
	}

	/*****************************************
					EVENTS
	****************************************/

	void on_update()
	{
		if (myhero->is_dead()) return;

		if (orbwalker->combo_mode())
		{
			Combo();
		}
		if (orbwalker->mixed_mode())
		{
			Harass();
		}
		if (farm_menu::toggle->get_bool())
		{
			if (orbwalker->lane_clear_mode())
			{
				Lane_clear();
				Jungle_clear();
			}
		}

		semi_q();
		auto_loop();
	}

	void on_draw()
	{
		if (myhero->is_dead()) return;

		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			if (dmg_draws::draw_damage->get_bool() && enemy->is_visible_on_screen())
			{
				float q_dmg = q->is_ready() ? q_damage(enemy) : 0.f;
				float e_dmg = e->is_ready() ? e_damage(enemy) : 0.f;
				float w_dmg = w->is_ready() ? w_damage(enemy, dmg_draws::w_ticks->get_int()) : 0.f;
				float r_dmg = r->is_ready() ? r_damage(enemy) : 0.f;

				if (!dmg_draws::draw_q->get_bool()) q_dmg = 0.f;
				if (!dmg_draws::draw_e->get_bool()) e_dmg = 0.f;
				if (!dmg_draws::draw_w->get_bool()) w_dmg = 0.f;
				if (!dmg_draws::draw_r->get_bool()) r_dmg = 0.f;

				const float total = q_dmg + e_dmg + w_dmg + r_dmg;
				helpers::draw_damage(enemy, total);
			}
		}
	}

	void on_env_draw()
	{
		if (myhero->is_dead()) return;

		if (draws::draw_q->get_bool())
		{
			helpers::draw_range(myhero->get_position(), misc().color_q->get_color(), misc().color_q2->get_color(), q->range(), misc().thicc->get_int(), helpers::get_glow());
		}
		if (draws::draw_w->get_bool())
		{
			helpers::draw_range(myhero->get_position(), misc().color_w->get_color(), misc().color_w2->get_color(), w->range(), misc().thicc->get_int(), helpers::get_glow());
		}
		if (draws::draw_e->get_bool())
		{
			helpers::draw_range(myhero->get_position(), misc().color_e->get_color(), misc().color_e2->get_color(), e->range(), misc().thicc->get_int(), helpers::get_glow());
		}
		if (draws::draw_r->get_bool())
		{
			helpers::draw_range(myhero->get_position(), misc().color_r->get_color(), misc().color_r2->get_color(), r->range(), misc().thicc->get_int(), helpers::get_glow());
		}
	}
}
