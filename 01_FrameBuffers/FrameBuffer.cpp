
/*
// FrameBuffer.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "FrameBuffer.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FRAMEBUFFER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FRAMEBUFFER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FRAMEBUFFER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FRAMEBUFFER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

*/


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


//*** OpenGL Headers ***//
#include "C:/glew-2.1.0/include/GL/glew.h"
#include <gl/GL.h>
#include "vmath.h"
#include "fbo.hpp"
#include "FrameBuffer.h"
using namespace vmath;


//*** MACRO'S ***
#define WIN_WIDTH  800
#define WIN_HIGHT  600


//*** link With OpenGL Library//
//#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")


//*** Globle Function Declarations ***
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//*** Globle Variable Declaration ***
FILE* gpFILE = NULL;
HWND ghwnd = NULL;
BOOL gbActive = FALSE;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
BOOL gbFullscreen = FALSE;
//*** OpenGL Related Globle Variables ***
PIXELFORMATDESCRIPTOR pfd;
int iPexelFormatIndex = 0;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
FBO* pFBO = NULL;
GLuint vao = 0;
enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLORS,
	AMC_ATTRIBUTE_NORMALS,
	AMC_ATTRIBUTE_TEXCOORDS
};

// CUBE Variables-----------------------------------------------------------------
GLuint shaderProgramObject_cube = 0;
GLfloat angleCube = 0.0f;
GLint fboWidth = WIN_WIDTH;
GLint fboHeight = WIN_HIGHT;
// Vao
GLuint vaoCube = 0;
// Vbo
GLuint vbo_position_cube = 0;
GLuint vbo_texcoord_cube = 0;
// Uniforms
GLuint mvpMatrixUniform_cube = 0;
GLuint textureSamplerUniform_cube = 0;
// Transforms
mat4 perspectiveProjectionMatrix_cube;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//*** Function Declarations ***
	int initialize_cube(void);
	void uninitialize_cube(void);
	void display_cube(void);
	void update_cube(void);

	//*** Local Variable Declarations ***
	WNDCLASSEX wndclass;
	HWND hwnd;
	TCHAR szAppName[] = TEXT("Tushar_Wagdare_Chi_Window");
	MSG msg;
	int iResult = 0;
	BOOL bDone = FALSE;
	//For resolution
	int ifboWidth, iWinHight;
	ifboWidth = GetSystemMetrics(SM_CXSCREEN);
	iWinHight = GetSystemMetrics(SM_CYSCREEN);


	//*** Code ***
	if (fopen_s(&gpFILE, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Log Create Hou Shakat Nahi"), TEXT("Error"), MB_OK || MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFILE, "Program Started Successfully...\n\n");

	//*** WNDCLASSEX INITIALIZATION ***
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_SMALL));


	//*** REGISTER WNDCLASSEX ***
	RegisterClassEx(&wndclass);


	//*** CREATE WINDOW ***
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Tushar Tulshiram Wagdare"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		(ifboWidth / 2) - (800 / 2),
		(iWinHight / 2) - (600 / 2),
		WIN_WIDTH,
		WIN_HIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);


	ghwnd = hwnd;


	//*** Initialization ***
	iResult = initialize_cube();
	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize() Failed !"), TEXT("Error"), MB_OK || MB_ICONERROR);
		DestroyWindow(hwnd);
	}


	//*** SHOW THE WINDOW ***
	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);


	//*** Game LOOP ***
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActive == TRUE)
			{
				//*** Render ***
				display_cube();

				//*** Update ***
				update_cube();
			}
		}
	}


	//*** Uninitialization ***
	uninitialize_cube();


	return((int)msg.wParam);
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//*** Function Declaration ***
	void ToggleFullscreen(void);
	void resize_cube(int, int);


	//*** Code ***
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActive = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;

	case WM_SIZE:
		resize_cube(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			if (gbFullscreen == FALSE)
			{
				ToggleFullscreen();
				gbFullscreen = TRUE;
			}
			else
			{
				ToggleFullscreen();
				gbFullscreen = FALSE;
			}
			break;
		}
		break;

	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'Q':
		case 'q':
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}


	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}



void ToggleFullscreen(void)
{
	//*** Local Variable Declaration ****
	MONITORINFO mi = { sizeof(MONITORINFO) };


	//*** Code ***
	if (gbFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			fprintf(gpFILE, "Window Contains WS_OVERLAPPEDWINDOW\n");

			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		fprintf(gpFILE, "Window Is Now Already In Fullscreen Mode\n");
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		fprintf(gpFILE, "Now Window Is Normal\n");
	}
}



