#include "SpaceScene.h"

SpaceScene* SpaceScene::instance = 0;

SpaceScene* SpaceScene::GetInstance()
{
	if (instance == 0)
		instance = new SpaceScene();

	return instance;
}

SpaceScene::SpaceScene()
{
	system_time = 0;
	time_scale = 1;
	time_increment = 0.025;
	system_scale = 1;
	distance_scale = 1;
	isDead = false;
	colorIDs = 0x000000;

	preloader = PreLoader::GetInstance();
			   
	camera = new GxCamera();
	camera->SetSensitivity(0.3f);
	camera->SetViewMatrixPointer(glm::value_ptr(viewMatrix));

	ResetCameraPosition();	

	background = new GxBackground();

	far_stars = new FarStars();

	GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

	GLfloat LightAmbient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);

	GLfloat LightDiffuse[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);

	GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glDepthRange(0.0, 1.0);
	glClearColor(0, 0, 0, 1);
}

SpaceScene::~SpaceScene()
{
	satellites.clear();
	planets.clear();
	system_stars.clear();

	delete background;
	delete far_stars;
	delete camera;
}

void SpaceScene::Update()
{
	system_time += time_increment * time_scale;		
}

void SpaceScene::Resize(const int width, const int height)
{
	projectionMatrix = glm::perspective(45.0f, (float)preloader->GetScreen().width / (float)preloader->GetScreen().height, 0.128f, 2048.0f);

	for (Star* s : system_stars)
		s->Resize();
}

void SpaceScene::SetDeadScene(bool isDead)
{
	this->isDead = isDead;
}

bool SpaceScene::GetDeadScene()
{
	return isDead;
}

void SpaceScene::SetTimeIncrement(double s)
{
	this->time_increment = s;
}

void SpaceScene::SetTimeNow(double time_now)
{
	system_time = time_now;
}

double SpaceScene::GetTimeNow()
{
	return system_time;
}

void SpaceScene::SetTimeScale(double time_scale)
{
	this->time_scale = time_scale;
}

double SpaceScene::GetTimeScale()
{
	return time_scale;
}

void SpaceScene::SetSystemScale(double scale)
{
	double deltaScale = system_scale / scale;

	for (Star* s : system_stars)
		s->SetScale(deltaScale);

	for (Planet* p : planets)
		p->SetScale(deltaScale);

	for (Satellite* s : satellites)
		s->SetScale(deltaScale);

	this->system_scale = scale;
}

double SpaceScene::GetSystemScale()
{
	return system_scale;
}

void SpaceScene::SetDistanceScale(double scale)
{
	double deltaScale = distance_scale / scale;
		
	for (Planet* p : planets)
		p->SetSemiMajorAxis(p->GetSemiMajorAxis() * deltaScale);

	for (Satellite* s : satellites)
		s->SetSemiMajorAxis(s->GetSemiMajorAxis() * deltaScale);

	distance_scale = scale;
}

double SpaceScene::GetDistanceScale()
{
	return distance_scale;
}

void SpaceScene::AddStar(Star* star)
{
	colorIDs++;
	star->SetColorId(colorIDs);
	system_stars.push_back(star);
}

void SpaceScene::AddPlanet(Planet* planet)
{
	colorIDs++;
	planet->SetColorId(colorIDs);
	planets.push_back(planet);
}

void SpaceScene::AddSatellite(Satellite* satellite)
{
	colorIDs++;
	satellite->SetColorId(colorIDs);
	satellites.push_back(satellite);
}

template <class T>
void SpaceScene::deleteMarkedObjects(vector<T>* _t)
{
	if (!_t->empty())
		_t->erase(std::remove_if(_t->begin(), _t->end(), [](T obj) { return obj->GetIsDead() == true; }), std::end(*_t));
}

void SpaceScene::DeleteDead()
{
	deleteMarkedObjects(&system_stars);
	deleteMarkedObjects(&planets);
	deleteMarkedObjects(&satellites);
}

void SpaceScene::SetDeadStar(string name)
{
	for (Star* s : system_stars)
		if (s->GetName() == name)
			s->SetIsDead(true);
}

void SpaceScene::SetDeadPlanet(string name, bool parent)
{
	SetDeadStatus(&planets, name, parent);
}

void SpaceScene::SetDeadSatellite(string name, bool parent)
{
	SetDeadStatus(&satellites, name, parent);
}

