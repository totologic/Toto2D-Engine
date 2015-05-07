#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#define _USE_MATH_DEFINES
using namespace std;
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils.h"

class RepeatTileBatcher
{
public:
	RepeatTileBatcher(void);
	~RepeatTileBatcher(void);

	static const int MAX_TILES = 40;
	static const int MAX_UVS = 10;
	int getCountTilesAdded();

	int init(GLfloat* screenMat, GLfloat* cameraMat);
	int addUV(int textureLeft, int textureTop, int textureWidth, int textureHeight, int& id);
	void resetUVs();
	int addTiles(int uvId, float x, float y, float width, float height, float scrollX, float scrollY);
	void applyDistoWaveVertical(float amplitude, float period, float phase);
	void applyDistoWaveHorizontal(float amplitude, float period, float phase);
	void applyDistoAccordeonVertical(float amplitude, float period, float phase);
	void applyDistoAccordeonHorizontal(float amplitude, float period, float phase);
	void applyDistoTunnel(float centerX, float centerY, float depth, float rotate, float zScale, float darkMult);
	void applyDistoTubeVertical(float width, float rotation, bool inside, float lightMult);
	void applyDistoTubeHorizontal(float width, float rotation, bool inside, float lightMult);
	void applyDistoSphere(float centerX, float centerY, float radius, float rotate, float zScale, float lightMult);
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
	glm::vec2 _dimensionData[MAX_TILES];
	glm::vec2 _scrollData[MAX_TILES];
	glm::vec3 _distortionAData[MAX_TILES];
	glm::vec3 _distortionBData[MAX_TILES];
	GLfloat _distortionTypeData[MAX_TILES];
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
	GLuint _dimensionUniform;
	GLuint _scrollUniform;
	GLuint _distortionAUniform;
	GLuint _distortionBUniform;
	GLuint _distortionTypeUniform;
	// ATTRIBUTE
	GLuint _indexAttribute;
	//
	int _uvIndex;
	int _tileIndex;
	//

	CallbackPtr _callbackPtr;
	void *_callbackOwner;
};