int initialize_cube(void)
{
	//*** Function Declarations ***
	void printGLInfo(void);
	void uninitialize_cube(void);
	void resize_cube(int, int);
	BOOL createFBO(GLint, GLint);
	BOOL initialize_sphere(GLint, GLint);

	//Variable Declaration
	BOOL bResult = FALSE;

	//01 -  Initialization Of PFD 
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;


	//02 - Get The DC
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFILE, "GetDC Failed !!!\n\n");
		return(-1);
	}


	//03 - Tell OS Hya PFD shi match honara PFD de
	iPexelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPexelFormatIndex == 0)
	{
		fprintf(gpFILE, "ChoosepixelFormat() Failed\n\n");
		return(-2);
	}


	//04 - Set Obtained Pixel Format
	if (SetPixelFormat(ghdc, iPexelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFILE, "SetPixelFormat() Failed\n\n");
		return(-3);
	}


	//05 - Tell WGL Bridging library To Give OpenGL Copatible DC From 'ghdc'
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFILE, "wglCreateContex() Failed\n\n");
		return(-4);
	}


	//06 - Now 'ghdc' End Its Roll And Give Controll To 'ghrc'
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFILE, "wglMakeCurrent() failed\n\n");
		return(-5);
	}


	//Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFILE, "glewInit() Failed !\n\n");
		return(-6);
	}
	//Print GLINFO
	printGLInfo();


	//----------------------------------------------------------------------------------------//
	//Vertex Shader
	const GLchar* vertexShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec4 aPosition;" \
		"in vec2 aTexCoord;" \
		"out vec2 oTexCoord;" \
		"void main(void)" \
		"{" \
		"gl_Position = aPosition;" \
		"oTexCoord = aTexCoord;" \
		"}";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObject);
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);//EC step 1
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Vertex Shader Compilation Error Log:%s\n", szInfoLog);
				fprintf(gpFILE, "********************************************************\n");
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize_cube();
	}


	//Fragment Shader
	const GLchar* fragmentShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec2 oTexCoord;" \
		"uniform sampler2D uTextureSampler;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = texture(uTextureSampler, oTexCoord);" \
		"}";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Fragment Shader Compilation Error Log:%s\n", szInfoLog);
				fprintf(gpFILE, "********************************************************\n");
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize_cube();
	}

	//Shader Program
	shaderProgramObject_cube = glCreateProgram();

	glAttachShader(shaderProgramObject_cube, vertexShaderObject);
	glAttachShader(shaderProgramObject_cube, fragmentShaderObject);

	glBindAttribLocation(shaderProgramObject_cube, AMC_ATTRIBUTE_POSITION, "aPosition");
	glBindAttribLocation(shaderProgramObject_cube, AMC_ATTRIBUTE_TEXCOORDS, "aTexCoord");

	glLinkProgram(shaderProgramObject_cube);
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;
	glGetProgramiv(shaderProgramObject_cube, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject_cube, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject_cube, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Shader Program Linking Error Log:%s\n", szInfoLog);
				fprintf(gpFILE, "********************************************************\n");
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize_cube();
	}

	//Get Shader Unifprm Location
	textureSamplerUniform_cube = glGetUniformLocation(shaderProgramObject_cube, "uTextureSampler");

	const GLfloat cube_position[] =
	{
		// front
		 1.0f,  1.0f,  0.0f, // top-right of front
		-1.0f,  1.0f,  0.0f, // top-left of front
		-1.0f, -1.0f,  0.0f, // bottom-left of front
		 1.0f, -1.0f,  0.0f, // bottom-right of front
	};

	const GLfloat cube_texcoord[] =
	{
		// front
		1.0f, 1.0f, // top-right of front
		0.0f, 1.0f, // top-left of front
		0.0f, 0.0f, // bottom-left of front
		1.0f, 0.0f, // bottom-right of front
	};


	// Square 
	//VAO
	glGenVertexArrays(1, &vaoCube);
	glBindVertexArray(vaoCube);

	//VBO for Position 
	glGenBuffers(1, &vbo_position_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_position), cube_position, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//VBO for texcoord
	glGenBuffers(1, &vbo_texcoord_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoord), cube_texcoord, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORDS);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//
	glBindVertexArray(0);

	//-------------------------------------------------------------------------------------//

	//07 - 2nd step Enabling Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	//Tell OpenGL To Enable Texture
	glEnable(GL_TEXTURE_2D);


	//08 - Set the Clear Color of Window To Blue
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


	//initialize orthographicProjectionMatrix
	perspectiveProjectionMatrix_cube = vmath::mat4::identity();


	//Warmup call
	resize_cube(WIN_WIDTH, WIN_HIGHT);














	// FBO related code
	// Vertex Shader
	const GLchar* vertexShaderSourceCodeFBO =
		"#version 460 core" \
		"\n" \
		"layout (location = 0) in vec4 aPosition;" \
		"layout (location = 1) in vec4 aColor;" \
		"uniform mat4 uMVPMatrix;" \
		"out vec4 oColor;" \

		"void main(void)" \
		"{" \
		"oColor = aColor;" \
		"gl_Position = uMVPMatrix*aPosition;" \
		"}";

	// Fragment Shader
	const GLchar* fragmentShaderSourceCodeFBO =
		"#version 460 core" \
		"\n" \
		"in vec4 oColor;" \
		"out vec4 FragColor;" \

		"void main(void)" \
		"{" \
		"FragColor = oColor;" \
		"}";

	pFBO = new FBO(fboWidth, fboHeight);

	int retVal = pFBO->createShaderProgram(vertexShaderSourceCodeFBO, fragmentShaderSourceCodeFBO);
	if (retVal < 0)
	{
		fprintf(gpFILE, "pFBO->createShaderProgram failed...\n");
		uninitialize_cube();
	}

	GLfloat triangle_positions[] =
	{
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	GLfloat triangle_color[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	// vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position vbo
	GLuint vboPos = 0;
	glGenBuffers(1, &vboPos);
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_positions), triangle_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// color vbo
	GLuint vboCol = 0;
	glGenBuffers(1, &vboCol);
	glBindBuffer(GL_ARRAY_BUFFER, vboCol);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_color), triangle_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return(0);
}



