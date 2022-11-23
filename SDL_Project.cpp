// SDL_Project.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_TTF.h"
#include "SDL_mixer.h"
#include "InputModule.h"
#include "GSLogoState.h"
#include <string>


using namespace std;

///////// Definicion de estructuras /////////////
#include "StructsDef.h"
///////// Definicion de estructuras /////////////

enum SUBSTATE {
    INIT_STATE = 0,
    GAMEPLAY_STATE,
    PAUSE_STATE,
    GAMEOVER_STATE,
};

///////// Variables y Constantes Globales /////////////
const int WIDTH = 640;
const int HEIGHT = 480; 
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_bool isGameRunning = SDL_TRUE;
float time_multiplier = 1.0f;
InputState gameInputState;
SpriteAssets spritesAssets;
TextAssets textAssets;
BluetxtAsset bluetxtAsset;
RedtxtAsset redtxtAsset;
BgmAssets musicAssets;
GameStages gameStages;

int subState = INIT_STATE;

string process = "start";

SpriteAssets blueSaberAssets;
SpriteAssets redSaberAssets;
SpriteAssets backgroundAssets;
SpriteAssets ballshootAssets;
SpriteAssets startAssets;
SpriteAssets gamePauseAssets;

SDL_Texture* saberTexture;

int ballSpeed = 1;
int _pelotaX = 320;
int _pelotaY = 240;

int spritebackgroundID = 0;
int gamePauseID = 0;
int ballshootID = 0;
int spriteBlueSaberID = 0;
int spriteRedSaberID = 0;
int startgroundID = 0;

float gameTimer = 0;


float VELOCIDAD_PELOTAX = 1;
float VELOCIDAD_PELOTAY = 1;

ResourceManager resourceManager;

///////// Variables y Constantes Globales /////////////
string _temporizador = "0";
string scorered = "0";
string scoreblue = "0";


///////// Funciones de inicializacion y destruccion /////////////
void initEngine()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

    TTF_Init();

    int initted = Mix_Init(MIX_INIT_MP3);
    if ( (initted & MIX_INIT_MP3) != MIX_INIT_MP3) {
        cout << "Mix_Init: Failed to init required ogg and mod support!" << endl;
        cout << "Mix_Init: " << Mix_GetError() << endl;
        // handle error
    }

    window = SDL_CreateWindow("SDL2 Template Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Initializing Resource Manager
    resourceManager.spritesAssets = &spritesAssets;
    resourceManager.backgroundAssets = &backgroundAssets;
    resourceManager.blueSaberAssets = &blueSaberAssets;
    resourceManager.redSaberAssets = &redSaberAssets;
    resourceManager.ballshootAssets = &ballshootAssets;
    resourceManager.startAssets = &startAssets;
    resourceManager.gamePauseAssets = &gamePauseAssets;
    resourceManager.textAssets = &textAssets;
    resourceManager.redtxtAsset = &redtxtAsset;
    resourceManager.bluetxtAsset = &bluetxtAsset;
    resourceManager.musicAssets = &musicAssets;
    resourceManager.gameStages = &gameStages;
    resourceManager.inputState = &gameInputState;
    resourceManager.renderer = renderer;

    // Starting Game stage
    GameStage logoGameStage;
    logoGameStage.game_stageID = GS_LOGO;
    logoGameStage.stage_name = "Logo";

    gameStages.push(logoGameStage);

    GameStage gameplayStage;
    gameplayStage.game_stageID = GS_GAMEPLAY;
    gameplayStage.stage_name = "Gameplay";

    gameStages.push(gameplayStage);
}

void destroyEngine() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

///////// Funciones de inicializacion y destruccion /////////////


