#include "GUI.h"

GUI::GUI()
{
	preloader = PreLoader::GetInstance();
	scene = SpaceScene::GetInstance();
	textEditBoxString = "";
	menu_level = ZERO_LVL;
	isShowInfo = false;

	SLMenu_Main();	
}

// [Menu]
void GUI::SLMenu_Main()
{
	vector<const char*> items = { "Create", "Delete", "Select", "Settings", "Load", "Save", "Quit" };
	vector<function <void()>> funcs = {
		bind(&GUI::SLMenu_Create, this),
		bind(&GUI::SLMenu_Delete, this),
		bind(&GUI::SLMenu_Select, this),
		bind(&GUI::SLMenu_Settings, this),
		bind(&GUI::SLMenu_Load, this),
		bind(&GUI::SLMenu_Save, this),
		bind(&GUI::SLMenu_Quit, this)
	};

	slMenu = new Menu(items, funcs);
}

// [Menu] -> [Create]
void GUI::SLMenu_Create()
{
	vector<const char*> items = { "Star", "Planet", "Satellite", "Back" };
	vector<function <void()>> funcs = {
		bind(&GUI::SLMenu_Create_Star, this),
		bind(&GUI::SLMenu_Create_Planet, this),
		bind(&GUI::SLMenu_Create_Satellite, this),
		bind(&GUI::SLMenu_Main, this)
	};

	slMenu = new Menu(items, funcs);
}

// [Menu] -> [Delete]
void GUI::SLMenu_Delete()
{
	vector<const char*> items = { "Star", "Planet", "Satellite", "Back" };
	vector<function <void()>> funcs = {
		bind(&GUI::SLMenu_Delete_Star, this),
		bind(&GUI::SLMenu_Delete_Planet, this),
		bind(&GUI::SLMenu_Delete_Satellite, this),
		bind(&GUI::SLMenu_Main, this)
	};

	slMenu = new Menu(items, funcs);
}

// [Menu] -> [Select]
void GUI::SLMenu_Select()
{
	vector<const char*> items = { "Star", "Planet", "Satellite", "Back" };
	vector<function <void()>> funcs = {
		bind(&GUI::SLMenu_Select_Star, this),
		bind(&GUI::SLMenu_Select_Planet, this),
		bind(&GUI::SLMenu_Select_Satellite, this),
		bind(&GUI::SLMenu_Main, this)
	};

	slMenu = new Menu(items, funcs);
}

// [Menu] -> [Settings]
void GUI::SLMenu_Settings()
{
	vector<const char*> items = { "Time", "System Scale", "Distance Scale", "Back" };
	vector<function <void()>> funcs = {
		bind(&GUI::SLMenu_Time, this),
		bind(&GUI::SLMenu_Scale, this),
		bind(&GUI::SLMenu_Distance_Scale, this),
		bind(&GUI::SLMenu_Main, this)
	};

	slMenu = new Menu(items, funcs);
}

// [Menu] -> [Load]
void GUI::SLMenu_Load()
{
	vector<string> items;
	ScanFolder("data\\saves", "xml", items);

	if (items.size() > 0)
		ShowListBox(items, bind(&GUI::LoadSystem, this, _1));
	else
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);
}

// [Menu] -> [Save]
void GUI::SLMenu_Save()
{
	ShowTextEditBox("Name", bind(&GUI::SaveSystem, this));
}

// [Menu] -> [Quit]
void GUI::SLMenu_Quit()
{
	scene->SetDeadScene(true);
}

// [Menu] -> [Create] -> [Star]
void GUI::SLMenu_Create_Star()
{
	vector<const char*> items = { "Name", "Equatorial Radius", "Obliquity", "Rotation", "Spectral class", "Create", "Cancel" };
	vector<function <void()>> funcs = {
		bind(&GUI::SLMenu_Create_Star_Name, this),
		bind(&GUI::SLMenu_Create_Star_EquatorialRadius, this),
		bind(&GUI::SLMenu_Create_Star_Obliquity, this),
		bind(&GUI::SLMenu_Create_Star_Rotation, this),
		bind(&GUI::SLMenu_Create_Star_Spectral, this),
		bind(&GUI::SLMenu_Create_Star_Create, this),
		bind(&GUI::SLMenu_Create, this)
	};

	slMenu = new Menu(items, funcs);
	star = new Star();
}

