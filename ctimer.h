#include "SDL/SDL.h"

#include <math.h>

class cTimer
{
	private:
		int StartTicks;
		bool Running;
	public:
		void Reset();
		
		void Start();
		void Stop();
		
		int GetTicks();
		bool Counting();
};

void cTimer::Reset()
{
	StartTicks = 0;
	Running = false;
}

void cTimer::Start()
{
	StartTicks = SDL_GetTicks();
	Running = true;
}

void cTimer::Stop()
{
	Running = false;
}

int cTimer::GetTicks()
{
	return SDL_GetTicks() - StartTicks;
}

bool cTimer::Counting()
{
	return Running;
}