void printGLInfo(void)
{
	//Variable Declaration 
	GLint numExtension;
	GLint i;

	//code
	fprintf(gpFILE, "OpenGL Vender       : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFILE, "OpenGL Renderer     : %s\n", glGetString(GL_RENDERER));//Driver Version
	fprintf(gpFILE, "OpenGL Version      : %s\n", glGetString(GL_VERSION));
	fprintf(gpFILE, "OpenGL GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(gpFILE, "********************************************************\n");

	//Supported Extensions List
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtension);
	for (i = 0; i < numExtension; i++)
	{
		fprintf(gpFILE, "%d : %s\n", i, glGetStringi(GL_EXTENSIONS, i));
	}
	fprintf(gpFILE, "********************************************************\n");
}



void resize_cube(int width, int height)
{
	//*** Code ***
	if (height <= 0)
	{
		height = 1;
	}

	fboWidth = width;
	fboHeight = height;

	//Set perspective projection matrix
	perspectiveProjectionMatrix_cube = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}



void display_cube(void)
{
	// Func
	void resize_cube(int, int);


	pFBO->resize(fboWidth, fboHeight);
	pFBO->render(vao);

	///Call resize cube again to change 
	resize_cube(fboWidth, fboHeight);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//*** Code ***
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramObject_cube);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pFBO->getTexture());
	glUniform1i(textureSamplerUniform_cube, 0);

	glBindVertexArray(vaoCube);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);



	SwapBuffers(ghdc);
}



void update_cube(void)
{
	//*** Code ***
}



void uninitialize_cube(void)
{
	//*** Function Declarations ***
	void ToggleFullscreen(void);

	//Free Shader Program Object
	if (shaderProgramObject_cube)
	{
		glUseProgram(shaderProgramObject_cube);

		GLint numShaders = 0;
		glGetProgramiv(shaderProgramObject_cube, GL_ATTACHED_SHADERS, &numShaders);
		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));
			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_cube, numShaders, NULL, pShaders);
				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject_cube, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
				free(pShaders);
				pShaders = NULL;
			}
		}

		glUseProgram(0);
		glDeleteProgram(shaderProgramObject_cube);
		shaderProgramObject_cube = 0;
	}

	//Delete VBO Position
	if (vbo_position_cube)
	{
		glDeleteBuffers(1, &vbo_position_cube);
		vbo_position_cube = 0;
	}

	//Delete VBO Position
	if (vbo_texcoord_cube)
	{
		glDeleteBuffers(1, &vbo_texcoord_cube);
		vbo_texcoord_cube = 0;
	}

	//Delete VAO
	if (vaoCube)
	{
		glDeleteVertexArrays(1, &vaoCube);
		vaoCube = 0;
	}


	//*** Code ***
	if (gbFullscreen == TRUE)
	{
		ToggleFullscreen();
		gbFullscreen = FALSE;
	}


	//*** Make The hdc As Current DC ***//
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}


	//*** Destroy Rendering Contex***
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}


	//*** Release HDC ***
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}


	//*** Destroy Window ***
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}


	//*** Close Log File ***
	if (gpFILE)
	{
		fprintf(gpFILE, "\nProgram Ended Successfully\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}
}





