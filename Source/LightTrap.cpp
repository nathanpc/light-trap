/**
 * LightTrap.cpp
 * A companion application to use while developing photographic film.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "LightTrap.h"

#include <windowsx.h>
#include <commctrl.h>

#include "StepsTracker.h"
#include "AboutDialog.h"

// Global variables.
static HINSTANCE hInst;
static TCHAR szWindowClass[20];
static TCHAR szAppTitle[20];
static StepsTracker *stepsTracker = NULL;

#ifdef SHELL_AYGSHELL
// Pocket PC specific components.
static HWND hwndMenuBar;
static SHACTIVATEINFO sai;
#else
// Handheld PC specific components.
static HWND hwndCB;
#endif

/**
 * Application's main entry point.
 *
 * @param hInstance     Program instance.
 * @param hPrevInstance Ignored: Leftover from Win16.
 * @param lpCmdLine     String with command line text.
 * @param nShowCmd      Initial state of the program's main window.
 *
 * @return wParam of the WM_QUIT message.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPWSTR lpCmdLine, int nShowCmd) {
	MSG msg;
	HACCEL hAccel;
	int rc;

	// Initialize the application.
	hInst = hInstance;
	rc = RegisterApplicationClass(hInstance);
	if (rc)
		return 0;

	// Initialize this single instance.
	if (!InitializeInstance(hInstance, lpCmdLine, nShowCmd))
		return 0x10;

	// Load accelerators.
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ACCELERATORS));

	// Application message loop.
	while (GetMessage(&msg, NULL, 0, 0)) {
		// Translate accelerators.
		if (!TranslateAccelerator(msg.hwnd, hAccel, &msg)) {
			// Translate message.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Terminate instance.
	rc = TerminateInstance(hInstance, (int)msg.wParam);

	return rc;
}

/**
 * Initializes the application and registers the application class.
 *
 * @param hInstance Application instance.
 *
 * @return TRUE if the class was registered.
 */
int RegisterApplicationClass(HINSTANCE hInstance) {
	WNDCLASS wc;

	// Load the application class and title.
	LoadString(hInstance, IDC_LIGHTTRAP, szWindowClass, 20);
	LoadString(hInst, IDS_APP_TITLE, szAppTitle, 20);
	
#ifdef SHELL_AYGSHELL
	// Only allow one instance of the application.
	HWND hWnd = FindWindow(szWindowClass, NULL);
	if (hWnd) {
		SetForegroundWindow((HWND)(((DWORD)hWnd) | 0x01));
		return 1;
	}
#endif

	// Register the application's main window class.
	wc.style         = CS_VREDRAW | CS_HREDRAW;   // Window style.
	wc.lpfnWndProc   = MainWindowProc;            // Main window procedure.
	wc.cbClsExtra    = 0;                         // Extra class data.
	wc.cbWndExtra    = 0;                         // Extra window data.
	wc.hInstance     = hInstance;                 // Owner handle.
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
	wc.hCursor       = NULL;                      // Default cursor. (Always NULL)
	wc.hbrBackground = GetSysColorBrush(COLOR_STATIC);
	wc.lpszMenuName  = NULL;                      // Menu name. (Always NULL)
	wc.lpszClassName = szWindowClass;             // Window class name.

	// Check if the class registration worked.
	if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Window Registration Failed!", L"Error",
			MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

	return 0;
}

/**
 * Initializes the application's instance and creates the window.
 *
 * @param hInstance Program instance.
 * @param lpCmdLine String with command line text.
 * @param nShowCmd  Initial state of the program's main window.
 *
 * @return Window handler.
 */
HWND InitializeInstance(HINSTANCE hInstance, LPTSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;

#ifdef SHELL_AYGSHELL
	// Initialize PocketPC controls.
	SHInitExtraControls();
#endif

	// Create the main window.
	hWnd = CreateWindow(szWindowClass,	// Window class.
						szAppTitle,		// Window title.
						WS_VISIBLE, 	// Style flags.
						CW_USEDEFAULT,	// X position.
						CW_USEDEFAULT,	// Y position.
						CW_USEDEFAULT,	// Initial width,
						CW_USEDEFAULT,	// Initial height.
						NULL,			// Parent window.
						NULL,			// Menu class. (Always NULL)
						hInstance,		// Application instance.
						NULL);			// Pointer to create parameters.

	// Check if the window creation worked.
	if (!IsWindow(hWnd)) {
		MsgBoxError(NULL, _T("Error Initializing Instance"),
			_T("Window creation failed."));
		return NULL;
	}

#ifdef SHELL_AYGSHELL
	// Take menu bar height into account.
	RECT rcMB;
	GetWindowRect(hWnd, &rcMB);
	rcMB.bottom -= 26;
	MoveWindow(hWnd, rcMB.left, rcMB.top, rcMB.right, rcMB.bottom, FALSE);
#endif // SHELL_AYGSHELL

	// Set the window task switching icon.
	HANDLE hIcon = LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON),
		IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON),
		0);
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	// Set window taskbar icon.
	hIcon = LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	// Show and update the window.
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

