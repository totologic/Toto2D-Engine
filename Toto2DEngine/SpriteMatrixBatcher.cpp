#include "SpriteMatrixBatcher.h"


SpriteMatrixBatcher::SpriteMatrixBatcher(void)
{
}


SpriteMatrixBatcher::~SpriteMatrixBatcher(void)
{
}

int SpriteMatrixBatcher::getCountSpritesAdded()
{
	return _spriteIndex;
}

int SpriteMatrixBatcher::init(GLfloat* screenMat, GLfloat* cameraMat)
{
	_spriteIndex = 0;
	
	// BASE MATRICES
	_screenMat = screenMat;
	_cameraMat = cameraMat;

	// INDEXES DATAS
	int i;
	int j;
	for (i=0 ; i<MAX_SPRITES ; i++)
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
	ifstream vertexShaderFile("/opt/vc/src/hello_pi/Toto2DEngine/sprite_matrix.vert");
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
	ifstream fragmentShaderFile("/opt/vc/src/hello_pi/Toto2DEngine/sprite_matrix.frag");
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
	_spriteUniform = glGetUniformLocation(_program, "sprite");

	// TRANSF UNIFORM
	_transfLine0Uniform = glGetUniformLocation(_program, "transfLine0");
	_transfLine1Uniform = glGetUniformLocation(_program, "transfLine1");

	// EFFECT UNIFORM
	_effectUniform = glGetUniformLocation(_program, "effect");

	// ATTRIBUTE
	_indexAttribute = glGetAttribLocation(_program, "index");
	glBindBuffer(GL_ARRAY_BUFFER, _indexBuffer);
	glEnableVertexAttribArray(_indexAttribute);
	glVertexAttribPointer(_indexAttribute, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	
	return 1;
}

int  SpriteMatrixBatcher::addSprite(int textureLeft, int textureTop, int textureWidth, int textureHeight, float x, float y)
{
	_callbackPtr(_callbackOwner);
	
	glm::vec4 &spriteData = _spriteData[_spriteIndex];
	spriteData[0] = (float)textureLeft;
	spriteData[1] = (float)textureTop;
	spriteData[2] = (float)textureWidth;
	spriteData[3] = (float)textureHeight;
	
	glm::vec3 &line0Data = _transfLine0Data[_spriteIndex];
	glm::vec3 &line1Data = _transfLine1Data[_spriteIndex];
	line0Data[0] = 1.0f;
	line1Data[0] = 0.0f;
	line0Data[1] = 0.0f;
	line1Data[1] = 1.0f;
	line0Data[2] = x;
	line1Data[2] = y;

	_effect[_spriteIndex][0] = 0.0f;
	
	_spriteIndex++;

	return 1;
}

int SpriteMatrixBatcher::addSprite(int textureLeft, int textureTop, int textureWidth, int textureHeight, glm::mat3 &transform)
{
	_callbackPtr(_callbackOwner);

	glm::vec4 &spriteData = _spriteData[_spriteIndex];
	spriteData[0] = (float)textureLeft;
	spriteData[1] = (float)textureTop;
	spriteData[2] = (float)textureWidth;
	spriteData[3] = (float)textureHeight;

	glm::vec3 &line0Data = _transfLine0Data[_spriteIndex];
	glm::vec3 &line1Data = _transfLine1Data[_spriteIndex];
	line0Data[0] = transform[0][0];
	line1Data[0] = transform[0][1];
	line0Data[1] = transform[1][0];
	line1Data[1] = transform[1][1];
	line0Data[2] = transform[2][0];
	line1Data[2] = transform[2][1];

	_effect[_spriteIndex][0] = 0.0f;

	_spriteIndex++;
	
	return 1;
}

void SpriteMatrixBatcher::applyTint(unsigned char r, unsigned char g, unsigned char b, unsigned char intensity)
{
	_effect[_spriteIndex-1][0] = 1.0f + intensity * 10.0f;
	_effect[_spriteIndex-1][1] = r + g * 256.0f + b * 65536.0f;
}

void SpriteMatrixBatcher::applyUnsaturation(unsigned char intensity)
{
	_effect[_spriteIndex-1][0] = 2.0f + intensity * 10.0f;
}

void SpriteMatrixBatcher::activeAtlas(GLuint id, int width, int height)
{
	_atlas = id;
	_atlasWidth = width;
	_atlasHeight = height;
}

int SpriteMatrixBatcher::render()
{
	glUseProgram(_program);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _atlas);
	glUniform1i(_atlasUniform, 0);
	glUniform2f(_atlasSizeUniform, (float)_atlasWidth, (float)_atlasHeight);

	glUniformMatrix4fv(_screenMatUniform, 1, GL_FALSE, _screenMat);
	glUniformMatrix4fv(_cameraMatUniform, 1, GL_FALSE, _cameraMat);

	glUniform4fv(_spriteUniform, _spriteIndex, &(_spriteData[0][0]));
	glUniform3fv(_transfLine0Uniform, _spriteIndex, &(_transfLine0Data[0][0]));
	glUniform3fv(_transfLine1Uniform, _spriteIndex, &(_transfLine1Data[0][0]));
	glUniform2fv(_effectUniform, _spriteIndex, &(_effect[0][0]));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
	glDrawElements(GL_TRIANGLES, 6*_spriteIndex, GL_UNSIGNED_SHORT, (void*)0);

	_spriteIndex = 0;
	
	return 1;
}

void SpriteMatrixBatcher::setCallback(CallbackPtr callbackPtr, void *callbackOwner)
{
	_callbackPtr = callbackPtr;
	_callbackOwner = callbackOwner;
}