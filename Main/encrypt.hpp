#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <thread>
#include "xorstr.hpp"
#pragma execution_character_set( "utf-8" )

namespace protector {
	int time01;

	bool scan; // scan_exe 
	bool Title; //scan_title
	bool Driver; //scan_driver
	bool loopkill; //loop_killdbg
	std::uint32_t find_dbg(const char* proc)
	{
		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		auto pe = PROCESSENTRY32{ sizeof(PROCESSENTRY32) };

		if (Process32First(snapshot, &pe)) {
			do {
				if (!_stricmp(proc, pe.szExeFile)) {
					CloseHandle(snapshot);
					return pe.th32ProcessID;
				}
			} while (Process32Next(snapshot, &pe));
		}
		CloseHandle(snapshot);
		return 0;
	}

	void anti_dbgSimple() { // anti DBG

		if (IsDebuggerPresent())
		{
			exit(1);
		}
		else
		{

		}
	}

	void anti_dbgO() { // ant dbg

		__try {
			DebugBreak();
		}
		__except (GetExceptionCode() == EXCEPTION_BREAKPOINT ?
			EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {

		}

	}	


	/* Exe Detection Function */
	void exe_detect()
	{

		if (scan == true) {

			if (find_dbg(XorStr("KsDumperClient.exe").c_str()))
			{

				exit(1);
			}
			else if (find_dbg(XorStr("HTTPDebuggerUI.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("HTTPDebuggerSvc.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("FolderChangesView.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("ProcessHacker.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("procmon.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("idaq.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("ida.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("idaq64.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("Wireshark.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("Fiddler.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("Xenos64.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("Cheat Engine.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("HTTP Debugger Windows Service (32 bit).exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("KsDumper.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("x64dbg.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("x64dbg.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("x32dbg.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("Fiddler Everywhere.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("die.exe").c_str()))
			{

				exit(1);			}
			else if (find_dbg(XorStr("Everything.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("OLLYDBG.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("HxD64.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("HxD32.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("snowman.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("taskmgr.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("qemu-ga.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("vboxservice.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("exeinfope.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("PE-bear.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("dnSpy.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("dnSpy.Console.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("PETools.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("cutter.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("rizin.exe").c_str()))
			{

				exit(1);			}

			else if (find_dbg(XorStr("binaryninja.exe").c_str()))
			{

				exit(1);			}

		}

	}

	/* Title Detection Function */
	void title_detect()
	{
		if (Title == true) {

			HWND window;
			window = FindWindow(0, XorStr(("IDA: Quick start")).c_str());
			if (window)
			{

				exit(1);
			}


			window = FindWindow(0, XorStr(("Memory Viewer")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Cheat Engine")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Cheat Engine 7.2")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Cheat Engine 7.1")).c_str());
			if (window)
			{

			}

			window = FindWindow(0, XorStr(("Cheat Engine 7.0")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Process List")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("x32DBG")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("x64DBG")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("KsDumper")).c_str());
			if (window)
			{

				exit(1);
			}
			window = FindWindow(0, XorStr(("Fiddler Everywhere")).c_str());
			if (window)
			{

				exit(1);
			}
			window = FindWindow(0, XorStr(("Fiddler Classic")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Fiddler Jam")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("FiddlerCap")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("FiddlerCore")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Scylla x86 v0.9.8")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Scylla x64 v0.9.8")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Scylla x86 v0.9.5a")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Scylla x64 v0.9.5a")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Scylla x86 v0.9.5")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Scylla x64 v0.9.5")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Detect It Easy v3.01")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Everything")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("OllyDbg")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("OllyDbg")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("HxD")).c_str());
			if (window)
			{

				exit(1);
			}

			window = FindWindow(0, XorStr(("Snowman")).c_str());
			if (window)
			{

				exit(1);
			}
			window = FindWindow(0, XorStr(("Task Manager")).c_str());
			if (window)
			{

				exit(1);
			}
			window = FindWindow(0, XorStr(("ghidra")).c_str());
			if (window)
			{

				exit(1);
			}
			window = FindWindow(0, XorStr(("ILSpy")).c_str());
			if (window)
			{

				exit(1);
			}

		}

	}

	/* Driver Detection Function */
	void driver_detect()
	{
		if (Driver == true) {

			const TCHAR* devices[] = {
		_T("\\\\.\\NiGgEr"),
		_T("\\\\.\\KsDumper")
			};

			WORD iLength = sizeof(devices) / sizeof(devices[0]);
			for (int i = 0; i < iLength; i++)
			{
				HANDLE hFile = CreateFile(devices[i], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				TCHAR msg[256] = _T("");
				if (hFile != INVALID_HANDLE_VALUE) {

					MessageBoxA(NULL, "Driver detected! Exiting program.", "Driver Detection", MB_OK | MB_ICONERROR);
					exit(1);

				}
				else
				{

				}
			}

		}
	}
	void one_killdbg()
	{

		system(XorStr("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1").c_str());
		system(XorStr("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1").c_str());
		system(XorStr("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1").c_str());
		system(XorStr("taskkill /FI \"IMAGENAME eq taskmgr*\" /IM * /F /T >nul 2>&1").c_str());

	}

	void loop_killdbg()
	{

		/*  For example, if you type 60 here, the killdebuger will run every 60 seconds. */
		std::this_thread::sleep_for(std::chrono::seconds(60));
		/* If there is anything else you want to add, you can write it here. */
		system(XorStr("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1").c_str());
		system(XorStr("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1").c_str());
		system(XorStr("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1").c_str());
		system(XorStr("taskkill /FI \"IMAGENAME eq taskmgr*\" /IM * /F /T >nul 2>&1").c_str());

	}

	void call_loop_killdbg() {

		if (loopkill == TRUE) {

			while (true) {

				protector::loop_killdbg();

				SleepEx(1, true);
			}

		}

	}
	/* Start Protector Main Function */
	void protector()
	{
		one_killdbg();
		while (true) {

			protector::exe_detect();
			protector::title_detect();
			protector::driver_detect();

			//* Optimize (CPU).
			SleepEx(time01, true);
		}
	}
	void Protection() { // Start Protection

		std::thread(protector).detach();
		std::thread(call_loop_killdbg).detach();

	}
}