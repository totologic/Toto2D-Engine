#pragma once
#include <iostream>
using namespace std;
#include <assert.h>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;
#include "bcm_host.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

class SetContext
{
public:
	SetContext(void);
	~SetContext(void);
	
	void setContext();
	uint32_t getScreenWidth();
	uint32_t getScreenHeight();
	EGLDisplay getDisplay();
	EGLSurface getSurface();

private:
	EGLDisplay display;
	EGLSurface surface;
	uint32_t screen_width;
	uint32_t screen_height;
	EGLContext context;
	EGLint num_config;
	EGLConfig config;
	EGL_DISPMANX_WINDOW_T nativewindow;
	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;

};