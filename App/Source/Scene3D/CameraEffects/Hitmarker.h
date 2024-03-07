/**
 CHitmarker
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CHitmarker : public CCameraEffects
{
public:
	// Constructor
	CHitmarker(void);
	// Destructor
	virtual ~CHitmarker(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
