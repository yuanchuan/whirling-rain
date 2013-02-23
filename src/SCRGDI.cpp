//
//Name:  Whirling Rain  (Screensaver)
//Autor: Yuan Chuan  
//Date:  2009.11
//
//Two extra lib's are necessary and their names will be different 
//according to your compliler.For instance,i'm using the MinGW and the their 
//names are libscrnsave.a and libcomctl32.a. But in VC they might be 
//ScrnSave.lib and comctl32.lib. Add them like this:

//#pragma comment(lib, "ScrnSave.lib")      
//#pragma comment(lib, "comctl32.lib")
//
//After being complied and linked succesfully, rename the final .exe file to be
//a .scr file.


#include <windows.h>
#include <scrnsave.h>
#include "ColorLines.h"


LRESULT WINAPI ScreenSaverProc(HWND hwnd, UINT message,
                               WPARAM wParam, LPARAM lParam)
{
    HDC     Hdc,BackBuffer;
    HBITMAP BitMap;	
    int frames_per_sec=100;
    static unsigned int timer;
    static ColorLines lines;
    
    
	switch (message) {
		case WM_CREATE:
			timer = SetTimer(hwnd, 1, 1000/frames_per_sec, NULL);
	        
			//Init everthing right here
			lines.Init();	
			break;

		case WM_ERASEBKGND:
			break;

		case WM_TIMER:	
			Hdc=GetDC(hwnd);
			
			//Update in each frame
			lines.Update(); 
            
            //create a back buffer for drawing
			BitMap = CreateCompatibleBitmap(Hdc,WIN_WIDTH,WIN_HEIGHT);		
            BackBuffer = CreateCompatibleDC(Hdc);           
		    
			SelectObject(BackBuffer,BitMap);
		    BitBlt(BackBuffer,0,0,WIN_WIDTH,WIN_HEIGHT,NULL,0,0,BLACKNESS);
			
			//draw graphics in backbuffer
			lines.Render(BackBuffer);
			
			//copy the whole backbuffer to the front DC
            BitBlt(Hdc, 0, 0, WIN_WIDTH,WIN_HEIGHT, BackBuffer, 0, 0, SRCCOPY);
		    
			DeleteObject(BackBuffer);		
			DeleteObject(BitMap);
            ReleaseDC(hwnd,Hdc);
         
			break;
		
		case WM_DESTROY:
			KillTimer(hwnd, timer);
			
			//Clean up
			lines.Clean();
			
			break;
		default:
			return DefScreenSaverProc(hwnd, message, wParam, lParam);
	}

	return 0;
}


BOOL WINAPI ScreenSaverConfigureDialog(HWND hdwnd, UINT message,
                                       WPARAM wParam, LPARAM lParam)
{
	return 0;
}


BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return 1;
}

//------------------------------------------------------------------------------
