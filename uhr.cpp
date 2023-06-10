#include "SDL2/SDL.h"
#include "time.h"
#include <SDL2/SDL_stdinc.h>

class Timer{
	int windowWidth;
	int windowHeight;

	char dateText[9];
	int wait;
	char mode;
	
	bool quit;
	bool change;
	char sec;
	char oldSec;

	SDL_Texture* textures[7];
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;

	bool initSDL();
	void closeSDL();
	void getTime();
	void incrementTime();
	void handleInput();
	SDL_Texture *drawNumber(char number, int width, int height, unsigned char red, unsigned char green, unsigned char blue);
	void draw0();
	void draw1();

public:
	Timer(int width, int height);
	void run();
};

Timer::Timer(int width, int height){
	windowWidth = width;
	windowHeight = height;

	gWindow = NULL;
	gRenderer = NULL;

	wait = 100;
	mode = 0;
	quit = change = false;
	sec ='a';
	oldSec = 'b';
}

bool Timer::initSDL() {
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}
		gWindow = SDL_CreateWindow("Time", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if( gRenderer == NULL )
			{
				printf( "Accelerated renderer could not be created! SDL Error: %s\nSwitching to software renderer", SDL_GetError() );
				gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_SOFTWARE);
			}
			if(gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
			}
		}
	}
	return success;
}

void Timer::closeSDL() {	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	SDL_Quit();
}

void Timer::getTime() {
    long sec;
    time(&sec);
    char *currentTime = ctime(&sec);
    dateText[0] = currentTime[11];
    dateText[1] = currentTime[12];
    dateText[2] = currentTime[13];
    dateText[3] = currentTime[14];
    dateText[4] = currentTime[15];
    dateText[5] = currentTime[16];
    dateText[6] = currentTime[17];
    dateText[7] = currentTime[18];
	dateText[8] = '\0';
}

void Timer::incrementTime() {
	if (dateText[4] != '9') {
		dateText[4]++;
	} else {
		if (dateText[3] != '5') {
			dateText[3]++;
			dateText[4] = '0';
		} else {
			if (dateText[1] != '9') {
				dateText[1]++;
				dateText[3] = dateText[4] = '0';
			} else {
				if (dateText[0] != '5') {
					dateText[0]++;
					dateText[1] = dateText[3] = dateText[4] = '0';
				} else {
					dateText[0] = dateText[1] = dateText[3] = dateText[4] = '0';
				}
			}
		}
	}
}

void Timer::handleInput(){
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT) {
			quit = true;
		} else if (e.type == SDL_WINDOWEVENT) {
			switch(e.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					windowWidth = e.window.data1;
					windowHeight = e.window.data2;
					change = true;
					break;
				}
		} else if (e.type == SDL_KEYDOWN) {
           	switch(e.key.keysym.sym) {
                case SDLK_r:
					mode = 1;
					change = true;
					dateText[0] = '5';
					dateText[1] = '9';
					dateText[2] = ':';
					dateText[3] = '5';
					dateText[4] = '4';
					dateText[5] = '\0';
                    break;
                case SDLK_u:
                   	mode = 0;
					change = true;
					oldSec = 0;
                    break;
				case SDLK_ESCAPE:
                   	quit = true;
					break;
				case SDLK_q:
                   	quit = true;
                    break;
            }
        }
    }
}

