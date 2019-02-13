#include <Windows.h>
#include <glew.h>

static bool GAME_IS_RUNNING = false;

static void DrawACubeBitch(HWND window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* draw six faces of a cube */
    glBegin(GL_QUADS);
    glNormal3f(0.0F, 0.0F, 1.0F);
    glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, 0.5F);
    glVertex3f(-0.5F, -0.5F, 0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F);

    glNormal3f(0.0F, 0.0F, -1.0F);
    glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(-0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, -0.5F);

    glNormal3f(0.0F, 1.0F, 0.0F);
    glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F);
    glVertex3f(-0.5F, 0.5F, -0.5F);
    glVertex3f(-0.5F, 0.5F, 0.5F);

    glNormal3f(0.0F, -1.0F, 0.0F);
    glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F);
    glVertex3f(-0.5F, -0.5F, 0.5F);

    glNormal3f(1.0F, 0.0F, 0.0F);
    glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F);
    glVertex3f(0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F);

    glNormal3f(-1.0F, 0.0F, 0.0F);
    glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(-0.5F, -0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, -0.5F);
    glEnd();

    SwapBuffers(GetDC(window));
}

static LRESULT CALLBACK windowsMainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (message)
    {

    case WM_MOVE:
    {
        DrawACubeBitch(window);
        OutputDebugString("MOVED\n");
    }
    break;

    case WM_SIZE:
    {
        DrawACubeBitch(window);
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
        WS_OVERLAPPEDWINDOW | WS_VISIBLE | CS_OWNDC,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        instance,
        NULL);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixelFormat = ChoosePixelFormat(GetDC(window), &pfd);

    SetPixelFormat(GetDC(window), pixelFormat, &pfd);

    HGLRC glRenderingContext = wglCreateContext(GetDC(window));

    wglMakeCurrent(GetDC(window), glRenderingContext);

    glewInit();

    /* set viewing projection */
    glMatrixMode(GL_PROJECTION);
    glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 3.0F);

    /* position viewer */
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0F, 0.0F, -2.0F);

    /* position object */
    glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
    glRotatef(30.0F, 0.0F, 1.0F, 0.0F);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    DrawACubeBitch(window);

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

    // Clean up GL context
    wglMakeCurrent(GetDC(window), NULL);
    wglDeleteContext(glRenderingContext);

    return 0;
}