void SpaceScene::SetDeadByParent()
{
	SetDeadStatusByDeadParent(&planets);
	SetDeadStatusByDeadParent(&satellites);
}

void SpaceScene::SetSelectedStar(string name)
{
	SetSelectedStatus(&system_stars, name);
}

void SpaceScene::SetSelectedPlanet(string name)
{	
	SetSelectedStatus(&planets, name);
}

void SpaceScene::SetSelectedSatellite(string name)
{
	SetSelectedStatus(&satellites, name);
}

void SpaceScene::UpdateEulerAngles()
{
	SetEulerAngles(&system_stars, camera->GetEulerAngles());
	SetEulerAngles(&planets, camera->GetEulerAngles());
	SetEulerAngles(&satellites, camera->GetEulerAngles());
}

template <class T>
void SpaceScene::SetDeadStatus(vector<T> *objects, string name, bool parent)
{
	for (int i = 0; i != objects->size(); i++)
	{
		string check = parent ? objects->at(i)->GetParent()->GetName() : objects->at(i)->GetName();
		if (check == name)
			objects->at(i)->SetIsDead(true);		
	}
}

template <class T>
void SpaceScene::SetDeadStatusByDeadParent(vector<T>* objects)
{
	for (int i = 0; i != objects->size(); i++)
		if (objects->at(i)->GetParent()->GetIsDead())
			objects->at(i)->SetIsDead(true);
}

template <class T>
void SpaceScene::SetSelectedStatus(vector<T> *objects, string name)
{
	DeselectAll();

	for (int i = 0; i != objects->size(); i++)
	{
		if (objects->at(i)->GetName() == name)
		{
			objects->at(i)->SetSelected(true);
			break;
		}
	}
}

template <class T>
T SpaceScene::SetSelectedStatus(vector<T>* objects, vec3b color)
{
	DeselectAll();

	for (int i = 0; i != objects->size(); i++)
	{
		if (CheckColor(objects->at(i)->GetColor(), color))
		{
			objects->at(i)->SetSelected(true);			
			return objects->at(i);
		}
	}

	return nullptr;
}

template <class T>
T SpaceScene::CheckSelectedStatus(vector<T>* objects, bool onFocus)
{
	for (int i = 0; i != objects->size(); i++)
	{
		if (objects->at(i)->GetSelected())
		{
			if (onFocus)
			{
				camera->LookAt(glm::vec3(objects->at(i)->GetPosition().x, objects->at(i)->GetPosition().y, objects->at(i)->GetPosition().z + objects->at(i)->GetExpDist()), glm::vec3(objects->at(i)->GetPosition().x, objects->at(i)->GetPosition().y, objects->at(i)->GetPosition().z), true);
				objects->at(i)->SetEulerAngles(camera->GetEulerAngles());
			}

			return objects->at(i);
		}
	}

	return nullptr;
}

template <class T>
void SpaceScene::SetEulerAngles(vector<T> *objects, glm::vec3 angles)
{
	for (int i = 0; i != objects->size(); i++)
		objects->at(i)->SetEulerAngles(angles);
}

void SpaceScene::SimpleDraw()
{
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(viewMatrix));

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	for (Star* s : system_stars)
		s->SimpleDraw();

	for (Planet* p : planets)
		p->SimpleDraw();

	for (Satellite* s : satellites)
		s->SimpleDraw();
}

void SpaceScene::Draw()
{
	background->Draw();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(viewMatrix));

	if (system_stars.size() > 0)
		glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(glm::vec4(system_stars[0]->GetPosition(), 1.0f)));

	far_stars->Draw();

	for (auto p : planets)
	{
		p->Update(system_time);
		p->Draw();
	}

	for (auto s : satellites)
	{
		s->Update(system_time);
		s->Draw();
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	for (auto s : system_stars)
	{
		s->Update(system_time);
		s->Draw(viewMatrix, projectionMatrix, *camera);
	}	

	CollisionMonitor();
}

void SpaceScene::MoveCamera(const unsigned int edge)
{
	camera->Move(camera->OnEdges(edge, 0.25f));
}

void SpaceScene::RotateCamera(const int x, const int y)
{
	camera->OnMouseMove(x, y);
}

void SpaceScene::ZoomCamera(const double deltaZ)
{
	camera->OnMouseWheel(deltaZ);
}

void SpaceScene::ResetCameraPosition()
{
	camera->LookAt(glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.0f, 0.0f, 0.0f), true);
}

