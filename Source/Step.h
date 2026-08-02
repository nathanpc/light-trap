/**
 * Step.h
 * Defines a single step in the development process.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _STEP_H
#define _STEP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

/**
 * Defines a single step in the development process.
 */
class Step {
private:
	UINT uDuration;
	TCHAR szTime[8];
	LPTSTR szChemical;
	bool bAgitate;
	bool bAutoMoveNext;

public:
	Step(UINT uDuration, LPCTSTR szChemical, bool bAgitate,
		 bool bAutoMoveNext) {
		this->uDuration = uDuration;
		DurationToString(this->szTime, uDuration);
		this->szChemical = _tcsdup(szChemical);
		this->bAgitate = bAgitate;
		this->bAutoMoveNext = bAutoMoveNext;
	};

	virtual ~Step() {
		if (this->szChemical)
			free(this->szChemical);
		this->szChemical = NULL;
		this->uDuration = 0;
		this->bAgitate = false;
		this->bAutoMoveNext = false;
	};

	/**
	 * Converts a duration in seconds to a formatted string in the MM:SS format.
	 *
	 * @param uDuration Duration of a step in seconds.
	 * @param szBuffer  Buffer long enough to contain the formatted string.
	 *
	 * @return Length of the formatted string, not including the NUL terminator.
	 */
	static int DurationToString(LPTSTR szBuffer, UINT uDuration) {
		USHORT usSeconds;
		USHORT usMinutes;

		// Calculate minutes and seconds individually.
		usSeconds = uDuration % 60;
		usMinutes = (USHORT)((uDuration - usSeconds) / 60);

		// Write the formatted string to the buffer.
		return _stprintf(szBuffer, _T("%02u:%02u"), usMinutes, usSeconds);
	}

	// Getters
	UINT Duration() const { return this->uDuration; };
	LPCTSTR TimeString() const { return this->szTime; };
	LPCTSTR Chemical() const { return (LPCTSTR)this->szChemical; };
	bool Agitate() const { return this->bAgitate; };
	bool AutoMoveToNext() const { return this->bAutoMoveNext; };
};

#endif // _STEP_H
