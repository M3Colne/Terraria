/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Main.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "ChiliException.h"

int WINAPI wWinMain( HINSTANCE hInst,HINSTANCE,LPWSTR pArgs,INT )
{
	try
	{
		MainWindow wnd( hInst,pArgs );		
		try
		{
			Game theGame( wnd );
			while( wnd.ProcessMessage() )
			{
				theGame.Go();
			}
		}
		catch( const ChiliException& e )
		{
			const std::wstring eMsg = e.GetFullMessage() + 
				L"\n\nException caught at Windows message loop.";
			wnd.ShowMessageBox( e.GetExceptionType(),eMsg );
		}
		catch( const std::exception& e )
		{
			// need to convert std::exception what() string from narrow to wide string
			const std::string whatStr( e.what() );
			const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) + 
				L"\n\nException caught at Windows message loop.";
			wnd.ShowMessageBox( L"Unhandled STL Exception",eMsg );
		}
		catch(int e)
		{
			switch (e)
			{
			case 0:
				wnd.ShowMessageBox(L"Exception caught at saving the world", L"Trying to save the world failed!");
				break;
			case 1:
				wnd.ShowMessageBox(L"Exception caught at loading the world", L"Trying to load the world failed!");
				break;
			case 2:
				wnd.ShowMessageBox(L"Exception caught at saving the player", L"Trying to save the player failed!");
				break;
			case 3:
				wnd.ShowMessageBox(L"Exception caught at loading the player", L"Trying to load the player failed!");
				break;
			case 4:
				wnd.ShowMessageBox(L"Exception caught at saving in editor mode", L"Trying to save the editor changes failed!");
				break;
			case 5:
				wnd.ShowMessageBox(L"Exception caught at loading in editor mode", L"Trying to load the editor changes failed!");
				break;
			case 6:
				wnd.ShowMessageBox(L"Exception caught at loading the settings points", L"Trying to load the settings changes failed!");
				break;
			case 7:
				wnd.ShowMessageBox(L"Exception caught at loading the option points", L"Trying to load the option changes failed!");
				break;
			case 8:
				wnd.ShowMessageBox(L"Exception caught at loading the game points", L"Trying to load the game changes failed!");
				break;
			}
		}
		catch( ... )
		{
			wnd.ShowMessageBox( L"Unhandled Non-STL Exception",
				L"\n\nException caught at Windows message loop." );
		}
	}
	catch( const ChiliException& e )
	{
		const std::wstring eMsg = e.GetFullMessage() +
			L"\n\nException caught at main window creation.";
		MessageBox( nullptr,eMsg.c_str(),e.GetExceptionType().c_str(),MB_OK );
	}
	catch( const std::exception& e )
	{
		// need to convert std::exception what() string from narrow to wide string
		const std::string whatStr( e.what() );
		const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) +
			L"\n\nException caught at main window creation.";
		MessageBox( nullptr,eMsg.c_str(),L"Unhandled STL Exception",MB_OK );
	}
	catch( ... )
	{
		MessageBox( nullptr,L"\n\nException caught at main window creation.",
			L"Unhandled Non-STL Exception",MB_OK );
	}

	return 0;
}