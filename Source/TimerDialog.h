/**
 * TimerDialog.h
 * Timer dialog that displays the current step to the user as a child of the
 * main window.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _TIMERDIALOG_H
#define _TIMERDIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

/**
 * Timer dialog that displays the current step to the user.
 */
class TimerDialog {
private:
	// Dialog and related handles.
	HINSTANCE hInst;
	HWND hwndParent;
	HWND hDlg;
	HFONT hfTimer;

	// Dialog components.
	HWND lblTimer;
	HWND pgbStep;
	HWND pgbTotal;
	HWND pgbAgitation;
	HWND btnPlay;
	HWND btnNext;

public:
	// Constructor and destructor.
	TimerDialog(HINSTANCE hInst, HWND hwndParent, RECT rc);
	virtual ~TimerDialog();

	// UI components.
	void SetupComponents(HWND hDlg);
};

#endif // _TIMERDIALOG_H
