#include "CModGame.h"

#include <iostream>
#include <string>
#include <thread>

std::unique_ptr<std::thread> p_GameThread;
std::unique_ptr<CModGame> p_ModGame;

void gameLoop()
{
	uint32_t DELAY_MS = 1000;

	while (1) {
		Sleep(DELAY_MS);

		if (p_ModGame->IsGameRunning()) {

			//std::cout << "[MAIN] Game is running" << "\n";
			if (!p_ModGame->IsGamePatched()) {
				p_ModGame->Patch();
			}

		} else {

			std::cout << "[MAIN] Game is not running" << "\n";
			p_ModGame->RetryConnection();

		}
	}
}

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

int main(int argc, char* argv[])
{
	p_ModGame = std::unique_ptr<CModGame>(new CModGame(L"Nioh 1.24.06", L"nioh.exe"));

	std::thread t_gameLoop(gameLoop);

	while (1) {
		std::cout << ("GOD MODE: ");

		if (p_ModGame->IsGodModeActive()) {
			std::cout << "ON";
		} else {
			std::cout << "OFF";
		}
		std::cout << "\n";

		std::cout << ("INFINITE KI MODE: ");

		if (p_ModGame->IsInfiniteKiModeActive()) {
			std::cout << "ON";
		} else {
			std::cout << "OFF";
		}
		std::cout << "\n";

		char option;
		std::cin >> option;

		if (!p_ModGame->IsGameRunning()) {
			continue;
		}

		switch (option) {
		case '1':
			p_ModGame->ToggleGodMode();
			break;
		case '2':
			p_ModGame->ToggleInfiniteKiMode();
			break;
		default:
			break;
		}
	}
}