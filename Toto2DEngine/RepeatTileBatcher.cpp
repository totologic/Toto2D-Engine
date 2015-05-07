#include "RepeatTileBatcher.h"


RepeatTileBatcher::RepeatTileBatcher(void)
{
}


RepeatTileBatcher::~RepeatTileBatcher(void)
{
}

int RepeatTileBatcher::getCountTilesAdded()
{
	return _tileIndex;
}

int RepeatTileBatcher::init(GLfloat* screenMat, GLfloat* cameraMat)
{
	_uvIndex = 0;
	_tileIndex = 0;

	// BASE MATRICES
	_screenMat = screenMat;
	_cameraMat = cameraMat;

	// INDEXES DATAS
	int i;
	int j;
	for (i=0 ; i<MAX_TILES ; i++)
	{
		for (j=0 ; j<4 ; j++)
			_indexData.push_back((float)(i*4+j));
		
		_elementData.push_back(0 + 4*i);
		_elementData.push_back(1 + 4*i);
		_elementData.push_back(2 + 4*i);
		_elementData.push_back(2 + 4*i);
		_elementData.push_back(3 + 4*i);
		_elementData.push_back(0 + 4*i);
	}

	// BUFFERING INDEXES DATAS
	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_indexData.size(), &(_indexData[0]), GL_STATIC_DRAW);
	
	glGenBuffers(1, &_elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*_elementData.size(), &(_elementData[0]), GL_STATIC_DRAW );

	// VERTEX SHADER
	string vertexShaderCode;
	ifstream vertexShaderFile ("/opt/vc/src/hello_pi/Toto2DEngine/repeat_tile.vert");
	string lineVertexShader;
	if(vertexShaderFile.is_open())
	{
		while(getline(vertexShaderFile, lineVertexShader))
		vertexShaderCode += "\n" + lineVertexShader;
		vertexShaderFile.close();
	}
	char const *vertexShaderPointer = vertexShaderCode.c_str();
	_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint vertexShaderCodeLength = vertexShaderCode.length();
	glShaderSource(_vertexShader, 1, &vertexShaderPointer , &vertexShaderCodeLength);
	glCompileShader(_vertexShader);

	// FRAGMENT SHADER
	string fragmentShaderCode;
	ifstream fragmentShaderFile ("/opt/vc/src/hello_pi/Toto2DEngine/repeat_tile.frag");
	string lineFragmentShader;
	if(fragmentShaderFile.is_open())
	{
		while(getline(fragmentShaderFile, lineFragmentShader))
		fragmentShaderCode += "\n" + lineFragmentShader;
		fragmentShaderFile.close();
	}
	char const *fragmentShaderPointer = fragmentShaderCode.c_str();
	_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint fragmentShaderCodeLength = fragmentShaderCode.length();
	glShaderSource(_fragmentShader, 1, &fragmentShaderPointer , &fragmentShaderCodeLength);
	glCompileShader(_fragmentShader);
	
	// PROGRAM CREATION
	_program = glCreateProgram();
	glAttachShader(_program, _vertexShader);
	glAttachShader(_program, _fragmentShader);
	glLinkProgram(_program);
	glUseProgram(_program);
	
	// SCREEN MAT4 UNIFORM
	_screenMatUniform = glGetUniformLocation(_program, "screenMat");

	// CAMERA MAT4 UNIFORM
	_cameraMatUniform = glGetUniformLocation(_program, "cameraMat");

	// TEXTURE UNIFORM
	_atlasUniform = glGetUniformLocation(_program, "atlas");
	_atlasSizeUniform = glGetUniformLocation(_program, "atlasSize");

	// SPRITE DATA UNIFORM
	_uvUniform = glGetUniformLocation(_program, "uv");

	// UV INDEX DATA UNIFORM
	_uvIndexUniform = glGetUniformLocation(_program, "uvIndex");

	// POSITION UNIFORM
	_positionUniform = glGetUniformLocation(_program, "position");
	
	// DIMENSION UNIFORM
	_dimensionUniform = glGetUniformLocation(_program, "dimension");

	// SCROLL UNIFORM
	_scrollUniform = glGetUniformLocation(_program, "scroll");
	
	// DISTORTION UNIFORMS
	//_distortionAUniform = glGetUniformLocation(_program, "distortionA");
	//_distortionBUniform = glGetUniformLocation(_program, "distortionB");
	//_distortionTypeUniform = glGetUniformLocation(_program, "distortionType");
	
	// ATTRIBUTE
	_indexAttribute = glGetAttribLocation(_program, "index");
	glBindBuffer(GL_ARRAY_BUFFER, _indexBuffer);
	glEnableVertexAttribArray(_indexAttribute);
	glVertexAttribPointer(_indexAttribute, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	return 1;
}

