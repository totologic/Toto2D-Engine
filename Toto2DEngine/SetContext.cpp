#include "SetContext.h"

SetContext::SetContext(void)
{
}

SetContext::~SetContext(void)
{
}

uint32_t SetContext::getScreenWidth()	{	return screen_width;	}
uint32_t SetContext::getScreenHeight()	{	return screen_height;	}
EGLDisplay SetContext::getDisplay()	{	return display;		}
EGLSurface SetContext::getSurface()	{	return surface;		}

void SetContext::setContext()
{
	bcm_host_init();
	
	EGLint attribute_list[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};
	
	EGLint context_attributes[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	
	EGLBoolean result;
	int32_t success;
	
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(display!=EGL_NO_DISPLAY);
	assert(glGetError() == 0);
	
	result = eglInitialize(display, NULL, NULL);
	assert(EGL_FALSE != result);
	assert(glGetError() == 0);
	
	result = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);
	assert(glGetError() == 0);
	
	result = eglBindAPI(EGL_OPENGL_ES_API);
	assert(EGL_FALSE != result);
	assert(glGetError() == 0);
	
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
	assert(context!=EGL_NO_CONTEXT);
	assert(glGetError() == 0);

	success = graphics_get_display_size(0 /*LCD*/, &screen_width, &screen_height);
	assert( success >= 0 );
	cout  << "display size: " << screen_width << "x" << screen_height << endl;
	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = screen_width;
	dst_rect.height = screen_height;
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = screen_width;// << 16;
	src_rect.height = screen_height;// << 16;
	
	dispman_display = vc_dispmanx_display_open( 0 /*LCD*/);
	dispman_update = vc_dispmanx_update_start( 0 );
	dispman_element = vc_dispmanx_element_add( dispman_update, dispman_display,
		0/*layer*/, &dst_rect, 0/*src*/,
		&src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);
	
	nativewindow.element = dispman_element;
	nativewindow.width = screen_width;
	nativewindow.height = screen_height;
	vc_dispmanx_update_submit_sync( dispman_update );
	
	assert(glGetError() == 0);
	
	surface = eglCreateWindowSurface( display, config, &nativewindow, NULL );
	assert(surface != EGL_NO_SURFACE);
	assert(glGetError() == 0);
	
	result = eglMakeCurrent(display, surface, surface, context);
	assert(EGL_FALSE != result);
	assert(glGetError() == 0);
}