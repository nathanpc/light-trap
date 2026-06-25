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
 * Timer states.
 */
enum timer_state {
	TIMER_DISABLED = -1,
	TIMER_RESET = 0,
	TIMER_PAUSED,
	TIMER_RUNNING,
	TIMER_FINISHED
};
typedef enum timer_state TMRSTATE;

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

	// Timer state.
	TMRSTATE timerState;
	UINT uTimerSetTotal;
	UINT uTimerSetStep;
	int iTimerStepSeconds;
	int iTimerTotalSeconds;

	// Internal methods.
	void SetButtonsState(bool bPlay, LPTSTR szPlay, bool bNext, LPTSTR szNext);

public:
	// Constructor and destructor.
	TimerDialog(HINSTANCE hInst, HWND hwndParent, RECT rc);
	virtual ~TimerDialog();

	// UI components.
	void SetupComponents(HWND hDlg);
	void UpdateComponents(bool bSkipButtons);
	void UpdateComponents();

	// Timer-related functions.
	void SetProcessTotal(UINT uSeconds);
	void SetStepTimer(UINT uSeconds, TMRSTATE tms);
	void StartTimer();
	void PauseTimer(bool bChangeState);
	void TimerTick();

	// Event handlers.
	void OnButtonPlay_Clicked();
	void OnButtonNext_Clicked();
};

#endif // _TIMERDIALOG_H