// [Menu] -> [Create] -> [Planet]
void GUI::SLMenu_Create_Planet()
{
	if (scene->HasVecObj(STAR_TYPE))
	{
		vector<const char*> items = { "Name", "Parent Star", "Mean Radius", "Rotation", "Obliquity", "Semi-major Axis", "Eccentricity", "Orbital Inclination", "Sidereal Period", "Texture", "Rings", "Create", "Cancel" };
		vector<function <void()>> funcs = {
			bind(&GUI::SLMenu_Create_Planet_Name, this),
			bind(&GUI::SLMenu_Create_Planet_Parent, this),
			bind(&GUI::SLMenu_Create_Planet_MeanRadius, this),
			bind(&GUI::SLMenu_Create_Planet_Rotation, this),
			bind(&GUI::SLMenu_Create_Planet_Obliquity, this),
			bind(&GUI::SLMenu_Create_Planet_SemimajorAxis, this),
			bind(&GUI::SLMenu_Create_Planet_Eccentricity, this),
			bind(&GUI::SLMenu_Create_Planet_OrbitalInclination, this),
			bind(&GUI::SLMenu_Create_Planet_SiderealPeriod, this),
			bind(&GUI::SLMenu_Create_Planet_Texture, this),
			bind(&GUI::SLMenu_Create_Planet_Rings, this),
			bind(&GUI::SLMenu_Create_Planet_Create, this),
			bind(&GUI::SLMenu_Create, this)
		};

		slMenu = new Menu(items, funcs);
		planet = new Planet();
	} else {
		ShowDlgBox("Error!", "The star was not created.", MENU_LVL);
	}
}

// [Menu] -> [Create] -> [Satellite]
void GUI::SLMenu_Create_Satellite()
{
	if (scene->HasVecObj(PLANET_TYPE))
	{
		vector<const char*> items = { "Name", "Parent Planet", "Mean Radius", "Rotation", "Obliquity", "Semi-major Axis", "Eccentricity", "Orbital Inclination", "Sidereal Period", "Texture", "Create", "Cancel" };
		vector<function <void()>> funcs = {
			bind(&GUI::SLMenu_Create_Satellite_Name, this),
			bind(&GUI::SLMenu_Create_Satellite_Parent, this),
			bind(&GUI::SLMenu_Create_Satellite_MeanRadius, this),
			bind(&GUI::SLMenu_Create_Satellite_Rotation, this),
			bind(&GUI::SLMenu_Create_Satellite_Obliquity, this),
			bind(&GUI::SLMenu_Create_Satellite_SemimajorAxis, this),
			bind(&GUI::SLMenu_Create_Satellite_Eccentricity, this),
			bind(&GUI::SLMenu_Create_Satellite_OrbitalInclination, this),
			bind(&GUI::SLMenu_Create_Satellite_SiderealPeriod, this),
			bind(&GUI::SLMenu_Create_Satellite_Texture, this),
			bind(&GUI::SLMenu_Create_Satellite_Create, this),
			bind(&GUI::SLMenu_Create, this)
		};

		slMenu = new Menu(items, funcs);
		satellite = new Satellite();
	} else {
		ShowDlgBox("Error!", "The planet was not created.", MENU_LVL);
	}
}

// [Menu] -> [Create] -> [Star] -> [Name]
void GUI::SLMenu_Create_Star_Name()
{
	ShowTextEditBox("Name", bind(&GUI::SetStarName, this));
}

// [Menu] -> [Create] -> [Star] -> [Equatorial Radius]
void GUI::SLMenu_Create_Star_EquatorialRadius()
{
	ShowTextEditBox("Equatorial Radius (km)", bind(&GUI::SetStarEquatorialRadius, this));
}

// [Menu] -> [Create] -> [Star] -> [Obliquity]
void GUI::SLMenu_Create_Star_Obliquity()
{
	ShowTextEditBox("Obliquity (degree)", bind(&GUI::SetStarObliquity, this));
}

