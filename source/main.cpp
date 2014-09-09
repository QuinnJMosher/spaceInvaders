#include "AIE.h"
#include <iostream>

//constants
const int iScreenWidth = 672;
const int iScreenHeight = 780;

const float SPEED = 150.0f;
const float ENEMY_SPEED = 0.25f;
const float ENEMY_SPEED_DOWN = 2.5f;

const int enemyArr1Length = 4;
const int enemyArr2Length = 6;

const char* pInvadersFont = "./fonts/invaders.fnt";

//score arrays
char pkScore1[10] = "00000";
char pkHighScore[10] = "00000";
char pkSScore2[10] = "00000";
char pkCredit[10] = "99";

//prototypes 
void UpdateMainMenu();
void UpdateGameState(float deltaTime);
void CreateEnemies();
void MoveEnemies(int deltaTime);
void ResetEnemies();

//states 
enum GAMESTATES
{
	eMAIN_MENU,
	eGAMEPLAY,
	eEND
};

//sprite vars
unsigned int iArcadeMarquee;

//playerStrct
struct PlayerCannon {
	unsigned int iSpriteID;

	float fWidth;
	float fHeight;
	void SetSize(float inWidth, float inHeight) {
		fWidth = inWidth;
		fHeight = inHeight;
	}

	float x;
	float y;
	void SetPosition(float inX, float inY) {
		x = inX;
		y = inY;
	}

	unsigned int iMoveLeftKey;
	unsigned int iMoveRightKey;
	void SetMovementKeys(unsigned int inMoveLeft, unsigned int inMoveRight) {
		iMoveLeftKey = inMoveLeft;
		iMoveRightKey = inMoveRight;
	}

	unsigned int iLeftMovementExtreme;
	unsigned int iRightMovementExtreme;
	void SetMovementExtremes(unsigned int inLeftExtreme, unsigned int inRightExtreme) {
		iLeftMovementExtreme = inLeftExtreme;
		iRightMovementExtreme = inRightExtreme;
	}

	void Move(float fTimeStep, float fSpeed)
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
};

enum DIRECTION {//values for the direction that the enimies are moveing
	eLEFT,
	eRIGHT,
	eDOWN
};

struct Enemy {
	unsigned int iSpriteID;

	float x;
	float y;

	bool Move(float fDeltaTime, int direction) {

		if (direction == eLEFT) {//check for direction moved
			x -= (ENEMY_SPEED);
			if (x < 0) {//check if screen edge is reached
				return true;//if we hit a wall then we need to complain about it;
			}
		} else if (direction == eRIGHT) {
			x += (ENEMY_SPEED);
			if (x > iScreenWidth) {
				return true;
			}
		}
		else if (direction == eDOWN) {
			y -= ENEMY_SPEED_DOWN;
			return true;//we dont need to track if we hit a wall going down because we will only ever move down once
		}

		return false;//if we dont hit any walls or move downward the we are fine
	}

};


DIRECTION enemyDirection;
DIRECTION nextDirection;
Enemy alienShips[enemyArr1Length][enemyArr2Length];

PlayerCannon player;


int main( int argc, char* argv[] )
{	

	Initialise(iScreenWidth, iScreenHeight, false, "Space Invaders");
    
	SetBackgroundColour(SColour(0x00, 0x00, 0x00, 0xFF));

	//player settings
	player.SetSize(64.0f, 32.0f);
	player.iSpriteID = CreateSprite("./images/cannon.png", player.fWidth, player.fHeight, true);
	player.SetMovementExtremes(0.0f, iScreenWidth);
	player.SetMovementKeys(65, 68);
	player.x = iScreenWidth * 0.5f;
	player.y = 88.0f;

	//create Marquee sprite
	iArcadeMarquee = CreateSprite("./images/Space-Invaders-Marquee.png", iScreenWidth, iScreenHeight, true);

	//enemy creation
	CreateEnemies();
	enemyDirection = eRIGHT;
	nextDirection = eRIGHT;

	//font setting
	AddFont(pInvadersFont);

	//game state declaration
	GAMESTATES eCurrentState = eMAIN_MENU;

    //Game Loop
    do
    {
		float fDeltaT = GetDeltaTime();

		switch (eCurrentState) {
		case eMAIN_MENU:

			UpdateMainMenu();

			//input
			if (IsKeyDown(257) && !IsKeyDown(256)) {
				eCurrentState = eGAMEPLAY;
				ResetEnemies();
			}

			break;

		case eGAMEPLAY:

			UpdateGameState(fDeltaT);

			//ChangeState
			if (IsKeyDown(256)) {
				eCurrentState = eMAIN_MENU;
			}

			break;

		default:
			break;
		}

		//clear screen
		ClearScreen();


    } while(!FrameworkUpdate());

	Shutdown();

    return 0;
}

