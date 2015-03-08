#if defined _contagion_included
 #endinput
#endif
#define _contagion_included

#include <sdktools>

/**
	Smlib fix
**/
#define MAX_WEAPONS				48	// Max number of weapons availabl

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
 * Sets extra ammunition for any weapon
 * #Note1: This doesn't apply for melee weapons
 * 
 * @param entity		Entity Index.
 * @param ammo			Set extra ammunition
 * @noreturn
 */
stock CONTAGION_SetExtraAmmo(entity, ammo)
{
	new GetCurrentAmmo = GetEntProp(entity, Prop_Send, "m_iClip1");
	return SetEntProp(entity, Prop_Send, "m_iClip1", GetCurrentAmmo + ammo);
}

/**
 * Sets ammunition for any weapon
 * #Note1: This doesn't apply for melee weapons
 * 
 * @param entity		Entity Index.
 * @param ammo			Set extra ammunition
 * @noreturn
 */
stock CONTAGION_SetAmmo(client, weapon, ammo)
{
	new offset_ammo = FindDataMapOffs(client, "m_iAmmo");
	new offset = offset_ammo + (Weapon_GetPrimaryAmmoType(weapon) * 4);
	SetEntData(client, offset, ammo, 4, true);
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
 */
stock CONTAGION_SetSurvivorCharacter(client, index)
{
	return SetEntProp(client, Prop_Send, "m_survivorCharacter", index);
}

/**
 * Gives the client a specific weapon
 * 
 * @param client	The client index.
 * @param weapon	The weapon classname.
 * @param ammo		The weapon start ammo.
 */
stock CONTAGION_GiveClientWeapon(client, const String:weapon[], ammo=0)
{
	if (StrEqual(weapon,""))
		return;
	
	GivePlayerItem(client, weapon);
	new GetWeapon = Client_GetWeapon(client, weapon);
	if (ammo > 0)
		CONTAGION_SetAmmo(client, GetWeapon, ammo);
}

/**
 * Checks if a client has a specific weapon. and then gives ammo for it (primary ammo)
 * 
 * @param client	The client index.
 * @param weapon	The weapon classname.
 * @param ammo		The weapon ammo.
 */
stock CONTAGION_ClientGiveExtraAmmo(client, const String:weapon[], ammo=0)
{
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
		
		GetEdictClassname(weapons[x], classname, sizeof(classname));
		if (StrEqual(weapon, classname, false))
		{
			if (ammo > 0)
				CONTAGION_SetExtraAmmo(weapons[x], ammo);
			else
				ThrowError("You need to specify how much ammo you want to give to the client's weapon (%s)!", weapon);
		}
	}
}

/**
 * Checks if a client has a specific weapon.
 * 
 * @param client	The client index.
 * @param weapon	The weapon classname.
 */
stock bool:CONTAGION_ClientHasWeapon(client, const String:weapon[])
{
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
 * Checks all weapons that the client has.
 * 
 * @param client	The client index.
 * @param weapon	The weapon classname.
 */
stock CONTAGION_CheckClientWeapons(client)
{
	new weapons[WeaponsSlot];
	CONTAGION_GetClientWeapons(client, weapons);
	
	decl String:classname[64];
	
	PrintToServer("[[ %N's Weapons ]]", client);
	
	// x = slot index
	for (new x = 0; x < WEAPONS_SLOTS_MAX; x++)
	{
		// If slot is empty, then stop.
		if (weapons[x] == -1)
		{
			continue;
		}
		
		GetEdictClassname(weapons[x], classname, sizeof(classname));
		PrintToServer("[[ Slot: %d | Weapon: %s ]]", x, classname);
	}
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
 * Get the survivor max speed
 * 
 * @param client		Client Index.
 * @return				Get survivor speed
 */
stock Float:CONTAGION_GetSurvivorSpeed(client)
{
	return GetEntPropFloat(client, Prop_Send, "m_flMaxspeed");
}

/**
 * If the survivor is sprinting
 * 
 * @param client		Client Index.
 * @return				Get sprinting bool
 */
stock CONTAGION_GetSurvivorSprinting(client)
{
	return GetEntProp(client, Prop_Send, "m_bIsSprinting");
}

/**
 * If the survivor has sprinting penalty
 * 
 * @param client		Client Index.
 * @return				Get penalty bool
 */
stock CONTAGION_GaveSprintPenalty(client)
{
	return GetEntProp(client, Prop_Send, "m_bGaveSprintPenalty");
}

/**
 * If the survivor has empty sprinting penalty
 * 
 * @param client		Client Index.
 * @return				Get penalty bool
 */
stock CONTAGION_EmptySprintPenalty(client)
{
	return GetEntProp(client, Prop_Send, "m_bEmptySprintPenalty");
}

/**
 * Get the survivor stamina
 * 
 * @param client		Client Index.
 * @return				Get stamina
 */
stock Float:CONTAGION_GetSurvivorStamina(client)
{
	return GetEntPropFloat(client, Prop_Send, "m_flStamina");
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

/*
	Smlib fix
*/
stock Client_GetWeapon(client, const String:className[])
{
	new offset = Client_GetWeaponsOffset(client) - 4;

	for (new i=0; i < MAX_WEAPONS; i++) {
		offset += 4;

		new weapon = GetEntDataEnt2(client, offset);
		
		if (!Weapon_IsValid(weapon)) {
			continue;
		}
		
		if (Entity_ClassNameMatches(weapon, className)) {
			return weapon;
		}
	}
	
	return INVALID_ENT_REFERENCE;
}

stock Weapon_IsValid(weapon)
{
	if (!IsValidEdict(weapon)) {
		return false;
	}

	return Entity_ClassNameMatches(weapon, "weapon_", true);
}

stock Client_GetWeaponsOffset(client)
{
	static offset = -1;

	if (offset == -1) {
		offset = FindDataMapOffs(client, "m_hMyWeapons");
	}
	
	return offset;
}

stock bool:Entity_ClassNameMatches(entity, const String:className[], partialMatch=false)
{
	decl String:entity_className[64];
	Entity_GetClassName(entity, entity_className, sizeof(entity_className));

	if (partialMatch) {
		return (StrContains(entity_className, className) != -1);
	}
	
	return StrEqual(entity_className, className);
}

stock Entity_GetClassName(entity, String:buffer[], size)
{
	GetEntPropString(entity, Prop_Data, "m_iClassname", buffer, size);
	
	if (buffer[0] == '\0') {
		return false;
	}
	
	return true;
}

stock Weapon_GetPrimaryAmmoType(weapon)
{
	return GetEntProp(weapon, Prop_Data, "m_iPrimaryAmmoType");
}