// [Menu] -> [Create] -> [Star] -> [Rotation]
void GUI::SLMenu_Create_Star_Rotation()
{
	ShowTextEditBox("Rotation (h)", bind(&GUI::SetStarRotation, this));
}

// [Menu] -> [Create] -> [Star] -> [Spectral Class]
void GUI::SLMenu_Create_Star_Spectral()
{
	ShowTextEditBox("Spectral Class", bind(&GUI::SetStarSpectral, this));
}

// [Menu] -> [Create] -> [Star] -> [Create]
void GUI::SLMenu_Create_Star_Create()
{
	star->BuildStar();
	star->Resize();
	scene->AddStar(star);		
	SLMenu_Main();	
	ShowDlgBox("Success!", "The star " + star->GetName() + " was created.", ZERO_LVL);
}

// [Menu] -> [Create] -> [Planet] -> [Name]
void GUI::SLMenu_Create_Planet_Name()
{
	ShowTextEditBox("Name", bind(&GUI::SetPlanetName, this));
}

// [Menu] -> [Create] -> [Planet] -> [Obliquity]
void GUI::SLMenu_Create_Planet_Obliquity()
{
	ShowTextEditBox("Obliquity (degree)", bind(&GUI::SetPlanetObliquity, this));
}

// [Menu] -> [Create] -> [Planet] -> [Rotation]
void GUI::SLMenu_Create_Planet_Rotation()
{
	ShowTextEditBox("Rotation (h)", bind(&GUI::SetPlanetRotation, this));
}

// [Menu] -> [Create] -> [Planet] -> [Mean Radius]
void GUI::SLMenu_Create_Planet_MeanRadius()
{
	ShowTextEditBox("Mean Radius (km)", bind(&GUI::SetPlanetMeanRadius, this));
}

// [Menu] -> [Create] -> [Planet] -> [Eccentricity]
void GUI::SLMenu_Create_Planet_Eccentricity()
{
	ShowTextEditBox("Eccentricity", bind(&GUI::SetPlanetEccentricity, this));
}

// [Menu] -> [Create] -> [Planet] -> [Semi-major Axis]
void GUI::SLMenu_Create_Planet_SemimajorAxis()
{
	ShowTextEditBox("Semi-major Axis (km)", bind(&GUI::SetPlanetSemimajorAxis, this));
}

// [Menu] -> [Create] -> [Planet] -> [Sidereal Period]
void GUI::SLMenu_Create_Planet_SiderealPeriod()
{
	ShowTextEditBox("Sidereal Period (h)", bind(&GUI::SetPlanetSiderealPeriod, this));
}

// [Menu] -> [Create] -> [Planet] -> [Orbital Inclination]
void GUI::SLMenu_Create_Planet_OrbitalInclination()
{
	ShowTextEditBox("Orbital Inclination (degree)", bind(&GUI::SetPlanetOrbitalInclination, this));
}

// [Menu] -> [Create] -> [Planet] -> [Texture]
void GUI::SLMenu_Create_Planet_Texture()
{
	vector<string> items;
	ScanFolder("data\\textures\\planet", "bmp", items);
	ScanFolder("data\\textures\\planet", "jpg", items);
	ScanFolder("data\\textures\\planet", "png", items);

	if (items.size() > 0)
	{
		sort(items.begin(), items.end(), compareStrings);
		ShowListBox(items, bind(&GUI::SetPlanetTexture, this, _1));
	} else {	
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);	
	}
}

// [Menu] -> [Create] -> [Planet] -> [Rings]
void GUI::SLMenu_Create_Planet_Rings()
{
	vector<string> items;
	ScanFolder("data\\textures\\ring", "bmp", items);
	ScanFolder("data\\textures\\ring", "jpg", items);
	ScanFolder("data\\textures\\ring", "png", items);

	if (items.size() > 0)
	{
		sort(items.begin(), items.end(), compareStrings);
		ShowListBox(items, bind(&GUI::SetPlanetRingTexture, this, _1));
	} else {
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);
	}
}

// [Menu] -> [Create] -> [Planet] -> [Parent]
void GUI::SLMenu_Create_Planet_Parent()
{
	ShowListBox(scene->GetVecNames(STAR_TYPE), bind(&GUI::SetPlanetParent, this, _1));
}

