//
// Bitwise.cpp
// version 1.3
//

#include "stdafx.h"

#include "SDL.h"

// sorry Mac users, please use Windows
#include <windows.h>

//=============================================================
//=============================================================

const int kViewportWidth = 800;
const int kViewportHeight = 600;

//=============================================================
//=============================================================

// 
// SDL is a common library used for input detection
// these are some scancodes from this list: https://wiki.libsdl.org/SDL_Keycode
//

//=============================================================
//=============================================================
//Enum with all the actions that the user can use 
//the values will be represented as binary 
enum Keyboard {
	Attack = (1 << 1),
	Jump = (1 << 2),
	Duck = (1 << 3),
	Forward = (1 << 4),
	Back = (1 << 5),
	Use = (1 << 6),
	Cancel = (1 << 7)
};
class InputState
{
public:

	// the "friend" keyword allows you to bypass encapsulation
	// in this case, it allows InputManager to reach into my private variables
	friend class InputManager;

	InputState();
	~InputState();

	bool IsAttack() const;
	bool IsJump() const;
	bool IsDuck() const;
	bool IsForward() const;
	bool IsBack() const;
	bool IsUse() const;
	bool IsCancel() const;

private:
	//every action that the user made every frame
	unsigned int mAction;
};

//=============================================================
//=============================================================

InputState::InputState()
	:mAction(0)
{
}

InputState::~InputState()
{
}

bool InputState::IsAttack() const
{
	//if the action has the flag of attacking it will return a value
	return (mAction & Attack) != 0u;
}

bool InputState::IsJump() const
{
	//if the action has the flag of jumping it will return a value
	return (mAction & Jump) != 0u;
}

bool InputState::IsDuck() const
{
	//if the action has the flag of ducking it will return a value
	return (mAction & Duck) != 0u;
}

bool InputState::IsForward() const
{
	//if the action has the flag of moving forward it will return a value
	return (mAction & Forward) != 0u;
}

bool InputState::IsBack() const
{
	//if the action has the flag of moving back it will return a value
	return (mAction & Back) != 0u;
}

bool InputState::IsUse() const
{
	//if the action has the flag of use something it will return a value
	return (mAction & Use) != 0u;
}

bool InputState::IsCancel() const
{
	//if the action has the flag of cancel it will return a value
	return (mAction & Cancel) != 0u;
}

//=============================================================
//=============================================================

class InputManager
{
public:

	// this gets called each frame to query the system for the button states
	void Update();

	bool IsButtonDown( int nScancode ) const;

	const InputState& GetState() const { mState; }

private:

	InputState mState;

};

//=============================================================
//=============================================================
//read every input from the user that every frame has
void InputManager::Update()
{
	mState.mAction = 0;
	if (IsButtonDown(SDL_SCANCODE_LCTRL))
	{
		mState.mAction |= Attack;
		OutputDebugString(L"I Attack\n");
	}
	if (IsButtonDown(SDL_SCANCODE_SPACE))
	{
		mState.mAction |= Jump;
		OutputDebugString(L"I Jump\n");
	}
	if (IsButtonDown(SDL_SCANCODE_LSHIFT))
	{
		mState.mAction |= Duck;
		OutputDebugString(L"I Duck\n");
	}
	if (IsButtonDown(SDL_SCANCODE_UP))
	{
		mState.mAction |= Forward;
		OutputDebugString(L"Moving Forward\n");
	}
	if (IsButtonDown(SDL_SCANCODE_DOWN))
	{
		mState.mAction |= Back;
		OutputDebugString(L"Moving Back\n");
	}
	if (IsButtonDown(SDL_SCANCODE_E))
	{
		mState.mAction |= Use;
		OutputDebugString(L"Use something\n");
	}
	if (IsButtonDown(SDL_SCANCODE_BACKSPACE))
	{
		mState.mAction |= Cancel;
		OutputDebugString(L"Cancel Action\n");
	}
}

bool InputManager::IsButtonDown( int nScancode ) const
{
	int nKeys = 0;
	const Uint8* pKeys = SDL_GetKeyboardState( &nKeys );

	if ( pKeys == nullptr )
	{
		return false;
	}

	if ( nScancode >= nKeys )
	{
		return false;
	}

	return pKeys[nScancode];
}

//=============================================================
//=============================================================

//
// using Windows subsystem for this project
// use https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362(v=vs.85).aspx
// for printing strings to debugger output
//

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// never allow two instances
	if ( hPrevInstance )
	{
		return 0;
	}

	// initialize SDL
	SDL_Init( SDL_INIT_VIDEO );

	SDL_Window* pWindow = SDL_CreateWindow( "Bitwise", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, kViewportWidth, kViewportHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS );
	SDL_Renderer* pRenderer = SDL_CreateRenderer( pWindow, -1, 0 );

	SDL_Rect topLeftViewport;
	topLeftViewport.x = 0;
	topLeftViewport.y = 0;
	topLeftViewport.w = kViewportWidth;
	topLeftViewport.h = kViewportHeight;
	SDL_RenderSetViewport( pRenderer, &topLeftViewport );
	SDL_RenderSetScale( pRenderer, 1.0f, 1.0f );

	SDL_SetRenderDrawColor( pRenderer, 0, 0, 0, 255 );
	SDL_RenderClear( pRenderer );

	SDL_RenderPresent( pRenderer );

	// simple game loop
	InputManager inputManager;

	while ( true )
	{
		SDL_PumpEvents();

		SDL_Event event;

		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
			{
				exit( 0 );
			}

			inputManager.Update();

			Sleep( 100 ); // don't burn the CPU
		}
	}

    return 0;
}
