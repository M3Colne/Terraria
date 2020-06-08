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
#include "SpriteEffects.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
    textSprite("./Assets/monospaceFontSheet.bmp"),
    backgroundMusic(L"./Audio/gameBeat.wav"),
    clickSound(L"./Audio/click.wav"),
    menuScreen("./Assets/menu.bmp"),
    pGrid(nullptr),
    pPlayer(nullptr)
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
    if (hasStarted)
    {
        //Delta time
        DT = ft.Mark();

        pPlayer->Update(wnd.kbd, wnd.mouse, DT);

        //World commands
        bool reseting = wnd.kbd.KeyIsPressed('4');
        if (wnd.kbd.KeyIsPressed('5')) //Saving
        {
            hasStarted = false;
            SaveGrid("./Worlds/world1.txt");
            SavePlayer("./Players/player1.txt");
            reseting = true;
        }
        if (reseting) //Reseting
        {
            hasStarted = false;
            DeleteGrid();
            DeletePlayer();
        }

        //Audio
        backgroundMusicLoopTimer += DT;
        if (backgroundMusicLoopTimer >= 4.1f)
        {
            backgroundMusicLoopTimer = 0.0f;
            backgroundMusic.Play(1.0f, 0.3333f);
        }
    }
    else
    {
        if (wnd.mouse.LeftIsPressed())
        {
            const int x = wnd.mouse.GetPosX();
            const int y = wnd.mouse.GetPosY();
            if (x - menuX >= 233 && x - menuX <= 406 && y - menuY >= 154 && y - menuY <= 207)
            {
                hasStarted = true;
                clickSound.Play(1.0f, 0.5f);
                CreateGrid(4200, 1200, 40, 20, 7, 10);
                CreatePlayer(pGrid->GetWidth() / 2);
            }
            if (x - menuX >= 233 && x - menuX <= 406 && y - menuY >= 208 && y - menuY <= 271)
            {
                hasStarted = true;
                clickSound.Play(1.0f, 0.5f);
                CreateGrid(6400, 1800, 40, 20, 7, 10);
                CreatePlayer(pGrid->GetWidth() / 2);
            }
            if (x - menuX >= 233 && x - menuX <= 406 && y - menuY >= 272 && y - menuY <= 318)
            {
                hasStarted = true;
                clickSound.Play(1.0f, 0.5f);
                CreateGrid(8400, 2400, 40, 20, 7, 10);
                CreatePlayer(pGrid->GetWidth() / 2);
            }
            if (x - menuX >= 233 && x - menuX <= 406 && y - menuY >= 319 && y - menuY <= 363)
            {
                hasStarted = true;
                clickSound.Play(1.0f, 0.5f);
                LoadGrid("./Worlds/world1.txt");
                LoadPlayer("./Players/player1.txt");
            }
        }
    }
}

void Game::CreateGrid(int _width, int _height, int surfaceLevel, int surfaceLevelOffset, int minDirtLayer, int maxDirtLayer)
{
    assert(pGrid == nullptr); //The pGrid pointer is pointing at an existing grid, why would you make another one?
    pGrid = new Grid(_width, _height, surfaceLevel, surfaceLevelOffset, minDirtLayer, maxDirtLayer);
}

void Game::SaveGrid(char* fileName)
{
    assert(pGrid != nullptr); //You can't save what doesn't exist, dummy
    pGrid->SaveWorld(fileName);
}

void Game::LoadGrid(char* fileName)
{
    assert(pGrid == nullptr); //The pGrid pointer is pointing at an existing grid, why would you make another one?
    pGrid = new Grid(fileName);
}

void Game::DeleteGrid()
{
    delete pGrid;
    pGrid = nullptr;
}

void Game::CreatePlayer(const int sX)
{
    assert(pPlayer == nullptr); //The pPlayer pointer is pointing at an existing player, why would you make another one?
    pPlayer = new Player(*pGrid, sX);
}

void Game::SavePlayer(char* fileName)
{
    assert(pPlayer != nullptr); //You can't save what doesn't exist, dummy
    pPlayer->SavePlayer(fileName); //Idk why this warning is still showing up...
}

void Game::LoadPlayer(char* fileName)
{
    assert(pPlayer == nullptr); //The pPlayer pointer is poiting at an existing player, why would you make another one?
    pPlayer = new Player(fileName, *pGrid);
}

void Game::DeletePlayer()
{
    delete pPlayer;
    pPlayer = nullptr;
}

void Game::ComposeFrame()
{
    if (hasStarted)
    {
        pGrid->DrawBlocks(gfx, int(pPlayer->GetCameraX()), int(pPlayer->GetCameraY()));
        pPlayer->Draw(gfx);
        const int wID = pGrid->GetId(int((wnd.mouse.GetPosX() + pPlayer->GetCameraX()) / Grid::cellWidth),
            int((wnd.mouse.GetPosY() + pPlayer->GetCameraY()) / Grid::cellHeight));
        std::string WdebuggingInfo = "Block WID: " + std::to_string(wID) + "\n" +
            "Block WX: " + std::to_string(pGrid->GetPosX(wID)) + "\n" +
            "Block WY: " + std::to_string(pGrid->GetPosY(wID)) + "\n";
        const int sID = int(wnd.mouse.GetPosX() / Grid::cellWidth) + Grid::cellsH * int(wnd.mouse.GetPosY() / Grid::cellHeight);
        std::string SdebuggingInfo = "Block SID: " + std::to_string(sID) + "\n" +
            "Block WX: " + std::to_string(sID % Grid::cellsH) + "\n" +
            "Block WY: " + std::to_string(int(sID / Grid::cellsH)) + "\n";
        const float fps = 1 / DT;
        std::string FPS = "FPS: " + std::to_string(fps) + "n";
        textSprite.Draw(WdebuggingInfo + SdebuggingInfo + FPS, { 0, 0 }, Colors::Red, gfx);
    }
    else
    {
        gfx.DrawTexture(80, 80, menuScreen, SpriteEffects::NoEffect());
    }
}