// [Menu] -> [Create] -> [Planet] -> [Create]
void GUI::SLMenu_Create_Planet_Create()
{
	if (planet->GetParent() == nullptr) 
	{	
		ShowDlgBox("Error!", "No parent star selected.", MENU_LVL);	
	} else if (scene->CheckPlanetExistName(planet->GetName())) {
		ShowDlgBox("Error!", "A planet with the same name already exists.", MENU_LVL);
	} else {
		planet->BuildPlanet();
		scene->AddPlanet(planet);
		SLMenu_Main();
		ShowDlgBox("Success!", "The planet " + planet->GetName() + " was created.", ZERO_LVL);
	}
}

// [Menu] -> [Create] -> [Satellite] -> [Name]
void GUI::SLMenu_Create_Satellite_Name()
{
	ShowTextEditBox("Name", bind(&GUI::SetSatelliteName, this));
}

// [Menu] -> [Create] -> [Satellite] -> [Obliquity]
void GUI::SLMenu_Create_Satellite_Obliquity()
{
	ShowTextEditBox("Obliquity (degrees)", bind(&GUI::SetSatelliteObliquity, this));
}

// [Menu] -> [Create] -> [Satellite] -> [Rotation]
void GUI::SLMenu_Create_Satellite_Rotation()
{
	ShowTextEditBox("Rotation (h)", bind(&GUI::SetSatelliteRotation, this));
}

// [Menu] -> [Create] -> [Satellite] -> [Mean Radius]
void GUI::SLMenu_Create_Satellite_MeanRadius()
{
	ShowTextEditBox("Mean Radius (km)", bind(&GUI::SetSatelliteMeanRadius, this));
}

// [Menu] -> [Create] -> [Satellite] -> [Eccentricity]
void GUI::SLMenu_Create_Satellite_Eccentricity()
{
	ShowTextEditBox("Eccentricity", bind(&GUI::SetSatelliteEccentricity, this));
}

// [Menu] -> [Create] -> [Satellite] -> [Semi-major Axis]
void GUI::SLMenu_Create_Satellite_SemimajorAxis()
{
	ShowTextEditBox("Semi-major Axis (km)", bind(&GUI::SetSatelliteSemimajorAxis, this));
}

// [Menu] -> [Create] -> [Satellite] -> [Sidereal Period]
void GUI::SLMenu_Create_Satellite_SiderealPeriod()
{
	ShowTextEditBox("Sidereal Period (h)", bind(&GUI::SetSatelliteSiderealPeriod, this));
}

// [Menu] -> [Create] -> [Satellite] -> [Orbital Inclination]
void GUI::SLMenu_Create_Satellite_OrbitalInclination()
{
	ShowTextEditBox("Orbital Inclination (degree)", bind(&GUI::SetSatelliteOrbitalInclination, this));
}

// [Menu] -> [Create] -> [Satellite] -> [Texture]
void GUI::SLMenu_Create_Satellite_Texture()
{
	vector<string> items;
	ScanFolder("data\\textures\\satellite", "bmp", items);
	ScanFolder("data\\textures\\satellite", "jpg", items);
	ScanFolder("data\\textures\\satellite", "png", items);

	if (items.size() > 0)
	{
		sort(items.begin(), items.end(), compareStrings);
		ShowListBox(items, bind(&GUI::SetSatelliteTexture, this, _1));
	} else {
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);
	}	
}

// [Menu] -> [Create] -> [Satellite] -> [Parent]
void GUI::SLMenu_Create_Satellite_Parent()
{
	ShowListBox(scene->GetVecNames(PLANET_TYPE), bind(&GUI::SetSatelliteParent, this, _1));
}

// [Menu] -> [Create] -> [Satellite] -> [Create]
void GUI::SLMenu_Create_Satellite_Create()
{
	if (satellite->GetParent() == nullptr)
	{
		ShowDlgBox("Error!", "No parent planet selected.", MENU_LVL);
	} else if (scene->CheckSatelliteExistName(satellite->GetName())) {
		ShowDlgBox("Error!", "A satellite with the same name already exists.", MENU_LVL);
	} else {
		satellite->BuildSatellite();
		scene->AddSatellite(satellite);
		SLMenu_Main();
		ShowDlgBox("Success!", "The satellite " + planet->GetName() + " was created.", ZERO_LVL);
	}
}

