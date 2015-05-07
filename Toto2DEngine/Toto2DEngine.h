#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils.h"
#include "SpriteMatrixBatcher.h"
#include "SpriteMatrixBatcher.cpp"
#include "SimpleTileBatcher.h"
#include "SimpleTileBatcher.cpp"
#include "RepeatTileBatcher.h"
#include "RepeatTileBatcher.cpp"

class Toto2DEngine
{
public:
	Toto2DEngine(void);
	~Toto2DEngine(void);

	void init(uint32_t screenWidth, uint32_t screenHeight, EGLDisplay display, EGLSurface surface);
	void setBlendModeNormal();
	void setBlendModeAdditive();
	void setCamera(float x, float y);
	void setCameraLookAt(float x, float y, float rotate=0.0f, float zoom=1.0f);
	void setCameraMatrix(glm::mat4 &transform);
	int uploadAtlas(int slot, void* pixels, int width, int height);
	void activeAtlas(int slot);
	int getCurrentAtlas();
	void setBGColor(float r, float g, float b);
	SpriteMatrixBatcher& getSpriteBatcher();
	SimpleTileBatcher& getSimpleTileBatcher();
	RepeatTileBatcher& getRepeatTileBatcher();
	void clear();
	void swap();
	
	void spriteMatrixBatcherCallback();
	static void spriteMatrixBatcherCallbackHelper(void *ptr)
	{
		((Toto2DEngine *)ptr)->spriteMatrixBatcherCallback();
	}
	
	void simpleTileBatcherCallback();
	static void simpleTileBatcherCallbackHelper(void *ptr)
	{
		((Toto2DEngine *)ptr)->simpleTileBatcherCallback();
	}
	
	void repeatTileBatcherCallback();
	static void repeatTileBatcherCallbackHelper(void *ptr)
	{
		((Toto2DEngine *)ptr)->repeatTileBatcherCallback();
	}

private:
	EGLDisplay _display;
	EGLSurface _surface;
	int _screenWidth;
	int _screenHeight;

	int currBlendMode;
	glm::mat4 _screenMat;
	glm::mat4 _cameraMat; glm::mat4 _cameraTrans1Mat; glm::mat4 _cameraRotMat;  glm::mat4 _cameraScaleMat; glm::mat4 _cameraTrans2Mat; glm::mat4 _cameraTemp1Mat; glm::mat4 _cameraTemp2Mat;

	struct textureData	{ GLuint id; int width; int height; };
	static const int MAX_ATLAS_SLOT = 9;
	int _currentSlot;
	bool _firstAtlas;
	textureData _atlasses[MAX_ATLAS_SLOT+1];
	struct color { float r; float g; float b; } _bgColor;
	SpriteMatrixBatcher _spriteMatrixBatcher;
	SimpleTileBatcher _simpleTileBatcher;
	RepeatTileBatcher _repeatTileBatcher;
	

};