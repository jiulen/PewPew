/**
 CFSM
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

class CFSM
{
public:
	enum class FSM : unsigned int
	{
		IDLE = 0,
		PATROL = 1,
		ATTACK = 2,
		RETREAT = 3,
		MOVE = 4,
		NUM_FSM
	};

	// Constructor
	CFSM(void);

	// Destructor
	virtual ~CFSM(void);

	// Init
	virtual bool Init(void);

	// Update
	virtual bool Update(const double dElapsedTime);

	// PrintSelf
	virtual void PrintSelf(void) const;

	// Current FSM
	FSM sCurrentFSM;
protected:	

	// FSM counter - count how many frames it has been in this FSM
	int iFSMCounter;

	// Max count in a state
	const int iMaxFSMCounter = 60;
};