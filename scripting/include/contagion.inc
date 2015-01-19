#if defined _contagion_included
 #endinput
#endif
#define _contagion_included

#include <sdktools>

/**
 * Maximum length of a weapon name string
 */
#define WEAPONS_MAX_LENGTH 32

/**
 * Number of weapon slots
 */
#define WEAPONS_SLOTS_MAX 4

/**
 * Weapon slots.
 */
enum WeaponsSlot
{
	Slot_Invalid		= -1,	/** Invalid weapon (slot). */
	Slot_Primary		= 0,	/** Primary weapon slot. */
	Slot_Secondary		= 1,	/** Secondary weapon slot. */
	Slot_Third			= 2,	/** 3rd weapon slot. */
	Slot_Forth			= 3,	/** 4th weapon slot. */
}

/**
 * Team numbers.
 */
enum CTEAM
{
	CTEAM_Unassigned = 0,
	CTEAM_Spectator = 1,
	CTEAM_Survivor = 2,
	CTEAM_Zombie = 3	
};

/**
 * Checks if the client is infected and how long until he gets infected.
 * 
 * @param client		Client Index.
 * @return				Infection Status.
 */
stock Float:CONTAGION_GetInfectionTime(client)
{
	return GetEntPropFloat(client, Prop_Send, "m_TimeTillInfection");
}

/**
 * Sets client infection status.
 * 
 * @param client		Client Index.
 * @param infection		Set infection status
 * @noreturn
 */
stock CONTAGION_SetInfectionTime(client, Float:infection)
{
	return SetEntPropFloat(client, Prop_Send, "m_TimeTillInfection", infection);
}

/**
 * Checks how much stamina the client haves.
 * 
 * @param client		Client Index.
 * @return				Stamina Status.
 */
stock Float:CONTAGION_GetStamina(client)
{
	return GetEntPropFloat(client, Prop_Send, "m_flStamina");
}

/**
 * Sets client stamina.
 * 
 * @param client		Client Index.
 * @param stamina		Set stamina
 * @noreturn
 */
stock CONTAGION_SetStamina(client, Float:stamina)
{
	return SetEntPropFloat(client, Prop_Send, "m_flStamina", stamina);
}

/**
 * Gets extra ammunition for any weapon
 * #Note1: This doesn't apply for melee weapons
 * 
 * @param entity		Entity Index.
 * @param ammo			Set extra ammunition
 * @noreturn
 */
stock CONTAGION_GetExtraAmmo(entity)
{
	return GetEntProp(entity, Prop_Send, "m_iExtraPrimaryAmmo");
}

/**
 * Sets extra ammunition for any weapon
 * #Note1: This doesn't apply for melee weapons
 * 
 * @param entity		Entity Index.
 * @param ammo			Set extra ammunition
 * @noreturn
 */
stock CONTAGION_SetExtraAmmo(entity, ammo)
{
	return SetEntProp(entity, Prop_Send, "m_iExtraPrimaryAmmo", ammo);
}

/**
 * Get the survivor character
 * 
 * @param client		Client Index.
 * @return				Get survivor character
 */
stock CONTAGION_GetSurvivorCharacter(client)
{
	return GetEntProp(client, Prop_Send, "m_survivorCharacter");
}

/**
 * Set the survivor character
 * 
 * @param client		Client Index.
 * @return				Set survivor character
 * @noreturn
 */
stock CONTAGION_SetSurvivorCharacter(client, index)
{
	return SetEntProp(client, Prop_Send, "m_survivorCharacter", index);
}


/**
 * Checks if a client has a specific weapon.
 * 
 * @param client	The client index.
 * @param weapon	The weapon classname.
 */