void SpaceScene::FocusOnSelectedObject()
{
	Star* star = CheckSelectedStatus(&system_stars, true);
	
	if (star == nullptr)
	{
		Planet* planet = CheckSelectedStatus(&planets, true);

		if (planet == nullptr)
			Satellite* satellite = CheckSelectedStatus(&satellites, true);
	}
}

void SpaceScene::SelectByColor(vec3b color)
{
	Star* s = SetSelectedStatus(&system_stars, color);

	if (s != nullptr)
	{
		SetStarInfo(s);
	} else {
		Planet* p = SetSelectedStatus(&planets, color);

		if (p != nullptr)
		{
			SetPlanetInfo(p);
		} else {
			Satellite* s = SetSelectedStatus(&satellites, color);
			if (s != nullptr)
				SetSatelliteInfo(s);
		}
	}
}

void SpaceScene::DeselectAll()
{
	info.clear();

	for (Star* s : system_stars)
		s->SetSelected(false);

	for (Planet* p : planets)
		p->SetSelected(false);

	for (Satellite* s : satellites)
		s->SetSelected(false);
}

void SpaceScene::RemoveAll()
{
	satellites.clear();
	planets.clear();
	system_stars.clear();
}

template <class T, class S>
void SpaceScene::SetParentByName(string name, T *obj, vector<S> vecs)
{
	for (unsigned int i = 0; i != vecs.size(); ++i)
		if (vecs[i]->GetName() == name)
			obj->SetParent(vecs[i]);
}

void SpaceScene::SetPlanetParent(string name, Planet *p)
{
	SetParentByName(name, p, system_stars);
}

void SpaceScene::SetSatelliteParent(string name, Satellite* s)
{
	SetParentByName(name, s, planets);
}

template <class T>
bool SpaceScene::CheckExistName(vector<T>* _t, string name)
{
	for (unsigned int i = 0; i < _t->size(); ++i)
	{
		if (_t->at(i)->GetName() == name)
			return true;
	}

	return false;
}

bool SpaceScene::CheckPlanetExistName(string name)
{
	return CheckExistName(&planets, name);
}

bool SpaceScene::CheckSatelliteExistName(string name)
{
	return CheckExistName(&satellites, name);
}

vector<string> SpaceScene::GetInfo()
{
	return info;
}

void SpaceScene::SetPlanetInfo(Planet* obj)
{
	string sats = "Satellites: ";
	bool satExist = false;

	for (Satellite* s : satellites)
	{
		if (s->GetParent()->GetName() == obj->GetName())
		{
			sats += s->GetName() + ", ";
			satExist = true;
		}
	}

	if (satExist)
		sats = sats.substr(0, sats.size() - 2);
	else
		sats += "none";

	info = {
		"[ Planet Info ]",
		"Name: " + obj->GetName(),
		"Mean radius: " + RemoveZerro(to_string(obj->GetMeanRadius() * GetSystemScale())) + " km",
		"Aphelion: " + RemoveZerro(to_string((obj->GetEccentricity() * obj->GetSemiMajorAxis() + obj->GetSemiMajorAxis()) * GetSystemScale())) + " km",
		"Perihelion: " + RemoveZerro(to_string((obj->GetSemiMajorAxis() - obj->GetEccentricity() * obj->GetSemiMajorAxis()) * GetSystemScale())) + " km",
		"Semi-major axis: " + RemoveZerro(to_string(obj->GetSemiMajorAxis() * GetSystemScale())) + " km",
		"Eccentricity: " + RemoveZerro(to_string(obj->GetEccentricity())),
		"Orbital period: " + RemoveZerro(to_string(obj->GetSiderealPeriod())) + " h",
		"Inclination: " + RemoveZerro(to_string(obj->GetOrbitalInclination())) + (char)-80 + " (ecliptic)",
		"Sidereal rotation period: " + RemoveZerro(to_string(obj->GetRotation())) + " h",
		"Axial tilt: " + RemoveZerro(to_string(obj->GetObliquity())) + (char)-80,
		sats
	};
}

