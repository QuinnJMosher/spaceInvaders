#include "PlayerCannon.h"
#include "AIE.h"

void PlayerCannon::SetSize(float inWidth, float inHeight) {
	fWidth = inWidth;
	fHeight = inHeight;
}

void PlayerCannon::SetPosition(float inX, float inY) {
	x = inX;
	y = inY;
}

void PlayerCannon::SetMovementKeys(unsigned int inMoveLeft, unsigned int inMoveRight) {
	iMoveLeftKey = inMoveLeft;
	iMoveRightKey = inMoveRight;
}

void PlayerCannon::SetMovementExtremes(unsigned int inLeftExtreme, unsigned int inRightExtreme) {
	iLeftMovementExtreme = inLeftExtreme;
	iRightMovementExtreme = inRightExtreme;
}

void PlayerCannon::Move(float fTimeStep, float fSpeed)
{
	if (IsKeyDown(iMoveLeftKey))//check if key is down
	{
		x -= (fTimeStep * fSpeed);//move based on the speed of the computer
		if (x < (iLeftMovementExtreme + fWidth * .5f))//check for edge of screen
		{
			x = (iLeftMovementExtreme + fWidth * .5f);//if edge reached then prevent from going off screen
		}
	}

	if (IsKeyDown(iMoveRightKey))
	{
		x += (fTimeStep * fSpeed);
		if (x >(iRightMovementExtreme - fWidth * .5f))
		{
			x = (iRightMovementExtreme - fWidth * .5f);
		}
	}

	MoveSprite(iSpriteID, x, y);//finalize movement
}