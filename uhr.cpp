#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <time.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
void cleanup();
void getTimeDifference(char *watchTime, int startTime);
void getTime(char *watchTime);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

class TextTexture {
	SDL_Texture *textTexture;
	int width;
	int height;
	TTF_Font *font;
public:
	TextTexture();
	void createTexture(const char *text);
	void free();
	void changeFontSize(int fontSize);

	SDL_Texture *getTexture();
	int getWidth();
	int getHeight();
};

TextTexture::TextTexture() {
	textTexture = NULL;
	width = height = 0;
	font = NULL;
}

void TextTexture::changeFontSize(int fontSize) {
	if(font != NULL) {
		TTF_CloseFont(font);
	}
	font = TTF_OpenFont("/usr/share/fonts/gnu-free/FreeMono.otf", fontSize);
}

SDL_Texture *TextTexture::getTexture() {
	return textTexture;
}

int TextTexture::getWidth() {
	return width;
}

int TextTexture::getHeight() {
	return height;
}

void TextTexture::free()
{
	if(textTexture != NULL) {
		SDL_DestroyTexture(textTexture);
		textTexture = NULL;
		width = 0;
		height = 0;
	}
}

void TextTexture::createTexture(const char *text) {
	free();

	SDL_Color color;
	color.r = 0;
	color.g = 255;
	color.b = 0;
	SDL_Texture* mTexture = NULL;
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);

	if(textSurface == NULL) {
		printf("Could not render Text! SDL_ttf Error: %s\n", TTF_GetError());
	} else {
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(mTexture == NULL) {
			printf("No texture was created! SDL Error: %s\n", SDL_GetError());
		} else {
			width = textSurface->w;
			height = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	textTexture = mTexture;
}

void getTime(char *watchTime) {
    long sec;
    time(&sec);
    char *currentTime = ctime(&sec);
    watchTime[0] = currentTime[11];
    watchTime[1] = currentTime[12];
    watchTime[2] = currentTime[13];
    watchTime[3] = currentTime[14];
    watchTime[4] = currentTime[15];
    watchTime[5] = currentTime[16];
    watchTime[6] = currentTime[17];
    watchTime[7] = currentTime[18];
	watchTime[8] = '\0';
}

void getTimeDifference(char *watchTime, int startTime) {
    long sec;
    time(&sec);
	long difference = sec - startTime;
    char *currentTime = ctime(&difference);
    watchTime[0] = currentTime[14];
    watchTime[1] = currentTime[15];
    watchTime[2] = currentTime[16];
    watchTime[3] = currentTime[17];
    watchTime[4] = currentTime[18];
	watchTime[5] = '\0';
}

bool init() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL was not initialized! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	//Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		printf("Warning: No Linear texture filtering!");
	}
	gWindow = SDL_CreateWindow("Time", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(gWindow == NULL) {
		printf("Window was not created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if(gRenderer == NULL) {
		printf("Renderer was not created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	if(TTF_Init() == -1) {
		printf("SDL_ttf was not initailized! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	return true;
}

void cleanup() {	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
}

int main(int argc, char* args[]) {
	SDL_Texture *textTexture;
	TextTexture textTextureGenerator = TextTexture();
	int windowWidth = SCREEN_WIDTH;
	int windowHeight = SCREEN_HEIGHT;
	char dateText[9];

	long startTime;
	time(&startTime);
	startTime += 5;

	char mode = 0;
	int fontSize = 0;
	
	if(!init()) {
		printf("Failed to initialize!\n");
	} else {
		textTextureGenerator.changeFontSize(windowWidth/5);
		textTextureGenerator.createTexture("dateText");

		bool quit = false;
		bool change = false;
		char sec ='a';
		char oldSec = 'b';
		SDL_Event e;

		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_WINDOWEVENT) {
					switch(e.window.event) {
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							windowWidth = e.window.data1;
							windowHeight = e.window.data2;
							if (mode==0) {
								textTextureGenerator.changeFontSize(windowWidth/5);
							} else if (mode==1) {
								textTextureGenerator.changeFontSize(windowWidth/3);
							}
						break;
					}
				} else if (e.type == SDL_KEYDOWN) {
                  	switch(e.key.keysym.sym) {
                        case SDLK_r:
							mode = 1;
							textTextureGenerator.changeFontSize(windowWidth/3);
							time(&startTime);
							startTime += 5;
							change = true;
                        break;

                        case SDLK_u:
                          	mode = 0;
							textTextureGenerator.changeFontSize(windowWidth/5); 
							change = true; 
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
			if (mode == 0) {
				getTime(dateText);
				sec = dateText[7];
			} else {
				getTimeDifference(dateText, startTime);
				sec = dateText[4];
			}
			if (sec != oldSec) {
				change = true;			
			}
			oldSec = sec;
			if (change) {
				change = false;
				textTextureGenerator.createTexture(dateText);
				textTexture = textTextureGenerator.getTexture();
				int width = textTextureGenerator.getWidth();
				int height = textTextureGenerator.getHeight();

				int x = (windowWidth  - width) / 2;
				int y = (windowHeight  - height) / 2;
				SDL_Rect renderQuad = {x, y, width, height};

				SDL_RenderClear(gRenderer);
				SDL_RenderCopyEx(gRenderer, textTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
				SDL_RenderPresent(gRenderer);
				
			}
			SDL_Delay(100);		
		}
	}
	cleanup();
	return 0;
}