// [Menu] -> [Delete] -> [Star]
void GUI::SLMenu_Delete_Star()
{
	if (scene->HasVecObj(STAR_TYPE))
		ShowListBox(scene->GetVecNames(STAR_TYPE), bind(&GUI::DeleteStar, this, _1));
	else 
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);
}

// [Menu] -> [Delete] -> [Planet]
void GUI::SLMenu_Delete_Planet()
{
	if (scene->HasVecObj(PLANET_TYPE))
		ShowListBox(scene->GetVecNames(PLANET_TYPE), bind(&GUI::DeletePlanet, this, _1));
	else
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);	
}

// [Menu] -> [Delete] -> [Satellite]
void GUI::SLMenu_Delete_Satellite()
{
	if (scene->HasVecObj(SATELLITE_TYPE))
		ShowListBox(scene->GetVecNames(SATELLITE_TYPE), bind(&GUI::DeleteSatellite, this, _1));
	else
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);
}

// [Menu] -> [Select] -> [Star]
void GUI::SLMenu_Select_Star()
{
	if (scene->HasVecObj(STAR_TYPE))
		ShowListBox(scene->GetVecNames(STAR_TYPE), bind(&GUI::SelectStar, this, _1));
	else
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);
}

// [Menu] -> [Select] -> [Planet]
void GUI::SLMenu_Select_Planet()
{
	if (scene->HasVecObj(PLANET_TYPE))
		ShowListBox(scene->GetVecNames(PLANET_TYPE), bind(&GUI::SelectPlanet, this, _1));
	else
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);
}

// [Menu] -> [Select] -> [Satellite]
void GUI::SLMenu_Select_Satellite()
{
	if (scene->HasVecObj(SATELLITE_TYPE))
		ShowListBox(scene->GetVecNames(SATELLITE_TYPE), bind(&GUI::SelectSatellite, this, _1));
	else
		ShowDlgBox("Error!", "The list is empty.", MENU_LVL);	
}

// [Menu] -> [Settings] -> [Time]
void GUI::SLMenu_Time()
{
	ShowTextEditBox("Time (X:1)", bind(&GUI::SetTimeSystem, this));
}

// [Menu] -> [Settings] -> [Scale]
void GUI::SLMenu_Scale()
{
	ShowTextEditBox("Scale (1:X)", bind(&GUI::SetScaleSystem, this));
}

// [Menu] -> [Settings] -> [Distance Scale]
void GUI::SLMenu_Distance_Scale()
{
	ShowTextEditBox("Distance Scale (1:X)", bind(&GUI::SetDistanceScale, this));
}

// Create Star
void GUI::SetStarName()
{
	if (textEditBoxString.length() > 0)
	{
		star->SetName((char*)textEditBoxString.c_str());
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Empty field.", TEXT_EDIT_LVL);
	}
}

