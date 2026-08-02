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
#include "TimerDialog.h"

/**
 * Manages the film development steps and the workflow.
 */
class StepsTracker {
private:
	HINSTANCE hInst;
	HWND hwndParent;
	HWND hwndList;

	TimerDialog *timer;

public:
	// Constructor and destructor.
	StepsTracker();
	virtual ~StepsTracker();
	void SetTimerDialog(TimerDialog *timer);

	// User interface.
	void SetupComponents(HINSTANCE hInst, HWND hwndParent, HWND hwndBar);
	void AddStep(UINT uDuration, LPTSTR szChemical, bool bAgitate,
		bool bAutoNext) const;
	int GetSelectedItemIndex() const;
	bool GetSelectedItem(LPLVITEM lvi) const;
	void NextStep(bool bAutoStart) const;

	// Event handlers.
	void OnNotify(LPNMHDR nmh, LPARAM lParam) const;
	void OnMessage(WPARAM wParam, LPARAM lParam);

	// Misc.
	UINT GetOverallDuration() const;
	HWND ListHandle() const;
};

#endif // _STEPSTRACKER_H
