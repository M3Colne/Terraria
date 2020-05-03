/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include <assert.h>

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
    cameraPos(0, 0)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
    //Testing
    if (wnd.kbd.KeyIsPressed('1'))
    {
        hasStarted = true;
        CreateGrid(4200, 1200);
    }
    else  if (wnd.kbd.KeyIsPressed('2'))
    {
        hasStarted = true;
        CreateGrid(6400, 1800);
    }
    else  if (wnd.kbd.KeyIsPressed('3'))
    {
        hasStarted = true;
        CreateGrid(8400, 2400);
    }
    else if (wnd.kbd.KeyIsPressed('4')) //Reseting
    {
        hasStarted = false;
        DeleteGrid();
    }
    else if (wnd.kbd.KeyIsPressed('S')) //Saving
    {
        hasStarted = false;
        SaveGrid("./Worlds/world1.txt");
    }
    else if (wnd.kbd.KeyIsPressed('L')) //Loading
    {
        hasStarted = true;
        LoadGrid("./Worlds/world1.txt");
    }

    if (wnd.kbd.KeyIsPressed('W') && cameraPos.y >= 1)
    {
        cameraPos.y -= cameraSpeed;
    }
    if (wnd.kbd.KeyIsPressed('S') && cameraPos.y < 10000)
    {
        cameraPos.y += cameraSpeed;
    }
    if (wnd.kbd.KeyIsPressed('A') && cameraPos.x >= 1)
    {
        cameraPos.x -= cameraSpeed;
    }
    if (wnd.kbd.KeyIsPressed('D') && cameraPos.x < 10000)
    {
        cameraPos.x += cameraSpeed;
    }
}

void Game::CreateGrid(int _width, int _height)
{
    assert(pGrid == nullptr); //The pGrid pointer is pointing at an existing grid, why would you make another one?
    pGrid = new Grid(_width, _height);
}

void Game::SaveGrid(char* fileName)
{
    assert(pGrid != nullptr); //You can't save what doesn't exist, dummy
    pGrid->SaveWorld(fileName);
}

void Game::LoadGrid(char* fileName)
{
    assert(pGrid == nullptr); //The pGrid pointer is pointing at an existing grid, why would you make another one?
    pGrid = new Grid();
    pGrid->LoadWorld(fileName);
}

void Game::DeleteGrid()
{
    delete pGrid;
    pGrid = nullptr;
}

void Game::ComposeFrame()
{
    if (hasStarted)
    {
        pGrid->DrawBlocks(gfx, cameraPos.x, cameraPos.y);
    }
}
