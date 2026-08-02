/**
 * Messages.h
 * Custom window messages implemented throughout the application.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _APP_MESSAGES_H
#define _APP_MESSAGES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

/**
 * Messages destined to be consumed by the StepsTracker class.
 *
 * WPARAM: ST_NEXT
 * LPARAM: Ignored.
 */
#define WM_STEPSTRACKER (WM_APP + 0x0A01)

/**
 * Jump to the next step. Sent as the WPARAM of WM_STEPSTRACKER.
 */
#define ST_NEXT (WPARAM)(1)

#endif // _APP_MESSAGES_H