int RepeatTileBatcher::addUV(int textureLeft, int textureTop, int textureWidth, int textureHeight, int& id)
{
	if (_uvIndex >= MAX_UVS)
		return 0;

	glm::vec4 &uvData = _uvData[_uvIndex];
	uvData[0] = (float)textureLeft;
	uvData[1] = (float)textureTop;
	uvData[2] = (float)textureWidth;
	uvData[3] = (float)textureHeight;

	id = _uvIndex;

	_uvIndex++;
	
	return 1;
}

void RepeatTileBatcher::resetUVs()
{
	_uvIndex = 0;
}

int RepeatTileBatcher::addTiles(int uvId, float x, float y, float width, float height, float scrollX, float scrollY)
{
	_callbackPtr(_callbackOwner);
	
	glm::vec2 &position = _positionData[_tileIndex];
	position[0] = x;
	position[1] = y;
	
	glm::vec2 &dimension = _dimensionData[_tileIndex];
	dimension[0] = width;
	dimension[1] = height;
	
	glm::vec2 &scroll = _scrollData[_tileIndex];
	scroll.x = scrollX;
	scroll.y = scrollY;
	
	glm::vec3 &distortion = _distortionAData[_tileIndex];
	distortion[0] = 0.0f;
	distortion = _distortionBData[_tileIndex];
	distortion[0] = 0.0f;
	_distortionTypeData[_tileIndex] = 0.0f;

	_uvIndexData[_tileIndex] = uvId;

	_tileIndex++;

	_callbackPtr(_callbackOwner);
	return 1;
}

void RepeatTileBatcher::applyDistoWaveVertical(float amplitude, float period, float phase)
{
	if (_distortionTypeData[_tileIndex-1] != 1.0f)
	{
		_distortionAData[_tileIndex-1][0] = 0.0f;
		_distortionTypeData[_tileIndex-1] = 1.0f;
	}

	glm::vec3 &distortion = _distortionBData[_tileIndex-1];
	distortion[0] = amplitude*0.5f/_atlasHeight;
	distortion[1] = 2.0f*M_PI / (period/_atlasWidth);
	distortion[2] = phase/_atlasWidth;
}

void RepeatTileBatcher::applyDistoWaveHorizontal(float amplitude, float period, float phase)
{
	if (_distortionTypeData[_tileIndex-1] != 1.0f)
	{
		_distortionBData[_tileIndex-1][0] = 0.0f;
		_distortionTypeData[_tileIndex-1] = 1.0f;
	}

	glm::vec3 &distortion = _distortionAData[_tileIndex-1];
	distortion[0] = amplitude*0.5f/_atlasWidth;
	distortion[1] = 2.0f*M_PI / (period/_atlasHeight);
	distortion[2] = phase/_atlasHeight;
}

void RepeatTileBatcher::applyDistoAccordeonVertical(float amplitude, float period, float phase)
{
	if (_distortionTypeData[_tileIndex-1] != 2.0f)
	{
		_distortionAData[_tileIndex-1][0] = 0.0f;
		_distortionTypeData[_tileIndex-1] = 2.0f;
	}

	glm::vec3 &distortion = _distortionBData[_tileIndex-1];
	distortion[0] = amplitude*0.5f/_atlasHeight;
	distortion[1] = 2.0f*M_PI / (period/_atlasWidth);
	distortion[2] = phase/_atlasWidth;
}

void RepeatTileBatcher::applyDistoAccordeonHorizontal(float amplitude, float period, float phase)
{
	if (_distortionTypeData[_tileIndex-1] != 2.0f)
	{
		_distortionBData[_tileIndex-1][0] = 0.0f;
		_distortionTypeData[_tileIndex-1] = 2.0f;
	}

	glm::vec3 &distortion = _distortionAData[_tileIndex-1];
	distortion[0] = amplitude*0.5f/_atlasWidth;
	distortion[1] = 2.0f*M_PI / (period/_atlasHeight);
	distortion[2] = phase/_atlasHeight;
}

