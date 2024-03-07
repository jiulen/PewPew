/**
 CPlayerIcon
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CPlayerIcon : public CCameraEffects
{
public:
	// Constructor
	CPlayerIcon(void);
	// Destructor
	virtual ~CPlayerIcon(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
