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
    pGrid(nullptr),
    pPlayer(nullptr)
{
    //Intializing the menucboxes
    {
        std::ifstream in("./Assets/MenuPoints.txt", std::ios::binary);
        if (in.good())
        {
            for (int i = 0; i < menucboxes; i++)
            {
                in.read((char*)&menuBoxes[i].p0.x, sizeof(int));
                in.read((char*)&menuBoxes[i].p0.y, sizeof(int));
                in.read((char*)&menuBoxes[i].p1.x, sizeof(int));
                in.read((char*)&menuBoxes[i].p1.y, sizeof(int));
            }
        }
        else
        {
            throw 5;
        }
        in.close();
    }

    //Initializing the settingscboxes
    {
        std::ifstream in("./Assets/SettingsPoints.txt", std::ios::binary);
        if (in.good())
        {
            for (int i = 0; i < settingcboxes; i++)
            {
                in.read((char*)&settingsBoxes[i].p0.x, sizeof(int));
                in.read((char*)&settingsBoxes[i].p0.y, sizeof(int));
                in.read((char*)&settingsBoxes[i].p1.x, sizeof(int));
                in.read((char*)&settingsBoxes[i].p1.y, sizeof(int));
            }
        }
        else
        {
            throw 6;
        }
        in.close();
    }

    //Initializing the optioncboxes
    {
        std::ifstream in("./Assets/OptionsPoints.txt", std::ios::binary);
        if (in.good())
        {
            for (int i = 0; i < optioncboxes; i++)
            {
                in.read((char*)&optionBoxes[i].p0.x, sizeof(int));
                in.read((char*)&optionBoxes[i].p0.y, sizeof(int));
                in.read((char*)&optionBoxes[i].p1.x, sizeof(int));
                in.read((char*)&optionBoxes[i].p1.y, sizeof(int));
            }
        }
        else
        {
            throw 7;
        }
        in.close();
    }

    //Initializing the gamecboxes
    {
        std::ifstream in("./Assets/GamePoints.txt", std::ios::binary);
        if (in.good())
        {
            for (int i = 0; i < gamecboxes; i++)
            {
                in.read((char*)&gameBoxes[i].p0.x, sizeof(int));
                in.read((char*)&gameBoxes[i].p0.y, sizeof(int));
                in.read((char*)&gameBoxes[i].p1.x, sizeof(int));
                in.read((char*)&gameBoxes[i].p1.y, sizeof(int));
            }
        }
        else
        {
            throw 8;
        }
        in.close();
    }

    //Initializing the scenes
    //0 = Editor mode, 1 = Main menu, 2 = Settings, 3 = Game, 4 = Options
    scenes[0].boxes = nullptr;
    scenes[0].nBoxes = NULL;
    scenes[1].texture = Texture2D("./Assets/menu.bmp");
    scenes[1].boxes = menuBoxes;
    scenes[1].nBoxes = menucboxes;
    scenes[2].texture = Texture2D("./Assets/settings.bmp");
    scenes[2].boxes = settingsBoxes;
    scenes[2].nBoxes = settingcboxes;
    scenes[3].boxes = gameBoxes;
    scenes[3].nBoxes = gamecboxes;
    scenes[4].boxes = optionBoxes;
    scenes[4].nBoxes = optioncboxes;
    scenes[4].texture = Texture2D("./Assets/options.bmp");



    //First scene to be seen
    ChangeScene(1);
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
    //Editor mode
    if (wnd.kbd.KeyIsPressed('7'))
    {
        if (editorInhib)
        {
            if (!editorExit)
            {
                AddScene(0);
                editorInhib = false;
                editorExit = true;
            }
            else
            {
                RemoveScene(0);
                editorInhib = false;
                editorExit = false;
            }
        }
    }
    else
    {
        editorInhib = true;
    }

    if (scenes[0].isShown)
    {
        const Vei2 m(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
        if (wnd.mouse.LeftIsPressed())
        {
            //Find the scenes we should work on
            for (int j = 1; j < nScenes; j++)
            {
                if (scenes[j].isShown)
                {
                    for (int i = 0; i < scenes[j].nBoxes; i++)
                    {
                        Vei2 dist0(scenes[j].boxes[i].p0 - m);
                        Vei2 dist1(scenes[j].boxes[i].p1 - m);
                        if (dist0.GetLengthSq() <= pRadius * pRadius)
                        {
                            scenes[j].boxes[i].p0 = m;
                            break;
                        }
                        else if (dist1.GetLengthSq() <= pRadius * pRadius)
                        {
                            scenes[j].boxes[i].p1 = m;
                            break;
                        }
                    }
                }
            }
        }

        //Saving the points
        if (wnd.kbd.KeyIsPressed('S'))
        {
            //Menu points
            {
                std::ofstream out("./Assets/MenuPoints.txt", std::ios::binary);
                if (out.good())
                {
                    for (int i = 0; i < menucboxes; i++)
                    {
                        out.write((char*)&scenes[1].boxes[i].p0.x, sizeof(int));
                        out.write((char*)&scenes[1].boxes[i].p0.y, sizeof(int));
                        out.write((char*)&scenes[1].boxes[i].p1.x, sizeof(int));
                        out.write((char*)&scenes[1].boxes[i].p1.y, sizeof(int));
                    }
                }
                else
                {
                    throw 4;
                }
            }
            
            //Settings points
            {
                std::ofstream out("./Assets/SettingsPoints.txt", std::ios::binary);
                if (out.good())
                {
                    for (int i = 0; i < menucboxes; i++)
                    {
                        out.write((char*)&scenes[2].boxes[i].p0.x, sizeof(int));
                        out.write((char*)&scenes[2].boxes[i].p0.y, sizeof(int));
                        out.write((char*)&scenes[2].boxes[i].p1.x, sizeof(int));
                        out.write((char*)&scenes[2].boxes[i].p1.y, sizeof(int));
                    }
                }
                else
                {
                    throw 4;
                }
            }

            //Options points
            {
                std::ofstream out("./Assets/OptionsPoints.txt", std::ios::binary);
                if (out.good())
                {
                    for (int i = 0; i < menucboxes; i++)
                    {
                        out.write((char*)&scenes[4].boxes[i].p0.x, sizeof(int));
                        out.write((char*)&scenes[4].boxes[i].p0.y, sizeof(int));
                        out.write((char*)&scenes[4].boxes[i].p1.x, sizeof(int));
                        out.write((char*)&scenes[4].boxes[i].p1.y, sizeof(int));
                    }
                }
                else
                {
                    throw 4;
                }
            }
        }
    }
    else if (scenes[1].isShown && !scenes[0].isShown)
    {
        //Legend for main menu buttons:
        //0 = Create small world button
        //1 = Create medium world button
        //2 = Create large world button
        //3 = Load world button
        //4 = Settings button
     
        const int x = wnd.mouse.GetPosX();
        const int y = wnd.mouse.GetPosY();

        if (wnd.mouse.LeftIsPressed())
        {
            if (scenes[1].boxes[0].isCollidingWithMouse(x, y))
            {
                ChangeScene(3);
                clickSound.Play(1.0f, 0.5f);
                //Create the grid
                assert(pGrid == nullptr); //The pGrid pointer is pointing at an existing grid, why would you make another one?
                pGrid = new Grid(4200, 1200, 40, 20, 7, 10);

                //Create the player
                assert(pPlayer == nullptr);//The pPlayer pointer is pointing at an existing player, why would you make another one?
                pPlayer = new Player(*pGrid, pGrid->GetWidth() / 2);
            }
            else if (scenes[1].boxes[1].isCollidingWithMouse(x, y))
            {
                ChangeScene(3);
                clickSound.Play(1.0f, 0.5f);
                //Create the grid
                assert(pGrid == nullptr); //The pGrid pointer is pointing at an existing grid, why would you make another one?
                pGrid = new Grid(6400, 1800, 40, 20, 7, 10);

                //Create the player
                assert(pPlayer == nullptr);//The pPlayer pointer is pointing at an existing player, why would you make another one?
                pPlayer = new Player(*pGrid, pGrid->GetWidth() / 2);
            }
            else if (scenes[1].boxes[2].isCollidingWithMouse(x, y))
            {
                ChangeScene(3);
                clickSound.Play(1.0f, 0.5f);
                //Create the grid
                assert(pGrid == nullptr); //The pGrid pointer is pointing at an existing grid, why would you make another one?
                pGrid = new Grid(8400, 2400, 40, 20, 7, 10);

                //Create the player
                assert(pPlayer == nullptr);//The pPlayer pointer is pointing at an existing player, why would you make another one?
                pPlayer = new Player(*pGrid, pGrid->GetWidth() / 2);
            }
            else if (scenes[1].boxes[3].isCollidingWithMouse(x, y))
            {
                ChangeScene(3);
                clickSound.Play(1.0f, 0.5f);
                //Loading the grid
                assert(pGrid == nullptr); //The pGrid pointer is pointing at an existing grid, why would you make another one?
                pGrid = new Grid("./Worlds/world1.txt");

                //Loading the player
                assert(pPlayer == nullptr); //The pPlayer pointer is poiting at an existing player, why would you make another one?
                pPlayer = new Player("./Players/player1.txt", *pGrid);
            }
            else if (scenes[1].boxes[4].isCollidingWithMouse(x, y))
            {
                ChangeScene(2);
                lastScene = 1;
                clickSound.Play(1.0f, 0.5f);
            }
        }
    }
    else if (scenes[2].isShown && !scenes[0].isShown)
    {
        //Legend for settings menu buttons:
        //0 = Exit button
        //1 = No sound button

        const int x = wnd.mouse.GetPosX();
        const int y = wnd.mouse.GetPosY();

        if (wnd.mouse.LeftIsPressed())
        {
            if (scenes[2].boxes[0].isCollidingWithMouse(x, y))
            {
                ChangeScene(lastScene);
                clickSound.Play(1.0f, 0.5f);
                lastScene = -1;
            }
            if (scenes[2].boxes[1].isCollidingWithMouse(x, y))
            {
                clickSound.Play(1.0f, 0.5f);
                if (!noSound)
                {
                    noSound = true;
                }
                else
                {
                    noSound = false;
                }
            }
        }
    }
    else if (scenes[3].isShown && !scenes[0].isShown)
    {
        //Delta time
        DT = ft.Mark();

        pPlayer->Update(wnd.kbd, wnd.mouse, DT);

        //World commands
        if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
        {
            if (optionsInhib)
            {
                if (scenes[4].isShown)
                {
                    RemoveScene(4);
                }
                else
                {
                    ChangeScene(4);
                }
                optionsInhib = false;
            }
        }
        else
        {
            optionsInhib = true;
        }
        if (wnd.kbd.KeyIsPressed(VK_F3))
        {
            debugOpened = !debugOpened;
        }

        //Audio
        if (!noSound)
        {
            backgroundMusicLoopTimer += DT;
            if (backgroundMusicLoopTimer >= 4.1f)
            {
                backgroundMusicLoopTimer = 0.0f;
                backgroundMusic.Play(1.0f, 0.3333f);
            }
        }
    }
    else if (scenes[4].isShown && !scenes[0].isShown)
    {
        //Legend for options menu buttons:
        //0 = Resume
        //1 = Settings
        //2 = Save and exit the game

        const int x = wnd.mouse.GetPosX();
        const int y = wnd.mouse.GetPosY();

        if (wnd.mouse.LeftIsPressed())
        {
            if (scenes[4].boxes[0].isCollidingWithMouse(x, y))
            {
                lastScene = 4;
                ChangeScene(3);
                clickSound.Play(1.0f, 0.5f);
            }
            if (scenes[4].boxes[1].isCollidingWithMouse(x, y))
            {
                lastScene = 4;
                ChangeScene(2);
                clickSound.Play(1.0f, 0.5f);
            }
            if (scenes[4].boxes[2].isCollidingWithMouse(x, y))
            {
                lastScene = 4;
                ChangeScene(1);
                clickSound.Play(1.0f, 0.5f);
                //Saving and deleting the world
                assert(pGrid != nullptr); //You can't save what doesn't exist, dummy
                pGrid->SaveWorld("./Worlds/world1.txt");
                delete pGrid;
                pGrid = nullptr;

                //Saving and deleting the player
                assert(pPlayer != nullptr); //You can't save what doesn't exist, dummy
                pPlayer->SavePlayer("./Players/player1.txt");
                delete pPlayer;
                pPlayer = nullptr;
            }
        }
    }
}

void Game::ChangeScene(int l)
{
    assert(l < nScenes); //This scene doesn't exist!
    for (int i = 0; i < nScenes; i++)
    {
        scenes[i].isShown = false;
    }

    scenes[l].isShown = true; //Ignore this warning. The assertion above takes care of any errors before it reaches this line anyway
}

void Game::AddScene(int l)
{
    assert(l < nScenes);
    scenes[l].isShown = true;
}

void Game::RemoveScene(int l)
{
    assert(l < nScenes);
    scenes[l].isShown = false;
}

void Game::ComposeFrame()
{
    //0 = EditorMode, 1 = Main menu, 2 = Settings, 3 = Game, 4 = Options
    if (scenes[1].isShown)
    {
        gfx.DrawTexture(75, 50, scenes[1].texture, SpriteEffects::NoEffect{});
    }

    if (scenes[2].isShown)
    {
        gfx.DrawTexture(0, 0, scenes[2].texture, SpriteEffects::NoEffect{});
    }

    if (scenes[3].isShown)
    {
        //Drawing the game
        pGrid->DrawBlocks(gfx, pPlayer->GetCameraX(), pPlayer->GetCameraY());
        pPlayer->Draw(gfx);

        //Debugging info
        if (debugOpened)
        {
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
    }

    if (scenes[4].isShown)
    {
        gfx.DrawTexture(0, 0, scenes[4].texture, SpriteEffects::NoEffect{});
    }

    //I put it here because I want it to be drawn on top of all the scenes
    if (scenes[0].isShown)
    {
        for (int j = 1; j < nScenes; j++)
        {
            if (scenes[j].isShown)
            {
                for (int i = 0; i < scenes[j].nBoxes; i++)
                {
                    //circles drawn at the points of the boxes
                    gfx.DrawCircle(scenes[j].boxes[i].p0, (float)pRadius, false, Colors::Green);
                    gfx.DrawCircle(scenes[j].boxes[i].p1, (float)pRadius, false, Colors::Red);

                    //cboxes drawing
                    gfx.DrawRectangle(scenes[j].boxes[i].p0.x, scenes[j].boxes[i].p0.y, 
                        scenes[j].boxes[i].p1.x, scenes[j].boxes[i].p1.y, false, Colors::Yellow);
                }
            }
        }
    }
}