///////// Funciones de carga y liberacion de recursos /////////////
void loadResource(ResourceManager& resource) {
	SDL_Renderer* renderer = resource.renderer;
	BackgroundAssets& backgroundAssets = *resource.backgroundAssets;
	RedSaberAssets& redSaberAssets = *(resource.redSaberAssets);
	BlueSaberAssets& blueSaberAssets = *(resource.blueSaberAssets);
	BallshootAssets& ballshootAssets = *(resource.ballshootAssets);
	TextAssets& textAssets = *(resource.textAssets);
    BluetxtAsset& bluetxtAsset= *(resource.bluetxtAsset);
    RedtxtAsset& redtxtAsset= *(resource.redtxtAsset);
    StartAssets& startAssets = *(resource.startAssets);
    GamePauseAssets& gamePauseAssets = *(resource.gamePauseAssets);



	string backgroundPath = "assets/img/bg.png";
	SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, backgroundPath.c_str());
	SDL_Rect backDestr;
	backDestr.x = 0;
	backDestr.y = 0;
	backDestr.w = WIDTH;
	backDestr.h = HEIGHT;

	Sprite backgroundSprite;
	backgroundSprite.dest = backDestr;
	backgroundSprite.texture = backgroundTexture;
	backgroundAssets.push_back(backgroundSprite);

	spritebackgroundID = backgroundAssets.size() - 1;

    string gamePausePath = "assets/img/pause.png";
    SDL_Texture* gamePauseTexture = IMG_LoadTexture(renderer, gamePausePath.c_str());
    SDL_Rect gamePauseDestr;
    gamePauseDestr.x = 0;
    gamePauseDestr.y = 0;
    gamePauseDestr.w = WIDTH;
    gamePauseDestr.h = HEIGHT;

    Sprite gamePauseSprite;
    gamePauseSprite.dest = gamePauseDestr;
    gamePauseSprite.texture = gamePauseTexture;
    gamePauseAssets.push_back(gamePauseSprite);

    gamePauseID = gamePauseAssets.size() - 1;

    string startPath = "assets/img/start.png";
    SDL_Texture* startTexture = IMG_LoadTexture(renderer, startPath.c_str());
    SDL_Rect startDestr;
    startDestr.x = 0;
    startDestr.y = 0;
    startDestr.w = WIDTH;
    startDestr.h = HEIGHT;

    Sprite startSprite;
    startSprite.dest = startDestr;
    startSprite.texture = startTexture;
    startAssets.push_back(startSprite);

    startgroundID = startAssets.size() - 1;

	string redSaberPath = "assets/img/redsaber.png";
	SDL_Texture* redSaberTexture = IMG_LoadTexture(renderer, redSaberPath.c_str());
	SDL_Rect saberDestr;
	saberDestr.x = 600;
	saberDestr.y = 240;
	saberDestr.w = 12;
	saberDestr.h = 70;

	Sprite redSaberSprite;
	redSaberSprite.dest = saberDestr;
	redSaberSprite.texture = redSaberTexture;
	redSaberAssets.push_back(redSaberSprite);

	spriteRedSaberID = redSaberAssets.size() - 1;


	string blueSaberPath = "assets/img/bluesaber.png";
	SDL_Texture* bluesaberTexture = IMG_LoadTexture(renderer, blueSaberPath.c_str());
	SDL_Rect saberDestb;
	saberDestb.x = 15;
	saberDestb.y = 240;
	saberDestb.w = 12;
	saberDestb.h = 70;

	Sprite blueSaberSprite;
	blueSaberSprite.dest = saberDestb;
	blueSaberSprite.texture = bluesaberTexture;
	blueSaberAssets.push_back(blueSaberSprite);

	spriteBlueSaberID = blueSaberAssets.size() - 1;

    string ballshootPath = "assets/img/deathstar1.png";
    SDL_Texture* ballshootTexture = IMG_LoadTexture(renderer, ballshootPath.c_str());
    SDL_Rect ballshootDestr;
    ballshootDestr.x = 320;
    ballshootDestr.y = 240;
    ballshootDestr.w = 30;
    ballshootDestr.h = 30;

    Sprite ballshootSprite;
    ballshootSprite.dest = ballshootDestr;
    ballshootSprite.texture = ballshootTexture;
    ballshootAssets.push_back(ballshootSprite);

    ballshootID = ballshootAssets.size() - 1;

    // Cargo el texto...
    string fontfilePath = "assets/fonts/starjedi.ttf";

    TTF_Font* starjedi = TTF_OpenFont(fontfilePath.c_str(), 24); //this opens a font style and sets a size

    SDL_Color Yellow = { 249, 215, 28 };  // this is the color in rgb format, maxing out all would give you the color Yellow, and it will be your text's color

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(starjedi, _temporizador.c_str(), Yellow); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

    SDL_Rect Message_rect;
    Message_rect.w = 20;
    Message_rect.h = 30;
    Message_rect.x = 320;
    Message_rect.y = 20;

    Text mainText;
    mainText.font = starjedi;
    mainText.color = Yellow;
    mainText.surface = surfaceMessage;
    mainText.texture = Message;
    mainText.dest = Message_rect;

    textAssets.push_back(mainText);

    string pathscoreb = "assets/fonts/starjedi.ttf";

    TTF_Font* blueS = TTF_OpenFont(fontfilePath.c_str(), 24); 

    SDL_Surface* scoreblueMessage = TTF_RenderText_Solid(starjedi, scoreblue.c_str(), Yellow); 

    SDL_Texture* scoreBMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage); 

    SDL_Rect scoreBlue;
    scoreBlue.w = 20;
    scoreBlue.h = 30;
    scoreBlue.x = 160;
    scoreBlue.y = 40;

    Text scoreBluemain;
    scoreBluemain.font = starjedi;
    scoreBluemain.color = Yellow;
    scoreBluemain.surface = surfaceMessage;
    scoreBluemain.texture = Message;
    scoreBluemain.dest = scoreBlue;

    bluetxtAsset.push_back(scoreBluemain);

    string pathscorer = "assets/fonts/starjedi.ttf";

    TTF_Font* redS = TTF_OpenFont(fontfilePath.c_str(), 24); 

    SDL_Surface* scoreredMessage = TTF_RenderText_Solid(starjedi, scorered.c_str(), Yellow); 

    SDL_Texture* scoreRMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect scoreRed; 
    scoreRed.w = 20;
    scoreRed.h = 30;
    scoreRed.x = 480;
    scoreRed.y = 40;

    Text scoreRedmain;
    scoreRedmain.font = starjedi;
    scoreRedmain.color = Yellow;
    scoreRedmain.surface = surfaceMessage;
    scoreRedmain.texture = Message;
    scoreRedmain.dest = scoreRed;

    redtxtAsset.push_back(scoreRedmain);

    // Cargo Sonidos y BGM
    string soundFilePath = "assets/bgm/introstarwars1hour.mp3";
    Mix_Music* music;
    music = Mix_LoadMUS(soundFilePath.c_str());

    Bgm bgm01;
    bgm01.music = music;

    musicAssets.push_back(bgm01);


}
	
