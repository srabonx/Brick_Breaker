#pragma once
#include <Bindu.h>
#include <HelperMethods.h>
#include <cereal/cereal.hpp>
#include <cereal/details/helpers.hpp>

#include "Constants.h"
#include "Font.h"
#include "MainScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "SoundSystem.h"


enum class GameState
{
	MENU,
	GAME
};

struct GameSave
{
	size_t HighScore{ 0 };

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(HighScore);
	}
};

struct ResourceFile
{
	std::string ButtonTexture;

	std::string SelectorTexture;

	std::string TitleTexture;

	std::string MenuBgTexture;

	std::string MenuParticleTexture;

	// Menu particle color (r,g,b)
	int Mpr;
	int Mpg;
	int Mpb;

	std::string MenuFontFile;

	std::string MenuFontWidthFile;

	// Menu font color (r,g,b)
	int Mfr;
	int Mfg;
	int Mfb;

	std::string MenuMovingFontFile;

	std::string MenuMovingFontWidthFile;

	// Menu Moving Font color (r,g,b)
	int Mmfr;
	int Mmfg;
	int Mmfb;

	std::string BallTexture;

	std::string BallParticleTexture;

	// Ball particle color (r,g,b)
	int Bpr;
	int Bpg;
	int Bpb;

	std::string PaddleTexture;

	std::string LaserTexture;

	std::string MuzzleTexture;

	std::string BoundaryTexture;

	std::string BoundaryShadowTexture;

	std::string BrickShadowTexture;

	std::string TransparentSheetTexture;

	std::string GameBgTexture;

	std::string PowerUpTexture;

	std::string HealthTexture;

	std::string GameParticleTexture;

	// Game particle color (r,g,b)
	int Gpr;
	int Gpg;
	int Gpb;

	std::string LevelFile;

	std::string GameFontFile;

	std::string GameFontWidthFile;

	// Game Font color (r,g,b)
	int Gfr;
	int Gfg;
	int Gfb;

	// Sound files
	std::string MenuMusic;

	std::string GameMusic;

	std::string ClickSound;

	std::string NavSound;

	std::string BallSound;

	std::string LaserSound;

	std::string LevelCompleteSound;

	std::string LifeLostSound;

	std::string PowerUpSound;

	std::string BrickFallSound;

	std::string SaveGameFile{"GameSave.bin"};



	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(ButtonTexture),

			CEREAL_NVP(SelectorTexture),

			CEREAL_NVP(TitleTexture),

			CEREAL_NVP(MenuBgTexture),

			CEREAL_NVP(MenuParticleTexture),

			// Menu particle color (r,g,b)
			CEREAL_NVP(Mpr),
			CEREAL_NVP(Mpg),
			CEREAL_NVP(Mpb),

			CEREAL_NVP(MenuFontFile),

			CEREAL_NVP(MenuFontWidthFile),

			// Menu font color (r,g,b)
			CEREAL_NVP(Mfr),
			CEREAL_NVP(Mfg),
			CEREAL_NVP(Mfb),

			CEREAL_NVP(MenuMovingFontFile),

			CEREAL_NVP(MenuMovingFontWidthFile),

			// Menu Moving Font color (r,g,b)
			CEREAL_NVP(Mmfr),
			CEREAL_NVP(Mmfg),
			CEREAL_NVP(Mmfb),

			CEREAL_NVP(BallTexture),

			CEREAL_NVP(BallParticleTexture),

			// Ball particle color (r,g,b)
			CEREAL_NVP(Bpr),
			CEREAL_NVP(Bpg),
			CEREAL_NVP(Bpb),

			CEREAL_NVP(PaddleTexture),

			CEREAL_NVP(LaserTexture),

			CEREAL_NVP(MuzzleTexture),

			CEREAL_NVP(BoundaryTexture),

			CEREAL_NVP(BoundaryShadowTexture),

			CEREAL_NVP(BrickShadowTexture),

			CEREAL_NVP(TransparentSheetTexture),

			CEREAL_NVP(GameBgTexture),

			CEREAL_NVP(PowerUpTexture),

			CEREAL_NVP(HealthTexture),

			CEREAL_NVP(GameParticleTexture),

			// Game particle color (r,g,b)
			CEREAL_NVP(Gpr),
			CEREAL_NVP(Gpg),
			CEREAL_NVP(Gpb),

			CEREAL_NVP(LevelFile),

			CEREAL_NVP(GameFontFile),

			CEREAL_NVP(GameFontWidthFile),

			// Game Font color (r,g,b)
			CEREAL_NVP(Gfr),
			CEREAL_NVP(Gfg),
			CEREAL_NVP(Gfb),

			// Sound files
			CEREAL_NVP(MenuMusic),

			CEREAL_NVP(GameMusic),

			CEREAL_NVP(ClickSound),

			CEREAL_NVP(NavSound),

			CEREAL_NVP(BallSound),

			CEREAL_NVP(LaserSound),

			CEREAL_NVP(LevelCompleteSound),

			CEREAL_NVP(LifeLostSound),

			CEREAL_NVP(PowerUpSound),

			CEREAL_NVP(BrickFallSound),

			CEREAL_NVP(SaveGameFile));
	}
};


class GameManager
{
private:

	BINDU::Bnd_Size		m_worldSize{ TILE_SIZE.x * 25,TILE_SIZE.y * 45 };

	std::unique_ptr<MenuScene>		m_menuScene;

	std::unique_ptr<MainScene>		m_mainScene;

	BINDU::SceneManager		m_sceneManager;

	static bool			m_musicOn;
	static bool			m_sfxOn;

	static GameState m_state;

	static BINDU::SoundSystem* m_soundSystem;

public:
	GameManager() = default;
	~GameManager();

	void Preload(const std::string& resourceFile = HM::RelativeResourcePath("ResourceFile.xml")) const;

	void Init();

	void onLoadResource();

	void Update(float dt);

	void Draw(BINDU::Graphics* graphics);

	static void SetState(GameState state) { m_state = state; }

	static BINDU::SoundSystem* getSoundSystem() { return m_soundSystem; }

	static void setMusic(bool value) { m_musicOn = value; }
	static bool getMusic() { return m_musicOn; }

	static void setSfx(bool value) { m_sfxOn = value; }
	static bool getSfx() { return m_sfxOn; }

	static void SaveGame();

	static ResourceFile	RC;
	static GameSave GS;
};