void SpaceScene::SetSatelliteInfo(Satellite* obj)
{
	info = {
		"[ Satellite Info ]",
		"Name: " + obj->GetName(),
		"Mean radius: " + RemoveZerro(to_string(obj->GetMeanRadius() * GetSystemScale())) + " km",
		"Apogee: " + RemoveZerro(to_string((obj->GetEccentricity() * obj->GetSemiMajorAxis() + obj->GetSemiMajorAxis()) * GetSystemScale())) + " km",
		"Perigee: " + RemoveZerro(to_string((obj->GetSemiMajorAxis() - obj->GetEccentricity() * obj->GetSemiMajorAxis()) * GetSystemScale())) + " km",
		"Semi-major axis: " + RemoveZerro(to_string(obj->GetSemiMajorAxis() * GetSystemScale())) + " km",
		"Eccentricity: " + RemoveZerro(to_string(obj->GetEccentricity())),
		"Orbital period: " + RemoveZerro(to_string(obj->GetSiderealPeriod())) + " h",
		"Inclination: " + RemoveZerro(to_string(obj->GetOrbitalInclination())) + (char)-80 + " (ecliptic)",
		"Sidereal rotation period: " + RemoveZerro(to_string(obj->GetRotation())) + " h",
		"Axial tilt: " + RemoveZerro(to_string(obj->GetObliquity())) + (char)-80,
		"Parent: " + obj->GetParent()->GetName()
	};
}

void SpaceScene::SetStarInfo(Star* obj)
{
	string sats = "Satellites: ";
	bool satExist = false;

	for (Planet* p : planets)
	{
		if (p->GetParent()->GetName() == obj->GetName())
		{
			sats += p->GetName() + ", ";
			satExist = true;
		}
	}

	if (satExist)
		sats = sats.substr(0, sats.size() - 2);
	else
		sats += "none";

	info = {
		"[ Star Info ]",
		"Name: " + obj->GetName(),
		"Equatorial radius: " + RemoveZerro(to_string(obj->GetEquatorialRadius() * GetSystemScale())) + " km",
		"Sidereal rotation period: " + RemoveZerro(to_string(obj->GetRotation())) + " h",
		"Axial tilt: " + RemoveZerro(to_string(obj->GetObliquity())) + (char)-80,
		"Spectral class: " + obj->GetSpectral(),
		sats
	};
}

bool SpaceScene::HasVecObj(int type_obj)
{
	switch (type_obj)
	{
		case STAR_TYPE:
			return (system_stars.size() > 0) ? true : false;
			break;
		case PLANET_TYPE:
			return (planets.size() > 0) ? true : false;
			break;
		case SATELLITE_TYPE:
			return (satellites.size() > 0) ? true : false;
			break;
		default:
			return false;
			break;
	}
}

vector<string> SpaceScene::GetVecNames(int type_obj)
{
	vector<string> res;

	switch (type_obj)
	{
	case STAR_TYPE:
		for (Star* s : system_stars)
			res.push_back(s->GetName());
		break;
	case PLANET_TYPE:
		for (Planet* p : planets)
			res.push_back(p->GetName());
		break;
	case SATELLITE_TYPE:
		for (Satellite* s : satellites)
			res.push_back(s->GetName());
		break;
	}

	return res;
}

