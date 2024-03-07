/**
 CCrossHair2
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CCrossHair2 : public CCameraEffects
{
public:
	// Constructor
	CCrossHair2(void);
	// Destructor
	virtual ~CCrossHair2(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
