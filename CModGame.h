#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <memory>
#include "CHandleProcess.h"

class CModGame {

private:

	enum class DATA_INDEX {
		MY_HEALTH,
		MY_KI,
		LAST
	};

	class CGameData {

	private:

		struct Data {
			std::string AOB_str = "";
			uintptr_t AOB_addr = 0;
		};

		std::vector<struct Data> m_info;

	public:

		CGameData()
		{
			m_info.resize(static_cast<int>(DATA_INDEX::LAST));
		}

		void set_AOB(DATA_INDEX i, std::string data)
		{
			m_info[static_cast<int>(i)].AOB_str = data;
		}

		void set_AOB_ADDR(DATA_INDEX i, uintptr_t data)
		{
			m_info[static_cast<int>(i)].AOB_addr = data;
		}

		std::string get_AOB(DATA_INDEX i) const
		{
			return m_info[static_cast<int>(i)].AOB_str;
		}

		uintptr_t get_AOB_ADDR(DATA_INDEX i) const
		{
			return m_info[static_cast<int>(i)].AOB_addr;
		}


	};

	std::unique_ptr<CHandleProcess> m_pHandleProcess;
	CGameData m_gameData;

	const std::string AOB_MY_HEALTH = "4889461848394620";
	const std::string AOB_MY_KI = "f30f1041480f2ec2";

	const std::vector<uint8_t> ACTIVATE_GOD_MODE = {0xe9, 0x75, 0xb4, 0x81, 0xff, 0x0f, 0x1f, 0x00};
	const std::vector<uint8_t> DEACTIVATE_GOD_MODE = {0x48, 0x89, 0x46, 0x18, 0x48, 0x39, 0x46, 0x20};
	const std::vector<uint8_t> CC_GOD_MODE = {
		0x50, 0x48, 0x8b, 0x06, 0x83, 0x38, 0x64, 0x0f, 
		0x85, 0x18, 0x00, 0x00, 0x00, 0x48, 0xb8, 0x10, 
		0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 
		0x89, 0x46, 0x18, 0x48, 0x89, 0x46, 0x20, 0x58, 
		0xe9, 0x69, 0x4b, 0x7e, 0x00, 0x58, 0xc7, 0x46, 
		0x18, 0x01, 0x00, 0x00, 0x00, 0xc7, 0x46, 0x20, 
		0x00, 0x00, 0x00, 0x00, 0x48, 0x39, 0x46, 0x20, 
		0xe9, 0x51, 0x4b, 0x7e, 0x00, 0x00, 0x00};
	
	const std::vector<uint8_t> ACTIVATE_INFINITE_KI_MODE = { 0xe9, 0x20, 0xbb, 0x81, 0xff };
	const std::vector<uint8_t> DEACTIVATE_INFINITE_KI_MODE = { 0xf3, 0x0f, 0x10, 0x41, 0x48 };
	const std::vector<uint8_t> CC_INFINITE_KI_MODE = {
		0x50, 0x48, 0x8b, 0x41, 0xc0, 0x83, 0x38, 0x64,
		0x0f, 0x85, 0x06, 0x00, 0x00, 0x00, 0xc7, 0x01,
		0x00, 0x00, 0x7a, 0x44, 0x58, 0xf3, 0x0f, 0x10, 
		0x41, 0x48, 0xe9, 0xc1, 0x44, 0x7e, 0x00 };

	bool godModeActive = false;
	bool infiniteKiModeActive = false;
	bool gamePatched = false;

private:

	void InitializeGameData();

	bool deactivateGodMode() const;

	bool activateGodMode() const;

	bool deactivateInfiniteKiMode() const;

	bool activateInfiniteKiMode() const;

public:

	CModGame(const std::wstring& str_window_name, const std::wstring& str_exe_name);

	bool RetryConnection();

	bool Patch();

	bool IsGameRunning() const;

	bool IsGamePatched() const;

	bool IsGodModeActive() const;

	bool IsInfiniteKiModeActive() const;

	void ToggleGodMode();
	
	void ToggleInfiniteKiMode();

};

