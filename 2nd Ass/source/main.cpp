#include "AIE.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <Windows.h>

int screenWidth = 800;
int screenHeight = 800;
float GetDeltaTime();
char gameTimeString[10];
float gameTime = 0;
float menuTime = 0;
unsigned int leftCap;
unsigned int rightCap;
unsigned int topCap;
unsigned int bottomCap;
int player1HighScore = 0;
int player2HighScore = 0;
char player1HighScoreString[10];
char player2HighScoreString[10];
bool IsTimeToShow = true;
char menuText1[20] = "AIE 2nd Ass";
char menuText2[30] = "Press Enter To Start";
char healthText[10] = "Health";
char playerText[10] = "Player";
char timeText[10] = "Time";
char gameOverText[10] = "Game Over";
char playerScoreText[10] = "Score";

enum GAMESTATE
{
	MAINMENU,
	GAMEPLAY,
	GAMEOVER,
};

void MovementCap(float objWidth, float objHeight)
{
	topCap = screenHeight - objHeight*.5;
	bottomCap = 0 + objHeight*.5;
	leftCap = screenWidth*.125 + objWidth*.5;
	rightCap = screenWidth*.875 - objWidth*.5;
}

void MenuFlash()
{
	menuTime += GetDeltaTime();
	if (IsTimeToShow)
	{
		DrawString(menuText2, screenWidth*.4, screenHeight*.6);
	}
	if (menuTime <= 1.5)
	{
		IsTimeToShow = true;
	}
	if (menuTime >= 1.5)
	{
		IsTimeToShow = false;
	}
	if (menuTime >= 3)
	{
		menuTime = 0;
	}

}
class Bullet
{
public:
	float bulletUpperBoundary = 0;
	float bulletLowerBoundary = 0;
	float bulletLeftBoundary = 0;
	float bulletRightBoundary = 0;
	float bulletX = 0;
	float bulletY = 0;
	float bulletWidth = screenWidth*.01;
	float bulletHeight = screenHeight*.02;
	float bulletCount = 0;
	int bulletSprite;
	
	

};

struct Player
{
public:
char playerHealthString[10];
int playerHealth = 5;
unsigned int playerSprite = 0;
unsigned int upKey = 0;
unsigned int downKey = 0;
unsigned int leftKey = 0;
unsigned int rightKey = 0;
float playerX = 0;
float playerY = 0;
float playerWidth = screenWidth*.05;
float playerHeight = screenHeight*.05;
float playerUpperBoundary = 0;
float playerLowerBoundary = 0;
float playerLeftBoundary = 0;
float playerRightBoundary = 0;
float timeWhenDamaged = 0;
char timeWhenDamagedString[10];
float invulTime = 2; //how long before player can be damaged again
float timeWhenShot = 0;
float firingDelay = .25;
float firingKey = 0;
bool BulletActive = true;

	void Move()
	{
		MovementCap(playerWidth, playerHeight);
		if (IsKeyDown(upKey))
		{
			playerY += screenHeight*.2*GetDeltaTime();
			if (playerY > topCap)
			{
				playerY = topCap;
			}
		}
		if (IsKeyDown(downKey))
		{
			playerY -= screenHeight*.2*GetDeltaTime();
			if (playerY < bottomCap)
			{
				playerY = bottomCap;
			}
		}
		if (IsKeyDown(leftKey))
		{
			playerX -= screenWidth*.35*GetDeltaTime();
			if (playerX < leftCap)
			{
				playerX = leftCap;
			}
		}
		if (IsKeyDown(rightKey))
		{
			playerX += screenWidth*.35*GetDeltaTime();
			if (playerX > rightCap)
			{
				playerX = rightCap;
			}
		}
	}
	std::vector<Bullet*> BulletVector;
	void Shoot()
	{
		if (BulletActive)
		{
			if (IsKeyDown(firingKey))
			{
				if (timeWhenShot + firingDelay < gameTime)
				{

					for (int i = 0; i < 4; ++i)
					{
						Bullet* dagger = new Bullet;
						dagger[i].bulletSprite = ("./images/dagger.png", dagger[i].bulletWidth, dagger[i].bulletHeight, true);
						dagger[i].bulletX = playerX;
						dagger[i].bulletY = playerY + (playerHeight*.5);
						dagger[i].bulletY += (screenHeight*.2) * GetDeltaTime();
						dagger[i].bulletUpperBoundary = dagger[i].bulletY + dagger[i].bulletHeight*.5;
						dagger[i].bulletLowerBoundary = dagger[i].bulletY - dagger[i].bulletHeight*.5;
						dagger[i].bulletRightBoundary = dagger[i].bulletX + dagger[i].bulletWidth*.5;
						dagger[i].bulletLeftBoundary = dagger[i].bulletX - dagger[i].bulletWidth*.5;
						timeWhenShot = gameTime;
						BulletVector.push_back(dagger);
					}
				}
			}
		}
	}
	
};


