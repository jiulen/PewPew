/**
 CRegenHealthScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CRegenHealthScreen : public CCameraEffects
{
public:
	// Constructor
	CRegenHealthScreen(void);
	// Destructor
	virtual ~CRegenHealthScreen(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