void SpaceScene::SaveSystem(string name)
{
	string path = GetModuleDirectory() + "data\\saves\\" + name + ".xml";
	ofstream fn;
	fn.open(path);

	fn << "<settings>\n\t<settings time_scale=\"" << GetTimeScale() << "\" "
		<< "system_scale=\"" << GetSystemScale() << "\" "
		<< "distance_scale=\"" << GetDistanceScale() << "\" "
		<< "time_now=\"" << GetTimeNow() << "\" "
		<< "/>\n</settings>\n";

	fn << "<stars>\n";

	for (Star* s : system_stars)
	{
		fn << "\t<star name=\"" << s->GetName() << "\" "
			<< "equatorial_radius=\"" << s->GetEquatorialRadius() << "\" "
			<< "obliquity=\"" << s->GetObliquity() << "\" "
			<< "rotation=\"" << s->GetRotation() << "\" "
			<< "spectral=\"" << s->GetSpectral() << "\" "
			<< "position_x=\"" << s->GetPosition().x << "\" "
			<< "position_y=\"" << s->GetPosition().y << "\" "
			<< "position_z=\"" << s->GetPosition().z << "\" "
			<< "/>\n";
	}

	fn << "</stars>\n";

	fn << "<planets>\n";

	for (Planet* p : planets)
	{
		fn << "\t<planet name=\"" << p->GetName() << "\" "
			<< "mean_radius=\"" << p->GetMeanRadius() << "\" "
			<< "obliquity=\"" << p->GetObliquity() << "\" "
			<< "rotation=\"" << p->GetRotation() << "\" "
			<< "orbital_inclination=\"" << p->GetOrbitalInclination() << "\" "
			<< "sidereal_period=\"" << p->GetSiderealPeriod() << "\" "
			<< "eccentricity=\"" << p->GetEccentricity() << "\" "
			<< "semimajor_axis=\"" << p->GetSemiMajorAxis() << "\" "
			<< "parent=\"" << p->GetParent()->GetName() << "\" "
			<< "texture=\"" << p->GetTextureName() << "\" "
			<< "ring=\"" << p->GetRingTextureName() << "\" "
			<< "/>\n";
	}

	fn << "</planets>\n";

	fn << "<satellites>\n";

	for (Satellite* s : satellites)
	{
		fn << "\t<satellite name=\"" << s->GetName() << "\" "
			<< "mean_radius=\"" << s->GetMeanRadius() << "\" "
			<< "obliquity=\"" << s->GetObliquity() << "\" "
			<< "rotation=\"" << s->GetRotation() << "\" "
			<< "orbital_inclination=\"" << s->GetOrbitalInclination() << "\" "
			<< "sidereal_period=\"" << s->GetSiderealPeriod() << "\" "
			<< "eccentricity=\"" << s->GetEccentricity() << "\" "
			<< "semimajor_axis=\"" << s->GetSemiMajorAxis() << "\" "
			<< "parent=\"" << s->GetParent()->GetName() << "\" "
			<< "texture=\"" << s->GetTextureName() << "\" "
			<< "/>\n";
	}

	fn << "</satellites>\n";

	fn.close();
}

void SpaceScene::LoadSystem(string name)
{
	colorIDs = 0;
	string fn = "data\\saves\\" + name;
	vector<string> lines = GetXMLDataWithTag(ReadFile(fn), "settings");

	for (string& line : lines)
	{
		vector<string> v = GetValuesFromXMLData(line);
		SetTimeScale(stod(v[0]));
		SetSystemScale(stod(v[1].c_str()));
		SetDistanceScale(stod(v[2].c_str()));
		SetTimeNow(stod(v[3].c_str()));
	}

	lines = GetXMLDataWithTag(ReadFile(fn), "star");

	RemoveAll();

	for (string& line : lines)
	{
		colorIDs++;
		vector<string> v = GetValuesFromXMLData(line);
		Star *star = new Star();
		star->SetName(v[0]);
		star->SetEquatorialRadius((float)stof(v[1].c_str()));
		star->SetObliquity((float)stof(v[2].c_str()));
		star->SetRotation((float)stof(v[3].c_str()));
		star->SetSpectral(v[4]);
		star->SetPosition((float)stof(v[5]), (float)stof(v[6]), (float)stof(v[7]));
		star->BuildStar();
		star->Resize();
		AddStar(star);
	}

	lines = GetXMLDataWithTag(ReadFile(fn), "planet");

	for (string& line : lines)
	{
		colorIDs++;
		vector<string> v = GetValuesFromXMLData(line);
		Planet* planet = new Planet();
		string texture_path = "data\\textures\\planet\\" + v[9];
		planet->SetTexture(texture_path.c_str());
		planet->SetName(v[0]);
		planet->SetMeanRadius((float)stof(v[1].c_str()));
		planet->SetObliquity((float)stof(v[2].c_str()));
		planet->SetRotation((float)stof(v[3].c_str()));
		planet->SetOrbitalInclination((float)stof(v[4].c_str()));
		planet->SetSiderealPeriod((float)stof(v[5].c_str()));
		planet->SetEccentricity((float)stof(v[6].c_str()));
		planet->SetSemiMajorAxis((float)stof(v[7].c_str()));
		planet->SetScale(GetSystemScale());

		if (v[10].size() > 0)
		{
			string ring_path = "data\\textures\\ring\\" + v[10];
			planet->SetRingTexture(ring_path.c_str());
		}

		planet->BuildPlanet();
		SetPlanetParent(v[8], planet);
		AddPlanet(planet);
	}

	lines = GetXMLDataWithTag(ReadFile(fn), "satellite");

	for (string& line : lines)
	{
		colorIDs++;
		vector<string> v = GetValuesFromXMLData(line);
		Satellite* satellite = new Satellite();
		satellite->SetName(v[0]);
		satellite->SetMeanRadius((float)stof(v[1].c_str()));
		satellite->SetObliquity((float)stof(v[2].c_str()));
		satellite->SetRotation((float)stof(v[3].c_str()));
		satellite->SetOrbitalInclination((float)stof(v[4].c_str()));
		satellite->SetSiderealPeriod((float)stof(v[5].c_str()));
		satellite->SetEccentricity((float)stof(v[6].c_str()));
		satellite->SetSemiMajorAxis((float)stof(v[7].c_str()));
		satellite->SetScale(GetSystemScale());
		string texture_path = "data\\textures\\satellite\\" + v[9];
		satellite->SetTexture(texture_path.c_str());
		satellite->BuildSatellite();
		SetSatelliteParent(v[8], satellite);
		AddSatellite(satellite);
	}

	ResetCameraPosition();
}

