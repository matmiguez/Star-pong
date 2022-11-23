#include "GSLogoState.h"
#include "StructsDef.h"

void GSLogoStateUpdate(float delta, ResourceManager& resource) {

	InputState inputState = *resource.inputState;
	GameStages gameStages = *resource.gameStages;

	if (inputState.down || inputState.up || inputState.down1 || inputState.up1 ) {

	}

}