void RepeatTileBatcher::applyDistoTunnel(float centerX, float centerY, float depth, float rotate, float zScale, float darkMult)
{
	_distortionTypeData[_tileIndex-1] = 3.0f;
	glm::vec3 &distortionA = _distortionAData[_tileIndex-1];
	distortionA[0] = centerX/(_atlasWidth - 1.0f);
	distortionA[1] = centerY/(_atlasHeight - 1.0f);
	distortionA[2] = darkMult;

	glm::vec3 &distortionB = _distortionBData[_tileIndex-1];
	distortionB[0] = depth;
	distortionB[1] = rotate;
	distortionB[2] = zScale;
}

void RepeatTileBatcher::applyDistoTubeVertical(float width, float rotation, bool inside, float lightMult)
{
	_distortionTypeData[_tileIndex-1] = 4.0f;
	glm::vec3 &distortionA = _distortionAData[_tileIndex-1];
	distortionA[0] = width/(_atlasWidth - 1.0f);
	distortionA[1] = rotation;
	distortionA[2] = lightMult;

	glm::vec3 &distortionB = _distortionBData[_tileIndex-1];
	distortionB[0] = inside ? 1.0f : 0.0f;
}

void RepeatTileBatcher::applyDistoTubeHorizontal(float height, float rotation, bool inside, float lightMult)
{
	_distortionTypeData[_tileIndex-1] = 5.0f;
	glm::vec3 &distortionA = _distortionAData[_tileIndex-1];
	distortionA[0] = height/(_atlasHeight - 1.0f);
	distortionA[1] = rotation;
	distortionA[2] = lightMult;

	glm::vec3 &distortionB = _distortionBData[_tileIndex-1];
	distortionB[0] = inside ? 1.0f : 0.0f;
}

void RepeatTileBatcher::applyDistoSphere(float centerX, float centerY, float radius, float rotate,  float zScale, float lightMult)
{
	_distortionTypeData[_tileIndex-1] = 6.0f;
	glm::vec3 &distortionA = _distortionAData[_tileIndex-1];
	distortionA[0] = centerX/(_atlasWidth - 1.0f);
	distortionA[1] = centerY/(_atlasHeight - 1.0f);
	distortionA[2] = lightMult;

	glm::vec3 &distortionB = _distortionBData[_tileIndex-1];
	distortionB[0] = (radius/_atlasWidth)*(radius/_atlasWidth);
	distortionB[1] = rotate;
	distortionB[2] = zScale;
}

void RepeatTileBatcher::activeAtlas(GLuint id, int width, int height)
{
	_atlas = id;
	_atlasWidth = width;
	_atlasHeight = height;
}

int RepeatTileBatcher::render()
{
	glUseProgram(_program);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _atlas);
	glUniform1i(_atlasUniform, 0);
	glUniform2f(_atlasSizeUniform, (float)_atlasWidth, (float)_atlasHeight);
	
	glUniformMatrix4fv(_screenMatUniform, 1, GL_FALSE, _screenMat);
	glUniformMatrix4fv(_cameraMatUniform, 1, GL_FALSE, _cameraMat);

	glUniform4fv(_uvUniform, _uvIndex, &(_uvData[0][0]));
	glUniform1fv(_uvIndexUniform, _tileIndex, &(_uvIndexData[0]));
	glUniform2fv(_positionUniform, _tileIndex, &(_positionData[0][0]));
	glUniform2fv(_dimensionUniform, _tileIndex, &(_dimensionData[0][0]));
	glUniform2fv(_scrollUniform, _tileIndex, &(_scrollData[0][0]));
	//glUniform3fv(_distortionAUniform, _tileIndex, &(_distortionAData[0][0]));
	//glUniform3fv(_distortionBUniform, _tileIndex, &(_distortionBData[0][0]));
	//glUniform1fv(_distortionTypeUniform, _tileIndex, &(_distortionTypeData[0]));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
	glDrawElements(GL_TRIANGLES, 6*_tileIndex, GL_UNSIGNED_SHORT, (void*)0);
	
	_tileIndex = 0;
	
	return 1;
}


void RepeatTileBatcher::setCallback(CallbackPtr callbackPtr, void *callbackOwner)
{
	_callbackPtr = callbackPtr;
	_callbackOwner = callbackOwner;
}


