#pragma once

using namespace std;
#include <fstream>
#include <vector>
#include <stack>
#include "SDL.h"
#include "SDL_TTF.h"
#include "SDL_mixer.h"

typedef struct _InputState {
    bool up = false;
    bool down = false;
    bool up1 = false;
    bool down1 = false;
    bool fire = false;
    bool stop = false;
    bool one = false;
    bool two = false;
}InputState;

typedef struct _Sprite {
    SDL_Texture* texture;
    SDL_Rect dest;
    bool isVisible = true;
}Sprite;

typedef struct _Text {
    TTF_Font* font;
    SDL_Color color;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect dest;
    bool isVisible = true;
}Text;

typedef struct _Bgm {
    Mix_Music* music;
}Bgm;

enum GAME_STAGES
{
    GS_INVALID = -1,
    GS_LOGO = 0,
    GS_MAIN_MENU,
    GS_GAMEPLAY,
    GS_GAMEPAUSE,
    GS_GAMEOVER,
    GS_CREDITS
};

typedef struct _GameStage {
    string stage_name;
    int game_stageID = GS_INVALID;
    int level;
}GameStage;


typedef vector<Sprite> SpriteAssets;
typedef vector<Sprite> RedSaberAssets;
typedef vector<Sprite> BlueSaberAssets;
typedef vector<Sprite> BackgroundAssets;
typedef vector<Sprite> BallshootAssets;
typedef vector<Sprite> StartAssets;
typedef vector<Sprite> GamePauseAssets;
typedef vector<Text> TextAssets;
typedef vector<Text> RedtxtAsset;
typedef vector<Text> BluetxtAsset;
typedef vector<Bgm> BgmAssets;
typedef stack<GameStage> GameStages;

typedef struct _ResourceManager {
    SpriteAssets* spritesAssets;
    BlueSaberAssets* blueSaberAssets;
    RedSaberAssets* redSaberAssets;
    BackgroundAssets* backgroundAssets;
    BallshootAssets* ballshootAssets;
    StartAssets* startAssets;
    GamePauseAssets* gamePauseAssets;
    TextAssets* textAssets;
    RedtxtAsset* redtxtAsset;
    BluetxtAsset* bluetxtAsset;
    BgmAssets* musicAssets;
    GameStages* gameStages;
    InputState* inputState;
    SDL_Renderer* renderer;
}ResourceManager;