struct Background
{
	float backgroundY;
	float backgroundY2;
	unsigned int stageSprite;
	unsigned int stageSprite2;
};

struct Enemy
{
	float enemyX = 0;
	float enemyY = 0;
	float enemyWidth = screenWidth*.1;
	float enemyHeight = screenHeight*.1;
	int enemySprite = 0;
	float enemyUpperBoundary = 0;
	float enemyLowerBoundary = 0;
	float enemyLeftBoundary = 0;
	float enemyRightBoundary = 0;
	float enemyHealth = 0;
	float timeToSpawn = 0;
	float enemyCount = 0;
	bool IsDead = false;
	bool IsDrawn = true;


	void Collided()
	{
		enemyHealth -= 1;
		if (enemyHealth == 0)
		{
			player1HighScore += 1;
			IsDead = true;
			if (IsDead)
			{
				IsDrawn = false;
				//remove boundaries
			}
		}
	}

	void Draw()
	{
		if (IsDrawn)
		{
			DrawSprite(enemySprite);
		}

	}


};

std::vector<Enemy> banditVector;

int main( int argc, char* argv[] )
{	
	std::fstream file;
	file.open("./highscore.txt", std::ios_base::out);
	file << player1HighScoreString << std::endl;
    Initialise(800, 800, false, "AIE 2nd Ass");
    SetBackgroundColour(SColour(0, 0, 0, 255));

	Background titleScreen;
	titleScreen.stageSprite = CreateSprite("./images/mainmenu.png", screenWidth*.75, screenHeight * 1, true);
	titleScreen.stageSprite2 = CreateSprite("./images/mainmenu.png", screenWidth*.75, screenHeight * 1, true);
	titleScreen.backgroundY = screenHeight*.5;
	titleScreen.backgroundY2 = screenHeight*1.5;

	Background stage1;
	stage1.stageSprite = CreateSprite("./images/stage1background.png", screenWidth*.75, screenHeight * 4, true);
	stage1.backgroundY = screenHeight * 2;

	Player lyn;
	lyn.playerSprite = CreateSprite("./images/lyn.png", lyn.playerWidth, lyn.playerHeight, true);
	lyn.playerX = screenWidth*.5;
	lyn.playerY = screenHeight*.05;
	lyn.firingKey = 'F';
	lyn.upKey = 'W';
	lyn.downKey = 'S';
	lyn.leftKey = 'A';
	lyn.rightKey = 'D';
	
	
    

	GAMESTATE currentState = MAINMENU;
    do
    {
        
		switch (currentState)
		{
		case MAINMENU:
			MoveSprite(titleScreen.stageSprite, screenWidth*.5, titleScreen.backgroundY*.975);
			DrawSprite(titleScreen.stageSprite);
			MoveSprite(titleScreen.stageSprite2, screenWidth*.5, titleScreen.backgroundY2*.975);
			DrawSprite(titleScreen.stageSprite2);
			MenuFlash();
			DrawString(menuText1, screenWidth*.4, screenHeight*.8);

			//movement and reset position for background
			titleScreen.backgroundY -= GetDeltaTime()*screenHeight*.2;
			titleScreen.backgroundY2 -= GetDeltaTime()*screenHeight*.2;
			if (titleScreen.backgroundY <= screenHeight*-.5)
			{
				titleScreen.backgroundY = screenHeight*1.5;
			}
			if (titleScreen.backgroundY2 <= screenHeight*-.5)
			{
				titleScreen.backgroundY2 = screenHeight*1.5;
			}

			if (IsKeyDown(GLFW_KEY_ENTER))
			{
				currentState = GAMEPLAY;
				ClearScreen();
			}
			break;

		case GAMEPLAY:
			gameTime += GetDeltaTime();
			MoveSprite(stage1.stageSprite, screenWidth*.5, stage1.backgroundY);
			DrawSprite(stage1.stageSprite);
			stage1.backgroundY -= screenHeight*.05*GetDeltaTime();
			if (gameTime > 60)
			{
				stage1.backgroundY = screenHeight*(-1);
				//DrawSprite(FirstBoss, screenWidth*.5, screenHeight*.9)
			}
			MoveSprite(lyn.playerSprite, lyn.playerX, lyn.playerY);
			DrawSprite(lyn.playerSprite);
			lyn.Move();
			lyn.Shoot();
			
			break;

		case GAMEOVER:
			std::cout << "thisisgameover";
			break;
		}
        ClearScreen();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
