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

class Utils
{
public:
	Utils(void);
	~Utils(void);

	static void multMat3(glm::mat3 &matA, glm::mat3 &matB, glm::mat3 &matResult)
	{
		matResult[0][0] = matA[0][0] * matB[0][0] + matA[1][0] * matB[0][1] + matA[2][0] * matB[0][2];
		matResult[1][0] = matA[0][0] * matB[1][0] + matA[1][0] * matB[1][1] + matA[2][0] * matB[1][2];
		matResult[2][0] = matA[0][0] * matB[2][0] + matA[1][0] * matB[2][1] + matA[2][0] * matB[2][2];

		matResult[0][1] = matA[0][1] * matB[0][0] + matA[1][1] * matB[0][1] + matA[2][1] * matB[0][2];
		matResult[1][1] = matA[0][1] * matB[1][0] + matA[1][1] * matB[1][1] + matA[2][1] * matB[1][2];
		matResult[2][1] = matA[0][1] * matB[2][0] + matA[1][1] * matB[2][1] + matA[2][1] * matB[2][2];

		matResult[0][2] = matA[0][2] * matB[0][0] + matA[1][2] * matB[0][1] + matA[2][2] * matB[0][2];
		matResult[1][2] = matA[0][2] * matB[1][0] + matA[1][2] * matB[1][1] + matA[2][2] * matB[1][2];
		matResult[2][2] = matA[0][2] * matB[2][0] + matA[1][2] * matB[2][1] + matA[2][2] * matB[2][2];
	}

	static void multMat4(glm::mat4 &matA, glm::mat4 &matB, glm::mat4 &matRes)
	{
		matRes[0][0] = matA[0][0] * matB[0][0] + matA[1][0] * matB[0][1] + matA[2][0] * matB[0][2] + matA[3][0] * matB[0][3];
		matRes[1][0] = matA[0][0] * matB[1][0] + matA[1][0] * matB[1][1] + matA[2][0] * matB[1][2] + matA[3][0] * matB[1][3];
		matRes[2][0] = matA[0][0] * matB[2][0] + matA[1][0] * matB[2][1] + matA[2][0] * matB[2][2] + matA[3][0] * matB[2][3];
		matRes[3][0] = matA[0][0] * matB[3][0] + matA[1][0] * matB[3][1] + matA[2][0] * matB[3][2] + matA[3][0] * matB[3][3];

		matRes[0][1] = matA[0][1] * matB[0][0] + matA[1][1] * matB[0][1] + matA[2][1] * matB[0][2] + matA[3][1] * matB[0][3];
		matRes[1][1] = matA[0][1] * matB[1][0] + matA[1][1] * matB[1][1] + matA[2][1] * matB[1][2] + matA[3][1] * matB[1][3];
		matRes[2][1] = matA[0][1] * matB[2][0] + matA[1][1] * matB[2][1] + matA[2][1] * matB[2][2] + matA[3][1] * matB[2][3];
		matRes[3][1] = matA[0][1] * matB[3][0] + matA[1][1] * matB[3][1] + matA[2][1] * matB[3][2] + matA[3][1] * matB[3][3];

		matRes[0][2] = matA[0][2] * matB[0][0] + matA[1][2] * matB[0][1] + matA[2][2] * matB[0][2] + matA[3][2] * matB[0][3];
		matRes[1][2] = matA[0][2] * matB[1][0] + matA[1][2] * matB[1][1] + matA[2][2] * matB[1][2] + matA[3][2] * matB[1][3];
		matRes[2][2] = matA[0][2] * matB[2][0] + matA[1][2] * matB[2][1] + matA[2][2] * matB[2][2] + matA[3][2] * matB[2][3];
		matRes[3][2] = matA[0][2] * matB[3][0] + matA[1][2] * matB[3][1] + matA[2][2] * matB[3][2] + matA[3][2] * matB[3][3];

		matRes[0][3] = matA[0][3] * matB[0][0] + matA[1][3] * matB[0][1] + matA[2][3] * matB[0][2] + matA[3][3] * matB[0][3];
		matRes[1][3] = matA[0][3] * matB[1][0] + matA[1][3] * matB[1][1] + matA[2][3] * matB[1][2] + matA[3][3] * matB[1][3];
		matRes[2][3] = matA[0][3] * matB[2][0] + matA[1][3] * matB[2][1] + matA[2][3] * matB[2][2] + matA[3][3] * matB[2][3];
		matRes[3][3] = matA[0][3] * matB[3][0] + matA[1][3] * matB[3][1] + matA[2][3] * matB[3][2] + matA[3][3] * matB[3][3];
	}
	
	static float cRot, sRot, sxcRot, sxsRot, sycRot, sysRot;
	static void mat3TRST(float& t2x, float &t2y, float &rot, float &sx, float &sy, float &t1x, float &t1y, glm::mat3 &matRes)
	{
		cRot = cos(rot);
		sRot = sin(rot);
		sxcRot = sx*cRot;
		sxsRot = sx*sRot;
		sycRot = sy*cRot;
		sysRot = sy*-sRot;
		matRes[0][0] = sxcRot;
		matRes[1][0] = sysRot;
		matRes[2][0] = t1x*sxcRot + t1y*sysRot + t2x;
		matRes[0][1] = sxsRot;
		matRes[1][1] = sycRot;
		matRes[2][1] = t1x*sxsRot + t1y*sycRot + t2y;
		matRes[0][2] = 0.0f;
		matRes[1][2] = 0.0f;
		matRes[2][2] = 1.0f;
	}

};

float Utils::cRot = 0.0;
float Utils::sRot = 0.0;
float Utils::sxcRot = 0.0;
float Utils::sxsRot = 0.0;
float Utils::sycRot = 0.0;
float Utils::sysRot = 0.0;
