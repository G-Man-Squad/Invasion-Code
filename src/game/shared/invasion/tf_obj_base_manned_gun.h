//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: A stationary gun that players can man
//
// $NoKeywords: $
//=============================================================================//

#ifndef TF_OBJ_BASE_MANNED_GUN_H
#define TF_OBJ_BASE_MANNED_GUN_H

#ifdef _WIN32
#pragma once
#endif

#include "basetfvehicle.h"
#include "tf_obj_manned_plasmagun_shared.h"
#include "env_laserdesignation.h"
#include "beam_shared.h"
#if defined( CLIENT_DLL )
	#include "c_obj_base_manned_gun.h"
#else
#endif

class CMoveData;

#if defined( CLIENT_DLL )

#define CObjectBaseMannedGun C_ObjectBaseMannedGun
#define CBaseTFVehicle		C_BaseTFVehicle

#else

// ------------------------------------------------------------------------ //
// A stationary gun that players can man that's built by the player
// ------------------------------------------------------------------------ //
class CObjectBaseMannedGun : public CBaseTFVehicle
{
public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_CLASS( CObjectBaseMannedGun, CBaseTFVehicle );

					CObjectBaseMannedGun();

	virtual void	Spawn();
	virtual void	Precache();
	virtual void	UpdateOnRemove( void );
	virtual void	GetControlPanelInfo( int nPanelIndex, const char *&pPanelName );
	virtual bool	CanTakeEMPDamage( void ) { return true; }
	virtual void	OnGoInactive( void );

	// Vehicle overrides
	virtual void	SetPassenger( int nRole, CBasePlayer *pEnt );
	virtual bool	IsPassengerVisible( int nRole = VEHICLE_ROLE_DRIVER ) { return true; }

	// Returns the eye position 
	virtual void	GetVehicleViewPosition( int nRole, Vector *pOrigin, QAngle *pAngles, float *pFOV = NULL );

	// Manned plasma passengers aren't damagable
	//virtual bool	IsPassengerDamagable( int nRole = VEHICLE_ROLE_DRIVER ) { return false; }

	virtual void	ProcessMovement( CBasePlayer *pPlayer, CMoveData *pMove );
	virtual void	SetupMove( CBasePlayer *player, CUserCmd *ucmd, IMoveHelper *pHelper, CMoveData *move );
	virtual void	FinishMove( CBasePlayer *player, CUserCmd *ucmd, CMoveData *move );

	virtual bool	ShouldAttachToParent( void ) { return true; }

	virtual bool	MustNotBeBuiltInConstructionYard( void ) const { return true; }

	virtual bool	ShouldUseThirdPersonVehicleView( void );

	virtual void	BaseMannedGunThink( void );

	float GetGunYaw() const;
	float GetGunPitch() const;

	// Buff
	bool			CanBeHookedToBuffStation( void );

protected:
	// Sets up various attachment points once the model is selected 
	// Derived classes should call this from within their SetTeamModel call
	void OnModelSelected();

	// Can we get into the vehicle?
	virtual bool CanGetInVehicle( CBaseTFPlayer *pPlayer );

	// Here's where we deal with weapons
	virtual void OnItemPostFrame( CBaseTFPlayer *pPassenger );

	// Fire the weapon
	virtual void Fire( void ) {}

	void		StopDesignating( void );
	void		UpdateDesignator( void );

	virtual void SetupAttachedVersion( void );
	virtual void SetupUnattachedVersion( void );

	// Sets the movement style
	void SetMovementStyle( MovementStyle_t style );

	// Calculate the max range of this gun
	void CalculateMaxRange( float flDefensiveRange, float flOffensiveRange );

protected:
	// Movement...
	CObjectMannedPlasmagunMovement m_Movement;

	float	m_flMaxRange;

	// attachment points
	int m_nBarrelAttachment;
	int m_nBarrelPivotAttachment;
	int m_nStandAttachment;
	int m_nEyesAttachment;

	// Movement style
	CNetworkVar( MovementStyle_t, m_nMoveStyle );

	// Barrel height...
	float	m_flBarrelHeight;

	CNetworkVar( int, m_nAmmoType );
	CNetworkVar( int, m_nAmmoCount );
	CNetworkVar( float, m_flGunYaw );		// 0 = front, 90 = left, 180 = back, 270 = right
	CNetworkVar( float, m_flGunPitch );	// 0 = forward, -90 = pointing down, 90 = pointing up..
	CNetworkVar( float, m_flBarrelPitch );

	float	m_flReturnToInitialTime;

	// Laser designation
	CNetworkHandle( CBeam, m_hBeam );
	CNetworkHandle( CEnvLaserDesignation, m_hLaserDesignation );
};


//-----------------------------------------------------------------------------
// Inline methods 
//-----------------------------------------------------------------------------
inline bool	CObjectBaseMannedGun::CanBeHookedToBuffStation( void ) 
{ 
	return true; 
}
#endif

#endif // TF_OBJ_BASE_MANNED_GUN_H
