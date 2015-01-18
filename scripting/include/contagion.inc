#if defined _contagion_included
 #endinput
#endif
#define _contagion_included

#include <sdktools>

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

