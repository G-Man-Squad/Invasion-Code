//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Client's sheild entity
//
// $Workfile:     $
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "C_Shield.h"
#include "tf_shieldshared.h"

enum
{
	NUM_SUBDIVISIONS = 21,
};

#define EMP_WAVE_AMPLITUDE 8.0f

//-----------------------------------------------------------------------------
// Mobile version of the shield 
//-----------------------------------------------------------------------------

class C_ShieldMobile;
class C_ShieldMobileActiveVertList : public IActiveVertList
{
public:
	void			Init( C_ShieldMobile *pShield, unsigned char *pVertList );

// IActiveVertList overrides.
public:

	virtual int		GetActiveVertState( int iVert );
	virtual void	SetActiveVertState( int iVert, int bOn );

private:
	C_ShieldMobile	*m_pShield;
	unsigned char	*m_pVertsActive;
};


class C_ShieldMobile : public C_Shield
{
	DECLARE_CLASS( C_ShieldMobile, C_Shield );
public:

	C_ShieldMobile();
	~C_ShieldMobile();

	void OnDataChanged( DataUpdateType_t updateType );
	virtual void GetBounds( Vector& mins, Vector& maxs );

	virtual void AddEntity( );

	// Return true if the panel is active 
	virtual bool IsPanelActive( int x, int y );

	// Gets at the control point data; who knows how it was made?
	virtual void GetShieldData( Vector const** ppVerts, float* pOpacity, float* pBlend );
	virtual const Vector& GetPoint( int x, int y ) { return m_ShieldEffect.GetPoint( x, y ); }

	virtual void SetThetaPhi( float flTheta, float flPhi ) { m_ShieldEffect.SetThetaPhi(flTheta,flPhi); }

public:
	// networked data
	unsigned char	m_pVertsActive[SHIELD_VERTEX_BYTES];
	unsigned char	m_ShieldState;

private:
	C_ShieldMobile( const C_ShieldMobile& );

	// Is a particular panel an edge?
	bool IsVertexValid( float s, float t ) const;
	void PreRender( );

private:
	CShieldEffect					m_ShieldEffect;
	C_ShieldMobileActiveVertList	m_VertList;
	float m_flTheta;
	float m_flPhi;
};


//-----------------------------------------------------------------------------
// C_ShieldMobileActiveVertList functions
//-----------------------------------------------------------------------------

void C_ShieldMobileActiveVertList::Init( C_ShieldMobile *pShield, unsigned char *pVertList )
{
	m_pShield = pShield;
	m_pVertsActive = pVertList;
}


int C_ShieldMobileActiveVertList::GetActiveVertState( int iVert )
{
	return m_pVertsActive[iVert>>3] & (1 << (iVert & 7));
}


void C_ShieldMobileActiveVertList::SetActiveVertState( int iVert, int bOn )
{
	if ( bOn )
		m_pVertsActive[iVert>>3] |= (1 << (iVert & 7));
	else
		m_pVertsActive[iVert>>3] &= ~(1 << (iVert & 7));
}