void unloadResource(ResourceManager& resource) {

	RedSaberAssets& redSaberAssets = *(resource.redSaberAssets);
	BlueSaberAssets& blueSaberAssets = *(resource.blueSaberAssets);
    StartAssets& startAssets = *(resource.startAssets);
    GamePauseAssets& gamePauseAssets = *(resource.gamePauseAssets);

	//Libero la textura
	// Remuevo el asset del vector y ya no sera pintado nunca mas.
	SDL_DestroyTexture(redSaberAssets[spriteRedSaberID].texture);
	redSaberAssets.erase(redSaberAssets.begin() + spriteRedSaberID);

	SDL_DestroyTexture(blueSaberAssets[spriteBlueSaberID].texture);
	blueSaberAssets.erase(blueSaberAssets.begin() + spriteBlueSaberID);

    SDL_DestroyTexture(startAssets[startgroundID].texture);
    startAssets.erase(startAssets.begin() + startgroundID);

    SDL_DestroyTexture(gamePauseAssets[gamePauseID].texture);
    gamePauseAssets.erase(gamePauseAssets.begin() + gamePauseID);

	/*
	for (int i = 0; i < textAssets.size(); i++) {
		SDL_FreeSurface(textAssets[i].surface);
		SDL_DestroyTexture(textAssets[i].texture);
	}

	textAssets.clear();*/
}


void unloadAssets() {
    for (int i = 0; i < spritesAssets.size(); i++) {
        SDL_DestroyTexture(spritesAssets[i].texture);
    }
    for (int i = 0; i < blueSaberAssets.size(); i++) {
        SDL_DestroyTexture(blueSaberAssets[i].texture);
    }

    for (int i = 0; i < textAssets.size(); i++) {
        SDL_FreeSurface(textAssets[i].surface);
        SDL_DestroyTexture(textAssets[i].texture);
    }
    for (int i = 0; i < bluetxtAsset.size(); i++) {
        SDL_FreeSurface(bluetxtAsset[i].surface);
        SDL_DestroyTexture(bluetxtAsset[i].texture);
    }
    for (int i = 0; i < redtxtAsset.size(); i++) {
        SDL_FreeSurface(redtxtAsset[i].surface);
        SDL_DestroyTexture(redtxtAsset[i].texture);
    }
    for (int i = 0; i < musicAssets.size(); i++) {
        Mix_FreeMusic(musicAssets[i].music);
    }
}