SDL_Texture *Timer::drawNumber(char number, int width, int height, unsigned char red, unsigned char green, unsigned char blue){
	SDL_Surface* drawnSurface = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
	switch (number) {
		case '0':{
			SDL_Rect upperrect = {0, 0, width, height/6};
			SDL_FillRect(drawnSurface, &upperrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect lowerrect = {0, (height/6)*5, width, height-((height/6)*5)};
			SDL_FillRect(drawnSurface, &lowerrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect rightrect = {(width/4)*3, 0, width-((width/4)*3), height};
			SDL_FillRect(drawnSurface, &rightrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			
			SDL_Rect lefttrect = {0, 0, (width/4), height};
			SDL_FillRect(drawnSurface, &lefttrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			break;
		}	
		case '1': {
			SDL_Rect verticalrect = {(width/4)*3, 0, width-(width/4)*3, height};
			SDL_FillRect(drawnSurface, &verticalrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			break;
		}
		case '2': {
			SDL_Rect upperrect = {0, 0, width, height/6};
			SDL_FillRect(drawnSurface, &upperrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect middlerect = {0, (height/12)*5, width, height/6};
			SDL_FillRect(drawnSurface, &middlerect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect lowerrect = {0, (height/6)*5, width, height-(height/6)*5};
			SDL_FillRect(drawnSurface, &lowerrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect rightrect = {(width/4)*3, 0, width-(width/4)*3, height/2};
			SDL_FillRect(drawnSurface, &rightrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect leftrect = {0, height/2, (width/4), height/2};
			SDL_FillRect(drawnSurface, &leftrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			break;
		}	
		case '3': {
			SDL_Rect upperrect = {0, 0, width, height/6};
			SDL_FillRect(drawnSurface, &upperrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect middlerect = {width/4, (height/12)*5, width, height/6};
			SDL_FillRect(drawnSurface, &middlerect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect lowerrect = {0, (height/6)*5, width, height-((height/6)*5)};
			SDL_FillRect(drawnSurface, &lowerrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect rightrect = {(width/4)*3, 0, width-((width/4)*3), height};
			SDL_FillRect(drawnSurface, &rightrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			break;
		}	
		case '4':{

			SDL_Rect middlerect = {0, (height/12)*5, width, 2*height/12};
			SDL_FillRect(drawnSurface, &middlerect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect rightrect = {(width/4)*3, 0, width-((width/4)*3), height};
			SDL_FillRect(drawnSurface, &rightrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			
			SDL_Rect lefttrect = {0, 0, (width/4), height/2};
			SDL_FillRect(drawnSurface, &lefttrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			break;
		}
		case '5':{
			SDL_Rect upperrect = {0, 0, width, height/6};
			SDL_FillRect(drawnSurface, &upperrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect middlerect = {0, (height/12)*5, width, height/6};
			SDL_FillRect(drawnSurface, &middlerect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect lowerrect = {0, (height/6)*5, width, height-((height/6)*5)};
			SDL_FillRect(drawnSurface, &lowerrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect rightrect = {(width/4)*3, height/2, width-((width/4)*3), height/2};
			SDL_FillRect(drawnSurface, &rightrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			
			SDL_Rect lefttrect = {0, 0, (width/4), height/2};
			SDL_FillRect(drawnSurface, &lefttrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			break;
		}
		case '6':{
			SDL_Rect upperrect = {0, 0, width, height/6};
			SDL_FillRect(drawnSurface, &upperrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect middlerect = {0, (height/12)*5, width, height/6};
			SDL_FillRect(drawnSurface, &middlerect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect lowerrect = {0, (height/6)*5, width, height-((height/6)*5)};
			SDL_FillRect(drawnSurface, &lowerrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect rightrect = {(width/4)*3, height/2, width-((width/4)*3), height/2};
			SDL_FillRect(drawnSurface, &rightrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			
			SDL_Rect lefttrect = {0, 0, (width/4), height};
			SDL_FillRect(drawnSurface, &lefttrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			break;
		}
		case '7':{
			SDL_Rect upperrect = {0, 0, width, height/6};
			SDL_FillRect(drawnSurface, &upperrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect rightrect = {(width/4)*3, 0, width-((width/4)*3), height};
			SDL_FillRect(drawnSurface, &rightrect, SDL_MapRGB(drawnSurface->format, red, green, blue));		
			break;
		}
		case '8':{
			SDL_Rect upperrect = {0, 0, width, height/6};
			SDL_FillRect(drawnSurface, &upperrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect middlerect = {0, (height/12)*5, width, height/6};
			SDL_FillRect(drawnSurface, &middlerect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect lowerrect = {0, (height/6)*5, width, height-((height/6)*5)};
			SDL_FillRect(drawnSurface, &lowerrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect rightrect = {(width/4)*3, 0, width-((width/4)*3), height};
			SDL_FillRect(drawnSurface, &rightrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			
			SDL_Rect lefttrect = {0, 0, (width/4), height};
			SDL_FillRect(drawnSurface, &lefttrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			break;
		}
		case '9':{
			SDL_Rect upperrect = {0, 0, width, height/6};
			SDL_FillRect(drawnSurface, &upperrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect middlerect = {0, (height/12)*5, width, height/6};
			SDL_FillRect(drawnSurface, &middlerect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect lowerrect = {0, (height/6)*5, width, height-((height/6)*5)};
			SDL_FillRect(drawnSurface, &lowerrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect rightrect = {(width/4)*3, 0, width-((width/4)*3), height};
			SDL_FillRect(drawnSurface, &rightrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			
			SDL_Rect lefttrect = {0, 0, (width/4), height/2};
			SDL_FillRect(drawnSurface, &lefttrect, SDL_MapRGB(drawnSurface->format, red, green, blue));
			break;
		}
		case ':':{
			SDL_Rect upperrect = {0, height/3-width/2, width, width};
			SDL_FillRect(drawnSurface, &upperrect, SDL_MapRGB(drawnSurface->format, red, green, blue));

			SDL_Rect lowerrect = {0, 2*height/3-width/2, width, width};
			SDL_FillRect(drawnSurface, &lowerrect, SDL_MapRGB(drawnSurface->format, red, green, blue));		
			break;
		}
	}
	SDL_Texture *drawnTexture = SDL_CreateTextureFromSurface(gRenderer, drawnSurface);
	SDL_FreeSurface(drawnSurface);
	return drawnTexture;
}

void Timer::draw0(){
	int numberWidth = windowWidth/10;
	int numberHeight = 3*numberWidth/2;
	unsigned char red = 200, green = 0, blue = 200;
			
	for (unsigned int i = 0; i < 7; i++) {
		if (i == 2) {
			textures[i] = drawNumber(dateText[i], numberWidth/3, numberHeight, red, green, blue);
		} else if (i < 5){
			textures[i] = drawNumber(dateText[i], numberWidth, numberHeight, red, green, blue);
		} else {
			textures[i] = drawNumber(dateText[i+1], numberWidth, numberHeight, red, green, blue);
		}
	}

	SDL_RenderClear(gRenderer);

	int x1 = windowWidth/30;
	int y = (windowHeight - numberHeight) / 2;
	SDL_Rect renderQuad1 = {x1, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[0], NULL, &renderQuad1);

	int x2 = windowWidth/6;
	SDL_Rect renderQuad2 = {x2, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[1], NULL, &renderQuad2);

	int x3 = 3*windowWidth/10;
	SDL_Rect renderQuad3 = {x3, y, numberWidth/3, numberHeight};
	SDL_RenderCopy(gRenderer, textures[2], NULL, &renderQuad3);

	int x4 = 11*windowWidth/30;
	SDL_Rect renderQuad4 = {x4, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[3], NULL, &renderQuad4);

	int x5 = windowWidth/2;
	SDL_Rect renderQuad5 = {x5, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[4], NULL, &renderQuad5);

	int x6 = 19*windowWidth/30;
	SDL_Rect renderQuad6 = {x6, y, numberWidth/3, numberHeight};
	SDL_RenderCopy(gRenderer, textures[2], NULL, &renderQuad6);

	int x7 = 7*windowWidth/10;
	SDL_Rect renderQuad7 = {x7, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[5], NULL, &renderQuad7);

	int x8 = 5*windowWidth/6;
	SDL_Rect renderQuad8 = {x8, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[6], NULL, &renderQuad8);

	SDL_RenderPresent(gRenderer);

	for (unsigned int i = 0; i < 7; i++) {
		SDL_DestroyTexture(textures[i]);
	}
}
void Timer::draw1(){
	int numberWidth = 3*windowWidth/19;
	int numberHeight = 3*numberWidth/2;
	unsigned char red = 200, green = 0, blue = 200;
					
	for (unsigned int i = 0; i < 5; i++) {
		if (i == 2) {
			textures[i] = drawNumber(dateText[i], numberWidth/3, numberHeight, 150, 100, 150);
		} else {
			textures[i] = drawNumber(dateText[i], numberWidth, numberHeight, red, green, blue);
		}
	}

	SDL_RenderClear(gRenderer);

	int x1 = windowWidth/19;
	int y = (windowHeight - numberHeight) / 2;
	SDL_Rect renderQuad1 = {x1, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[0], NULL, &renderQuad1);

	int x2 = 5*windowWidth/19;
	SDL_Rect renderQuad2 = {x2, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[1], NULL, &renderQuad2);

	int x3 = 9*windowWidth/19;
	SDL_Rect renderQuad3 = {x3, y, numberWidth/3, numberHeight};
	SDL_RenderCopy(gRenderer, textures[2], NULL, &renderQuad3);

	int x4 = 11*windowWidth/19;
	SDL_Rect renderQuad4 = {x4, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[3], NULL, &renderQuad4);

	int x5 = 15*windowWidth/19;
	SDL_Rect renderQuad5 = {x5, y, numberWidth, numberHeight};
	SDL_RenderCopy(gRenderer, textures[4], NULL, &renderQuad5);

	SDL_RenderPresent(gRenderer);

	for (unsigned int i = 0; i < 5; i++) {
		SDL_DestroyTexture(textures[i]);
	}
}

void Timer::run(){
	int counter = 0;
	if(!initSDL()) {
		printf("Failed to initialize!\n");
	} else {
		while(!quit) {
			handleInput();
			if (mode == 0) {
				getTime();
				sec = dateText[7];
				change = sec != oldSec;
				oldSec = sec;
			} else {
				counter++;
				if(counter == 10) {
					counter = 0;
					incrementTime();
					change = true;
				}				
			}		
			if (change) {
				change = false;
				if (mode == 0) {
					draw0();
				} else {
					draw1();
				}		
			}
			SDL_Delay(wait);		
		}
	}
	closeSDL();
}

int main(int argc, char* args[]) {
	Timer timer = Timer(640, 480);
	timer.run();
	return 0;
}