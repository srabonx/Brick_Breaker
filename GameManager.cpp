#include "GameManager.h"

#include "Input.h"

#include <HelperMethods.h>
#include <Serializer.h>
#include <stdexcept>

GameState GameManager::m_state = GameState::MENU;
BINDU::SoundSystem* GameManager::m_soundSystem = nullptr;

bool GameManager::m_musicOn = true;
bool GameManager::m_sfxOn = true;

ResourceFile	GameManager::RC;
GameSave		GameManager::GS;

GameManager::~GameManager()
{
	if (m_soundSystem)
		delete m_soundSystem;
}

void GameManager::Preload(const std::string& resourceFile) const
{
	BINDU::EngineProperties props;

#if defined (DEBUG) || defined (_DEBUG)
	props.versionNotice = true;
#else
	props.versionNotice = false;
#endif

	props.windowTitle = "Brick Breaker";
	props.windowWidth = m_worldSize.width;
	props.windowHeight = m_worldSize.height;
	props.windowIcon = "application_icon.ico";

	g_engine->setEngineProperties(props);
	m_state = GameState::MENU;

	FILE* file{ nullptr };
	BINDU::Serializer serializer;

	if (fopen_s(&file,resourceFile.c_str(), "r") == 0)
	{
		serializer.DeserializeFromXML("ResourceFile", resourceFile, RC);
	}
	else
	{
		serializer.SerializeToXML("ResourceFile", resourceFile, RC);
	}

	if (file)
	{
		if (fclose(file) != 0)
			throw std::runtime_error("Cannot open resource file");
	}

	auto saveGameFile = HM::RelativeResourcePath(RC.SaveGameFile);

	if(fopen_s(&file, saveGameFile.c_str(), "r") == 0)
	{
		serializer.DeserializeFromBinary("SaveGameFile", saveGameFile, GS);
	}
	else
	{
		serializer.SerializeToBinary("SaveGameFile", saveGameFile, GS);
	}

	if (file)
	{
		if (fclose(file) != 0)
			throw std::runtime_error("Cannot open save file");
	}
}

void GameManager::Init()
{
	m_soundSystem = new BINDU::SoundSystem();
	m_soundSystem->Init();

	m_menuScene = std::make_unique<MenuScene>();
	m_menuScene->Init();
	m_menuScene->setActive(true);

	m_mainScene = std::make_unique<MainScene>();
	m_mainScene->Init(m_worldSize);
	m_mainScene->setActive(false);
		
}

void GameManager::onLoadResource()
{
	//m_mainScene->onLoadResource();
	//m_menuScene->onLoadResource();

	m_soundSystem->Load("menumusic", HM::RelativeResourcePath(RC.MenuMusic).c_str());
	m_soundSystem->Load("gamemusic", HM::RelativeResourcePath(RC.GameMusic).c_str());
	m_soundSystem->Load("clicksound", HM::RelativeResourcePath(RC.ClickSound).c_str());
	m_soundSystem->Load("navsound", HM::RelativeResourcePath(RC.NavSound).c_str());


	m_sceneManager.AddScene(std::move(m_menuScene), "menu");
	m_sceneManager.AddScene(std::move(m_mainScene), "main");

	m_sceneManager.onLoadResource();
}

void GameManager::Update(float dt)
{
//	m_mainScene->ProcessInput();

	if(m_state == GameState::MENU)
	{
		if (m_musicOn && !m_soundSystem->isPlaying("menumusic"))
			m_soundSystem->Play("menumusic");

		if (!m_sceneManager.getScene("menu")->isActive())
			m_sceneManager.getScene("menu")->onLoadResource();


		m_sceneManager.getScene("main")->setActive(false);
		m_sceneManager.getScene("menu")->setActive(true);
	}
	else if(m_state == GameState::GAME)
	{
		if (m_musicOn && !m_soundSystem->isPlaying("gamemusic"))
			m_soundSystem->Play("gamemusic");

		if (m_sceneManager.getScene("menu")->isActive())
			m_sceneManager.getScene("menu")->onReleaseResource();

		m_sceneManager.getScene("main")->setActive(true);
		m_sceneManager.getScene("menu")->setActive(false);
	}


	m_sceneManager.ProcessInput();
	m_sceneManager.Update(dt);
//	m_mainScene->Update(dt);
	m_soundSystem->Update();
}

void GameManager::Draw(BINDU::Graphics* graphics)
{
	graphics->getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	
	m_sceneManager.Draw(graphics, D2D1::Matrix3x2F::Identity());
	//m_mainScene->Draw(graphics, D2D1::Matrix3x2F::Identity());


}

void GameManager::SaveGame()
{
	BINDU::Serializer serializer;
	serializer.SerializeToBinary("SaveGameFile", RC.SaveGameFile, GS);
}
