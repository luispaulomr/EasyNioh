#include "CModGame.h"

#include <iomanip>

CModGame::CModGame(const std::wstring& str_window_name, const std::wstring& str_exe_name)
{
	InitializeGameData();

	auto handle_process = std::make_unique<CHandleProcess>(str_window_name, str_exe_name);

	m_pHandleProcess = std::move(handle_process);

	if (!m_pHandleProcess) {
		std::cout << "[CModGame::CModGame] ERROR: Failed to create CModGame::CModGame object." << "\n";
		return;
	}

	if (!m_pHandleProcess->AttachToProcess()) {
		std::cout << "[CModGame::CModGame] ERROR: Call to CHandleProcess::AttachToProcess failed." << "\n";
		return;
	}
}

bool CModGame::RetryConnection()
{
	gamePatched = false;

	if (!m_pHandleProcess) {
		std::cout << "[CModGame::RetryConnection] ERROR: Trying to access an invalid pointer.." << "\n";
		return false;
	}

	if (!m_pHandleProcess->AttachToProcess()) {
		std::cout << "[CModGame::RetryConnection] ERROR: Call to CHandleProcess::AttachToProcess failed." << "\n";
		return false;
	}

	return true;
}

bool CModGame::IsGameRunning() const
{
	if (!m_pHandleProcess) {
		std::cout << "[CModGame::IsGameRunning] ERROR: Trying to access an invalid pointer." << "\n";
		return false;
	}

	return m_pHandleProcess->IsProcessRunning();
}

void CModGame::InitializeGameData()
{
	m_gameData.set_AOB(DATA_INDEX::MY_HEALTH, AOB_MY_HEALTH);
	m_gameData.set_AOB(DATA_INDEX::MY_KI, AOB_MY_KI);
}

bool CModGame::IsGodModeActive() const
{
	return godModeActive;
}

bool CModGame::IsInfiniteKiModeActive() const
{
	return infiniteKiModeActive;
}

void CModGame::ToggleGodMode()
{
	if (godModeActive) {
		if (deactivateGodMode()) {
			godModeActive = false;
		}
	} else {
		if (activateGodMode()) {
			godModeActive = true;
		}
	}
}

void CModGame::ToggleInfiniteKiMode()
{
	if (infiniteKiModeActive) {
		if (deactivateInfiniteKiMode()) {
			infiniteKiModeActive = false;
		}
	} else {
		if (activateInfiniteKiMode()) {
			infiniteKiModeActive = true;
		}
	}
}

bool CModGame::deactivateGodMode() const
{
	uintptr_t addr_aob = m_gameData.get_AOB_ADDR(DATA_INDEX::MY_HEALTH);

	if (!addr_aob) {

		return false;

	}

	m_pHandleProcess->WriteMemory(addr_aob, DEACTIVATE_GOD_MODE);

	return true;
}

bool CModGame::activateGodMode() const
{
	uintptr_t addr_aob = m_gameData.get_AOB_ADDR(DATA_INDEX::MY_HEALTH);

	if (!addr_aob) {

		return false;

	}

	//std::cout << "ADDR: " << std::hex << addr_aob << "\n";

	m_pHandleProcess->WriteMemory(addr_aob - 0x7e4b86, CC_GOD_MODE);
	m_pHandleProcess->WriteMemory(addr_aob, ACTIVATE_GOD_MODE);

	return true;
}

bool CModGame::deactivateInfiniteKiMode() const
{
	uintptr_t addr_aob = m_gameData.get_AOB_ADDR(DATA_INDEX::MY_KI);

	if (!addr_aob) {

		return false;

	}

	m_pHandleProcess->WriteMemory(addr_aob, DEACTIVATE_INFINITE_KI_MODE);

	return true;
}

bool CModGame::activateInfiniteKiMode() const
{
	uintptr_t addr_aob = m_gameData.get_AOB_ADDR(DATA_INDEX::MY_KI);

	if (!addr_aob) {

		return false;

	}

	//std::cout << "ADDR: " << std::hex << addr_aob << "\n";

	m_pHandleProcess->WriteMemory(addr_aob - 0x7e44db, CC_INFINITE_KI_MODE);
	m_pHandleProcess->WriteMemory(addr_aob, ACTIVATE_INFINITE_KI_MODE);

	return true;
}

bool CModGame::IsGamePatched() const
{
	return gamePatched;
}

bool CModGame::Patch()
{
	uintptr_t addr_aob = m_gameData.get_AOB_ADDR(DATA_INDEX::MY_HEALTH);

	if (!addr_aob) {

		addr_aob = m_pHandleProcess->FindPattern(m_gameData.get_AOB(DATA_INDEX::MY_HEALTH).data());

		if (!addr_aob) {
			std::cout << "GAME NOT PATCHED 1!!" << "\n";
			return false;
		}

		m_gameData.set_AOB_ADDR(DATA_INDEX::MY_HEALTH, addr_aob);

	}

	addr_aob = m_gameData.get_AOB_ADDR(DATA_INDEX::MY_KI);

	if (!addr_aob) {

		addr_aob = m_pHandleProcess->FindPattern(m_gameData.get_AOB(DATA_INDEX::MY_KI).data());

		if (!addr_aob) {
			std::cout << "GAME NOT PATCHED 2!!" << "\n";
			return false;
		}

		m_gameData.set_AOB_ADDR(DATA_INDEX::MY_KI, addr_aob);

	}

	gamePatched = true;

	std::cout << "GAME PATCHED!!" << "\n";

	return true;
}