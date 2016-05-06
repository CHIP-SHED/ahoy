/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 272;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gImgSurface = NULL;

//Mouse position structure
SDL_Point mPosition;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "ahoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			int imgFlags= IMG_INIT_PNG;
			if( !( IMG_Init(imgFlags) & imgFlags ) )
			{
				printf( "SDL image could not be created! SDL_Error: %s\n", IMG_GetError() );
				success = false;
			}
			else
			{
				//Get window surface
				gScreenSurface = SDL_GetWindowSurface( gWindow );
			}
		}
	}

	return success;
}

bool loadMedia( char* av[], int slide)
{
	//Loading success flag
	bool success = true;

	//Load splash image
	gImgSurface = IMG_Load(av[slide]);
	if( gImgSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", av[slide], SDL_GetError() );
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface( gImgSurface );
	gImgSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* argv[] )
{
	int slide = 2;

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia(argv, slide) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				SDL_GetMouseState( &mPosition.x, &mPosition.y);
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					switch( e.type ) {
						case SDL_QUIT:
							quit = true;
							break;
						case SDL_KEYDOWN:
							if( e.key.keysym.sym == SDLK_ESCAPE )	quit = true;
							if( e.key.keysym.sym == SDLK_0 ){								
								ofstream fout(argv[1]);
								fout << "0";
								fout.close();
								quit = true;
							}
							if( e.key.keysym.sym == SDLK_LEFT && slide > 2){
								slide -= 1;
								if (!loadMedia(argv, slide))
									quit = true;								
							}
							if( e.key.keysym.sym == SDLK_RIGHT ){
								if(slide != (argc - 1)){
									slide += 1;
									if(!loadMedia(argv, slide)){
										quit = true;
									}
								}else
									quit = true;								
							}
							break;
						case SDL_MOUSEBUTTONUP:
							if (mPosition.x < 100 && slide > 2){
								slide -= 1;
								if (!loadMedia(argv, slide))
									quit = true;
							}else if (mPosition.x > 380){
								if(slide != (argc - 1)){
									slide += 1;
									if(!loadMedia(argv, slide)){
										quit = true;
									}
								}else
									quit = true;
							}
							break;
					}
				}

				//Apply the image
				SDL_BlitSurface( gImgSurface, NULL, gScreenSurface, NULL );
				
				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

