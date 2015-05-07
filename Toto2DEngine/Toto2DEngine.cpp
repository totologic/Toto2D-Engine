#include "Toto2DEngine.h"


Toto2DEngine::Toto2DEngine(void)
{
}


Toto2DEngine::~Toto2DEngine(void)
{
}

void Toto2DEngine::setBlendModeNormal()
{
	if (currBlendMode == 0)
		return;
	
	if (_repeatTileBatcher.getCountTilesAdded() > 0)
		_repeatTileBatcher.render();
	if (_simpleTileBatcher.getCountTilesAdded() > 0)
		_simpleTileBatcher.render();
	if (_spriteMatrixBatcher.getCountSpritesAdded() > 0)
		_spriteMatrixBatcher.render();
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	
	currBlendMode = 0;
}

void Toto2DEngine::setBlendModeAdditive()
{
	if (currBlendMode == 1)
		return;
	
	if (_repeatTileBatcher.getCountTilesAdded() > 0)
		_repeatTileBatcher.render();
	if (_simpleTileBatcher.getCountTilesAdded() > 0)
		_simpleTileBatcher.render();
	if (_spriteMatrixBatcher.getCountSpritesAdded() > 0)
		_spriteMatrixBatcher.render();
	glBlendFunc(GL_ONE, GL_ONE);
	
	currBlendMode = 1;
}

void Toto2DEngine::init(uint32_t screenWidth, uint32_t screenHeight, EGLDisplay display, EGLSurface surface)
{
	_display = display;
	_surface = surface;
	
	currBlendMode = 0;
	glEnable(GL_BLEND);	
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	
	// SCREEN PROJECTION MATRIX
	glm::mat4 screenScaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f/(float)screenWidth, 2.0f/(float)screenHeight, 1.0f));
	glm::mat4 screenTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f));
	glm::mat4 screenAxisMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));
	_screenMat = screenAxisMat*screenTransMat*screenScaleMat;

	// CAMERA MATRICES
	_cameraTrans1Mat = glm::mat4(1.0f);
	_cameraScaleMat = glm::mat4(1.0f);
	_cameraRotMat = glm::mat4(1.0f);
	_cameraTrans2Mat = glm::mat4(1.0f);
	_cameraMat = glm::mat4(1.0f);

	// NO ATLAS UPLOADED
	_firstAtlas = true;
	
	// DEFAUT BG COLOR
	setBGColor(1.0f, 1.0f, 1.0f);
	
	// LAYERS INIT
	_spriteMatrixBatcher.init(glm::value_ptr(_screenMat), glm::value_ptr(_cameraMat));
	_simpleTileBatcher.init(glm::value_ptr(_screenMat), glm::value_ptr(_cameraMat));
	_repeatTileBatcher.init(glm::value_ptr(_screenMat), glm::value_ptr(_cameraMat));
	
	// SET CALLBACKS
	_spriteMatrixBatcher.setCallback(&spriteMatrixBatcherCallbackHelper, this);
	_simpleTileBatcher.setCallback(&simpleTileBatcherCallbackHelper, this);
	_repeatTileBatcher.setCallback(&repeatTileBatcherCallbackHelper, this);
}

void Toto2DEngine::setCamera(float x, float y)
{
	 _cameraMat[3][0] = -x;
	 _cameraMat[3][1] = -y;
}

void Toto2DEngine::setCameraLookAt(float x, float y, float rotate, float zoom)
{
	_cameraTrans1Mat[3][0] = -x;
	_cameraTrans1Mat[3][1] = -y;
	_cameraScaleMat[0][0] = zoom;
	_cameraScaleMat[1][1] = zoom;
	_cameraRotMat[0][0] = cos(-rotate);
	_cameraRotMat[1][0] = -sin(-rotate);
	_cameraRotMat[0][1] = sin(-rotate);
	_cameraRotMat[1][1] = cos(-rotate);
	_cameraTrans2Mat[3][0] = floor(_screenWidth/2.0f);
	_cameraTrans2Mat[3][1] = floor(_screenHeight/2.0f);
	Utils::multMat4(_cameraScaleMat, _cameraTrans1Mat, _cameraTemp1Mat);
	Utils::multMat4(_cameraTrans2Mat, _cameraRotMat, _cameraTemp2Mat);
	Utils::multMat4(_cameraTemp2Mat, _cameraTemp1Mat, _cameraMat);
}

void Toto2DEngine::setCameraMatrix(glm::mat4 &transform)
{
	 _cameraMat = glm::inverse(transform);
}