void UpdateMainMenu() {
	//purty tings
	MoveSprite(iArcadeMarquee, iScreenWidth / 2, iScreenHeight / 2);
	DrawSprite(iArcadeMarquee);

	//text
	SetFont(pInvadersFont);
	DrawString("INSERT COINS", iScreenWidth * 0.37f, iScreenHeight * 0.5f);
	DrawString(pkCredit, iScreenWidth* (0.37f + 0.1f), iScreenHeight * 0.45f);

}

void UpdateGameState(float deltaTime) {

	player.Move(deltaTime, SPEED);
	MoveEnemies(deltaTime);

	//draw sprites
	DrawSprite(player.iSpriteID);

	for (int i = 0; i < enemyArr1Length; i++) {
		for (int j = 0; j < enemyArr2Length; j++) {
			DrawSprite(alienShips[i][j].iSpriteID);
		}
	}

	DrawLine(0, 40, iScreenWidth, 40, SColour(0x00, 0xFC, 0x00, 0xFF)); //doesn't acctually draw anything?

	//drawstrings
	SetFont(pInvadersFont);
	DrawString("SCORE < 1 >", iScreenWidth * 0.025f, iScreenHeight - 2);
	DrawString("0000", iScreenWidth * 0.11f, iScreenHeight - 32);
	DrawString("HI-SCORE", iScreenWidth * 0.35f, iScreenHeight - 2);
	DrawString("0020", iScreenWidth * (0.05f + 0.35f), iScreenHeight - 32);
	DrawString("SCORE < 2 >", iScreenWidth * 0.65f, iScreenHeight - 2);
}

void CreateEnemies() {
	float fEnemyX = iScreenWidth * 0.2f;// prepare start variables	
	float fEnemyY = iScreenHeight * 0.7f;

	for (int i = 0; i < enemyArr1Length; i++) {// double loops to get to the multiple arrays
		for (int j = 0; j < enemyArr2Length; j++) {
			alienShips[i][j].iSpriteID = CreateSprite("./images/invaders/invaders_1_00.png", 64, 62, true);//create
			alienShips[i][j].x = fEnemyX;// set the x and y values in their proper place based on the screen and sprite sizes
			alienShips[i][j].y = fEnemyY;
			MoveSprite(alienShips[i][j].iSpriteID, alienShips[i][j].x, alienShips[i][j].y);//put in positions
			fEnemyX += 0.12f * iScreenWidth; //change position for next iteration
		}
		fEnemyY -= 0.1f * iScreenHeight;//change height
		fEnemyX = iScreenWidth * 0.2f;//reset x value
	}

	enemyDirection = eRIGHT;
}

void ResetEnemies() {
	enemyDirection = eRIGHT;
	float fEnemyX = iScreenWidth * 0.2f;
	float fEnemyY = iScreenHeight * 0.7f;

	for (int i = 0; i < enemyArr1Length; i++) {
		for (int j = 0; j < enemyArr2Length; j++) {
			alienShips[i][j].x = fEnemyX;
			alienShips[i][j].y = fEnemyY;
			MoveSprite(alienShips[i][j].iSpriteID, alienShips[i][j].x, alienShips[i][j].y);//put in positions
			fEnemyX += 0.12f * iScreenWidth; //change position for next iteration
		}
		fEnemyY -= 0.1f * iScreenHeight;//change height
		fEnemyX = iScreenWidth * 0.2f;//reset x value
	}
}

void MoveEnemies(int deltaTime) {

	bool changeDirection = false;

	for (int i = 0; i < enemyArr1Length; i++) {//loops to get to each array
		for (int j = 0; j < enemyArr2Length; j++) {

			if (alienShips[i][j].Move(deltaTime, enemyDirection)) {//if something hits a wall it will Return true
			changeDirection = true;// this will allow the direction to change after all enemies have moved
			}
			MoveSprite(alienShips[i][j].iSpriteID, alienShips[i][j].x, alienShips[i][j].y);//commit sptie movement (multiple returns in move meathod make it easier to have this live here instaead)

		}
	}

	if (changeDirection) {//if someone hits a wall then we need to change direction.
		if (enemyDirection != eDOWN) {//if we arent alredy moving down then we need to.

			if (enemyDirection == eRIGHT) {//puts what direction we will be moving next in memory bassed on what direction we are curently going
				nextDirection = eLEFT;
			} else {
				nextDirection = eRIGHT;
			}

			enemyDirection = eDOWN;//finally changes the direction to down

		} else {// if we are moving down then we need to change to the next direction that we alredy set 
			enemyDirection = nextDirection;//we dont need to change next direction again because the next direction will always be down
		}
	}

}