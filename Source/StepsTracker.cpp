/**
 * StepsTracker.cpp
 * Manages the film development steps and the workflow.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "StepsTracker.h"

#include <commctrl.h>
#include <tchar.h>

#include "Step.h"

// Private variables.
static TCHAR szAgitate[8];
static TCHAR szStand[6];
static LONG lAutoStart;

/**
 * Initializes the steps tracker.
 */
StepsTracker::StepsTracker() {
	_tcscpy(szAgitate, _T("Agitate"));
	_tcscpy(szStand, _T("Stand"));
	lAutoStart = 123;
}

/**
 * Cleans up everything that was dynamically allocated by us.
 */
StepsTracker::~StepsTracker() {
}

/**
 * Sets the timer dialog internally to be called from event handlers.
 *
 * @param timer TimerDialog object.
 */
void StepsTracker::SetTimerDialog(TimerDialog *timer) {
	this->timer = timer;

	// DELETEME:
	timer->SetProcessTotal(GetOverallDuration());
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
		LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
		rc.left, rc.top, rc.right, rc.bottom,
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
	this->AddStep(60, _T("Developer"), true, true);
	this->AddStep(60, _T("Developer"), false, true);
	this->AddStep(10, _T("Developer"), true, true);
	this->AddStep(50, _T("Developer"), false, false);
	this->AddStep(60, _T("Stop"), true, false);
	this->AddStep(60, _T("Fixer"), true, true);
	this->AddStep(60, _T("Fixer"), false, true);
	this->AddStep(10, _T("Fixer"), true, true);
	this->AddStep(50, _T("Fixer"), false, false);
}

/**
 * Adds a step to the step tracker.
 *
 * @param uDuration  Duration of the step.
 * @param szChemical Chemical to be used.
 * @param bAgitate   Should we agitate during this step?
 * @param bAutoNext  Should automatically move to the next step when finished?
 */
void StepsTracker::AddStep(UINT uDuration, LPTSTR szChemical, bool bAgitate,
						   bool bAutoNext) const {
	LVITEM lvi;
	Step *step = new Step(uDuration, szChemical, bAgitate, bAutoNext);

	// Populate ListView item structure.
	lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE | LVIF_NORECOMPUTE;
	lvi.state = 0; 
	lvi.stateMask = 0; 
   	lvi.iItem = ListView_GetItemCount(this->hwndList);
	lvi.iSubItem = 0;
	lvi.lParam = (LPARAM)step;
	lvi.pszText = LPSTR_TEXTCALLBACK;

	// Insert the item into the ListView.
	if (ListView_InsertItem(this->hwndList, &lvi) == -1) {
		MsgBoxError(this->hwndParent, _T("Steps list error"),
			_T("An error occurred while trying to add a step to the ListView"));
	}

	// Populate and set the chemical sub-item.
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = LPSTR_TEXTCALLBACK;
	if (!ListView_SetItem(this->hwndList, &lvi)) {
		MsgBoxError(this->hwndParent, _T("Steps list error"),
			_T("An error occurred while trying to set the chemical of a step"));
	}

	// Populate and set the agitation sub-item.
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 2;
	lvi.pszText = LPSTR_TEXTCALLBACK;
	if (!ListView_SetItem(this->hwndList, &lvi)) {
		MsgBoxError(this->hwndParent, _T("Steps list error"),
			_T("An error occurred while trying to set the agitation of a step"));
	}

}

/**
 * Gets the index of the selected item from the ListView.
 *
 * @return Index of the selected item or -1 if there are none selected.
 */
int StepsTracker::GetSelectedItemIndex() const {
	return ListView_GetNextItem(this->hwndList, -1, LVNI_SELECTED);
}

/**
 * Get the selected list item object from the ListView.
 *
 * @param lvi Pointer to a ListView item structure with the flags field already
 *            populated.
 *
 * @return TRUE if the operation was successful, FALSE if no item is selected.
 */
