#include "GDLevelLoader.h"
#include "../../../Encryption/Encryption.h"
#include "../../../String/StringAPI.h"
#include "../../../Web/Web.h"
#include "../../../Physics/Colliders/BoxCollider.h"
//#include "Graphics/Cores/Texture/rectpack2D/finders_interface.h"

void GDLevelLoader::OnStart()
{
	/*WebRequest request("http://www.boomlings.com/database/downloadGJLevel22.php", WebRequest::HttpMethod::HTTP_POST);
	WebResponse response;
	WebForm form;

	form.AddField("gameVersion", "20");
	form.AddField("binaryVersion", "35");
	form.AddField("gdw", "0");
	form.AddField("levelID", "1650666");
	form.AddField("secret", "Wmfd2893gb7");

	request.SendWebRequest(&response, form);

	std::string urlResult = response.body;*/

	/*std::string content = Files::Read("Game/Textures/Blocks.zip", true);
	content = Encryption::LZMA::Compress(content);
	Files::Write("Game/Textures/Blocks.retropkg", content);*/

	//std::cout << Files::GetDirectoryOf("Game/Textures/Blocks/blocks.gtxp") << std::endl;

	LoadVerifier();

	//SetupScene();
}

void GDLevelLoader::ReadLevel(std::string rawData)
{
	std::vector<std::string> allObjects = StringAPI::SplitIntoVector(rawData, ";");
	LoadCustomProperties();

	TextureManager::UploadToGPU();

	for (auto i : allObjects)
	{
		if (!StringAPI::StartsWith(i, "kS"))
		{
			int objId = 0, sortingLayer = 0, sortingOrder = 0;
			Vector3 position, rotation, scale(1);
			bool fX = false, fY = false, noC = false;
			std::vector<std::string> allValues = StringAPI::SplitIntoVector(i, ",");
			for (int id = 0; id < allValues.size(); id += 2)
			{
				if (allValues[id] != "" && allValues[id + 1] != "")
				{
					int value = std::stoi(allValues[id]);
					float result = 0;
					try
					{
						result = std::stof(allValues[id + 1]);
					}
					catch (...)
					{

					}

					switch (value)
					{
					// Object ID
					case 1:
						objId = result;
						for (auto size : customSize)
						{
							if (size.first == result)
							{
								scale = size.second;
								break;
							}
						}

						for (auto collisions : noCollisions)
						{
							if (collisions == result)
							{
								noC = true;
								break;
							}
						}
						break;

					// X Position
					case 2:
						position.x = result / 30.0f;
						break;

					// Y Position
					case 3:
						position.y = result / 30.0f;
						break;

					//Flip X Texture
					case 4:
						if (allValues[id + 1] == "1")
							fX = true;
						break;

					//Flip Y Texture
					case 5:
						if (allValues[id + 1] == "1")
							fY = true;
						break;

					// Z Rotation
					case 6:
						rotation.z -= result;
						break;

					/*case 21:
						colorID = std::stoi(results[i]);
						break;*/

					//Sorting Layer
					case 24:
						sortingLayer = result;
						break;

					//Sorting Order
					case 25:
						sortingOrder = result;
						break;

					// XY Scale
					case 32:
						if (scale.x != 0 && scale.y != 0)
						{
							scale.x *= result;
							scale.y *= result;
						}
						break;
					}
				}
			}
			
			Model* newObj = new Model(Model::Square(), position, rotation, scale);

			newObj->sortingLayer = sortingLayer;
			newObj->sortingOrder = sortingOrder;

			newObj->FlipTexture(fX, fY);

			if (!noC)
			{
				bool gotCustomSize = false;
				for (auto collSize : customCollisionSize)
				{
					if (collSize.first == objId)
					{
						BoxCollider* b = newObj->AddScript<BoxCollider>(collSize.second);
						gotCustomSize = true;
						break;
					}
				}

				if(!gotCustomSize)
					newObj->AddScript<BoxCollider>();
			}

			if(objId < GDTextures.size())
				newObj->SetTexture(*GDTextures[objId]);

			RendererCore::AddModel(*newObj);
		}
	}

	RendererCore::UpdateSorting(*SceneManager::MainScene().MainDrawCall());
}

