//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "basetfplayer_shared.h"
#include "engine/IEngineSound.h"
#include "grenade_rocket.h"

extern short	g_sModelIndexFireball;

#if !defined( CLIENT_DLL )
IMPLEMENT_SERVERCLASS_ST( CGrenadeRocket, DT_GrenadeRocket)
END_SEND_TABLE()
LINK_ENTITY_TO_CLASS( grenade_rocket, CGrenadeRocket );
PRECACHE_REGISTER(grenade_rocket);
#endif

BEGIN_DATADESC( CGrenadeRocket )

	DEFINE_FIELD( m_flDamage, FIELD_FLOAT ),

	// Function Pointers
#if !defined( CLIENT_DLL )
	DEFINE_FUNCTION( MissileTouch ),
	DEFINE_FUNCTION( FollowThink ),
#endif

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CGrenadeRocket::CGrenadeRocket()
{
	m_pRealOwner = NULL;
	m_hLockTarget = NULL;
	UseClientSideAnimation();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CGrenadeRocket::Precache( void )
{
	PrecacheModel( "models/weapons/w_missile.mdl" );

	PrecacheScriptSound( "GrenadeRocket.FlyLoop" );

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeRocket::Spawn( void )
{
	Precache();

	SetMoveType( MOVETYPE_FLY );
	SetSolid( SOLID_BBOX );
	SetModel( "models/weapons/w_missile.mdl" );
#if !defined( CLIENT_DLL )
	UTIL_SetSize( this, vec3_origin, vec3_origin );
#endif

	SetCollisionGroup( TFCOLLISION_GROUP_WEAPON );
	SetTouch( &CGrenadeRocket::MissileTouch );

	SetDamage( 50 );

	// Forward!
	Vector forward;
	AngleVectors( GetLocalAngles(), &forward, NULL, NULL );
	SetAbsVelocity( forward * ROCKET_VELOCITY );

	EmitSound( "GrenadeRocket.FlyLoop" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeRocket::MissileTouch( CBaseEntity *pOther )
{
	Assert( pOther );
	if ( !pOther->IsSolid() )
		return;

#if !defined( CLIENT_DLL )
	Vector vecAbsOrigin = GetAbsOrigin();
	CPASFilter filter( vecAbsOrigin );
	te->Explosion( filter, 0.0,	&vecAbsOrigin, g_sModelIndexFireball, 2.0, 15, TE_EXPLFLAG_NONE, 100, m_flDamage );
#endif

	StopSound( "GrenadeRocket.FlyLoop" );

	// Don't apply explosive damage if it hit a shield of any kind...
	bool bHittingShield = false;
	if (pOther->GetCollisionGroup() == TFCOLLISION_GROUP_SHIELD)
	{
		bHittingShield = true;
	}
	else if ( pOther->IsPlayer() )
	{
		CBaseTFPlayer *pPlayer = static_cast<CBaseTFPlayer*>(pOther);

		trace_t tr;
		float flDamage = m_flDamage;
		bHittingShield = pPlayer->IsHittingShield( GetAbsVelocity(), &flDamage );
	}

#if !defined( CLIENT_DLL )
	if (!bHittingShield)
	{
		RadiusDamage( CTakeDamageInfo( this, m_pRealOwner, m_flDamage, DMG_BLAST ), vecAbsOrigin, 100, CLASS_NONE, NULL );
	}

	UTIL_Remove( this );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Make this rocket lock onto it's target and track it
//-----------------------------------------------------------------------------
void CGrenadeRocket::LockOnto( CBaseEntity *pTarget )
{
	m_hLockTarget = pTarget;
	SetThink( &CGrenadeRocket::FollowThink );
	SetNextThink( gpGlobals->curtime + 0.1f );
}

//-----------------------------------------------------------------------------
// Purpose: Try and turn towards the target point
//-----------------------------------------------------------------------------
void CGrenadeRocket::FollowThink( void )
{
	if ( m_hLockTarget == NULL )
		return;

	// Weave slightly drunkenly to target
	Vector vecTarget = m_hLockTarget->GetAbsOrigin() - GetLocalOrigin();
	VectorNormalize( vecTarget );

	QAngle angles;
	VectorAngles( vecTarget, angles );
	SetLocalAngles( angles );
	
	Vector vecVelocity = GetAbsVelocity();
	float flSpeed = vecVelocity.Length();
	vecVelocity = vecVelocity * 0.2 + vecTarget * flSpeed * 1.2;
	// Clip to maxspeed
	if ( vecVelocity.Length() > ROCKET_VELOCITY )
	{
		VectorNormalize( vecVelocity );
		vecVelocity *= ROCKET_VELOCITY;
	}
	SetAbsVelocity( vecVelocity );

	SetNextThink( gpGlobals->curtime + 0.1f );
}

//-----------------------------------------------------------------------------
// Purpose: Create a missile
//-----------------------------------------------------------------------------
CGrenadeRocket *CGrenadeRocket::Create( const Vector &vecOrigin, const Vector &vecForward, edict_t *pentOwner = NULL, CBaseEntity *pRealOwner = NULL )
{
	CGrenadeRocket *pRocket = (CGrenadeRocket *)CreateEntityByName("grenade_rocket" );

#if !defined( CLIENT_DLL )
	UTIL_SetOrigin( pRocket, vecOrigin );
	QAngle angles;
	VectorAngles( vecForward, angles );
	pRocket->SetLocalAngles( angles );
	pRocket->Spawn();
	pRocket->SetOwnerEntity( Instance( pentOwner ) );
	pRocket->m_pRealOwner = pRealOwner;

	if (pentOwner)
	{
		CBaseEntity *pOwnerEnt = GetContainingEntity( pentOwner );
		pRocket->ChangeTeam( pOwnerEnt->GetTeamNumber() );
	}
#endif

	return pRocket;
}
