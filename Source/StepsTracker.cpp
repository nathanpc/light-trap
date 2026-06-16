/**
 * StepsTracker.cpp
 * Manages the film development steps and the workflow.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "StepsTracker.h"

#include <commctrl.h>
#include <tchar.h>

/**
 * Initializes the steps tracker.
 */
StepsTracker::StepsTracker() {
}

/**
 * Cleans up everything that was dynamically allocated by us.
 */
StepsTracker::~StepsTracker() {
}

/**
 * Sets up the UI components for the steps tracker.
 *
 * @param hInst      Application's intance.
 * @param hwndParent Parent window handle.
 * @param hwndBar    Application's menu bar to be avoided.
 */
void StepsTracker::SetupComponents(HINSTANCE hInst, HWND hwndParent,
								   HWND hwndBar) {
	RECT rc;

	// Store parent window and instance for later use.
	this->hInst = hInst;
	this->hwndParent = hwndParent;

	// Calculate the ListView control size and position.
	GetClientRect(hwndParent, &rc);
#ifndef SHELL_AYGSHELL
	rc.top += CommandBar_Height(hwndBar);
	rc.bottom -= rc.top;
	rc.right = 220;
#endif // SHELL_AYGSHELL

	// Create the ListView window.
	this->hwndList = CreateWindowEx(0, WC_LISTVIEW, _T(""),
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER |
		LVS_SHAREIMAGELISTS, rc.left, rc.top, rc.right, rc.bottom,
		hwndParent, (HMENU)IDC_STEPSLIST, hInst, NULL);
	if (this->hwndList == NULL) {
		MsgBoxError(hwndParent, _T("UI Error"),
			_T("Could not create ListView with development steps."));
		return;
	}

	// Apply extended attributes to the ListView.
	ListView_SetExtendedListViewStyle(this->hwndList, LVS_EX_FULLROWSELECT |
		LVS_EX_TRACKSELECT);

	// Setup common column properties.
	int iCol = 0;
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_CENTER;

	// Create time label column.
	lvc.iSubItem = iCol;
	lvc.pszText = _T("Time");
	lvc.cx = 50;
	if (ListView_InsertColumn(this->hwndList, iCol++, &lvc) == -1) {
		MsgBoxError(hwndParent, _T("UI Error"),
			_T("Could not insert steps ListView time column."));
		return;
	}

	// Create chemical label column.
	lvc.iSubItem = iCol;
	lvc.pszText = _T("Chemical");
	lvc.cx = 98;
	if (ListView_InsertColumn(this->hwndList, iCol++, &lvc) == -1) {
		MsgBoxError(hwndParent, _T("UI Error"),
			_T("Could not insert steps ListView chemical column."));
		return;
	}

	// Create agitation label column.
	lvc.iSubItem = iCol;
	lvc.pszText = _T("Agitation");
	lvc.cx = 70;
	if (ListView_InsertColumn(this->hwndList, iCol++, &lvc) == -1) {
		MsgBoxError(hwndParent, _T("UI Error"),
			_T("Could not insert steps ListView agitation column."));
		return;
	}

	// Show the steps ListView window.
	ShowWindow(this->hwndList, SW_SHOW);

	// Test items:
	this->AddStep(60, _T("Developer"), true);
	this->AddStep(60, _T("Developer"), false);
	this->AddStep(10, _T("Developer"), true);
	this->AddStep(50, _T("Developer"), false);
	this->AddStep(60, _T("Stop"), true);
	this->AddStep(60, _T("Fixer"), true);
	this->AddStep(60, _T("Fixer"), false);
	this->AddStep(10, _T("Fixer"), true);
	this->AddStep(50, _T("Fixer"), false);
}

/**
 * Adds a step to the step tracker.
 *
 * @param uDuration  Duration of the step.
 * @param szChemical Chemical to be used.
 * @param bAgitate   Should we agitate during this step?
 */
void StepsTracker::AddStep(UINT uDuration, LPTSTR szChemical, bool bAgitate) {
	LVITEM lvi;
	TCHAR szTime[8];

	// Populate ListView item structure.
	this->DurationToString(szTime, uDuration);
	lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
	lvi.state = 0; 
	lvi.stateMask = 0; 
   	lvi.iItem = ListView_GetItemCount(this->hwndList);
	lvi.iSubItem = 0;
	lvi.lParam = (LPARAM)uDuration;
	lvi.pszText = szTime;

	// Insert the item into the ListView.
	if (ListView_InsertItem(this->hwndList, &lvi) == -1) {
		MsgBoxError(this->hwndParent, _T("Steps list error"),
			_T("An error occurred while trying to add a step to the ListView"));
	}

	// Populate and set the chemical sub-item.
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = szChemical;
	if (!ListView_SetItem(this->hwndList, &lvi)) {
		MsgBoxError(this->hwndParent, _T("Steps list error"),
			_T("An error occurred while trying to set the chemical of a step"));
	}

	// Populate and set the agitation sub-item.
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 2;
	lvi.pszText = bAgitate ? _T("Agitate") : _T("Stand");
	if (!ListView_SetItem(this->hwndList, &lvi)) {
		MsgBoxError(this->hwndParent, _T("Steps list error"),
			_T("An error occurred while trying to set the agitation of a step"));
	}

}

/**
 * Converts a duration in seconds to a formatted string in the MM:SS format.
 *
 * @param uDuration Duration of a step in seconds.
 * @param szBuffer  Buffer long enough to contain the formatted string.
 *
 * @return Length of the formatted string, not including the NUL terminator.
 */
int StepsTracker::DurationToString(LPTSTR szBuffer, UINT uDuration) {
	USHORT usSeconds;
	USHORT usMinutes;

	// Calculate minutes and seconds individually.
	usSeconds = uDuration % 60;
	usMinutes = (USHORT)((uDuration - usSeconds) / 60);

	// Write the formatted string to the buffer.
	return _stprintf(szBuffer, _T("%02u:%02u"), usMinutes, usSeconds);
}