///////// Funciones de carga y liberacion de recursos /////////////


///////// Funciones de actualizacion y pintado /////////////

void inputUpdate() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        
        switch (event.type) {
        case SDL_KEYDOWN:
            onKeyDown(event.key.keysym.sym, gameInputState);
            break;
        case SDL_KEYUP:
            onKeyUp(event.key.keysym.sym, gameInputState);
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_MOUSEBUTTONUP:
            break;
        case SDL_QUIT:
            isGameRunning = SDL_FALSE;
            break;
        default:
            break;
        }
    }
}


void updateGame(float deltaTime, ResourceManager& resource) {
    InputState& inputState = *resource.inputState;
    GameStages& gameStages = *resource.gameStages;
    RedSaberAssets& redSaberAssets = *resource.redSaberAssets;
    BlueSaberAssets& blueSaberAssets = *resource.blueSaberAssets;
    BackgroundAssets& backgroundAssets = *resource.backgroundAssets;
    BallshootAssets& ballshootAssets = *(resource.ballshootAssets);
    StartAssets& startAssets = *(resource.startAssets);
    GamePauseAssets& gamePauseAssets = *(resource.gamePauseAssets);
    TextAssets& textAssets = *(resource.textAssets);

  
    int seconds = floor(deltaTime);
    _temporizador = to_string(seconds);

    int ibluesaber = (blueSaberAssets[spriteBlueSaberID].dest.y);
    int fbluesaber = (blueSaberAssets[spriteBlueSaberID].dest.y) + 70;

    int iredsaber = (redSaberAssets[spriteRedSaberID].dest.y);
    int fredsaber = ((redSaberAssets[spriteRedSaberID].dest.y) + 70);

    int scoreblue = 0;
    int scorered = 0;

    switch (subState) {
    case INIT_STATE:
        startAssets[0].isVisible = !startAssets[0].isVisible;
        if (inputState.one) {
            subState = GAMEPLAY_STATE;
        }
        break;
    case GAMEPLAY_STATE:
        // Pelota
        /*
        if (_pelotaX > 1205) {
            VELOCIDAD_PELOTAX = -9;
        }
        if (_pelotaX < 5) {
            VELOCIDAD_PELOTAX = 9;
        }

        if (_pelotaY > 640) {
            VELOCIDAD_PELOTAY = -9;
        }
        if (_pelotaY < 5) {
            VELOCIDAD_PELOTAY = 9;
        }
        */
        // Mover pelota
        
        
        /*
        if ((ballshootAssets[ballshootID].dest.x) > WIDTH) {
            VELOCIDAD_PELOTAX = -1;
        }
        if ((ballshootAssets[ballshootID].dest.x) < 0) {
            VELOCIDAD_PELOTAX = 1;
        }
        if ((ballshootAssets[ballshootID].dest.y) > HEIGHT) {
            VELOCIDAD_PELOTAX = -1;
        }
        if ((ballshootAssets[ballshootID].dest.y) < 5) {
            VELOCIDAD_PELOTAX = 1;
        }
        */

        ballshootAssets[ballshootID].dest.x += VELOCIDAD_PELOTAX;
        //ballshootAssets[ballshootID].dest.y += VELOCIDAD_PELOTAY;

        if ((ballshootAssets[ballshootID].dest.x) < (blueSaberAssets[spriteBlueSaberID].dest.x) && (ballshootAssets[ballshootID].dest.y) > ibluesaber && (ballshootAssets[ballshootID].dest.y) < fbluesaber) {
            VELOCIDAD_PELOTAX = 1;
        }
        if ((ballshootAssets[ballshootID].dest.x) > (redSaberAssets[spriteRedSaberID].dest.x) && (ballshootAssets[ballshootID].dest.y) > iredsaber && (ballshootAssets[ballshootID].dest.y) < fredsaber) {
            VELOCIDAD_PELOTAX = -1;
        }

        if ((ballshootAssets[ballshootID].dest.x) == 640 || (ballshootAssets[ballshootID].dest.x) == 0) {
            ballshootAssets[ballshootID].dest.x = 320;
            ballshootAssets[ballshootID].dest.y = 240;
        }

       

        if (inputState.down && ((blueSaberAssets[spriteBlueSaberID].dest.y) + (blueSaberAssets[spriteBlueSaberID].dest.h)) < HEIGHT) {
            blueSaberAssets[spriteBlueSaberID].dest.y += 0.5 * deltaTime;
        }
        if (inputState.up && (blueSaberAssets[spriteBlueSaberID].dest.y) > -1) {
            blueSaberAssets[spriteBlueSaberID].dest.y -= 0.5 * deltaTime;
        }
        
 
        if (inputState.down1 && ((redSaberAssets[spriteRedSaberID].dest.y) + (redSaberAssets[spriteRedSaberID].dest.h)) < HEIGHT) {
            redSaberAssets[spriteRedSaberID].dest.y += 0.5 * deltaTime;
        }
        if (inputState.up1 && (redSaberAssets[spriteRedSaberID].dest.y) > -1) {
            redSaberAssets[spriteRedSaberID].dest.y -= 0.5 * deltaTime;
        }


        if ((ballshootAssets[ballshootID].dest.x) > (redSaberAssets[spriteRedSaberID].dest.x)) {
            scoreblue += 1;
        }
        if ((ballshootAssets[ballshootID].dest.x) < (blueSaberAssets[spriteBlueSaberID].dest.x)) {
            scorered += 1;
        }

        if (inputState.stop) {
            subState = PAUSE_STATE;
        }
        break;
    case PAUSE_STATE:
        if (inputState.stop) {
            subState = GAMEPLAY_STATE;
        }
        break;
    case GAMEOVER_STATE:
        subState = GAMEOVER_STATE;
        unloadResource(resource);
        break;
    }
}

