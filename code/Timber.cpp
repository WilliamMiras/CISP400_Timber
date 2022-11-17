#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
	Event event;
	Clock clock;
	bool paused = true;
	int score = 0;

	// Create a video mode object

	VideoMode vm(1920, 1080);
	VideoMode monitor = vm.getDesktopMode();

	// Create and open a window for the game
	RenderWindow window(monitor, "Timber!!!", Style::Default);

	// Creating Text
	Text messageText;
	Text scoreText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// Creating the time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;

	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	// Making the background
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Creating six branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	for(int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}


	// Make a bee sprite
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	bool beeActive = false;
	float beeSpeed = 0.0f;

	// Make three cloud sprites
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1, spriteCloud2, spriteCloud3;

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

	// Making the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	side playerSide = side::LEFT;

	// Making the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Making the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Making the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Handles player input
	bool acceptInput = false;

	// Adding chopping sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// Adding death sound
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Adding out of time sound
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);


	while (window.isOpen())
	{

		/*
		****************************************
		Handle the players input
		****************************************
		*/
		while(window.pollEvent(event))
		{
			if(event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}

			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			}

			if(event.type == Event::Closed)
			{
				window.close();
			}

			if(Keyboard::isKeyPressed(Keyboard::Return))
			{
				paused = false;
				score = 0;
				timeRemaining = 6;

				for(int i = 1; i < NUM_BRANCHES; i++)
				{
					branchPositions[i] = side::NONE;
				}
				
				spriteRIP.setPosition(675, 2000);
				spritePlayer.setPosition(580, 720);

				acceptInput = true;
			}

			if(acceptInput)
			{
				if(Keyboard::isKeyPressed(Keyboard::Right))
				{
					playerSide = side::RIGHT;
					score++;
					timeRemaining += (2 / score) + 0.15;

					spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
					spritePlayer.setPosition(1200, 720);
					updateBranches(score);

					spriteLog.setPosition(810, 720);
					logSpeedX = -5000;
					logActive = true;

					acceptInput = false;
					chop.play();
				}

				if(Keyboard::isKeyPressed(Keyboard::Left))
				{
					playerSide = side::LEFT;
					score++;
					timeRemaining += (2 / score) + 0.15;
					
					spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
					spritePlayer.setPosition(580, 720);
					updateBranches(score);

					spriteLog.setPosition(810, 720);
					logSpeedX = 5000;
					logActive = true;

					acceptInput = false;
					chop.play();
				}
			}
		}
		/*
		****************************************
		Update the scene
		****************************************
		*/

		if(!paused)
		{
			// Measuring Time
			Time dt = clock.restart();

			// Resizing the time bar
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if(timeRemaining <= 0.0f)
			{
				paused = true;

				messageText.setString("Im sorry, but you have run out of time!");
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				outOfTime.play();
			}

			// Setting up the Bee
			if(!beeActive)
			{
				// How fast the Bee is
				srand((int)time(0));
				beeSpeed = (rand()% 200) + 200;

				// How high the Bee is
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);

				beeActive = true;
			}
			else
			// Moving the Bee
			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

				// Detect if bee reaches left-hand edge of screen
				if(spriteBee.getPosition().x < - 100)
				{
					beeActive = false;
				}
			}

			// Setting up the Clouds
			// Cloud #1
			if(!cloud1Active)
			{
				// How fast the Cloud is
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// How high the Cloud is
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);

				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);

				// Detect if the cloud reaches the right edge of screen
				if(spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}

			// Cloud #2
			if(!cloud2Active)
			{
				// How fast the Cloud is
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// How high the Cloud is
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);

				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

				// Detect if the cloud reaches the right edge of screen
				if(spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			// Cloud #3
			if(!cloud3Active)
			{
				// How fast the Cloud is
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// How high the Cloud is
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);

				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

				// Detect if the cloud reaches the right edge of screen
				if(spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}

			// Updating the score
			stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// Updating branches
			for(int i =0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if(branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}

			// Updating logs
			if(logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				if(spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// Player death
			if(branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				spriteRIP.setPosition(525, 760);
				spritePlayer.setPosition(200, 660);

				messageText.setString("SQUISHED!!");
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				death.play();
			}
		}
		/*
		****************************************
		Draw the scene
		****************************************
		*/

		// Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		for(int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		window.draw(spriteTree);

		window.draw(spritePlayer);
		window.draw(spriteAxe);

		window.draw(spriteLog);
		window.draw(spriteRIP);

		window.draw(spriteBee);

		window.draw(scoreText);
		window.draw(timeBar);

		if(paused)
		{
			window.draw(messageText);
		}

		

		// Show everything we just drew
		window.display();


	}

	return 0;
}

void updateBranches(int seed)
{
	// Moves branches down by one
	for(int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// Creates new branch at pos. 0
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch(r)
	{
		case 0:
			branchPositions[0] = side::LEFT;
			break;

		case 1:
			branchPositions[0] = side::RIGHT;
			break;
		
		default:
			branchPositions[0] = side::NONE;
			break;
	}
}	
