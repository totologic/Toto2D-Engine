#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils.h"

class SimpleTileBatcher
{
public:
	SimpleTileBatcher(void);
	~SimpleTileBatcher(void);

	static const int MAX_TILES = 40;
	static const int MAX_UVS = 10;
	int getCountTilesAdded();

	int init(GLfloat* screenMat, GLfloat* cameraMat);
	int addUV(int textureLeft, int textureTop, int textureWidth, int textureHeight, int& id);
	void resetUVs();
	int addTile(int uvId, float x, float y);
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
	glm::vec4 _uvData[MAX_UVS];
	GLfloat _uvIndexData[MAX_TILES];
	glm::vec2 _positionData[MAX_TILES];
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
	GLuint _uvUniform;
	GLuint _uvIndexUniform;
	GLuint _positionUniform;
	// ATTRIBUTE
	GLuint _indexAttribute;
	//
	int _uvIndex;
	int _tileIndex;
	//
	
	CallbackPtr _callbackPtr;
	void *_callbackOwner;
};

