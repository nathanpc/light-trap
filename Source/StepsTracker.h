/**
 * StepsTracker.h
 * Manages the film development steps and the workflow.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _STEPSTRACKER_H
#define _STEPSTRACKER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

/**
 * Manages the film development steps and the workflow.
 */
class StepsTracker {
private:
	HINSTANCE hInst;
	HWND hwndParent;
	HWND hwndList;

public:
	// Constructor and destructor.
	StepsTracker();
	virtual ~StepsTracker();

	// User interface.
	void SetupComponents(HINSTANCE hInst, HWND hwndParent, HWND hwndBar);
	void AddStep(UINT uDuration, LPTSTR szChemical, bool bAgitate);

	// Misc.
	int DurationToString(LPTSTR szBuffer, UINT uDuration);
};

#endif // _STEPSTRACKER_H