stock bool:CONTAGION_ClientHasWeapon(client, const String:weapon[])
{
	// Get all of client's current weapons.
	new weapons[WeaponsSlot];
	CONTAGION_GetClientWeapons(client, weapons);
	
	decl String:classname[64];
	
	// x = slot index
	for (new x = 0; x < WEAPONS_SLOTS_MAX; x++)
	{
		// If slot is empty, then stop.
		if (weapons[x] == -1)
		{
			continue;
		}
		
		// If the weapon's classname matches, then return true.
		GetEdictClassname(weapons[x], classname, sizeof(classname));
		ReplaceString(classname, sizeof(classname), "weapon_", "");
		if (StrEqual(weapon, classname, false))
		{
			return true;
		}
	}
	
	return false;
}

/**
 * Return an array that contains all client's weapon indexes.
 * 
 * @param client	The client index.
 * @param weapons	The weapon index array.
 *					-1 if no weapon in slot. 
 */
stock CONTAGION_GetClientWeapons(client, weapons[WeaponsSlot])
{
	// x = Weapon slot.
	for (new x = 0; x < WEAPONS_SLOTS_MAX; x++)
	{
		weapons[x] = GetPlayerWeaponSlot(client, x);
	}
}

/**
 * Removes all weapons from a client's weapon slot
 *
 * @param client		Player's index.
 * @param slot			Slot index (0-5)
 * @noreturn
 * @error				Invalid client, invalid slot or lack of mod support
 */
stock CONTAGION_RemoveWeaponSlot(client, slot)
{
	new weaponIndex;
	while ((weaponIndex = GetPlayerWeaponSlot(client, slot)) != -1)
	{
		RemovePlayerItem(client, weaponIndex);
		AcceptEntityInput(weaponIndex, "Kill");
	}
}

/**
 * Removes all firearms from a client
 *
 * @param client		Player's index.
 * @noreturn
 */
stock CONTAGION_RemoveAllFirearms(client)
{
	new weapons[WeaponsSlot];
	CONTAGION_GetClientWeapons(client, weapons);
	
	decl String:classname[64];
	
	for (new x = 0; x < WEAPONS_SLOTS_MAX; x++)
	{
		if (weapons[x] == -1)
		{
			continue;
		}
		
		GetEdictClassname(weapons[x], classname, sizeof(classname));
		
		if (StrEqual(classname, "weapon_melee", false) || StrEqual(classname, "weapon_phone", false)
			|| StrEqual(classname, "weapon_boltcutter", false) || StrEqual(classname, "weapon_coin", false)
			|| StrEqual(classname, "weapon_ied", false) || StrEqual(classname, "weapon_grenade", false)
			|| StrEqual(classname, "weapon_key", false) || StrEqual(classname, "weapon_firstaid", false)
			|| StrEqual(classname, "weapon_keycard", false) || StrEqual(classname, "weapon_nailgun", false)
			|| StrEqual(classname, "weapon_plight", false) || StrEqual(classname, "weapon_zombie", false))
		{} // Do nothing
		else
		{
	//		PrintToServer("[[ delete: %s ]]", classname);
	//		PrintToServer("[[ delete: %d ]]", weapons[x]);
			RemovePlayerItem(client, weapons[x]);
			AcceptEntityInput(weapons[x], "Kill");
		}
	}
}

/**
 * Removes all weapons from a client
 *
 * @param client		Player's index.
 * @noreturn
 */
stock CONTAGION_RemoveAllWeapons(client)
{
	for (new i = 0; i <= 5; i++)
	{
		CONTAGION_RemoveWeaponSlot(client, i);
	}
}

/**
 * Removes all weapons from a client's weapon slot
 *
 * @param client		Player's index.
 * @param health		Health Index
 * @param newmaxhealth	Maxhealth Intex
 * @noreturn
 * @error				Invalid client, invalid health or invalid maxhealth
 */
stock CONTAGION_SetNewHealth(client, health, newmaxhealth)
{
	if (health > newmaxhealth)
		ThrowError("The max health (%d) can't be lower than the new health (%d)!", newmaxhealth, health);
	
	SetEntProp(client, Prop_Send, "m_iMaxHealth", newmaxhealth);
	SetEntProp(client, Prop_Send, "m_iHealth", health);
}

