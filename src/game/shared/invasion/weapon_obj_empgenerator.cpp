//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "basetfplayer_shared.h"
#if !defined( CLIENT_DLL )
#include "tf_basecombatweapon.h"
#include "tf_obj_empgenerator.h"
#else
#include "c_tf_basecombatweapon.h"
#endif
#include "baseobject_shared.h"
#include "weapon_basecombatobject.h"

//-----------------------------------------------------------------------------
// Purpose: Combat object weapon for the EMP Generator
//-----------------------------------------------------------------------------
class CWeaponObjEMPGenerator : public CWeaponBaseCombatObject
{
	DECLARE_CLASS( CWeaponObjEMPGenerator, CWeaponBaseCombatObject );
public:
	CWeaponObjEMPGenerator( void );

#if !defined( CLIENT_DLL )
	DECLARE_SERVERCLASS();
#endif
};

#if !defined( CLIENT_DLL )
IMPLEMENT_SERVERCLASS_ST( CWeaponObjEMPGenerator, DT_WeaponObjEMPGenerator )
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_obj_empgenerator, CWeaponObjEMPGenerator );
PRECACHE_WEAPON_REGISTER(weapon_obj_empgenerator);
#endif

CWeaponObjEMPGenerator::CWeaponObjEMPGenerator( void )
{
#if !defined( CLIENT_DLL )
	m_szObjectName = "obj_empgenerator";
	m_vecBuildMins = EMPGENERATOR_MINS - Vector( 4,4,4 );
	m_vecBuildMaxs = EMPGENERATOR_MAXS + Vector( 4,4,4 );
#endif
}