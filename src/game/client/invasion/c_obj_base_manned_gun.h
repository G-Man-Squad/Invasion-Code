//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef C_OBJ_BASE_MANNED_GUN_H
#define C_OBJ_BASE_MANNED_GUN_H
#ifdef _WIN32
#pragma once
#endif

#include "basetfvehicle.h"
#include "tf_obj_manned_plasmagun_shared.h"
#include "ObjectControlPanel.h"
#include "tf_obj_base_manned_gun.h"

// ------------------------------------------------------------------------ //
// A stationary gun that players can man that's built by the player
// ------------------------------------------------------------------------ //
class C_ObjectBaseMannedGun : public C_BaseTFVehicle
{
public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_CLASS( C_ObjectBaseMannedGun, C_BaseTFVehicle );

	C_ObjectBaseMannedGun();

	virtual void	Spawn();
	virtual void	Precache();
	virtual void	UpdateOnRemove( void );
	virtual void	GetControlPanelInfo( int nPanelIndex, const char *&pPanelName );
	virtual bool	CanTakeEMPDamage( void ) { return true; }
	virtual void	OnGoInactive( void );

	// Vehicle overrides
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

#if defined ( CLIENT_DLL )
// IClientVehicle overrides.
public:
	virtual void DrawHudElements( void );
	virtual void UpdateViewAngles( C_BasePlayer *pLocalPlayer, CUserCmd *pCmd );

	virtual QAngle GetPassengerAngles( QAngle angCurrent, int nRole );

// C_BaseEntity overrides.
public:
	virtual void OnDataChanged( DataUpdateType_t updateType );
	virtual void GetBoneControllers(float controllers[MAXSTUDIOBONECTRLS], float dadt);

private:
	void	DrawCrosshair( void );

#endif

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

	CHudTexture 					*iconCrosshair;

private:
	C_ObjectBaseMannedGun( const C_ObjectBaseMannedGun& ); // not defined, not accessible
};

//-----------------------------------------------------------------------------
// Inline methods 
//-----------------------------------------------------------------------------
inline bool	C_ObjectBaseMannedGun::CanBeHookedToBuffStation( void ) 
{ 
	return true; 
}

//-----------------------------------------------------------------------------
// Control screen 
//-----------------------------------------------------------------------------
class CMannedPlasmagunControlPanel : public CRotatingObjectControlPanel
{
	DECLARE_CLASS( CMannedPlasmagunControlPanel, CRotatingObjectControlPanel );

public:
	CMannedPlasmagunControlPanel( vgui::Panel *parent, const char *panelName );
	virtual bool Init( KeyValues* pKeyValues, VGuiScreenInitData_t* pInitData );
	virtual void OnTick();
	virtual void OnCommand( const char *command );

	void GetInGun( void );

private:
	vgui::Label		*m_pMannedLabel;
	vgui::Button	*m_pOccupyButton;
};

#endif // C_OBJ_BASE_MANNED_GUN_H
