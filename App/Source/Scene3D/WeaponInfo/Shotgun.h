/**
 CShotgun
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CShotgun :	public CWeaponInfo
{
public:
	// Default Constructor
	CShotgun(void);
	// Destructor
	virtual ~CShotgun(void);

	// Initialise this instance to default values
	bool Init(void);
};

