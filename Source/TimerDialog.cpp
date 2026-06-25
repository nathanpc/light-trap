/**
 * TimerDialog.cpp
 * Timer dialog that displays the current step to the user as a child of the
 * main window.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "TimerDialog.h"

#include "StepsTracker.h"

// Internal timer ID.
#define INT_TIMER_ID 1020

// Dialog procedure prototype.
BOOL CALLBACK TimerDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);

/**
 * Constructs the dialog object and inserts it as a child of another window.
 *
 * @param hInst      Application's intance.
 * @param hwndParent Parent window handle.
 * @param rc         Rectangle where the dialog should be placed.
 */
TimerDialog::TimerDialog(HINSTANCE hInst, HWND hwndParent, RECT rc) {
	this->hInst = hInst;
	this->hwndParent = hwndParent;

	// Load and embed the detail view dialog.
	HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(IDD_TIMER), RT_DIALOG);
	HGLOBAL hDialogGlobal = LoadResource(hInst, hRes);
	this->hDlg = CreateDialogIndirectParam(hInst, (LPCDLGTEMPLATE)hDialogGlobal,
		hwndParent, TimerDlgProc, (LPARAM)this);
	SetWindowPos(this->hDlg, HWND_TOP, rc.left, rc.top, rc.right, rc.bottom,
		SWP_SHOWWINDOW);
}

/**
 * Destroys the dialog and all of its resources.
 */
TimerDialog::~TimerDialog() {
	// Ensure we kill the system timer if it's running.
	PauseTimer(false);

	// End the dialog and free all of its temporary resources.
	if (this->hDlg)
		EndDialog(this->hDlg, 0);
	if (this->hfTimer)
		DeleteObject(this->hfTimer);
}

/**
 * Setup UI components and gather references to window handles.
 *
 * @param hDlg Dialog window handle.
 */
void TimerDialog::SetupComponents(HWND hDlg) {
	RECT rcWindow;

	// Get all window handles.
	this->hDlg = hDlg;
	this->lblTimer = GetDlgItem(hDlg, IDC_LBTIMER);
	this->pgbStep = GetDlgItem(hDlg, IDC_PBSTEP);
	this->pgbTotal = GetDlgItem(hDlg, IDC_PBTOTAL);
	this->pgbAgitation = GetDlgItem(hDlg, IDC_PBAGITATE);
	this->btnNext = GetDlgItem(hDlg, IDC_BTNEXT);
	this->btnPlay = GetDlgItem(hDlg, IDC_BTPLAY);

	// Grow the timer label to the top of the dialog window.
	RECT rcTimer;
	GetClientRect(hDlg, &rcWindow);
	// TODO: Move timer window to top 0 of dialog to make it look more natural.

	// Setup timer font.
	LOGFONT lf = { 0 };
	lf.lfHeight = 100;
	lf.lfWeight = FW_BOLD;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = FIXED_PITCH;
	_tcscpy(lf.lfFaceName, _T("Courier New"));
    this->hfTimer = CreateFontIndirect(&lf);
	SendMessage(lblTimer, WM_SETFONT, (WPARAM)this->hfTimer, TRUE);

	// Reset the timer.
	//SetTimer(0, TIMER_DISABLED);
	SetTimer(5, TIMER_RESET);
}

/**
 * Updates the dialog components depending on the current state of the timer.
 *
 * @param bSkipButtons Should we skip updating the buttons?
 */
void TimerDialog::UpdateComponents(bool bSkipButtons) {
	// Update timer label and progress bars.
	TCHAR szTimer[7];
	StepsTracker::DurationToString(szTimer, this->iTimerSeconds);
	SetWindowText(this->lblTimer, szTimer);

	// Skip updating the buttons?
	if (bSkipButtons)
		return;

	// Update the label of the buttons in the dialog.
	switch (this->timerState) {
	case TIMER_DISABLED:
		SetButtonsState(false, _T("Continue"), false, _T("Next"));
		break;
	case TIMER_RESET:
		SetButtonsState(true, _T("Start"), true, _T("Skip"));
		break;
	case TIMER_PAUSED:
		SetButtonsState(true, _T("Continue"), true, _T("Next"));
		break;
	case TIMER_RUNNING:
		SetButtonsState(true, _T("Pause"), false, _T("Next"));
		break;
	case TIMER_FINISHED:
		SetButtonsState(false, _T("Continue"), true, _T("Next"));
		break;
	}
}

/**
 * Updates the dialog components depending on the current state of the timer.
 */
void TimerDialog::UpdateComponents() {
	UpdateComponents(false);
}

/**
 * Resets the state of the timer and sets the time the timer should run for.
 *
 * @param uSeconds Number of seconds the timer should run for.
 * @param tms      State the timer should be in after setting it.
 */
