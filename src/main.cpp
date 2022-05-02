#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;

Sprite branches[NUM_BRANCHES];

enum class side {LEFT, RIGHT, NONE};

side branchPositions[NUM_BRANCHES];

int main()
{
	//Create video mode object.
	VideoMode vm(1920, 1080);

	// Create and open a window for the game.
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Backgroud Texture
	Texture textureBackground;
	textureBackground.loadFromFile("assets/graphics/background.png");

	// Background Sprite
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Tree Texture
	Texture textureTree;
	textureTree.loadFromFile("assets/graphics/tree.png");
	
	// Tree Sprite
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Prepare the bee.
	Texture textureBee;
	textureBee.loadFromFile("assets/graphics/bee.png");

	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	bool beeActive = false;
	float beeSpeed = 0.0f;

	// Prepare the clouds.

	Texture textureCloud;
	textureCloud.loadFromFile("assets/graphics/cloud.png");

	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;

	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	
	// Setting up HUD elements.
	int score = 0;

	Text messageText;
	Text scoreText;

	Font font;
	font.loadFromFile("assets/fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin
	(
		textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f
	);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// Setup 6 branches.
	Texture textureBranch;
	textureBranch.loadFromFile("assets/graphics/branch.png");

	// Set texture for each branch sprite.
	for (int i = 0; i < NUM_BRANCHES; ++i)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	Clock clock;

	// Setting up the time bar.
	RectangleShape timeBar;

	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition
	(
		(1920 / 2) - timeBarStartWidth / 2,
		980
	);

	Time gameTimeTotal;

	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	bool paused = true;

	// Prepare the player.
	Texture texturePlayer;
	texturePlayer.loadFromFile("assets/graphics/player.png");

	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	// The player starts on the left side.
	side playerSide = side::LEFT;

	// Prepare the gravestone.
	Texture textureRIP;
	textureRIP.loadFromFile("assets/graphics/rip.png");
	
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Prepare the axe.
	Texture textureAxe;
	textureAxe.loadFromFile("assets/graphics/axe.png");

	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// Line up the axe with the tree.
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Prepare the flying log.
	Texture textureLog;
	textureLog.loadFromFile("assets/graphics/log.png");

	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		/*
		**************************************
		* Handle the player's input.
		**************************************
		*/

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			// Reset time and the score.
			score = 0;
			timeRemaining = 6;

			// Make all branches disappear, starting in the second position.
			for (int i = 1; i < NUM_BRANCHES; ++i)
			{
				branchPositions[i] = side::NONE;
			}

			// Make gravestone hidden.
			spriteRIP.setPosition(675, 2000);

			// Move player into position.
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}

		if (acceptInput)
		{
			// Handle the right cursor key input.
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;

				score++;
				timeRemaining = timeRemaining + (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);

				updateBranches(score);
				
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;


			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;

				score++;
				timeRemaining = timeRemaining + (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
			}


		}

		/*
		****************************************
		* Update the scene.
		****************************************
		*/
		
		if (!paused)
		{
			Time dt = clock.restart();

			// Resizing the time bar.
			timeRemaining = timeRemaining - dt.asSeconds();

			timeBar.setSize
			(
				Vector2f(timeBarWidthPerSecond * timeRemaining, 
						timeBarHeight)
			);

			// Checking if time has expired.
			if (timeRemaining <= 0.0f)
			{
				paused = true;

				messageText.setString("Out of time!!!");

				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin
				(
					textRect.left +  textRect.width/2.0f, 
					textRect.top + textRect.height/2.0f
				);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

			}

			// Setup the bee.
			if (!beeActive)
			{
				srand((int)time(nullptr));
				beeSpeed = (rand() % 200) + 200;

				float height = (rand() % 500) + 500;

				spriteBee.setPosition(2000, height);
				beeActive = true;
			}

			else
			{
				spriteBee.setPosition
				(
					spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y
				);

				// Bee has reached the left-hand edge of the screen.
				if (spriteBee.getPosition().x < -100)
				{
					// Setup to be a new bee in the next frame.
					beeActive = false;
				}
			}

			// Manage the clouds.
			// Cloud 1:

			if (!cloud1Active)
			{
				srand((int)time(nullptr) * 10);

				cloud1Speed = (rand() % 200);

				srand((int)time(nullptr) * 10);

				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);

				cloud1Active = true;
			}

			else
			{
				spriteCloud1.setPosition
				(
					spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y
				);

				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}

			// Cloud 2:

			if (!cloud2Active)
			{
				srand((int)time(nullptr) * 20);

				cloud2Speed = (rand() % 200);

				srand((int)time(nullptr) * 20);

				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);

				cloud2Active = true;
			}

			else
			{
				spriteCloud2.setPosition
				(
					spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y
				);

				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			// Cloud 3:

			if (!cloud3Active)
			{
				srand((int)time(nullptr) * 30);

				cloud3Speed = (rand() % 200);

				srand((int)time(nullptr) * 30);

				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);

				cloud3Active = true;
			}

			else
			{
				spriteCloud3.setPosition
				(
					spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y
				);

				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}

			// Updating score on the HUD.
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			for (int i = 0; i < NUM_BRANCHES; ++i)
			{
				float height = i * 150;

				if (branchPositions[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height);

				}
			}

			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// Handling player death.
			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;
				
				// Position gravestone and hide player.
				spriteRIP.setPosition(525, 760);
				spritePlayer.setPosition(2000, 660);

				spriteRIP.setPosition(525, 760);
				spritePlayer.setPosition(2000, 660);

				// Draw text to the center of the screen.
				messageText.setString("SQUISHED!!!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin
				(
					textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f
				);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}
		}

		/*
		*****************************************
		* Draw the scene.
		*****************************************
		*/

		// Clear the window from the previous frame.
		window.clear();

		// Draw our game scene here.

		// Draw the background.
		window.draw(spriteBackground);

		// Draw the clouds.
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		for (int i = 0; i < NUM_BRANCHES; ++i)
		{
			window.draw(branches[i]);
		}

		// Draw the tree;
		window.draw(spriteTree);

		// Draw the player.
		window.draw(spritePlayer);

		// Draw the axe.
		window.draw(spriteAxe);

		// Draw the log.
		window.draw(spriteLog);

		// Draw the gravestone.
		window.draw(spriteRIP);

		// Draw the bee.
		window.draw(spriteBee);

		window.draw(scoreText);

		window.draw(timeBar);

		if (paused)
		{
			window.draw(messageText);
		}

		// Show everything we just drew.
		window.display();
	}

	return 0;
}

void updateBranches(int seed)
{
	// Move branches to the right in the array.
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at the first position in the array.
	srand((int)time(nullptr) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}