void render(float deltaTime)
{
    // Limpio la pantalla 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    switch (subState) {
    case INIT_STATE:
        for (int i = 0; i < backgroundAssets.size(); i++) {
            if (backgroundAssets[i].isVisible) {
                SDL_RenderCopy(renderer, backgroundAssets[i].texture, NULL, &backgroundAssets[i].dest);
            }
        }
        for (int i = 0; i < startAssets.size(); i++) {
            if (startAssets[i].isVisible) {
                SDL_RenderCopy(renderer, startAssets[i].texture, NULL, &startAssets[i].dest);
            }
        }
        break;
    case GAMEPLAY_STATE:
        for (int i = 0; i < backgroundAssets.size(); i++) {
            if (backgroundAssets[i].isVisible) {
                SDL_RenderCopy(renderer, backgroundAssets[i].texture, NULL, &backgroundAssets[i].dest);
            }
        }
        for (int i = 0; i < ballshootAssets.size(); i++) {
            if (ballshootAssets[i].isVisible) {
                SDL_RenderCopy(renderer, ballshootAssets[i].texture, NULL, &ballshootAssets[i].dest);
            }
        }
        for (int i = 0; i < blueSaberAssets.size(); i++) {
            if (blueSaberAssets[i].isVisible) {
                SDL_RenderCopy(renderer, blueSaberAssets[i].texture, NULL, &blueSaberAssets[i].dest);
            }
        }
        for (int i = 0; i < redSaberAssets.size(); i++) {
            if (redSaberAssets[i].isVisible) {
                SDL_RenderCopy(renderer, redSaberAssets[i].texture, NULL, &redSaberAssets[i].dest);
            }
        }
        for (int i = 0; i < textAssets.size(); i++) {
            if (textAssets[i].isVisible) {
                SDL_RenderCopy(renderer, textAssets[i].texture, NULL, &textAssets[i].dest);
            }
        }
        for (int i = 0; i < bluetxtAsset.size(); i++) {
            if (bluetxtAsset[i].isVisible) {
                SDL_RenderCopy(renderer, bluetxtAsset[i].texture, NULL, &bluetxtAsset[i].dest);
            }
        }
        for (int i = 0; i < redtxtAsset.size(); i++) {
            if (redtxtAsset[i].isVisible) {
                SDL_RenderCopy(renderer, redtxtAsset[i].texture, NULL, &redtxtAsset[i].dest);
            }
        }
        break;
    case PAUSE_STATE:
        for (int i = 0; i < backgroundAssets.size(); i++) {
            if (backgroundAssets[i].isVisible) {
                SDL_RenderCopy(renderer, backgroundAssets[i].texture, NULL, &backgroundAssets[i].dest);
            }
        }
        for (int i = 0; i < ballshootAssets.size(); i++) {
            if (ballshootAssets[i].isVisible) {
                SDL_RenderCopy(renderer, ballshootAssets[i].texture, NULL, &ballshootAssets[i].dest);
            }
        }
        for (int i = 0; i < blueSaberAssets.size(); i++) {
            if (blueSaberAssets[i].isVisible) {
                SDL_RenderCopy(renderer, blueSaberAssets[i].texture, NULL, &blueSaberAssets[i].dest);
            }
        }
        for (int i = 0; i < redSaberAssets.size(); i++) {
            if (redSaberAssets[i].isVisible) {
                SDL_RenderCopy(renderer, redSaberAssets[i].texture, NULL, &redSaberAssets[i].dest);
            }
        }
        for (int i = 0; i < textAssets.size(); i++) {
            if (textAssets[i].isVisible) {
                SDL_RenderCopy(renderer, textAssets[i].texture, NULL, &textAssets[i].dest);
            }
        }
        for (int i = 0; i < gamePauseAssets.size(); i++) {
            if (gamePauseAssets[i].isVisible) {
                SDL_RenderCopy(renderer, gamePauseAssets[i].texture, NULL, &gamePauseAssets[i].dest);
            }
        }
        for (int i = 0; i < bluetxtAsset.size(); i++) {
            if (bluetxtAsset[i].isVisible) {
                SDL_RenderCopy(renderer, bluetxtAsset[i].texture, NULL, &bluetxtAsset[i].dest);
            }
        }
        for (int i = 0; i < redtxtAsset.size(); i++) {
            if (redtxtAsset[i].isVisible) {
                SDL_RenderCopy(renderer, redtxtAsset[i].texture, NULL, &redtxtAsset[i].dest);
            }
        }

    }

   
    // Presento la imagen en pantalla
    SDL_RenderPresent(renderer);
}

