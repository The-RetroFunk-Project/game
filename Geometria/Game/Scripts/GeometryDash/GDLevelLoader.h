#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

struct GDLevelLoader : public ScriptBehaviour
{
	void OnStart();
	void OnUpdate();

	std::vector<std::pair<int, Vector3>> customSize;
	std::vector<int> noCollisions;
	std::vector<std::pair<int, Vector3>> customCollisionSize;

	std::vector<Texture*> GDTextures;
	void ReadLevel(std::string rawData);
	void LoadCustomProperties();

	void AddCustomSize(int id, Vector3 size);
	void AddNoCollisions(int id);
	void AddCustomCollisionSize(int id, Vector3 size);

	float testing = 0;

	std::string username, password;

	void LoadVerifier();
	void SetupScene(std::string url, int id);

	void OpenServer(std::string url);

	std::string SetURLPrefixes(std::string getUrl)
	{
		std::string url = getUrl;

		if (url == "1.9gdps://")
			url = "https://absolllute.com/gdps/gdapi";
		else if (url == "gdpseditor2.2://")
			url = "https://game.gdpseditor.com/server";
		else if (url == "geometrydash://")
			url = "http://boomlings.com";

		return url;
	}
};