void GUI::SetStarEquatorialRadius()
{
	if (isNumber(textEditBoxString))
	{
		star->SetEquatorialRadius(stod(textEditBoxString.c_str()) / scene->GetSystemScale());
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetStarObliquity()
{
	if (isNumber(textEditBoxString))
	{
		star->SetObliquity(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetStarRotation()
{
	if (isNumber(textEditBoxString))
	{
		star->SetRotation(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetStarSpectral()
{
	if (textEditBoxString.length() > 0)
	{
		star->SetSpectral((char*)textEditBoxString.c_str());
		FinishTextEditBox();
	} else {		
		ShowDlgBox("Error!", "Empty field.", TEXT_EDIT_LVL);
	}
}

// Create planet
void GUI::SetPlanetName()
{	
	if (scene->CheckPlanetExistName(textEditBoxString))
	{
		ShowDlgBox("Error!", "A planet with the same name already exists.", TEXT_EDIT_LVL);
	} else if (textEditBoxString.length() > 0) {
		planet->SetName((char*)textEditBoxString.c_str());
		FinishTextEditBox();
	} else {	
		ShowDlgBox("Error!", "Empty field.", TEXT_EDIT_LVL);	
	}
}

void GUI::SetPlanetObliquity()
{
	if (isNumber(textEditBoxString))
	{
		planet->SetObliquity(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetPlanetRotation()
{
	if (isNumber(textEditBoxString))
	{
		planet->SetRotation(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetPlanetMeanRadius()
{
	if (isNumber(textEditBoxString))
	{
		planet->SetMeanRadius(stod(textEditBoxString.c_str()) / scene->GetSystemScale());
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetPlanetSiderealPeriod()
{
	if (isNumber(textEditBoxString))
	{
		planet->SetSiderealPeriod(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetPlanetOrbitalInclination()
{
	if (isNumber(textEditBoxString))
	{
		planet->SetOrbitalInclination(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetPlanetEccentricity()
{
	if (isNumber(textEditBoxString))
	{
		planet->SetEccentricity(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetPlanetSemimajorAxis()
{
	if (isNumber(textEditBoxString))
	{
		planet->SetSemiMajorAxis(stod(textEditBoxString.c_str()) / scene->GetSystemScale());
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetPlanetTexture(string texture)
{
	string fn = "data\\textures\\planet\\" + texture;
	planet->SetTexture(fn.c_str());
	FinishListBox();
}

void GUI::SetPlanetRingTexture(string texture)
{
	string fn = "data\\textures\\ring\\" + texture;
	planet->SetRingTexture(fn.c_str());
	FinishListBox();
}

void GUI::SetPlanetParent(string name)
{
	scene->SetPlanetParent(name, planet);
	FinishListBox();
}

// Create satellite
void GUI::SetSatelliteName()
{
	if (scene->CheckSatelliteExistName(textEditBoxString))
	{
		ShowDlgBox("Error!", "A satellite with the same name already exists.", TEXT_EDIT_LVL);
	} else if (textEditBoxString.length() > 0) {
		satellite->SetName((char*)textEditBoxString.c_str());
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Empty field.", TEXT_EDIT_LVL);
	}
}

void GUI::SetSatelliteObliquity()
{
	if (isNumber(textEditBoxString))
	{
		satellite->SetObliquity(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetSatelliteRotation()
{
	if (isNumber(textEditBoxString))
	{
		satellite->SetRotation(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetSatelliteMeanRadius()
{
	if (isNumber(textEditBoxString))
	{
		satellite->SetMeanRadius(stod(textEditBoxString.c_str()) / scene->GetSystemScale());
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetSatelliteSiderealPeriod()
{
	if (isNumber(textEditBoxString))
	{
		satellite->SetSiderealPeriod(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetSatelliteOrbitalInclination()
{
	if (isNumber(textEditBoxString))
	{
		satellite->SetOrbitalInclination(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetSatelliteEccentricity()
{
	if (isNumber(textEditBoxString))
	{
		satellite->SetEccentricity(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetSatelliteSemimajorAxis()
{
	if (isNumber(textEditBoxString))
	{
		satellite->SetSemiMajorAxis(stod(textEditBoxString.c_str()) / scene->GetSystemScale());
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetSatelliteTexture(string texture)
{
	string fn = "data\\textures\\satellite\\" + texture;
	satellite->SetTexture(fn.c_str());
	FinishListBox();
}

void GUI::SetSatelliteParent(string name)
{
	scene->SetSatelliteParent(name, satellite);
	FinishListBox();
}

// Delete
void GUI::DeleteStar(string name)
{	
	scene->SetDeadStar(name);
	scene->SetDeadByParent();
	scene->DeleteDead();
	SLMenu_Main();
	FinishListBox();
}

void GUI::DeletePlanet(string name)
{
	scene->SetDeadPlanet(name, false);
	scene->SetDeadByParent();
	scene->DeleteDead();
	SLMenu_Main();
	FinishListBox();
	ShowDlgBox("Success!", "The planet " + name + " was deleted.", ZERO_LVL);
}

void GUI::DeleteSatellite(string name)
{
	scene->SetDeadSatellite(name, false);
	scene->DeleteDead();
	SLMenu_Main();
	FinishListBox();
	ShowDlgBox("Success!", "The satellite " + name + " was deleted.", ZERO_LVL);
}

// Select
void GUI::SelectStar(string name)
{
	scene->SetSelectedStar(name);
	scene->FocusOnSelectedObject();
	SLMenu_Main();	
	FinishListBox();
	menu_level = ZERO_LVL;
}

void GUI::SelectPlanet(string name)
{
	scene->SetSelectedPlanet(name);
	scene->FocusOnSelectedObject();
	SLMenu_Main();
	FinishListBox();
	menu_level = ZERO_LVL;
}

void GUI::SelectSatellite(string name)
{
	scene->SetSelectedSatellite(name);
	scene->FocusOnSelectedObject();
	SLMenu_Main();
	FinishListBox();
	menu_level = ZERO_LVL;
}

// Settings
void GUI::SetTimeSystem()
{
	if (isNumber(textEditBoxString))
	{
		scene->SetTimeScale(atof(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

void GUI::SetScaleSystem()
{
	if (isNumber(textEditBoxString))
	{
		scene->SetSystemScale(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}	
}

void GUI::SetDistanceScale()
{
	if (isNumber(textEditBoxString))
	{
		scene->SetDistanceScale(stod(textEditBoxString.c_str()));
		FinishTextEditBox();
	} else {
		ShowDlgBox("Error!", "Unacceptable symbols. Enter only numbers.", TEXT_EDIT_LVL);
	}
}

// Load
void GUI::LoadSystem(string filename)
{
	scene->LoadSystem(filename);
	FinishListBox();
	ShowDlgBox("Success!", "The solar system was loaded.", ZERO_LVL);
}

// Save
void GUI::SaveSystem()
{
	scene->SaveSystem(textEditBoxString);
	FinishTextEditBox();
	ShowDlgBox("Success!", "The solar system was saved.", ZERO_LVL);
}

// TextEdit
void GUI::AddCharToTextEditBoxString(char key)
{
	this->textEditBoxString += key;
	slTextEditBox->SetText(textEditBoxString);
}

void GUI::RemoveLastCharFromTextEditBoxString()
{
	textEditBoxString = textEditBoxString.substr(0, textEditBoxString.size() - 1);
	slTextEditBox->SetText(textEditBoxString);
}

void GUI::ShowTextEditBox(string title, function <void()> callback)
{
	slTextEditBox = new EditText(title, callback);
	textEditBoxString = "";	
	menu_level = TEXT_EDIT_LVL;
}

void GUI::FinishTextEditBox()
{
	delete slTextEditBox;
	slTextEditBox = NULL;
	menu_level = MENU_LVL;
}

// List
void GUI::ShowListBox(vector<string> items, function<void(string)> callback)
{
	slListBox = new ListBox(items, callback);
	menu_level = LIST_LVL;
}

void GUI::FinishListBox()
{
	delete slListBox;
	slListBox = NULL;
	menu_level = MENU_LVL;
}

// Dialog
void GUI::ShowDlgBox(string title, string text, int mode)
{
	dlgBox = new DlgBox(title, text, bind(&GUI::DlgBoxOK, this, _1), mode);		
	menu_level = DIALOG_LVL;
}

void GUI::DlgBoxOK(int mode)
{
	delete dlgBox;
	dlgBox = NULL;
	menu_level = mode;
}

// Info
void GUI::LoadInfo()
{
	if (scene->GetInfo().size() > 0)
		infoBox = new InfoBox(scene->GetInfo());
	else
		infoBox = nullptr;
}

void GUI::SwitchShowInfo()
{
	if (menu_level == ZERO_LVL)
		isShowInfo = isShowInfo ? false : true;
}

// System Info
void GUI::ShowSystemInfo()
{
	string scale_string = "System scale: ";	

	if (scene->GetSystemScale() >= 1)		
		scale_string += "1:" + RemoveZerro(to_string(scene->GetSystemScale()));
	else
		scale_string += RemoveZerro(to_string(scene->GetSystemScale())) + ":1";

	vector<string> lines =
	{
		"[ System Info ]",
		"Time: " + GetStringTime(scene->GetTimeNow()),
		"Time scale: 1:" + RemoveZerro(to_string(scene->GetTimeScale())),
		scale_string,
		"Distance scale: 1:" + RemoveZerro(to_string(scene->GetDistanceScale())),
		"FPS: " + to_string(fps)
	};

	systemInfoBox = new InfoBox(lines);
	systemInfoBox->SetInfoBoxSize(preloader->GetScreen().width*0.89, preloader->GetScreen().height*0.99, preloader->GetScreen().width*0.99, preloader->GetScreen().height * 0.99 - systemInfoBox->GetBoxHeight());
	systemInfoBox->Draw();
}

// Bottom Button
void GUI::ShowBottomButton()
{
	vector<string> lines = { "CLICK TO START OR PRESS 'ESC'" };
	bottomButton = new InfoBox(lines);
	bottomButton->SetInfoBoxSize(preloader->GetScreen().width * 0.445, preloader->GetScreen().height * 0.05 + bottomButton->GetBoxHeight(), preloader->GetScreen().width * 0.555, preloader->GetScreen().height * 0.05);
	bottomButton->Draw();
}

// Navigation
void GUI::PressedEnter()
{
	switch (menu_level)
	{
		case MENU_LVL:
			slMenu->Finish();
			break;
		case TEXT_EDIT_LVL:
			slTextEditBox->Finish();
			break;
		case LIST_LVL:
			slListBox->EnterSelectedItem();
			break;
		case DIALOG_LVL:
			dlgBox->Finish();
			break;
	}
}

void GUI::PressedEsc()
{
	switch (menu_level)
	{
	case ZERO_LVL:
		SLMenu_Main();
		menu_level = MENU_LVL;
		break;
	case MENU_LVL:
		SLMenu_Main();
		menu_level = ZERO_LVL;
		break;
	case TEXT_EDIT_LVL:
		FinishTextEditBox();
		break;
	case LIST_LVL:
		FinishListBox();
		break;
	case DIALOG_LVL:
		dlgBox->Finish();
		break;
	}
}

void GUI::PressedKeyUp()
{
	switch (menu_level)
	{
	case MENU_LVL:
		slMenu->SetPreviousPosition();
		break;	
	case LIST_LVL:
		slListBox->SetPreviousPosition();
		break;
	}
}

void GUI::PressedKeyDown()
{
	switch (menu_level)
	{
	case MENU_LVL:
		slMenu->SetNextPosition();
		break;
	case LIST_LVL:
		slListBox->SetNextPosition();
		break;
	}
}

void GUI::PressedBackspace()
{
	switch (menu_level)
	{
		case TEXT_EDIT_LVL:
			RemoveLastCharFromTextEditBoxString();
			break;
	}
}

void GUI::PressedSymbol(int key)
{
	switch (menu_level)
	{
		case TEXT_EDIT_LVL:
			AddCharToTextEditBoxString(key);
			break;
	}
}

void GUI::MouseMove(int x, int y)
{
	switch (menu_level)
	{
	case MENU_LVL:
		slMenu->CheckIntersection(x, y);
		break;
	case LIST_LVL:
		slListBox->CheckIntersection(x, y);
		break;
	}
}

void GUI::MouseClick(int x, int y)
{
	switch (menu_level)
	{
	case MENU_LVL:
		if (slMenu->CheckIntersection(x, y))
			slMenu->Finish();
		break;
	case LIST_LVL:
		if (slListBox->CheckIntersection(x, y))
			slListBox->EnterSelectedItem();
		break;
	case DIALOG_LVL:
		dlgBox->Finish();
		break;
	}

	if (bottomButton->CheckIntersection(x, y))
		PressedEsc();
}

void GUI::SetFPS(int fps)
{
	this->fps = fps;
}

bool GUI::ZerroMode()
{
	return menu_level == ZERO_LVL;
}

void GUI::Draw()
{
	if (infoBox != nullptr && isShowInfo)
		infoBox->Draw();

	ShowSystemInfo();

	ShowBottomButton();

	switch (menu_level)
	{
	case MENU_LVL:
		slMenu->Draw();
		break;
	case TEXT_EDIT_LVL:
		slTextEditBox->Draw();
		break;
	case LIST_LVL:
		slListBox->Draw();
		break;
	case DIALOG_LVL:
		dlgBox->Draw();
		break;
	}
}