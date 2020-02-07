#ifndef __SPACE_SCENE_H__
#define __SPACE_SCENE_H__

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "StringUtils.h"
#include "PreLoader.h"
#include "Star.h"
#include "Planet.h"
#include "Satellite.h"
#include "FarStars.h"
#include "GxBackground.h"
#include "GxCamera.h"
#include "GxCursor.h"

#define STAR_TYPE 1
#define PLANET_TYPE 2
#define SATELLITE_TYPE 3

class SpaceScene
{
public:
	~SpaceScene();
	static SpaceScene* GetInstance();
	void Update();
	void SimpleDraw();
	void Draw();
	void Resize(const int width, const int height);
	void SetDeadByParent();
	void DeleteDead();
	void DeselectAll();
	void SelectByColor(vec3b color);	
	void SetPlanetParent(string name, Planet *p);
	void SetSatelliteParent(string name, Satellite* s);	
	bool CheckPlanetExistName(string name);
	bool CheckSatelliteExistName(string name);
	void SetTimeIncrement(double s);
	void SetTimeNow(double time_now);
	double GetTimeNow();
	void SetTimeScale(double time_scale);
	double GetTimeScale();
	void SetSystemScale(double scale);
	double GetSystemScale();
	void SetDistanceScale(double scale);
	double GetDistanceScale();
	void AddStar(Star* star);
	void AddPlanet(Planet* planet);
	void AddSatellite(Satellite* satellite);
	void SetDeadStar(string name);
	void SetDeadPlanet(string name, bool parent);
	void SetDeadSatellite(string name, bool parent);
	void SetSelectedStar(string name);
	void SetSelectedPlanet(string name);
	void SetSelectedSatellite(string name);
	void UpdateEulerAngles();	
	void MoveCamera(const unsigned int edge);
	void RotateCamera(const int x, const int y);
	void ZoomCamera(const double deltaZ);
	void ResetCameraPosition();
	void FocusOnSelectedObject();
	void SetDeadScene(bool isDead);
	bool GetDeadScene();
	void RemoveAll();
	vector<string> GetInfo();	
	bool HasVecObj(int type_obj);
	vector<string> GetVecNames(int type_obj);
	void SaveSystem(string name);
	void LoadSystem(string name);
	string GetCollideMessage();
				
private:
	SpaceScene();		
	template <class T>
	void deleteMarkedObjects(vector<T>* _t);
	template <class T>
	void SetDeadStatus(vector<T>* objects, string name, bool parent);
	template <class T>
	void SetSelectedStatus(vector<T>* objects, string name);
	template <class T>
	T SetSelectedStatus(vector<T>* objects, vec3b color);
	template <class T>
	void SetEulerAngles(vector<T>* objects, glm::vec3 angles);
	template <class T>
	T CheckSelectedStatus(vector<T>* objects, bool onFocus);
	template <class T>
	void SetDeadStatusByDeadParent(vector<T>* objects);
	template <class T, class S>
	void SetParentByName(string name, T* obj, vector<S> vecs);
	template <class T>
	bool CheckExistName(vector<T>* _t, string name);
	void SetStarInfo(Star* obj);
	void SetPlanetInfo(Planet* obj);
	void SetSatelliteInfo(Satellite* obj);
	void CollisionMonitor();
	
	vector<Star*> system_stars;
	vector<Planet*> planets;
	vector<Satellite*> satellites;
	static SpaceScene* instance;	
	PreLoader* preloader;
	double system_time;
	double system_scale;
	double distance_scale;
	double time_scale;
	double time_increment;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	GxCamera* camera;
	FarStars* far_stars;
	GxBackground* background;	
	bool isDead;
	vector<string> info;
	uint32_t colorIDs;
	string collideMessage;
};

#endif // !__SPACE_SCENE_H__