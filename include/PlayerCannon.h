#ifndef _PlayerCannon_H_
#define _PlayerCannon_H_

class PlayerCannon{
public:

	unsigned int iSpriteID;

	float fWidth;
	float fHeight;
	float x;
	float y;
	unsigned int iMoveLeftKey;
	unsigned int iMoveRightKey;
	unsigned int iLeftMovementExtreme;
	unsigned int iRightMovementExtreme;

	void SetSize(float inWidth, float inHeight);
	void SetPosition(float inX, float inY);
	void SetMovementKeys(unsigned int inMoveLeft, unsigned int inMoveRight);
	void SetMovementExtremes(unsigned int inLeftExtreme, unsigned int inRightExtreme);

	void Move(float fTimeStep, float fSpeed);

};

#endif