#include <Windows.h>
#include <iostream>
#include <ctime>
#include <string>

class ScanContents {
public:
	int StartX;
	int StartY;

	// Values to pinpoint which quarter to scan
	int DeductX;
	int DeductY;
	int CompareX;
	int CompareY;

	// Thread Number
	int ThreadNum;

	// Grab our object from the window
	HDC Hdc;

	ScanContents(
		int startX, int startY, HDC hdc,
		int compareX = 0, int compareY = 0,
		int deductX = 0, int deductY = 0,
		int threadNum = 0)
	{
		StartX = startX;
		StartY = startY;
		CompareX = compareX;
		CompareY = compareY;
		DeductX = deductX;
		DeductY = deductY;
		ThreadNum = threadNum;
		Hdc = hdc;
	}	
};