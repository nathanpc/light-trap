/**
 * LightTrap.h
 * A companion application to use while developing photographic film.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _LIGHTTRAP_H
#define _LIGHTTRAP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Limits.
#define MAX_LOADSTRING 100

// Control IDs.
#define IDC_CMDBAR   201

// Main includes.
#include "stdafx.h"

// Application initialization.
int RegisterApplicationClass(HINSTANCE hInstance);
HWND InitializeInstance(HINSTANCE hInstance, LPTSTR lpCmdLine, int nCmdShow);
int TerminateInstance(HINSTANCE hInstance, int nDefRC);

// Window message handlers.
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainCreate(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainCommand(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainNotify(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainSettingChange(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainActivate(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainClose(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainDestroy(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);

#endif // _LIGHTTRAP_H
