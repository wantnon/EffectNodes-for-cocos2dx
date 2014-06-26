#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	AllocConsole();   // create console
	SetConsoleTitle(_T("Debug Output"));      // console title
	// redirect STDIN
	{
		FILE*fp;
		errno_t eno=freopen_s(&fp,"CONIN$", "r+t", stdin);
		if(eno!=0){//failed
			MessageBox(NULL,_T("redirect stdin failed!"),_T("error"),MB_OK|MB_ICONEXCLAMATION);
			exit(0);
		}
	}
	// redirect STDOUT
	{
		FILE*fp;
		errno_t eno=freopen_s(&fp,"CONOUT$", "w+t", stdout);
		if(eno!=0){//failed
			MessageBox(NULL,_T("redirect stdout failed!"),_T("error"),MB_OK|MB_ICONEXCLAMATION);
			exit(0);
		}

	}
	
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("HelloCpp");
    eglView->setFrameSize(960,640);//(480,320);//(2048, 1536);
    // The resolution of ipad3 is very large. In general, PC's resolution is smaller than it.
    // So we need to invoke 'setFrameZoomFactor'(only valid on desktop(win32, mac, linux)) to make the window smaller.
    eglView->setFrameZoomFactor(0.7f);//(0.4f);
    return CCApplication::sharedApplication()->run();
}
