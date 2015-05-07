#pragma once
#include <iostream>
using namespace std;
#include <time.h>
#include <vector>
#include "stb_image_aug.h"
#include "Toto2DEngine.h"

class FPSCounter
{
public:
	FPSCounter(void);
	~FPSCounter(void);
	
	void init(Toto2DEngine *toto2DEngine, int fontAtlasSlot);
	void start();
	void compute();
	void display();

private:
	Toto2DEngine *toto2d;
	int atlasSlot;
	timespec clock;
	unsigned long long int prevTime;
	unsigned long long int currTime;
	int elapsTime;
	vector<int> histoElapsTime;
	int histoLength;
	int sumElapsTime;
	int avgElapsTime;
	int digit1;
	int digit2;
	int digit3;
};