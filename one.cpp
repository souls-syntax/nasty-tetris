#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <string>

/* CONSTANTS */
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };

/// constexpr garuntee compile time initialization, const doesn't garauntee it.

//Starts up sdl and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shutdown SDL
void close();

/* GLOBAL VARIABLE */
// The window we will be rendering to
SDL_Window* gWindow{ nullptr };

// The surface contained by the window
SDL_Surface* gScreenSurface{ nullptr };

// The image we will load and show on the screen
SDL_Surface* gHelloWorld{ nullptr };

/// Using nullprt is standard way in modern Cpp

bool init()
{
	bool success{ true };

	// Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) == false ) {
		SDL_Log( "SDL could not be initialized! SDL error: %s\n", SDL_GetError() );
		success = false;
	} else {
		//create window

		if(gWindow = SDL_CreateWindow("SDL Window", kScreenWidth, kScreenHeight, 0); gWindow == nullptr ) {
			SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
			success = false;
		} else {
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return success;
}

bool loadMedia()
{
	bool success{ true };

	// Load splash image
	std::string imagePath{ "/home/souls-end/Documents/Coding/souls-syntax-repos/nasty-tetris/assets/sample.bmp" };
	if ( gHelloWorld = SDL_LoadBMP(imagePath.c_str()); gHelloWorld == nullptr ) {

		SDL_Log( "Unable to load image %s! SDL Error %s\n", imagePath.c_str(), SDL_GetError());
		success = false;
	} 
	return success;
}

void close()
{
	//clean up the surface 
	SDL_DestroySurface(gHelloWorld);
	gHelloWorld = nullptr;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gScreenSurface = nullptr;

	//Quit SDL subsystem
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	// Final exit code
	int exitCode{ 0 };

	// Initialize
	if( init() == false)  {
		SDL_Log( "Unable to initialize the porgrma!\n" );
		exitCode = 1;
	} else {
		
		// Loadmedia
		if ( loadMedia() == false ) {
			SDL_Log( "Unable to load media!\n" );
			exitCode = 2;
		} else {
			//The quit flag
			bool quit{ false };

			//The Even data
			SDL_Event e;
			SDL_zero( e );
			
			// The main loop
			while (quit == false) {
				//Gte event data
				while ( SDL_PollEvent( &e ) == true ) {
					
					// If event is quit type
					if ( e.type == SDL_EVENT_QUIT ) {
						//End the loop
						quit = true;
					}
				}

				// Fill the surface white
				SDL_FillSurfaceRect(gScreenSurface, nullptr, SDL_MapSurfaceRGB(gScreenSurface, 0xFF, 0xFF, 0xFF));

				//Render image on scree
				SDL_BlitSurface( gHelloWorld, nullptr, gScreenSurface, nullptr );

				//Update the surface
				SDL_UpdateWindowSurface( gWindow );

			}
		}
	}
	// clean up
	close();

	return exitCode;
}
