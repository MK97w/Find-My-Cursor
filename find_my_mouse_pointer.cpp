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
#include <tuple>

using OEMResourceOrdinalNumbers = int;
using StandardCursorID = LPWSTR;
using cursorPointer = std::wstring;
using registeredPath = std::optional<std::wstring>;

using cursorPathInfo = std::tuple<registeredPath, OEMResourceOrdinalNumbers, StandardCursorID>;
using userSettingsMap = std::unordered_map<cursorPointer, cursorPathInfo>;


//using userSettingsMap = std::unordered_map<cursorPointer, cursorPathInfo>;


userSettingsMap settingsMap = {
    { L"Arrow", { std::nullopt, OCR_NORMAL, IDC_ARROW } },
    { L"Hand",  { std::nullopt, OCR_HAND, IDC_HAND  } },
    { L"IBeam", { std::nullopt, OCR_IBEAM, IDC_IBEAM  } },
    //{ L"Help",  { std::nullopt, OCR_HAND, IDC_HAND }}, -> no windows handle found
    { L"No", { std::nullopt, OCR_NO, IDC_NO  } },
    //{L"NWPen",{ std::nullopt, , IDC_NO  } }, -> no windows handle found OCR
    // { L"Person", { std::nullopt, , IDC_NO  } }, -> no windows handle found OCR 
    // { L"Pin", { std::nullopt, OCR_NO, IDC_NO  } },-> no windows handle found OCR 
    { L"SizeAll", { std::nullopt, OCR_SIZEALL, IDC_SIZEALL } },
    { L"SizeNESW", { std::nullopt, OCR_SIZENESW, IDC_SIZENESW } },
    { L"SizeNS", { std::nullopt, OCR_SIZENS, IDC_SIZENS} },
    { L"SizeNWSE", { std::nullopt, OCR_SIZENWSE, IDC_SIZENWSE } },
    { L"SizeWE", { std::nullopt, OCR_SIZEWE, IDC_SIZEWE } },
    { L"UpArrow", { std::nullopt, OBM_UPARROW, IDC_UPARROW } }, //not sure UpArrow would work!
    { L"Wait", { std::nullopt, OCR_WAIT, IDC_WAIT } },
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

        for (auto& [cursor, info] : setMap)  //structured binding 
        {
            get<0>(info) = getRegistryValue(hKey, cursor.c_str());
#if 1
            std::wcout << cursor << ": " << get<0>(info).value() << std::endl;
#endif       
        }

        RegCloseKey(hKey);
    }
    else 
    {
        std::cerr << "Error opening registry key: " << openResult << std::endl;
    }

}

std::wstring replaceBackslashes( std::wstring& str ) 
{
    for (wchar_t& ch : str) 
    {
        if (ch == L'\\') 
        {
            str.insert(&ch - &str[0], 1, L'\\');
        }
    }
    return str;
}

std::wstring get_CompatiblePath(std::wstring& str)
{
    const std::wstring pathWithoutSystemRoot = L"%SystemRoot%";

    size_t pos = str.find(pathWithoutSystemRoot);
    if (pos == std::wstring::npos) 
    {
        return str; // Return the original string if SystemRoot is not found
    }
    else {
        wchar_t systemRoot[MAX_PATH];
        DWORD len = GetEnvironmentVariable(L"SystemRoot", systemRoot, MAX_PATH);
        if (len != 0 && len < MAX_PATH) 
        {
            str.replace(pos, pathWithoutSystemRoot.size(), systemRoot);
            return str;
        }
        else 
        {
            std::cerr << "Failed to get SystemRoot environment variable." << std::endl;
            return str; // Return the original string if SystemRoot is found but GetEnvironmentVariable fails
        }
    }

}

bool hasDoneBefore { false };

double Distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void setCurserSize(const userSettingsMap& setMap, int size)
{
    HCURSOR customCursor;
    
    for (const auto& info : setMap)
    {
        if (get<0>(info.second).has_value())
        {
            auto val = get<0>(info.second).value();
            auto str = get_CompatiblePath(val);

            customCursor = static_cast<HCURSOR>(LoadImage(nullptr, str.c_str(), IMAGE_CURSOR, size, size, LR_LOADFROMFILE));
            SetSystemCursor(customCursor, get<1>(info.second));
        }
        else
        {
            customCursor = LoadCursor(nullptr, get<2>(info.second));
            SetSystemCursor(customCursor, get<1>(info.second));
        }

    }



/*
    customCursor = static_cast<HCURSOR>(LoadImage(nullptr, L"C:\\Windows\\Cursors\\arrow_i.cur", IMAGE_CURSOR, size, size, LR_LOADFROMFILE));
    SetSystemCursor(customCursor, OCR_NORMAL);
    if (size > 32)
    {
        customCursor = static_cast<HCURSOR>(LoadImage(nullptr, L"C:\\Windows\\Cursors\\beam_i.cur", IMAGE_CURSOR, size, size, LR_CREATEDIBSECTION));
        SetSystemCursor(customCursor, OCR_IBEAM);
    }
    else
    {
        HCURSOR hStandardIBeamCursor = LoadCursor(nullptr, IDC_IBEAM);
        SetSystemCursor(hStandardIBeamCursor, OCR_IBEAM);
    }
    customCursor = static_cast<HCURSOR>(LoadImage(nullptr, L"C:\\Windows\\Cursors\\beam_i.cur", IMAGE_CURSOR, size, size, LR_LOADFROMFILE));
    SetSystemCursor(customCursor, OCR_IBEAM);
    customCursor = static_cast<HCURSOR>(LoadImage(nullptr, L"C:\\Windows\\Cursors\\aero_link.cur", IMAGE_CURSOR, size, size, LR_LOADFROMFILE));
    SetSystemCursor(customCursor, OCR_HAND);*/
}
int main() 
{
    HINSTANCE hinst;            // handle to current instance 
    HCURSOR hCurs1, hCurs2;     // cursor handles 

    // Create a standard hourglass cursor. 
    fetchUserSettings(settingsMap);
    POINT prevPos;
    GetCursorPos(&prevPos);
    auto prevTime = std::chrono::steady_clock::now();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

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
           setCurserSize(settingsMap,150);
        }
        else 
        {
            std::cout << "smaller"<<'\n';
            setCurserSize(settingsMap,32);
        }

        prevPos = currentPos;
        prevTime = currentTime;
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