void SpaceScene::CollisionMonitor()
{
	bool dead = false; 

	for (int i = 0; i < (int)satellites.size() - 1; ++i)	
	{
		for (int j = i + 1; j < (int)satellites.size(); ++j)
		{
			if (satellites[i]->GetPosition(), satellites[j]->GetPosition(), satellites[i]->GetMeanRadius(), satellites[j]->GetMeanRadius())
			{
				collideMessage = "The satellite " + satellites[i]->GetName() + " collided with the satellite " + satellites[j]->GetName();
				satellites[i]->SetIsDead(true);
				satellites[j]->SetIsDead(true);
				dead = true;
			}
		}

		for (int j = 0; j < (int)planets.size(); ++j)
		{
			if (CheckCollide(satellites[i]->GetPosition(), planets[j]->GetPosition(), satellites[i]->GetMeanRadius(), planets[j]->GetMeanRadius()))
			{
				collideMessage = "The satellite " + satellites[i]->GetName() + " collided with the planet " + planets[j]->GetName();
				satellites[i]->SetIsDead(true);
				planets[j]->SetIsDead(true);
				dead = true;
			}
		}

		for (int j = 0; j < (int)system_stars.size(); j++)
		{
			if (CheckCollide(satellites[i]->GetPosition(), system_stars[j]->GetPosition(), satellites[i]->GetMeanRadius(), system_stars[j]->GetEquatorialRadius()))
			{
				collideMessage = "The satellite " + satellites[i]->GetName() + " collided with the star " + system_stars[j]->GetName();
				satellites[i]->SetIsDead(true);
				system_stars[j]->SetIsDead(true);
				dead = true;
			}
		}
	}

	for (int i = 0; i < (int)planets.size() - 1; i++)
	{
		for (int j = i + 1; j < (int)planets.size(); j++)
		{
			if (CheckCollide(planets[i]->GetPosition(), planets[j]->GetPosition(), planets[i]->GetMeanRadius(), planets[j]->GetMeanRadius()))
			{
				collideMessage = "The planet " + planets[i]->GetName() + " collided with the planet " + planets[j]->GetName();
				planets[i]->SetIsDead(true);
				planets[j]->SetIsDead(true);
				dead = true;
			}
		}

		for (int j = 0; j < (int)system_stars.size(); j++)
		{
			if (CheckCollide(planets[i]->GetPosition(), system_stars[j]->GetPosition(), planets[i]->GetMeanRadius(), system_stars[j]->GetEquatorialRadius()))
			{
				collideMessage = "The planet " + planets[i]->GetName() + " collided with the star " + system_stars[j]->GetName();
				planets[i]->SetIsDead(true);
				system_stars[j]->SetIsDead(true);
				dead = true;
			}
		}
	}

	for (int i = 0; i < (int)system_stars.size() - 1; i++)
	{
		for (int j = i + 1; j < (int)system_stars.size(); j++)
		{
			if (CheckCollide(system_stars[i]->GetPosition(), system_stars[j]->GetPosition(), system_stars[i]->GetEquatorialRadius(), system_stars[j]->GetEquatorialRadius()))
			{
				collideMessage = "The star " + system_stars[i]->GetName() + " collided with the star " + system_stars[j]->GetName();
				system_stars[i]->SetIsDead(true);
				system_stars[j]->SetIsDead(true);
				dead = true;
			}
		}
	}

	if (dead)
	{
		SetDeadByParent();
		DeleteDead();
	}
}

string SpaceScene::GetCollideMessage()
{
	string temp = collideMessage;
	collideMessage = "";
	return temp;
}