int Toto2DEngine::uploadAtlas(int slot, void* pixels, int width, int height)
{
	if (slot > MAX_ATLAS_SLOT)
	{
		cout << "Max atlas slot is " + MAX_ATLAS_SLOT << endl;
		return 0;
	}

	_atlasses[slot].width = width;
	_atlasses[slot].height = height;
	
	glGenTextures(1, &(_atlasses[slot].id));
	glBindTexture(GL_TEXTURE_2D, _atlasses[slot].id);
	glTexImage2D( GL_TEXTURE_2D, 0				/* target, level of detail */
				, GL_RGBA			/* internal format */
				, width				/* width */
				, height, 0			/* height, border */
				, GL_RGBA, GL_UNSIGNED_BYTE	/* external format, type */
				, pixels			/* pixels */ );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST, GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_NEAREST, GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
	
	if (_firstAtlas)
	{
		activeAtlas(slot);
		_firstAtlas = false;
	}

	return 1;
}

void Toto2DEngine::activeAtlas(int slot)
{
	if (_repeatTileBatcher.getCountTilesAdded() > 0)
		_repeatTileBatcher.render();
	if (_simpleTileBatcher.getCountTilesAdded() > 0)
		_simpleTileBatcher.render();
	if (_spriteMatrixBatcher.getCountSpritesAdded() > 0)
		_spriteMatrixBatcher.render();
	
	_spriteMatrixBatcher.activeAtlas(_atlasses[slot].id, _atlasses[slot].width, _atlasses[slot].height);
	_simpleTileBatcher.activeAtlas(_atlasses[slot].id, _atlasses[slot].width, _atlasses[slot].height);
	_repeatTileBatcher.activeAtlas(_atlasses[slot].id, _atlasses[slot].width, _atlasses[slot].height);
	
	_currentSlot = slot;
}

int Toto2DEngine::getCurrentAtlas()
{
	return _currentSlot;
}

void Toto2DEngine::setBGColor(float r, float g, float b)
{
	_bgColor.r = r;
	_bgColor.g = g;
	_bgColor.b = b;
}

SpriteMatrixBatcher& Toto2DEngine::getSpriteBatcher()
{
	return _spriteMatrixBatcher;
}

SimpleTileBatcher& Toto2DEngine::getSimpleTileBatcher()
{
	return _simpleTileBatcher;
}

RepeatTileBatcher& Toto2DEngine::getRepeatTileBatcher()
{
	return _repeatTileBatcher;
}

void Toto2DEngine::clear()
{
	_simpleTileBatcher.resetUVs();
	_repeatTileBatcher.resetUVs();
	glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Toto2DEngine::swap()
{
	if (_repeatTileBatcher.getCountTilesAdded() > 0)
		_repeatTileBatcher.render();
	if (_simpleTileBatcher.getCountTilesAdded() > 0)
		_simpleTileBatcher.render();
	if (_spriteMatrixBatcher.getCountSpritesAdded() > 0)
		_spriteMatrixBatcher.render();
	
	glFlush();
	glFinish();
	eglSwapBuffers(_display, _surface);
}

void Toto2DEngine::spriteMatrixBatcherCallback()
{
	if (_repeatTileBatcher.getCountTilesAdded() > 0)
		_repeatTileBatcher.render();
	if (_simpleTileBatcher.getCountTilesAdded() > 0)
		_simpleTileBatcher.render();
	
	if (_spriteMatrixBatcher.getCountSpritesAdded() == SpriteMatrixBatcher::MAX_SPRITES)
		_spriteMatrixBatcher.render();
}

void Toto2DEngine::simpleTileBatcherCallback()
{
	if (_repeatTileBatcher.getCountTilesAdded() > 0)
		_repeatTileBatcher.render();
	if (_spriteMatrixBatcher.getCountSpritesAdded() > 0)
		_spriteMatrixBatcher.render();
	
	if (_simpleTileBatcher.getCountTilesAdded() == SimpleTileBatcher::MAX_TILES)
		_simpleTileBatcher.render();
}

void Toto2DEngine::repeatTileBatcherCallback()
{
	if (_simpleTileBatcher.getCountTilesAdded() > 0)
		_simpleTileBatcher.render();
	if (_spriteMatrixBatcher.getCountSpritesAdded() > 0)
		_spriteMatrixBatcher.render();
	
	if (_repeatTileBatcher.getCountTilesAdded() == RepeatTileBatcher::MAX_TILES)
		_repeatTileBatcher.render();
}
