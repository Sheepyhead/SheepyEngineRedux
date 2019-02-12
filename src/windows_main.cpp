#include <Windows.h>

static bool GAME_IS_RUNNING = false;

static LRESULT CALLBACK windowsMainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (message)
    {
    case WM_MOVE:
    {
        OutputDebugString("MOVED\n");
    }
    break;

    case WM_SIZE:
    {
        OutputDebugString("RESIZED\n");
    }
    break;

    case WM_DESTROY:
    {
        GAME_IS_RUNNING = false;
    }
    break;

    case WM_CLOSE:
    {
        GAME_IS_RUNNING = false;
    }
    break;

    default:
    {
        result = DefWindowProc(window, message, wParam, lParam);
    }
    break;
    }

    return result;
}

ATOM windowsCreateClass(HINSTANCE instance)
{
    WNDCLASS customClass = {};

    customClass.style = CS_HREDRAW | CS_VREDRAW;           // The class style(s)
    customClass.lpfnWndProc = &windowsMainWindowCallback;  // A pointer to the window procedure WindowProc
    customClass.hInstance = instance;                      // A handle to the instance that contains the window procedure for the class
    customClass.lpszClassName = "SheepyengineWindowClass"; // String that's the name of the window class

    return RegisterClass(&customClass);
}

int WinMain(
    HINSTANCE instance,     // Handle to the current instance of the application
    HINSTANCE prevInstance, // Handle to the previous instance of the application, is always NULL
    LPSTR cmdLine,          // Command line for the application, excluding the program name6
    int showCmd             // Unknown, probably useless
)
{
    ATOM customClass = windowsCreateClass(instance);

    HWND window = CreateWindowEx(
        NULL,
        "SheepyengineWindowClass",
        "SheepyEngine",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        instance,
        NULL);

    ShowWindow(window, showCmd);

    // START GAME LOOP

    GAME_IS_RUNNING = true;
    while (GAME_IS_RUNNING)
    {
        MSG message;
        while (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                GAME_IS_RUNNING = false;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }

    // END GAME LOOP

    return 0;
}