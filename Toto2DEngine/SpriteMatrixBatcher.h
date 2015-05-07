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

class SpriteMatrixBatcher
{
public:
	SpriteMatrixBatcher(void);
	~SpriteMatrixBatcher(void);

	static const int MAX_SPRITES = 20;
	int getCountSpritesAdded();

	int init(GLfloat* screenMat, GLfloat* cameraMat);
	int addSprite(int textureLeft, int textureTop, int textureWidth, int textureHeight, float x, float y);
	int addSprite(int textureLeft, int textureTop, int textureWidth, int textureHeight, glm::mat3 &transform);
	void applyTint(unsigned char r, unsigned char g, unsigned char b, unsigned char intensity);
	void applyUnsaturation(unsigned char intensity);
	void activeAtlas(GLuint id, int width, int height);
	int render();
	
	typedef void(*CallbackPtr)(void*);
	void setCallback(CallbackPtr callbackPtr, void *callbackOwner);

private:
	// BASE MATRICES
	GLfloat* _screenMat;
	GLfloat* _cameraMat;
	// DATAS
	vector<GLfloat> _indexData;
	glm::vec4 _spriteData[MAX_SPRITES];
	glm::vec3 _transfLine0Data[MAX_SPRITES];
	glm::vec3 _transfLine1Data[MAX_SPRITES];
	glm::vec2 _effect[MAX_SPRITES];
	vector<GLushort> _elementData;
	// BUFFERS
	GLuint _indexBuffer;
	GLuint _elementBuffer;
	// TEXTURE
	GLuint _atlas;
	int _atlasWidth;
	int _atlasHeight;
	// SHADERS PROGRAM
	GLuint _vertexShader;
	GLuint _fragmentShader;
	GLuint _program;
	// UNIFORM
	GLuint _screenMatUniform;
	GLuint _cameraMatUniform;
	GLuint _atlasUniform;
	GLuint _atlasSizeUniform;
	GLuint _spriteUniform;
	GLuint _transfLine0Uniform;
	GLuint _transfLine1Uniform;
	GLuint _effectUniform;
	// ATTRIBUTE
	GLuint _indexAttribute;
	//
	int _spriteIndex;
	//
	
	CallbackPtr _callbackPtr;
	void *_callbackOwner;

};