void TimerDialog::SetTimer(UINT uSeconds, TMRSTATE tms) {
	// Set internal state variables.
	this->iTimerSeconds = uSeconds;
	this->timerState = tms;

	// Display changes in the UI.
	UpdateComponents();

	// Start the timer if we were told to hit the ground running.
	if (this->timerState == TIMER_RUNNING)
		StartTimer();
}

/**
 * Starts (unpauses) the internal timer, sets the internal state and updates
 * the UI components.
 */
void TimerDialog::StartTimer() {
	// Start the internal system timer.
	if (!::SetTimer(this->hDlg, INT_TIMER_ID, 1000, NULL)) {
		MsgBoxError(this->hDlg, _T("Timer error"),
			_T("Failed to start internal system timer."));
		MsgBoxLastError(this->hDlg);

		this->timerState = TIMER_DISABLED;
		UpdateComponents();
		return;
	}

	// Set the timer state and update the UI accordingly.
	this->timerState = TIMER_RUNNING;
	UpdateComponents();
}

/**
 * Pauses the internal timer, sets the internal state and updates the UI
 * components.
 *
 * @param bChangeState Should we change the state of the timer to paused?
 */
void TimerDialog::PauseTimer(bool bChangeState) {
	// Kill the internal timer if needed.
	if ((timerState == TIMER_RUNNING) && !KillTimer(hDlg, INT_TIMER_ID)) {
		MsgBoxError(this->hDlg, _T("Timer error"),
			_T("Failed to kill internal system timer."));
		MsgBoxLastError(this->hDlg);
		return;
	}

	if (bChangeState) {
		// Set the timer state and update the UI accordingly.
		this->timerState = TIMER_PAUSED;
		UpdateComponents();
	}
}

/**
 * Handles the tick of the internal system timer and updates the application's
 * UI and state accordingly.
 */
void TimerDialog::TimerTick() {
	// Tick the timer counter.
	this->iTimerSeconds--;

	// Have we finished this step?
	if (this->iTimerSeconds <= 0) {
		PauseTimer(false);
		this->timerState = TIMER_FINISHED;

		UpdateComponents(false);
		return;
	}

	// Update the UI components.
	UpdateComponents(true);
}

/**
 * Handles the click event of the Play/Pause button.
 */
void TimerDialog::OnButtonPlay_Clicked() {
	switch (this->timerState) {
	case TIMER_DISABLED:
	case TIMER_RESET:
	case TIMER_PAUSED:
	case TIMER_FINISHED:
		// Start or Continue
		StartTimer();
		break;
	case TIMER_RUNNING:
		// Pause
		PauseTimer(true);
		break;
	}
}

/**
 * Handles the click event of the Next/Reset button.
 */
void TimerDialog::OnButtonNext_Clicked() {
	switch (this->timerState) {
	case TIMER_DISABLED:
		SetButtonsState(false, _T("Continue"), false, _T("Next"));
		break;
	case TIMER_RESET:
		SetButtonsState(true, _T("Start"), true, _T("Skip"));
		break;
	case TIMER_PAUSED:
		SetButtonsState(true, _T("Continue"), true, _T("Next"));
		break;
	case TIMER_RUNNING:
		SetButtonsState(true, _T("Pause"), false, _T("Next"));
		break;
	case TIMER_FINISHED:
		SetButtonsState(false, _T("Continue"), true, _T("Next"));
		break;
	}
}

/**
 * Sets the text and enabled state of the buttons in the dialog.
 *
 * @param bPlay  Should the Play/Pause button be enabled?
 * @param szPlay Label for the Play/Pause button.
 * @param bNext  Should the Next/Reset button be enabled?
 * @param szNext Label for the Next/Reset button.
 */
void TimerDialog::SetButtonsState(bool bPlay, LPTSTR szPlay, bool bNext,
								  LPTSTR szNext) {
	// Set the button's labels.
	SetWindowText(this->btnPlay, szPlay);
	SetWindowText(this->btnNext, szNext);

	// Enable or disable the buttons.
	EnableWindow(this->btnPlay, bPlay);
	EnableWindow(this->btnNext, bNext);
}

/**
 * Mesage handler for the dialog.
 *
 * @param  hDlg   Dialog window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
BOOL CALLBACK TimerDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	static TimerDialog *pThis;

	switch (wMsg) {
	case WM_INITDIALOG: {
		// Initialize the UI and grab the window handle.
		pThis = (TimerDialog*)lParam;
		pThis->SetupComponents(hDlg);
		return 1;
	}
	case WM_COMMAND:
		// Handle the buttons in the dialog.
		switch (LOWORD(wParam)) {
		case IDC_BTPLAY:
			pThis->OnButtonPlay_Clicked();
			break;
		case IDC_BTNEXT:
			pThis->OnButtonNext_Clicked();
			break;
		}
		break;
	case WM_TIMER:
		// Handle the internal timer tick.
		pThis->TimerTick();
		break;
	}

    return 0;
}
