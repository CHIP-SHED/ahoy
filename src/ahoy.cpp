/*This piece of code was derived from Lazy Foo' Productions SDL image tutorial code
(http://lazyfoo.net/)*/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 272;

bool init();

bool loadMedia();

void close();

SDL_Window* nWindow = NULL;

SDL_Surface* nScreenSurface = NULL;

SDL_Surface* nImgSurface = NULL;

SDL_AudioDeviceID nAudioDev = 0;

SDL_Point nPosition;

void null_callback(void* userdata, unsigned char* stream, int len) {

}
bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		success = false;
	}
	else
	{
		SDL_AudioSpec want, have;

		SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
		want.freq = 48000;
		want.format = AUDIO_F32;
		want.channels = 2;
		want.samples = 4096;
		want.callback = null_callback;  // you wrote this function elsewhere.

		nAudioDev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
		if (nAudioDev == 0) {
			success = false;
		}

		nWindow = SDL_CreateWindow( "ahoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( nWindow == NULL )
		{
			success = false;
		}
		else
		{
			int imgFlags= IMG_INIT_PNG;
			if( !( IMG_Init(imgFlags) & imgFlags ) )
			{
				success = false;
			}
			else
			{
				nScreenSurface = SDL_GetWindowSurface( nWindow );
			}
		}
	}

	return success;
}

bool loadMedia( char* av[], int slide)
{
	bool success = true;

	nImgSurface = IMG_Load(av[slide]);
	if( nImgSurface == NULL )
	{
		success = false;
	}

	return success;
}

void close()
{
	SDL_FreeSurface( nImgSurface );
	nImgSurface = NULL;

	SDL_DestroyWindow( nWindow );
	nWindow = NULL;

	SDL_CloseAudioDevice(nAudioDev);

	SDL_Quit();
}

int main( int argc, char* argv[] )
{
	int slide = 2;

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if( !loadMedia(argv, slide) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			while( !quit )
			{
				SDL_GetMouseState( &nPosition.x, &nPosition.y);
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
							if (nPosition.x < 100 && slide > 2){
								slide -= 1;
								if (!loadMedia(argv, slide))
									quit = true;
							}else if (nPosition.x > 380){
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

				SDL_BlitSurface( nImgSurface, NULL, nScreenSurface, NULL );

				SDL_UpdateWindowSurface( nWindow );
			}
		}
	}

	close();

	return 0;
}

