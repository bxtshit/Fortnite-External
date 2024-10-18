#pragma once
#include <dwmapi.h>
#include "../cipher/hash.hpp"
#include "../cipher/callstack.hpp"

class key_state_handler {
public:
	void load( )
	{
		hide;
		HMODULE user32 = hide_call( LoadLibraryA )(hash_str( "user32.dll" ));
		if ( !user32 ) {
			return;
		}

		void* proc_addr = (void*)GetProcAddress( user32, hash_str( "GetAsyncKeyState" ) );
		if ( !proc_addr ) {
			hide_call( FreeLibrary )(user32);
			return;
		}

		*(void**)&_GetAsyncKeyState = proc_addr;
		hide_call( FreeLibrary )(user32);
	}

	SHORT get( int vkey ) {
		if ( _GetAsyncKeyState != nullptr ) {
			return _GetAsyncKeyState( vkey );
		}
		return 0;
	}

private:
	SHORT( *_GetAsyncKeyState )(int vKey);
}; inline std::unique_ptr<key_state_handler> key_handler = std::make_unique<key_state_handler>( );