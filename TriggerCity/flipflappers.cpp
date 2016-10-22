#include "cocona.h"
#include <process.h>

// Typedef's for function referencing (like PHP includes or external CSS files)
void MainScan(ScanContents scan);
void CheckColor(COLORREF pixel, int x, int y);
void ScanThread(ScanContents * scan);
bool ColorMatch(COLORREF pixel);

// Variables
std::string chosenColor;

int main() {
	// Prompts user to input their desired game
	std::string gameWindow;
	std::cout << "Anime: Flip Flappers! (2016)" << std::endl;
	std::cout << "Enter you're favorite anime: " << std::endl;
	std::getline(std::cin, gameWindow);

	// Attempts to find the user's desired game
	HWND appWindow = FindWindow(0, gameWindow.c_str());
	RECT rcClientPositioning;

	while (!appWindow) {
		system("CLS");
		appWindow = FindWindow(0, gameWindow.c_str());
		std::cout << "Looking for " << gameWindow << std::endl;
		Sleep(3000);
	}
	std::cout << "Found " << gameWindow << std::endl;

	// After finding the user's game,
	// Prompts user to choose a color and stores it in a color variable
	while (atoi(chosenColor.c_str()) < 1 || atoi(chosenColor.c_str()) > 6) {
		system("CLS");
		std::cout << "Choose which color to trigger against\n" << "1. Red\n2. Green\n3. Blue" << std::endl;
		std::getline(std::cin, chosenColor);
	}

	std::string color;
	if		(chosenColor == "1")		color = "Red";
	else if (chosenColor == "2")		color = "Green";
	else if (chosenColor == "3")		color = "Blue";
	system("CLS");

	std::cout << "Status: Enabled\n" << "You Selected: " << color << std::endl;

	// Retrieves the dimensions of the bounding rectangle of the specified window
	GetWindowRect(appWindow, &rcClientPositioning);

	// Obtain our desktop's screen
	HDC hdcMain		= GetDC(HWND_DESKTOP);
	HDC hdcThread1	= GetDC(HWND_DESKTOP);
	HDC hdcThread2	= GetDC(HWND_DESKTOP);
	HDC hdcThread3	= GetDC(HWND_DESKTOP);

	// Starting scan inside crosshair square
	int startingX = rcClientPositioning.right - ((rcClientPositioning.right - rcClientPositioning.left) / 2);
	int startingY = rcClientPositioning.bottom - ((rcClientPositioning.bottom - rcClientPositioning.top) / 2);

	// The box on our crosshair
	ScanContents scan			(startingX, startingY, hdcMain, 0, 0, -40, -41, 0);
	ScanContents scanContents1	(startingX, startingY, hdcMain, 0, 40, -40, 1, 1);
	ScanContents scanContents2	(startingX, startingY, hdcMain, 42, 0, 2, -41, 2);
	ScanContents scanContents3	(startingX, startingY, hdcMain, 42, 40, 2, 1, 3);

	_beginthread((void(*)(void*))ScanThread, 0, (void*)&scanContents1);
	_beginthread((void(*)(void*))ScanThread, 0, (void*)&scanContents2);
	_beginthread((void(*)(void*))ScanThread, 0, (void*)&scanContents3);

	// Init Scan
	MainScan(scan);

	system("PAUSE");
}

void ScanThread(ScanContents * scan) {
	//int debugRuntime = clock();

	// The matrix
	while (true) {
		// Scan up and down the screen
		for (int y = scan -> StartY + scan-> DeductY; y < scan -> StartY + scan -> CompareY; y += 3) {
			// Scan accross the screen
			for (int x = scan -> StartX + scan -> DeductX; x < scan -> StartX + scan -> CompareX; x += 3) {
				// Debugging Purposes
				/* SetCursorPos(x, y);
				mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
				ladiesman++; */

				// CheckColor function
				CheckColor(GetPixel(scan -> Hdc, x, y), x, y);
			}
		}
		//std::cout << "[MAIN THREAD]Took  " << clock() - debugRuntime << " milliseconds to scan whole page" << std::endl;
	}
	_endthread();
}

void MainScan(ScanContents scan) {
	//int debugRuntime = clock();

	// The matrix
	while (true) {
		// Scan up and down the screen
		for (int y = scan.StartY + scan.DeductY; y < scan.StartY + scan.CompareY; y += 3) {
			// Scan accross the screen
			for (int x = scan.StartX + scan.DeductX; x < scan.StartX + scan.CompareX; x += 3) {
				// Debugging Purposes
				/* SetCursorPos(x, y);
				mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
				ladiesman++; */

				// CheckColor function
				CheckColor(GetPixel(scan.Hdc, x, y), x, y);
				
				// If the DELETE key is hit, exit
				if (GetAsyncKeyState(VK_DELETE)) {
					exit(0);
				}
			}
		}
		//std::cout << "[MAIN THREAD]Took  " << clock() - debugRuntime << " milliseconds to scan whole page" << std::endl;
	}
}

void CheckColor(COLORREF pixel, int x, int y) {
	// If the ColorMatch() function returns true
	// This function will shoot
	if (ColorMatch(pixel)) {
		int rep = 0;
		while (rep < 5) {
			mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
			Sleep(100);
			rep++;
		}
	}
}

bool ColorMatch(COLORREF pixel) {
	// Obtaining our RGB values
	int r = GetRValue(pixel);
	int g = GetGValue(pixel);
	int b = GetBValue(pixel);

	// Red
	if (chosenColor == "1") {
		if (r > 50 && g < 40 && b < 40) {
			return true;
		}
	}

	// Green
	else if (chosenColor == "2") {
		if (r < 50 && g > 40 && b < 40) {
			return true;
		}
	}

	// Blue
	else if (chosenColor == "3") {
		if (r < 40 && g < 40 && b > 50) {
			return true;
		}
	}

	return false;
}