#pragma once

#include "CComponent.h"

class CIncident :
	public CComponent
{
	

public:
	int Update();

	CLONE(CIncident);

public:
	CIncident();
	~CIncident();
};