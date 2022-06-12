// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "mem.h"
#include "proc.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Super Nintendo, Sega Genesis");
    std::cout << "Fuck all you hoes! Get a grip, motherfucker!\n\n";

    std::cout << "[F1] Health Hack\n";
    std::cout << "[F2] Gun Ammo Hack\n";
    std::cout << "[F3] Speedhack\n";
    std::cout << "[F4] Armor Hack\n";
    std::cout << "[F5] Grenade Hack\n";
    std::cout << "[F6] No Recoil Hack\n\n";

    std::cout << "[+] Build: 12 June\n";
    std::cout << "[+] Made by hj.#0010\n";

    //get module base :D
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

    moduleBase = (uintptr_t)GetModuleHandle(NULL);

    bool bHealth = false, bAmmo = false, bRecoil = false, bSpeedhack = false, bArmor = false, bGrenade = false;

    //Hack loop
    while (true)
    {
        //key input
        if (GetAsyncKeyState(VK_END) & 1) //Unload internal cheat
        {
            break;
        }
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            bHealth = !bHealth;
        }
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            bAmmo = !bAmmo;
        }
        if (GetAsyncKeyState(VK_F3) & 1)
        {
            bSpeedhack = !bSpeedhack;
        }
        if (GetAsyncKeyState(VK_F4) & 1)
        {
            bArmor = !bArmor;
        }
        if (GetAsyncKeyState(VK_F5) & 1)
        {
            bGrenade = !bGrenade;
        }
        if (GetAsyncKeyState(VK_F6) & 1)
        {
            bRecoil = !bRecoil;

            if (bRecoil)
            {
                //nop
                mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
            }
            else
            {
                //50 8D 4C 24 1C 51 8B CE FF D2
                mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
            }
        }
        uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10F4F4);

        if (localPlayerPtr)
        {
            if (bHealth)
            {
                *(int*)(*localPlayerPtr + 0xf8) = 1337;
            }
            if (bArmor)
            {
                *(int*)(*localPlayerPtr + 0x158) = 69;
            }
            if (bGrenade)
            {
                *(int*)(*localPlayerPtr + 0x0FC) = 69;
            }
            if (bAmmo)
            {
                uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 });
                int* ammo = (int*)ammoAddr;
                *ammo = 1337;

                //Or This
                *(int*)mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 }) = 1337;
            }
            if (bSpeedhack)
            {
                mem::Patch((BYTE*)moduleBase + 0x5BEA0, (BYTE*)"\xB8\x03\x00\x00\x00", 5);
                mem::Patch((BYTE*)moduleBase + 0x5BE40, (BYTE*)"\xB8\xFD\xFF\xFF\xFF", 5);
                mem::Patch((BYTE*)moduleBase + 0x5BF00, (BYTE*)"\xB8\x03\x00\x00\x00", 5);
                mem::Patch((BYTE*)moduleBase + 0x5BF60, (BYTE*)"\xB8\xFD\xFF\xFF\xFF", 5);
            }

            else
            {
                mem::Patch((BYTE*)moduleBase + 0x5BEA0, (BYTE*)"\xB8\x01\x00\x00\x00", 5);
                mem::Patch((BYTE*)moduleBase + 0x5BE40, (BYTE*)"\xB8\xFF\xFF\xFF\xFF", 5);
                mem::Patch((BYTE*)moduleBase + 0x5BF00, (BYTE*)"\xB8\x01\x00\x00\x00", 5);
                mem::Patch((BYTE*)moduleBase + 0x5BF60, (BYTE*)"\xB8\xFF\xFF\xFF\xFF", 5);
            }
        }
        Sleep(5);
    }

    //cleanup & eject
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}