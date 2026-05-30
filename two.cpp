#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <string>


/* CONSTANTS */
constexpr int kScreenWidth{ 1800 };
constexpr int kScreenHeight{ 1000 };

/* Class Prototype */

class LTexture
{

	public:
		LTexture();
		~LTexture();

		// Loads texture from disk
		bool loadFromFile( std::string path );

		// Cleans up texture
		void destroy();

		//Draw texture
		void render( float x, float y );

		//Gets texture attributes
		int getWidth();
		int getHeight();
		bool isLoaded();

	private:

		// Contains texture data
		SDL_Texture* mTexture;

		// Texture dimensions
		int mWidth;
		int mHeight;
};

/* Gloabal Vars */

// The window we will be rendering to
SDL_Window* gWindow{ nullptr };

// The renderer used to draw to the function
SDL_Renderer* gRenderer{ nullptr };

// The PNG image we will render;
LTexture gPngTexture;

/* class Implementation */
//LTexture Implementation
LTexture::LTexture():
/// Setting up values in member initializer gives better pref optimization
	//Initialize texture variable
	mTexture{ nullptr },
	mWidth{ 0 },
	mHeight{ 0 }
{

}

LTexture::~LTexture()
{
	//clean up texture
	destroy();
}

bool LTexture::loadFromFile( std::string path )
{
	// Clean up texture if it already exist
	destroy();

	//Load surface
	if( SDL_Surface* loadedSurface = IMG_Load( path.c_str() ); loadedSurface == nullptr )
	{
		SDL_Log( "Unable to load image %s! SDL_Image error %s\n", path.c_str(), SDL_GetError() );
	}
	else 
	{
		// Create texture from surface
		if ( mTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface ); mTexture == nullptr )
		{
			SDL_Log( "Unable to create texture from loaded pixels! SDL error: %s\n", SDL_GetError() );
		}
		else 
		{
			//Get image dimensions;
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_DestroySurface( loadedSurface );
	}

	return mTexture != nullptr;
}

void LTexture::destroy()
{
	//Clean up texture
	SDL_DestroyTexture( mTexture );
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
}

void LTexture::render( float x, float y )
{
	// Set texture position
	SDL_FRect dstRect{ x, y, static_cast<float>( mWidth ), static_cast<float>( mHeight ) };
	
	// Render texture
	SDL_RenderTexture(gRenderer, mTexture, nullptr, &dstRect );
}

int LTexture::getHeight()
{
	return mHeight;
}

int LTexture::getWidth()
{
	return mWidth;
}

bool LTexture::isLoaded()
{
	return mTexture != nullptr;
}


bool init()
{
	bool success{ true };

	// Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) == false )
	{
		SDL_Log( "SDL could not be initialized! SDL error: %s\n", SDL_GetError() );
		success = false;
	} 
	else 
	{
		//create window with renderer
		if( SDL_CreateWindowAndRenderer("SDL3 tutorial", kScreenWidth, kScreenHeight, 0, &gWindow, &gRenderer) == false )
		{
			SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
			success = false;
		}

	}
	return success;
}

bool loadMedia()
{
	bool success{ true };

	// Load splash image
	std::string imagePath{ "/home/souls-end/Documents/Coding/souls-syntax-repos/nasty-tetris/assets/08tcen8w74y11.png" };
	if ( gPngTexture.loadFromFile(imagePath) == false ) {

		SDL_Log( "Unable to load PNG image %s! SDL Error %s\n", imagePath.c_str(), SDL_GetError());
		success = false;
	} 
	return success;
}

void close()
{
	//clean up the surface 
	gPngTexture.destroy();

	//Destroy renderer
	SDL_DestroyRenderer( gRenderer );
	gRenderer = nullptr;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	//Quit SDL subsystem
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Final exit code
	int exitCode = 0;

	//Initialize
	if ( init() == false )
	{
		SDL_Log ( "Unable to initialize program !\n" );
		exitCode = 1;
	}
	else
	{
		// Load media
		if ( loadMedia() == false )
		{
			SDL_Log( "Unable to load media!\n" );
			exitCode = 2;
		}
		else
		{
			// The quit flag
			bool quit{ false };
			
			//The even data
			SDL_Event e;
			SDL_zero( e );

			//The main loop
			while (quit == false) 
			{
				//Get event data
				while ( SDL_PollEvent( &e ) == true )
				{
					//If event is quit type
					if( e.type == SDL_EVENT_QUIT )
					{
						quit = true;
					}
				}
				
				// Fill the background white
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				// Rendere the image on scree
				gPngTexture.render(0.f, 0.f);

				//Update Scree
				SDL_RenderPresent( gRenderer );
			}
		}
	}
	close();

	return exitCode;
}