///////// Funciones de actualizacion y pintado /////////////

///////// Funcione principal y GameLoop /////////////

int main(int argc, char* argv[])
{
    initEngine();

    // Cargo Assets
    loadResource(resourceManager);

    //Mix_PlayMusic(musicAssets[0].music, -1);

    Uint64 currentTime = SDL_GetTicks64();

    while (isGameRunning) {

        Uint64 previousTime = currentTime;

        currentTime = SDL_GetTicks64();

        Uint64 deltaTime = currentTime - previousTime;

        inputUpdate();

        updateGame(deltaTime * time_multiplier, resourceManager);

        render(deltaTime);
    }

    // Detendo la musica 
    Mix_HaltMusic();

    // Descargo Assets
    unloadAssets(); 
    destroyEngine();
    return 0;
}

///////// Funcione principal y GameLoop /////////////

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln


/*
    // Small state machine using stack collection
    switch (gameStages.top().game_stageID)
    {
    case GS_LOGO:
        GSLogoStateUpdate(deltaTime, resourceManager);
        break;
    case GS_MAIN_MENU:
        GSGameMenuUpdate(deltaTime, resourceManager);
        break;
    case GS_GAMEPLAY:
        GSGameplayState(deltaTime, resourceManager);
        break;
    case GS_INVALID:
    default:
        break;
    }
    if (gameInputState.down) {
        blueSaberAssets[spriteBlueSaberID].dest.y += 0.5 * deltaTime;
    }
    if (gameInputState.up) {
        blueSaberAssets[spriteBlueSaberID].dest.y -= 0.5 * deltaTime;
    }
    if (gameInputState.left) {
        redSaberAssets[spriteRedSaberID].dest.y += 0.5 * deltaTime;
    }
    if (gameInputState.right) {
        redSaberAssets[spriteRedSaberID].dest.y -= 0.5 * deltaTime;
    }
    */