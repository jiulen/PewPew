/**
 CLowHealthScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CLowHealthScreen : public CCameraEffects
{
public:
	// Constructor
	CLowHealthScreen(void);
	// Destructor
	virtual ~CLowHealthScreen(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
