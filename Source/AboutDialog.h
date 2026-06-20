/**
 * AboutDialog.h
 * Application's main about dialog window.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _ABOUTDIALOG_H
#define _ABOUTDIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

void ShowAboutDialog(HINSTANCE hInst, HWND hWnd);
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);

#endif // _ABOUTDIALOG_H
