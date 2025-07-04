//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef C_TF_CLASS_SAPPER_H
#define C_TF_CLASS_SAPPER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_tf_playerclass.h"
#include "dt_recv.h"

class C_PlayerClassSapper : public C_PlayerClass
{
	DECLARE_CLASS( C_PlayerClassSapper, C_PlayerClass );

public:

	C_PlayerClassSapper( C_BaseTFPlayer *pPlayer );
	virtual ~C_PlayerClassSapper();

	DECLARE_PREDICTABLE();

	PlayerClassSapperData_t* GetClassData( void ) { return &m_ClassData; } 

	// Energy handling
	float	GetDrainedEnergy( void );
	void	DeductDrainedEnergy( float flEnergy );

	PlayerClassSapperData_t	m_ClassData;

};

EXTERN_RECV_TABLE( DT_PlayerClassSapperData )

#endif // C_TF_CLASS_SAPPER_H