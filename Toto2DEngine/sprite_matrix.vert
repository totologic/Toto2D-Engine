
uniform mat4 screenMat;
uniform mat4 cameraMat;
uniform vec2 atlasSize;
uniform vec4 sprite[20];
uniform vec3 transfLine0[20];
uniform vec3 transfLine1[20];
uniform vec2 effect[20];

attribute float index;

varying vec2 interpolatedUV;
varying vec2 spriteEffect;

void main()
{
	int spriteIndex = int(floor(index*0.25));
	int vertexIndex = int(index-floor(index*0.25)*4.0);
	
	spriteEffect = effect[spriteIndex];
	
	mat4 transfMat;
	vec3 line0 = transfLine0[spriteIndex];
	vec3 line1 = transfLine1[spriteIndex];
	transfMat[0][0] = line0[0];	transfMat[1][0] = line0[1];	transfMat[2][0] = 0.0;	transfMat[3][0] = line0[2];
	transfMat[0][1] = line1[0];	transfMat[1][1] = line1[1];	transfMat[2][1] = 0.0;	transfMat[3][1] = line1[2];
	transfMat[0][2] = 0.0;		transfMat[1][2] = 0.0;		transfMat[2][2] = 1.0;	transfMat[3][2] = 0.0;
	transfMat[0][3] = 0.0;		transfMat[1][3] = 0.0;		transfMat[2][3] = 0.0;	transfMat[3][3] = 1.0;
	
	vec4 vertexData = sprite[spriteIndex];
	vec4 vertex = vec4(0.0, 0.0, 0.0, 1.0);
	vec2 uv;
	if (vertexIndex == 0)
	{
		vertex[0] = 0.0;
		vertex[1] = 0.0;
		uv[0] = vertexData[0] / atlasSize[0];
		uv[1] = vertexData[1] / atlasSize[1];
	}
	else if (vertexIndex == 1)
	{
		vertex[0] = 0.0;
		vertex[1] = vertexData[3];
		uv[0] = vertexData[0] / atlasSize[0];
		uv[1] = (vertexData[1] + vertexData[3]) / atlasSize[1];
	}
	else if (vertexIndex == 2)
	{
		vertex[0] = vertexData[2];
		vertex[1] = vertexData[3];
		uv[0] = (vertexData[0] + vertexData[2]) / atlasSize[0];
		uv[1] = (vertexData[1] + vertexData[3]) / atlasSize[1];
	}
	else if (vertexIndex == 3)
	{
		vertex[0] = vertexData[2];
		vertex[1] = 0.0;
		uv[0] = (vertexData[0] + vertexData[2]) / atlasSize[0];
		uv[1] = vertexData[1] / atlasSize[1];
	}

	gl_Position = screenMat * cameraMat * transfMat * vertex;
	interpolatedUV = uv;
}