bool StepsTracker::GetSelectedItem(LPLVITEM lvi) const {
	// Get the selected item.
	int iItem = GetSelectedItemIndex();
	if (iItem == -1)
		return false;

	lvi->iItem = iItem;
	return ListView_GetItem(this->hwndList, lvi) != 0;
}

/**
 * Goes to the next step in the ListView and notifies the timer of the change.
 *
 * @param bAutoStart Automatically start the timer for the step.
 */
void StepsTracker::NextStep(bool bAutoStart) const {
	// Get the inde of the next selected item.
	int iItem = GetSelectedItemIndex();
	if (iItem == -1)
		return;
	iItem++;

	// Check if the next item exists.
	if (iItem >= ListView_GetItemCount(this->hwndList)) {
		MsgBoxError(this->hwndParent, _T("No next step"),
			_T("You have reached the end of the steps list. Congratulations!"));
		return;
	}

	// Select the next item.
	ListView_SetItemState(this->hwndList, iItem, LVIS_SELECTED, LVIS_SELECTED);
	NMHDR nmh = { 0 };
	nmh.code = LVN_ITEMACTIVATE;
	OnNotify(&nmh, bAutoStart ? lAutoStart : NULL);
}

/**
 * Handles WM_NOTIFY messages sent to the ListView control.
 *
 * @param nmh    Structure that contains information about the notification.
 * @param lParam Pointer to an NMHDR structure containing the notification code
 *               and additional information.
 */
void StepsTracker::OnNotify(LPNMHDR nmh, LPARAM lParam) const {
	if (nmh->code == LVN_ITEMACTIVATE) {
		LVITEM lvi = { 0 };

		// Get the selected item.
		lvi.mask = LVIF_PARAM | LVIF_TEXT;
		if (!GetSelectedItem(&lvi))
			return;

		// Get the duration and pass it along to the timer dialog.
		this->timer->SetStepTimer((Step *)lvi.lParam,
			(lParam == lAutoStart) ? TIMER_RUNNING : TIMER_RESET);
	} else if (nmh->code == LVN_GETDISPINFO) {
		NMLVDISPINFO *plvdi = (NMLVDISPINFO *)lParam;
		Step *step = (Step *)plvdi->item.lParam;

		switch (plvdi->item.iSubItem) {
		case 0:
			plvdi->item.pszText = (LPTSTR)step->TimeString();
			break;
		case 1:
			plvdi->item.pszText = (LPTSTR)step->Chemical();
			break;
		case 2:
			plvdi->item.pszText = step->Agitate() ? szAgitate : szStand;
			break;
		}
	}
}

/**
 * Handles WM_STEPSTRACKER messages sent to the main application window.
 *
 * @param wParam Type of action to be taken.
 * @param lParam Associated parameter of the action.
 */
void StepsTracker::OnMessage(WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case ST_NEXT:
		NextStep(lParam ? true : false);
		break;
	default:
		MsgBoxWarning(this->hwndParent, _T("Unknown message"),
			_T("StepsTracker received an unknown message."));
	}
}

/**
 * Gets the duration of the entire process, summing up all the steps in the
 * list.
 *
 * @return Duration of the entire process.
 */
UINT StepsTracker::GetOverallDuration() const {
	UINT uDuration = 0;

	// Prepare the ListView item structure.
	LVITEM lvi = { 0 };
	lvi.mask = LVIF_PARAM | LVIF_TEXT;

	// Go through each item summing up all the durations.
	int iIndex = -1;
	while ((iIndex = ListView_GetNextItem(this->hwndList, iIndex, LVNI_ALL)) >= 0) {
		lvi.iItem = iIndex;
		if (ListView_GetItem(this->hwndList, &lvi))
			uDuration += (UINT)lvi.lParam;
	}

	return uDuration;
}

/**
 * Gets the ListView window handle used to track the steps.
 *
 * @return ListView window handle.
 */
HWND StepsTracker::ListHandle() const {
	return this->hwndList;
}
