/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Grid.h"
#include "Player.h"
#include "FrameTimer.h"
#include "Text.h"
#include "Sound.h"
#include "Scene.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
    void ChangeScene(int l);
    void AddScene(int l);
    void RemoveScene(int l);
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
    FrameTimer ft;
    float DT = 0.0f;
    Text textSprite;
    Sound backgroundMusic;
    bool noSound = false;
    Sound clickSound;
    float backgroundMusicLoopTimer = 4.1f;
    Grid* pGrid;
    Player* pPlayer;
    bool debugOpened = false;

    //Scenes
    static constexpr int menucboxes = 5; //The menu buttons
    static constexpr int settingcboxes = 2; //Exit, No sound
    static constexpr int optioncboxes = 3; //Resume, settings and save and exit
    static constexpr int gamecboxes = 10; //Inventory
    CBox gameBoxes[gamecboxes];
    CBox menuBoxes[menucboxes];
    CBox settingsBoxes[settingcboxes];
    CBox optionBoxes[optioncboxes];
    static constexpr int nScenes = 5;
    Scene scenes[nScenes];
    int lastScene = -1; //Used for the settings

    bool editorInhib = true;
    bool editorExit = false;
    const int pRadius = 15;
    bool optionsInhib = true;
	/********************************/
};