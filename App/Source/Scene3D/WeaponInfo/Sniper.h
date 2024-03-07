/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CSniper :	public CWeaponInfo
{
public:
	// Default Constructor
	CSniper(void);
	// Destructor
	virtual ~CSniper(void);

	// Initialise this instance to default values
	bool Init(void);
};

