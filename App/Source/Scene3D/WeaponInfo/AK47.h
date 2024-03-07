/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CAK47 :public CWeaponInfo
{
public:
	// Default Constructor
	CAK47(void);
	// Destructor
	virtual ~CAK47(void);

	// Initialise this instance to default values
	bool Init(void);
};

