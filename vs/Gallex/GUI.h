#ifndef _GUI_H_
#define _GUI_H_

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Menu.h"
#include "EditText.h"
#include "ListBox.h"
#include "DlgBox.h"
#include "InfoBox.h"
#include "Star.h"
#include "Planet.h"
#include "Satellite.h"
#include "SpaceScene.h"
#include "PreLoader.h"

#define ZERO_LVL 0
#define MENU_LVL 1
#define TEXT_EDIT_LVL 2
#define LIST_LVL 3
#define DIALOG_LVL 4

using namespace std;
using namespace std::placeholders;

class GUI
{
public:
	GUI();	
	void ShowDlgBox(string title, string text, int mode);
	void SwitchShowInfo();
	void LoadInfo();	
	void PressedEnter();
	void PressedEsc();
	void PressedKeyUp();
	void PressedKeyDown();
	void PressedBackspace();
	void PressedSymbol(int key);	
	void Draw();	
	void SetFPS(int fps);
	void MouseMove(int x, int y);
	void MouseClick(int x, int y);
	bool ZerroMode();
	
private:
	void SLMenu_Main();

	void SLMenu_Create();
	void SLMenu_Delete();
	void SLMenu_Select();
	void SLMenu_Settings();
	void SLMenu_Load();
	void SLMenu_Save();
	void SLMenu_Quit();

	void SLMenu_Create_Star();
	void SLMenu_Create_Planet();
	void SLMenu_Create_Satellite();

	void SLMenu_Create_Star_Name();
	void SLMenu_Create_Star_EquatorialRadius();
	void SLMenu_Create_Star_Obliquity();
	void SLMenu_Create_Star_Rotation();
	void SLMenu_Create_Star_Spectral();
	void SLMenu_Create_Star_Create();

	void SLMenu_Create_Planet_Name();
	void SLMenu_Create_Planet_Parent();
	void SLMenu_Create_Planet_MeanRadius();
	void SLMenu_Create_Planet_Rotation();
	void SLMenu_Create_Planet_Obliquity();
	void SLMenu_Create_Planet_SemimajorAxis();
	void SLMenu_Create_Planet_Eccentricity();
	void SLMenu_Create_Planet_OrbitalInclination();
	void SLMenu_Create_Planet_SiderealPeriod();
	void SLMenu_Create_Planet_Texture();
	void SLMenu_Create_Planet_Rings();
	void SLMenu_Create_Planet_Create();

	void SLMenu_Create_Satellite_Name();
	void SLMenu_Create_Satellite_Parent();
	void SLMenu_Create_Satellite_MeanRadius();
	void SLMenu_Create_Satellite_Rotation();
	void SLMenu_Create_Satellite_Obliquity();
	void SLMenu_Create_Satellite_SemimajorAxis();
	void SLMenu_Create_Satellite_Eccentricity();
	void SLMenu_Create_Satellite_OrbitalInclination();
	void SLMenu_Create_Satellite_SiderealPeriod();
	void SLMenu_Create_Satellite_Texture();
	void SLMenu_Create_Satellite_Create();

	void SLMenu_Delete_Star();
	void SLMenu_Delete_Planet();
	void SLMenu_Delete_Satellite();

	void SLMenu_Select_Star();
	void SLMenu_Select_Planet();
	void SLMenu_Select_Satellite();

	void SLMenu_Time();
	void SLMenu_Scale();
	void SLMenu_Distance_Scale();

	void SetStarName();
	void SetStarEquatorialRadius();
	void SetStarObliquity();
	void SetStarRotation();
	void SetStarSpectral();

	void SetPlanetName();
	void SetPlanetObliquity();
	void SetPlanetRotation();
	void SetPlanetMeanRadius();
	void SetPlanetSiderealPeriod();
	void SetPlanetOrbitalInclination();
	void SetPlanetEccentricity();
	void SetPlanetSemimajorAxis();
	void SetPlanetTexture(string texture);
	void SetPlanetRingTexture(string texture);
	void SetPlanetParent(string name);

	void SetSatelliteName();
	void SetSatelliteObliquity();
	void SetSatelliteRotation();
	void SetSatelliteMeanRadius();
	void SetSatelliteSiderealPeriod();
	void SetSatelliteOrbitalInclination();
	void SetSatelliteEccentricity();
	void SetSatelliteSemimajorAxis();
	void SetSatelliteTexture(string texture);
	void SetSatelliteParent(string name);

	void DeleteStar(string name);
	void DeletePlanet(string name);
	void DeleteSatellite(string name);

	void SelectStar(string name);
	void SelectPlanet(string name);
	void SelectSatellite(string name);

	void SetTimeSystem();
	void SetScaleSystem();
	void SetDistanceScale();

	void LoadSystem(string filename);

	void SaveSystem();

	void AddCharToTextEditBoxString(char key);
	void RemoveLastCharFromTextEditBoxString();
	void ShowTextEditBox(string title, function <void()> callback);
	void FinishTextEditBox();
		
	void DlgBoxOK(int mode);

	void ShowListBox(vector<string> items, function<void(string)> callback);
	void FinishListBox();

	void ShowSystemInfo();
	void ShowBottomButton();	

	PreLoader* preloader;
	Menu* slMenu;
	ListBox* slListBox;
	EditText* slTextEditBox;
	DlgBox* dlgBox;
	InfoBox* infoBox;
	InfoBox* systemInfoBox;
	InfoBox* bottomButton;
	SpaceScene* scene;
	string textEditBoxString;
	Star* star;
	Planet* planet;
	Satellite* satellite;
	int menu_level;
	bool isShowInfo;
	int fps;
};

#endif // !_GUI_H_