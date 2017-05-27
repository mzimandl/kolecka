#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "ctimer.h"

#define width 1024
#define height 768
#define bpp 0

#define Tick 40

#define Xc 28
#define Yc 20
#define minRc 2
#define maxRc 18

#define w 10000

SDL_Surface *screen = NULL;
SDL_Event event;

int dXc, dYc;
int Xod, Yod;
int poleX, poleY;
int stred,rozptyl;

float px,py,vx,vy;
float velikost = 0;
#define wheelstep 0.1

cTimer Timer;

int round(float cislo)
{
	return (int)(cislo+0.5); 
}

bool init()
{
	int i,j;
	
	printf("Inicializuji SDL ...");
	
	if ( SDL_Init(SDL_INIT_EVERYTHING) == -1 )
	{
		printf("\n Nemohu inicializovat SDL: %s\n", SDL_GetError());
		return false;
	}
	
	screen = SDL_SetVideoMode(width, height, bpp, SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( screen == NULL )
	{
		printf("\n Nemohu nastavit obraz: %s\n", SDL_GetError());
		return false;
	}
	
	printf(" hotovo\n");
	
	printf("Nacitam data ...");
	printf(" hotovo\n");
	
	printf("Nastavuji zbytek ...");
	srand(time(0));
	
	dXc = width/(Xc+1);
	dYc = height/(Yc+1);
	if (dXc>dYc)
	{
		dXc=dYc;
		Xod=(width-(Xc+1)*dXc)/2;
		Yod=0;
	} else {
		dYc=dXc;
		Xod=0;
		Yod=(height-(Yc+1)*dYc)/2;
	}
	poleX = dXc*Xc;
	poleY = dYc*Yc;
	
	px = rand()%(poleX+1);
	py = rand()%(poleY+1);
	
	vx = rand()%101 - 50;
	vy = rand()%101 - 50;
	
	stred = (maxRc-minRc)/2 + minRc;
	rozptyl = maxRc - stred;
	
	printf(" hotovo\n");
	
	return true;
}

void final()
{
	printf("Ukoncuji ...");
	SDL_Quit();
	printf(" hotovo\n");
}

void vykresli_castice(int x, int y, float rust)
{
	int i,j;
	float polomer,polomer2;
	
	for (i=0;i<Xc;i++)
	{
		for (j=0;j<Yc;j++)
		{
			polomer = rust*exp( -( pow(Xod+dXc*(i+1)-x,2) + pow(Yod+dYc*(j+1)-y,2) )/w )*rozptyl;
			polomer2 = stred + exp( -( pow(dXc*i-px,2) + pow(dYc*j-py,2) )/w )*rozptyl;
						
			if (rust<0)
				filledCircleRGBA(screen,Xod+dXc*(i+1),Yod+dYc*(j+1),round(polomer+polomer2),(int)(255*(-polomer)/rozptyl),(int)(255*(polomer2-stred)/rozptyl),0,255);
			else
				filledCircleRGBA(screen,Xod+dXc*(i+1),Yod+dYc*(j+1),round(polomer+polomer2),0,(int)(255*(polomer2-stred)/rozptyl),(int)(255*(polomer)/rozptyl),255);
			aacircleRGBA(screen,Xod+dXc*(i+1),Yod+dYc*(j+1),round(polomer+polomer2),50,50,50,255);
		}
	}
}

void vykresli_kliknuti(int x, int y, float rust, float relative)
{
	#define min 50
	#define minrust 50
	#define zvetseni 100
	float pomocne;
	
	if (relative < 0) pomocne = 1+relative;
	else pomocne = 1-relative;
	
	if (rust == 0) aacircleRGBA(screen,x,y,min,100,100,100,round(200*pomocne));
	else {
		aacircleRGBA(screen,x,y,min,100,100,100,round(100*pomocne));
		
		aacircleRGBA(screen,x,y,round(min + rust*rust*minrust + rust*relative*zvetseni),100,100,100,round(200*pomocne));
		aacircleRGBA(screen,x,y,round(min + rust*rust*minrust + rust*relative*zvetseni*2/3),100,100,100,round(200*pomocne));
		aacircleRGBA(screen,x,y,round(min + rust*rust*minrust + rust*relative*zvetseni/3),100,100,100,round(200*pomocne));
	}
}

int main(int argc, char **argv)
{	
	#define maxucin 10.0
	
	if ( !init() ) return 1;
	
	bool cycling = true;
	
	int posx,posy;
	bool lklik, rklik, vyrovnat = false;
	int ucinek = 0;
	
	while ( cycling )
	{
		Timer.Start();
		
		//Udalosti		
		while ( SDL_PollEvent(&event) )
		{
			switch ( event.type )
			{
				case SDL_KEYDOWN:
					switch ( event.key.keysym.sym )
					{
						case SDLK_ESCAPE:
							cycling = false;
							break;
					}
					break;				
				case SDL_QUIT:
					cycling = false;
					break;
				case SDL_MOUSEMOTION:
					posx = event.motion.x;
					posy = event.motion.y;
					break;
				case SDL_MOUSEBUTTONDOWN:
					switch ( event.button.button )
					{
						case SDL_BUTTON_MIDDLE:
							velikost = 0;
							break;
						case SDL_BUTTON_WHEELUP:
							velikost+=wheelstep;
							break;
						case SDL_BUTTON_WHEELDOWN:
							velikost-=wheelstep;
							break;
					}
					break;
				case SDL_ACTIVEEVENT:   
					if(event.active.state != SDL_APPMOUSEFOCUS) if(event.active.gain == 1)
						{   
      							SDL_ShowCursor(0);   
						} else {
      							SDL_ShowCursor(1);   
     						}   
				break;
			}
		}
		
		if ( velikost > 1 ) velikost = 1; else if ( velikost < 0 ) velikost = 0;
		
		if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1) && !rklik) 
			lklik = true;
			else lklik = false;
		
		if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(3) && !lklik)
			rklik = true;
			else rklik = false;
		
		if (lklik) {
			ucinek += 1;
			if (ucinek>maxucin) ucinek=maxucin;
			vyrovnat = true;
		} else if (rklik) {
			ucinek -= 1;
			if (ucinek<-maxucin) ucinek=-maxucin;
			vyrovnat = true;
		} else if (vyrovnat) {
			if (ucinek>0) ucinek -= 1;
			else if (ucinek<0) ucinek += 1;
			if (ucinek==0) vyrovnat = false;
		}
		//Konec Udalosti
	
		//Fyzika
		vx += -velikost*ucinek/maxucin*20*(posx-px-Xod-dXc)*Tick*exp( -( pow(posx-px-Xod-dXc,2) + pow(posy-py-Yod-dYc,2) )/(2*w) )/w;
		vy += -velikost*ucinek/maxucin*20*(posy-py-Yod-dYc)*Tick*exp( -( pow(posx-px-Xod-dXc,2) + pow(posy-py-Yod-dYc,2) )/(2*w) )/w;
		
		if ((px+vx*Tick/1000<0) || (px+vx*Tick/1000>poleX)) vx = -vx;
		if ((py+vy*Tick/1000<0) || (py+vy*Tick/1000>poleY)) vy = -vy;
		
		px += vx*Tick/1000;
		py += vy*Tick/1000;
		//Konec Fyzika
		
		//Vykreslovani
		SDL_FillRect(screen,NULL,0);
		vykresli_castice(posx, posy, velikost*ucinek/maxucin);
		vykresli_kliknuti(posx, posy, velikost, ucinek/maxucin);
		SDL_Flip(screen);
		//Konec Vykreslovani
		
		while (Timer.GetTicks() < Tick) {}
	}
	
	final();
	
	return 0;
}
