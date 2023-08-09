#include "../plugin_sdk/plugin_sdk.hpp"
#include "Amumu.h"
#include "Helpers.h"

PLUGIN_NAME(AIO_NAME);
SUPPORTED_CHAMPIONS(champion_id::Amumu);

PLUGIN_API bool on_sdk_load(plugin_sdk_core* plugin_sdk_good)
{
	DECLARE_GLOBALS(plugin_sdk_good);

	switch (myhero->get_champion())
	{
		case champion_id::Amumu:
			amumu::load();
			return true;
		default:
			break;
	}
	return false;
}

PLUGIN_API void on_sdk_unload()
{
	switch (myhero->get_champion())
	{
		case champion_id::Amumu:
			amumu::unload();
			break;
		default:
			break;
	}
}
