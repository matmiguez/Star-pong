#include "GSGameMenu.h"
#include "StructsDef.h"

void GSGameMenuUpdate(float delta, ResourceManager& resource) {

	InputState inputState = *resource.inputState;
	GameStages gameStages = *resource.gameStages;

	// Si presione cualquier tecla (arriba, abajo, izquierda
	if (inputState.down || inputState.up || inputState.left || inputState.right || inputState.fire) {

	}

}

void GSGameMenu(float delta, ResourceManager& resource)
{
}
