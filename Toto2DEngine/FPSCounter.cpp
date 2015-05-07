#include "FPSCounter.h"

FPSCounter::FPSCounter(void)
{
	
}

FPSCounter::~FPSCounter(void)
{
}

void FPSCounter::init(Toto2DEngine *toto2DEngine, int fontAtlasSlot)
{
	toto2d = toto2DEngine;
	atlasSlot = fontAtlasSlot;
	elapsTime = 16;
	histoLength = 10;
	avgElapsTime = 0;
	
	int width, height, channels;
	void* pixels = stbi_load("/opt/vc/src/hello_pi/Toto2DEngine/numerics.png"
				, &width, &height, &channels, STBI_rgb_alpha );
	toto2d->uploadAtlas(atlasSlot, pixels, width, height);
}

void FPSCounter::start()
{
	clock_gettime(CLOCK_REALTIME, &clock);
	prevTime = clock.tv_sec * 1000 + clock.tv_nsec / 1000000;
}

void FPSCounter::compute()
{
	clock_gettime(CLOCK_REALTIME, &clock);
	currTime = clock.tv_sec * 1000 + clock.tv_nsec / 1000000;
	elapsTime = currTime - prevTime;
	histoElapsTime.push_back(elapsTime);
	if (histoElapsTime.size() == histoLength+1)
	{
		histoElapsTime.erase(histoElapsTime.begin());
		sumElapsTime = 0;
		for (int i=0 ; i<histoLength ; i++)
			sumElapsTime += histoElapsTime[i];
		avgElapsTime = sumElapsTime / histoLength;
	}
	prevTime = currTime;
}

void FPSCounter::display()
{
	int currSlot = toto2d->getCurrentAtlas();
	
	toto2d->activeAtlas(atlasSlot);
	digit1 = elapsTime % 10;
	digit2 = ((elapsTime - digit1) / 10) % 10;
	digit3 = ((elapsTime - digit1 - digit2) / 100) % 10;
	toto2d->getSpriteBatcher().addSprite(26*digit3, 0, 26, 40, 50.0f, 50.0f);
	toto2d->getSpriteBatcher().addSprite(26*digit2, 0, 26, 40, 76.0f, 50.0f);
	toto2d->getSpriteBatcher().addSprite(26*digit1, 0, 26, 40, 102.0f, 50.0f);
	digit1 = avgElapsTime % 10;
	digit2 = ((avgElapsTime - digit1) / 10) % 10;
	digit3 = ((avgElapsTime - digit1 - digit2) / 100) % 10;
	toto2d->getSpriteBatcher().addSprite(26*digit3, 0, 26, 40, 50.0f, 90.0f);
	toto2d->getSpriteBatcher().addSprite(26*digit2, 0, 26, 40, 76.0f, 90.0f);
	toto2d->getSpriteBatcher().addSprite(26*digit1, 0, 26, 40, 102.0f, 90.0f);
	
	toto2d->activeAtlas(currSlot);
}
