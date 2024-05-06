#define OEMRESOURCE

#include <Windows.h>
#include <WinUser.h>
#include <chrono>
#include <string>
#include <thread>
#include <cmath>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <optional>

using registeredPath = std::optional<std::wstring>;
using userSettingsMap = std::unordered_map<std::wstring, registeredPath>;

userSettingsMap settingsMap =
{
    {L"Arrow",{}},//same as assigning std::nullopt but it's good to know your syntax options, right?
    {L"IBeam", std::nullopt},
    {L"Hand",std::nullopt},
    {L"Help",std::nullopt},
    {L"No",std::nullopt},
    {L"NWPen",std::nullopt},
    {L"Person",std::nullopt},
    {L"Pin",std::nullopt},
    {L"SizeAll",std::nullopt},
    {L"SizeNESW",std::nullopt},
    {L"SizeNS",std::nullopt},
    {L"SizeNWSE",std::nullopt},
    {L"SizeWE",std::nullopt},
    {L"UpArrow",std::nullopt},
    {L"Wait",std::nullopt},
};


std::wstring getRegistryValue(HKEY hKey, const wchar_t* valueName) 
{
    const DWORD bufferSize = 1024;
    std::wstring result;
    DWORD dataSize = bufferSize * sizeof(wchar_t);
    std::vector<wchar_t> buffer(bufferSize);
    LONG queryResult = RegQueryValueEx(hKey, valueName, nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer.data()), &dataSize);
    if (queryResult == ERROR_SUCCESS && dataSize > 0) 
    {
        result.assign(buffer.data(), dataSize / sizeof(wchar_t) - 1); // Subtract 1 to remove null terminator
    }

    return result;
}



void fetchUserSettings(userSettingsMap& setMap)
{
    HKEY hKey;
    LONG openResult = RegOpenKeyEx(HKEY_CURRENT_USER, L"Control Panel\\Cursors", 0, KEY_READ, &hKey);
    if (openResult == ERROR_SUCCESS) 
    {

        for (auto& pair : setMap)
        {
            pair.second = getRegistryValue(hKey, pair.first.c_str());
#if 1
            std::wcout << pair.first << ": " << pair.second.value() << std::endl;
#endif       
        }

        RegCloseKey(hKey);
    }
    else 
    {
        std::cerr << "Error opening registry key: " << openResult << std::endl;
    }

}



bool hasDoneBefore { false };

double Distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void setCurserSize(int x, int y)
{
    HCURSOR customCursor;
    customCursor = static_cast<HCURSOR>(LoadImage(nullptr, L"C:\\Windows\\Cursors\\arrow_i.cur", IMAGE_CURSOR, x, y, LR_LOADFROMFILE));
    SetSystemCursor(customCursor, OCR_NORMAL);
    if (x > 32)
    {
        customCursor = static_cast<HCURSOR>(LoadImage(nullptr, L"C:\\Windows\\Cursors\\beam_i.cur", IMAGE_CURSOR, x, y, LR_CREATEDIBSECTION));
        SetSystemCursor(customCursor, OCR_IBEAM);
    }
    else
    {
        HCURSOR hStandardIBeamCursor = LoadCursor(nullptr, IDC_IBEAM);
        SetSystemCursor(hStandardIBeamCursor, OCR_IBEAM);
    }
    customCursor = static_cast<HCURSOR>(LoadImage(nullptr, L"C:\\Windows\\Cursors\\beam_i.cur", IMAGE_CURSOR, x, y, LR_LOADFROMFILE));
    SetSystemCursor(customCursor, OCR_IBEAM);
    customCursor = static_cast<HCURSOR>(LoadImage(nullptr, L"C:\\Windows\\Cursors\\aero_link.cur", IMAGE_CURSOR, x, y, LR_LOADFROMFILE));
    SetSystemCursor(customCursor, OCR_HAND);
}

int main() 
{
   /* HINSTANCE hinst;            // handle to current instance 
    HCURSOR hCurs1, hCurs2;     // cursor handles 

    // Create a standard hourglass cursor. 

    POINT prevPos;
    GetCursorPos(&prevPos);
    auto prevTime = std::chrono::steady_clock::now();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        POINT currentPos;
        GetCursorPos(&currentPos);

        auto currentTime = std::chrono::steady_clock::now();

        auto distance = Distance(prevPos.x, prevPos.y, currentPos.x, currentPos.y);

        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevTime).count() / 1000.0;

        auto speed = distance / elapsedTime;
        //speed = 5001;
        if (speed > 3000)
        {
           std::cout << "bigger" << '\n';
           setCurserSize(150, 150);
        }
        else 
        {
            std::cout << "smaller"<<'\n';
            setCurserSize(32, 32);
        }

        prevPos = currentPos;
        prevTime = currentTime;
    }*/

    while (false)
    {
        getchar();
        setCurserSize(172,172);
        getchar();
        setCurserSize(32, 32);
    }

    fetchUserSettings(settingsMap);

    return 0;
}



/*
std::wstring GetCursorIconName()
{
    CURSORINFO cursorInfo = { sizeof(cursorInfo) };
    GetCursorInfo(&cursorInfo);

    HCURSOR customCursor;
    customCursor = static_cast<HCURSOR>(LoadImage(nullptr, L"C:\\Windows\\Cursors\\arrow_i.cur", IMAGE_CURSOR, x, y, LR_CREATEDIBSECTION));

    HCURSOR hCursor = cursorInfo.hCursor;
    if (hCursor != nullptr)
    {
        if (hCursor == LoadCursor(nullptr, IDC_ARROW))
        {
            return L"IDC_ARROW";
        }
        else if (hCursor == LoadCursor(nullptr, IDC_IBEAM))
        {
            return L"IDC_IBEAM";
        }
        else if (hCursor == LoadCursor(nullptr, IDC_HAND))
        {
            return L"IDC_HAND";
        }
        // Add more comparisons for other cursor icons as needed

        // If the cursor doesn't match any known icons, return "Unknown"
    }

    return L"Unknown";
}

int main()
{
    while (true) {
        getchar();
        std::wstring cursorIconName = GetCursorIconName();
        std::wcout << L"Current cursor icon: " << cursorIconName << std::endl;
    }
    return 0;
}
*/