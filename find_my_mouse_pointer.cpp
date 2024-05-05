#define OEMRESOURCE

#include <Windows.h>
#include <WinUser.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <iostream>
#include <vector>

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

    while (true)
    {
        getchar();
        setCurserSize(172,172);
        getchar();
        setCurserSize(32, 32);
    }

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