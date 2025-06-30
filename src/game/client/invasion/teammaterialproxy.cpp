//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "proxyentity.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imaterialvar.h"
#include "c_team.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// $sineVar : name of variable that controls the alpha level (float)
class CTeamMaterialProxy : public CEntityMaterialProxy
{
public:
	CTeamMaterialProxy();
	virtual ~CTeamMaterialProxy();
	virtual bool Init( IMaterial *pMaterial, KeyValues* pKeyValues );
	virtual void OnBind( C_BaseEntity *pEnt );
	virtual IMaterial *GetMaterial();

private:
	IMaterialVar* m_FrameVar;
};


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------

CTeamMaterialProxy::CTeamMaterialProxy()
{
	m_FrameVar = 0;
}

CTeamMaterialProxy::~CTeamMaterialProxy()
{
}


//-----------------------------------------------------------------------------
// Init baby...
//-----------------------------------------------------------------------------
bool CTeamMaterialProxy::Init( IMaterial *pMaterial, KeyValues* pKeyValues )
{
	bool foundVar;
	m_FrameVar = pMaterial->FindVar( "$frame", &foundVar, false );
	if( !foundVar )
		m_FrameVar = 0;
	return true;
}

//-----------------------------------------------------------------------------
// Set the appropriate texture...
//-----------------------------------------------------------------------------
void CTeamMaterialProxy::OnBind( C_BaseEntity *pEnt )
{
	if( !m_FrameVar )
		return;

	int team = pEnt->GetRenderTeamNumber();
	team--;

	// Use that as an animated frame number
	m_FrameVar->SetIntValue( team );
}

IMaterial * CTeamMaterialProxy::GetMaterial()
{
	if ( !m_FrameVar)
		return NULL;

	return m_FrameVar->GetOwningMaterial();
}

EXPOSE_INTERFACE( CTeamMaterialProxy, IMaterialProxy, "TeamTexture" IMATERIAL_PROXY_INTERFACE_VERSION );
