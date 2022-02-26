#include "geometria.h"
#include "RFPLevelScene.h"
#include "../Physics/Colliders/BoxCollider.h"
#include "../Physics/Rigidbody/Rigidbody.h"
#include "Scripts/Player/PlayerController.h"
#include "Scripts/GeometryDash/GDLevelLoader.h"
#include "../Audio/AudioSource.h"

Model* RFPPlayer = NULL,
* RFPBaseGround = NULL;
DrawCall* UIDrawCall = NULL;

ScriptBehaviour* CreatePlayer()
{
	RFPPlayer = new Model(Model::Square(), Vector3(0, 0.5, 0), Vector3(0), Vector3(1));
	RFPPlayer->color = Color::yellow();
	RFPPlayer->AddScript<BoxCollider>();
	RFPPlayer->AddScript<Rigidbody>();
	RFPPlayer->GetScript<Rigidbody>()->freezePositionZ = true;
	RFPPlayer->GetScript<Rigidbody>()->freezePositionX = true;
	RFPPlayer->AddScript<PlayerController>();
	RFPPlayer->GetScript<PlayerController>()->camera = Graphics::MainCamera();
	RFPPlayer->AddScript<AudioSource>();
	RendererCore::AddModel(*RFPPlayer);
	return RFPPlayer;
}

void RFPLevelScene::Init()
{
	SceneManager::MainScene().MainDrawCall()->type = DrawCall::Type::Dynamic;
	SceneManager::MainScene().MainDrawCall()->sort = DrawCall::Sorting::AtStartup;
	SceneManager::MainScene().MainDrawCall()->mode = DrawCall::Mode::To2D;
	//Model Initialization

	Graphics::MainCamera()->transform.position = Vector3(0, 10, 6.5);

	CreatePlayer();

	Empty* GDLoader = new Empty();
	GDLoader->AddScript<GDLevelLoader>();

	RFPBaseGround = new Model(Model::Square(), Vector3(0, -2, 0), Vector3(0), Vector3(10000, 4, 1000));
	RFPBaseGround->AddScript<BoxCollider>();
	RendererCore::AddModel(*RFPBaseGround);
}