void GDLevelLoader::LoadCustomProperties()
{
	//Custom Size
	//|
	//|
	//- Rods
	AddCustomSize(15, Vector3(0.21 * 1.5, 1.33 * 1.5, 1));
	AddCustomSize(16, Vector3(0.2 * 1.5, 0.88 * 1.5, 1));
	AddCustomSize(17, Vector3(0.18 * 1.5, 0.41 * 1.5, 1));
	//- Portals		[portal_XX_front_001.png]
	AddCustomSize(10, Vector3(50.f / 60, 150.f / 60, 1));//gravity down		( 1)
	AddCustomSize(11, Vector3(50.f / 60, 150.f / 60, 1));//gravity up			( 2)
	AddCustomSize(12, Vector3(68.f / 60, 172.f / 60, 1));//cube				( 3)
	AddCustomSize(13, Vector3(68.f / 60, 172.f / 60, 1));//ship				( 4)
	AddCustomSize(45, Vector3(88.f / 60, 184.f / 60, 1));//mirror				( 5)
	AddCustomSize(46, Vector3(88.f / 60, 184.f / 60, 1));//unmirror			( 6)
	AddCustomSize(47, Vector3(68.f / 60, 172.f / 60, 1));//ball				( 7)
	AddCustomSize(99, Vector3(62.f / 60, 180.f / 60, 1));//unmini				( 8)
	AddCustomSize(101, Vector3(62.f / 60, 180.f / 60, 1));//mini				( 9)
	AddCustomSize(111, Vector3(68.f / 60, 172.f / 60, 1));//ufo				(10)
	AddCustomSize(286, Vector3(82.f / 60, 182.f / 60, 1));//dual mode			(11)
	AddCustomSize(287, Vector3(82.f / 60, 182.f / 60, 1));//undual mode		(12)
	AddCustomSize(660, Vector3(68.f / 60, 172.f / 60, 1));//wave				(13)
	AddCustomSize(745, Vector3(68.f / 60, 172.f / 60, 1));//robot				(14)
	AddCustomSize(747, Vector3(78.f / 60, 180.f / 60, 1));//teleport in		(15)
	AddCustomSize(749, Vector3(78.f / 60, 180.f / 60, 1));//teleport out		(16)
	AddCustomSize(1331, Vector3(68.f / 60, 172.f / 60, 1));//robot				(17)
	//- differently-scaled blocks
	// what the fuck rob

	//No Collisions
	//|
	//|
	//- Rods
	AddNoCollisions(15);
	AddNoCollisions(16);
	AddNoCollisions(17);

	//Custom Collision Size
	//|
	//|
	//- Spikes
	AddCustomCollisionSize(8, Vector3(0.2, 0.2, 1));
	AddCustomCollisionSize(39, Vector3(0.2, 0.2, 1));
	AddCustomCollisionSize(103, Vector3(0.2, 0.2, 1));
}

void GDLevelLoader::AddCustomSize(int id, Vector3 size)
{
	customSize.push_back(std::make_pair(id, size));
}

void GDLevelLoader::AddNoCollisions(int id)
{
	noCollisions.push_back(id);
}

void GDLevelLoader::AddCustomCollisionSize(int id, Vector3 size)
{
	customCollisionSize.push_back(std::make_pair(id, size));
}

void CanICount()
{
	std::cout << "Can i Count? ofc i can!" << '\n';
	std::cout << "1.." << '\n';
	std::cout << "2.." << '\n';
	std::cout << "3!" << '\n';
}
ImGUIElement* win = nullptr;
DrawCall* VerifyUI = nullptr;
void GDLevelLoader::LoadVerifier()
{
	VerifyUI = SceneManager().MainScene().CreateDrawCall();

	VerifyUI->sort = DrawCall::Sorting::AtStartup;
	VerifyUI->type = DrawCall::Type::UI;
	VerifyUI->Close();

	win = GUIML::NewGUIML("Game/UI/FirstTime.guiml");

	TextureManager::UploadToGPU();

	RendererCore::AddImGUIElement(*win, VerifyUI->Target());
}

