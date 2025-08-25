#include "Renderer.h"

Renderer::Renderer(SDL_Renderer* pRenderer)
	: mRenderer(pRenderer)
{}

void Renderer::drawAll(uint32_t pDelay)
{
	mTimer.startTimer();


	mTimer.setProperFPS(pDelay);
}