/**
 * Terminates the application instance.
 *
 * @param hInstance Application instance.
 * @param nDefRC    Return code.
 *
 * @return Previous return code.
 */
int TerminateInstance(HINSTANCE hInstance, int nDefRC) {
	return nDefRC;
}

/**
 * Main window procedure.
 *
 * @param hWnd   Window handler.
 * @param wMsg   Message type.
 * @param wParam Message parameter.
 * @param lParam Message parameter.
 *
 * @return 0 if everything worked.
 */
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	switch (wMsg) {
	case WM_CREATE:
		return WndMainCreate(hWnd, wMsg, wParam, lParam);
	case WM_COMMAND:
		return WndMainCommand(hWnd, wMsg, wParam, lParam);
	case WM_NOTIFY:
		return WndMainNotify(hWnd, wMsg, wParam, lParam);
	case WM_SETTINGCHANGE:
		return WndMainSettingChange(hWnd, wMsg, wParam, lParam);
	case WM_ACTIVATE:
		return WndMainActivate(hWnd, wMsg, wParam, lParam);
	case WM_CLOSE:
		return WndMainClose(hWnd, wMsg, wParam, lParam);
	case WM_DESTROY:
		return WndMainDestroy(hWnd, wMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

/**
 * Process the WM_CREATE message for the window.
 *
 * @param hWnd   Window handler.
 * @param wMsg   Message type.
 * @param wParam Message parameter.
 * @param lParam Message parameter.
 *
 * @return 0 if everything worked.
 */
LRESULT WndMainCreate(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	// Ensure that the common controls DLL is loaded. 
    InitCommonControls();

	// Initialize auxiliary parts of the application.
	stepsTracker = new StepsTracker();

#ifdef SHELL_AYGSHELL
	SHMENUBARINFO mbi = {0};
	SIPINFO si = {0};
	int cx, cy;

	// Initialize the shell to activate the info structure.
	memset(&sai, 0, sizeof(sai));
	sai.cbSize = sizeof(sai);

	// Setup the menu bar.
	mbi.cbSize = sizeof(SHMENUBARINFO);  // Size field.
	mbi.hwndParent = hWnd;               // Parent window.
	mbi.nToolBarId = IDR_MENUBAR;        // ID of the toolbar resource.
	mbi.hInstRes = hInst;                // Instance handle of our application.
	mbi.nBmpId = 0;                      // Bitmap resource ID.
	mbi.cBmpImages = 0;                  // Number of images in the bitmap.
	mbi.hwndMB = 0;                      // Returned handle of the menu bar.
	
	// Create the menu bar.
	if (!SHCreateMenuBar(&mbi)) {
		MsgBoxError(hWnd, _T("UI Error"), _T("Couldn't create the menu bar."));
		DestroyWindow(hWnd);
	}

	// Save the menu bar handle.
	hwndMenuBar = mbi.hwndMB;

	// Query the SIP state and size our window appropriately.
	si.cbSize = sizeof(si);
	SHSipInfo(SPI_GETSIPINFO, 0, (PVOID)&si, 0);
	cx = si.rcVisibleDesktop.right - si.rcVisibleDesktop.left;
	cy = si.rcVisibleDesktop.bottom - si.rcVisibleDesktop.top;

	// Correct the window height based on the menu bar height.
	if (!(si.fdwFlags & SIPF_ON) || ((si.fdwFlags & SIPF_ON) && (si.fdwFlags & SIPF_DOCKED))) {
		RECT rcMenuBar;
		GetWindowRect(hwndMenuBar, &rcMenuBar);

		cy -= (rcMenuBar.bottom - rcMenuBar.top);
	}

	// Resize our window appropriately.
	SetWindowPos(hWnd, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);

	// Setup steps tracker component.
	stepsTracker->SetupComponents(hInst, hWnd, hwndMenuBar);
#else
	// Create CommandBar.
	hwndCB = CommandBar_Create(hInst, hWnd, IDC_CMDBAR);
	if (!hwndCB)
		MsgBoxError(hWnd, _T("UI Error"), _T("CommandBar creation failed."));
	
	/*
    // Add the Standard and View bitmaps to the toolbar.
    CommandBar_AddBitmap(hwndCB, HINST_COMMCTRL, IDB_STD_SMALL_COLOR,
		STD_BMPS_LEN, 16, 16);
    CommandBar_AddBitmap(hwndCB, HINST_COMMCTRL, IDB_VIEW_SMALL_COLOR,
		VIEW_BMPS_LEN, 16, 16);
	*/

	// Insert menu bar, toolbar buttons, and the exit button.
	CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
    //CommandBar_AddButtons(hwndCB, sizeof(tbButtons) / sizeof(TBBUTTON),
	//	tbButtons);
	CommandBar_AddAdornments(hwndCB, 0, 0);
	CommandBar_Show(hwndCB, TRUE);

	// Setup steps tracker component.
	stepsTracker->SetupComponents(hInst, hWnd, hwndCB);
#endif

	return 0;
}

/**
 * Process the WM_COMMAND message for the window.
 *
 * @param hWnd   Window handler.
 * @param wMsg   Message type.
 * @param wParam Message parameter.
 * @param lParam Message parameter.
 *
 * @return 0 if everything worked.
 */
LRESULT WndMainCommand(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	switch (GET_WM_COMMAND_ID(wParam, lParam)) {
	case IDM_FILE_EXIT:
		PostMessage(hWnd, WM_CLOSE, (WPARAM)0, (LPARAM)0);
		break;
	case IDM_HELP_ABOUT:
		ShowAboutDialog(hInst, hWnd);
		return 0;
	}

	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

/**
 * Process the WM_NOTIFY message for the window.
 *
 * @param hWnd   Window handler.
 * @param wMsg   Message type.
 * @param wParam Identifier of the common control sending the message. Not
 *               always unique. hwndFrom or idFrom of the NMHDR should be used.
 * @param lParam Pointer to an NMHDR structure containing the notification code
 *               and additional information.
 *
 * @return 0 if everything worked.
 */
LRESULT WndMainNotify(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	LPNMHDR nmh = (LPNMHDR)lParam;
	switch (nmh->code) {
	default:
		return DefWindowProc(hWnd, wMsg, wParam, lParam);
	}
}

/**
 * Process the WM_SETTINGCHANGE message for the window.
 *
 * @param hWnd   Window handler.
 * @param wMsg   Message type.
 * @param wParam Message parameter.
 * @param lParam Message parameter.
 *
 * @return 0 if everything worked.
 */
LRESULT WndMainSettingChange(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
#ifdef SHELL_AYGSHELL
	// Notify shell of our setting change message.
	SHHandleWMSettingChange(hWnd, wParam, lParam, &sai);
#endif

	return 0;
}

/**
 * Process the WM_ACTIVATE message for the window.
 *
 * @param hWnd   Window handler.
 * @param wMsg   Message type.
 * @param wParam Message parameter.
 * @param lParam Message parameter.
 *
 * @return 0 if everything worked.
 */
LRESULT WndMainActivate(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
#ifdef SHELL_AYGSHELL
	// Notify shell of our activate message.
	SHHandleWMActivate(hWnd, wParam, lParam, &sai, 0);
#endif

	return 0;
}

/**
 * Process the WM_CLOSE message for the window.
 *
 * @param hWnd   Window handler.
 * @param wMsg   Message type.
 * @param wParam This parameter is not used.
 * @param lParam This parameter is not used.
 *
 * @return 0 if everything worked.
 */
LRESULT WndMainClose(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	// TODO: Check if we can close.
	// if () return 0

	// Call any destructors that might be needed.
	if (stepsTracker) {
		delete stepsTracker;
		stepsTracker = NULL;
	}

	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

/**
 * Process the WM_DESTROY message for the window.
 *
 * @param hWnd   Window handler.
 * @param wMsg   Message type.
 * @param wParam This parameter is not used.
 * @param lParam This parameter is not used.
 *
 * @return 0 if everything worked.
 */
LRESULT WndMainDestroy(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
/*
#ifdef SHELL_AYGSHELL
	CommandBar_Destroy(hwndMenuBar);
#endif // SHELL_AYGSHELL
*/

	// Post quit message and return.
	PostQuitMessage(0);
	return 0;
}