bool GDL_CanLoadScene = true;
void GDLevelLoader::OnUpdate()
{
	std::string bMessage = Broadcast::ShowBroadcastMessage("--rfp-play {Server URL}|{Level ID}");
	if (Broadcast::Recieve(bMessage) && GDL_CanLoadScene)
	{
		std::string res = StringAPI::RemoveAll(bMessage, "--rfp-play ");
		std::vector<std::string> values = StringAPI::SplitIntoVector(res, "|");

		values[0] = SetURLPrefixes(values[0]);

		if (values.size() == 1)
			OpenServer(values[0]);
		else if(values[1] == "")
			OpenServer(values[0]);
		else
		{
			SetupScene(values[0], std::stoi(values[1]));
		}

		Broadcast::Send("--rfp-loading");
		GDL_CanLoadScene = false;
	}
}

void GDLevelLoader::SetupScene(std::string url, int id)
{
	GDTextures = TextureManager::OpenTexturePack("Game/Textures/Blocks/blocks.gtxp");

	//TextureManager::OpenSpriteSheet("Game/Textures/Blocks/GJ_GameSheet.plist");

	// Stereo Madness: 1556066
	// How by Spu7nix: 63395980
	// Future Funk by JonathanGD: 44062068

	if (StringAPI::EndsWith(url, "/"))
	{
		std::cout << "Ends with '/'" << std::endl;
	}

	std::string finalUrl = "{}/database/downloadGJLevel22.php\0";

	if (url.find("/server") != std::string::npos || url.find("/gdps/gdapi") != std::string::npos)
		finalUrl = StringAPI::ReplaceAll(finalUrl, "/database", "");

	finalUrl = StringAPI::ReplaceAll(finalUrl, "{}", url);

	std::string formPost = "gameVersion=20&binaryVersion=35&gdw=0&levelID={}&secret=Wmfd2893gb7\0";
	formPost = StringAPI::ReplaceAll(formPost, "{}", std::to_string(id));

	std::string urlResult;

	// I'm hardcoding the URL because 
	// GD's CloudFlare protection actually 
	// looks for the size of a const char*
	// for some reason.
	if(url == "http://boomlings.com")
		urlResult = Web::Post("http://www.boomlings.com/database/downloadGJLevel22.php", formPost.c_str());
	else
		urlResult = Web::Post(finalUrl.c_str(), formPost.c_str());

	std::string levelSubstring = StringAPI::GetSubstringBetween(urlResult, ":4:", ":5:");

	if (!StringAPI::StartsWith(levelSubstring, "kS"))
	{
		if (!StringAPI::StartsWith(levelSubstring, "-1") || levelSubstring != "")
		{
			//std::cout << levelSubstring << std::endl;
			std::string decodeBase64 = Encryption::Base64::Decode(levelSubstring, true);
			std::string decodeZLib = Encryption::ZLib::Inflate(decodeBase64, Encryption::ZLib::Format::GZIP);

			if (decodeZLib == "")
			{
				// This means the level is old, decode in ZLIB instead.
				decodeZLib = Encryption::ZLib::Inflate(decodeBase64, Encryption::ZLib::Format::ZLIB);
			}

			ReadLevel(decodeZLib);
		}
		else
		{
			std::cout << "Returned -1" << std::endl;
		}
	}
	else
	{
		ReadLevel(levelSubstring);
	}
}

void GDLevelLoader::OpenServer(std::string url)
{
	std::string finalUrl = url;
	finalUrl += "/rfp/index.guiml";
	std::cout << finalUrl << std::endl;
	ImGUIElement* serverUI = GUIML::NewGUIML(finalUrl);
	RendererCore::AddImGUIElement(*serverUI, VerifyUI->Target());
}
