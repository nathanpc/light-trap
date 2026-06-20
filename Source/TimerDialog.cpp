/**
 * TimerDialog.cpp
 * Timer dialog that displays the current step to the user as a child of the
 * main window.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "TimerDialog.h"

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
	EndDialog(this->hDlg, 0);
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
	// TODO: Move timer window to top 0 of dialog since it has importance.

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
		//12,0,0,0,0,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"Microsoft Yahei");//Create font
    SendMessage(lblTimer, WM_SETFONT, (WPARAM)this->hfTimer, TRUE);

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
	switch (wMsg) {
	case WM_INITDIALOG: {
		// Initialize the UI and grab the window handle.
		TimerDialog *pThis = (TimerDialog*)lParam;
		pThis->SetupComponents(hDlg);
		return 1;
	}
	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL)) {
			EndDialog(hDlg, LOWORD(wParam));
			return 1;
		}
		break;
	}

    return 0;
}
