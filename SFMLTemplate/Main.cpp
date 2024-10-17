#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <iostream>

enum class Sides
{
	Left,
	Right,
	None,
};

void resetBee(sf::Sprite& sprBee, sf::Vector2f& spdBee, int bndGround, sf::Vector2u& sizeWindow, bool first = false);
void resetCloud(sf::Sprite& sprCloud, sf::Vector2f& spdCloud, int bndGround, sf::Vector2u& sizeWindow, bool first = false);

float sclVector(sf::Vector2f vector);
float randFloat(const float& start, const float& end);
int randInt(const int& start, const int& end);

int main()
{
	srand(time(NULL));

	sf::VideoMode vm(1920, 1080);
	sf::RenderWindow window(vm, "Timber!!!", sf::Style::Default);

#pragma region CommonVariable
	sf::Vector2u sizeWindow = window.getSize();
	sf::Vector2u posCenter = sizeWindow / 2u;

	int dScore = 0;
	int dArrowPressed = 0;
	int dEnterPressed = 0;
	int dSpacePressed = 0;

	bool bPause = true;
	bool bDeath = true;

	sf::Clock clock;
	sf::Time dt;
	float deltaTime;
	float envTime = 0;
	float gamTime = 0;
	float scaleTime = 1.f;

	sf::Vector2f sizeTimeBar(400.f, 80.f);

	float durTimebar = 3.f;
	float spdTimebar = sizeTimeBar.x / durTimebar;
#pragma endregion CommonVariable

#pragma region Ranges
	sf::Vector2u rngGround(400, sizeWindow.y);
	sf::Vector2f rngScreenBound(0 - 200, sizeWindow.x + 200);
	sf::Vector2f rngCloudSpeed(100.f, 200.f); //좌표를 최소값, 최대값으로 사용
	sf::Vector2f rngCloudY(0.f, 400.f);
#pragma endregion Ranges

#pragma region UI
	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	sf::Text txtScore;
	sf::Text txtMessage;
	sf::RectangleShape timebar;

	txtScore.setFont(font);
	txtScore.setString("Score = " + std::to_string(dScore));
	txtScore.setCharacterSize(100);
	txtScore.setFillColor(sf::Color::White);
	txtScore.setPosition(19.2f, 10.8f);

	txtMessage.setFont(font);
	txtMessage.setString("PRESS ENTER TO START!");
	txtMessage.setCharacterSize(100);
	txtMessage.setFillColor(sf::Color::White);

	timebar.setSize(sizeTimeBar);
	timebar.setFillColor(sf::Color::Red);
	timebar.setPosition(posCenter.x - sizeTimeBar.x * 0.5f, 950);

	auto bndMessage = txtMessage.getLocalBounds();
	txtMessage.setOrigin(bndMessage.width * 0.5f, bndMessage.height * 0.5f);
	txtMessage.setPosition(posCenter.x, posCenter.y);
#pragma endregion UI

#pragma region Background
	sf::Texture texBackground;
	texBackground.loadFromFile("graphics/background.png");
	sf::Sprite sprBackground;
	sprBackground.setTexture(texBackground);
#pragma endregion Background

#pragma region Cloud
	int cntCloud = randInt(5, 13);

	sf::Texture texCloud;
	sf::Sprite* sprCloud;
	sf::Vector2f* velCloud;
	int* dirRmnCloud;
	float* angCloud;

	texCloud.loadFromFile("graphics/cloud.png");
	sprCloud = new sf::Sprite[cntCloud];
	velCloud = new sf::Vector2f[cntCloud];
	dirRmnCloud = new int[cntCloud];
	angCloud = new float[cntCloud];

	for (int i = 0;i < cntCloud;++i)
	{
		sprCloud[i].setTexture(texCloud);
		resetCloud(sprCloud[i], velCloud[i], rngGround.x, sizeWindow, true);
		dirRmnCloud[i] = randInt(0, 10);
		angCloud[i] = randFloat(-(acos(-1) * 0.5f), acos(-1) * 0.5f);
	}
#pragma endregion Cloud

#pragma region Bee
	int cntBee = randInt(5, 9);
	sf::Texture texBee;
	texBee.loadFromFile("graphics/bee.png");
	sf::Sprite* sprBee;
	sf::Vector2f* velBee; // v = d / t
	sf::Vector2f* initposBee;

	sprBee = new sf::Sprite[cntBee];
	velBee = new sf::Vector2f[cntBee];
	initposBee = new sf::Vector2f[cntBee];

	for (int i = 0; i < cntBee;++i)
	{
		sprBee[i].setTexture(texBee);
		resetBee(sprBee[i], velBee[i], rngGround.x, sizeWindow, true);
	}
#pragma endregion Bee

#pragma region Tree
	sf::Texture texTree;
	sf::Sprite sprTree;
	sf::Vector2u sizeTree;
	texTree.loadFromFile("graphics/tree.png");

	sizeTree = texTree.getSize();
	sprTree.setTexture(texTree);
	sprTree.setOrigin(texTree.getSize().x * 0.5f, 0);
	sprTree.setPosition(posCenter.x, 0);
#pragma endregion Tree

#pragma region Branch
	int cntBranch = 6;
	sf::Texture texBranch;
	texBranch.loadFromFile("graphics/branch.png");
	sf::Sprite* sprBranch;
	sprBranch = new sf::Sprite[cntBranch];

	Sides* sidBranch;
	sidBranch = new Sides[cntBranch];
	sf::Vector2f orgBranch;
	orgBranch.x = -(texTree.getSize().x * 0.5f);
	orgBranch.y = texBranch.getSize().y * 0.5f;
	sf::Vector2f posBranch;

	posBranch = sf::Vector2f(posCenter.x, 750.f);
	for (int i = 0;i < cntBranch;++i)
	{
		sprBranch[i].setTexture(texBranch);
		sprBranch[i].setOrigin(orgBranch);

		sidBranch[i] = (i != 0) ? (Sides)(rand() % 3) : Sides::None;

		sprBranch[i].setPosition(posBranch);
		posBranch.y -= 150.f;

		if (sidBranch[i] == Sides::Left)
		{
			sprBranch[i].setScale(-1, 1);
		}
	}
#pragma endregion Branch

#pragma region Player
	sf::Texture texPlayer;
	texPlayer.loadFromFile("graphics/player.png");
	sf::Texture texRip;
	texRip.loadFromFile("graphics/rip.png");
	sf::Sprite sprPlayer;
	sf::Vector2f orgPlayer;
	sf::Vector2f posPlayer;
	sprPlayer.setTexture(texPlayer);
	orgPlayer.x = -(float)(texTree.getSize().x);
	orgPlayer.y = texPlayer.getSize().y;
	sprPlayer.setOrigin(orgPlayer);
	posPlayer.x = posCenter.x;
	posPlayer.y = texTree.getSize().y;
	sprPlayer.setPosition(posPlayer);
#pragma endregion Player

	while (window.isOpen())
	{
		dt = clock.restart();
		deltaTime = dt.asSeconds();
		envTime += deltaTime;
		gamTime += deltaTime * scaleTime;
		// 메시지 (이벤트) 루프
		sf::Event ev;

		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
			case sf::Event::EventType::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (ev.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::Space:
					if (dSpacePressed < 2)
					{
						dSpacePressed = 2;
					}
					break;
				case sf::Keyboard::Return:
					if (dEnterPressed < 2)
					{
						dEnterPressed = 2;
					}
					break;
				case sf::Keyboard::Left:
					if (!bPause && dArrowPressed < 2)
					{
						dArrowPressed = 2;
					}
					break;
				case sf::Keyboard::Right:
					if (!bPause && dArrowPressed < 2)
					{
						dArrowPressed = 4;
					}
					break;
				}
				break;
			case sf::Event::KeyReleased:
				switch (ev.key.code)
				{
				case sf::Keyboard::Left:
				case sf::Keyboard::Right:
					dArrowPressed = 0;
					break;
				case sf::Keyboard::Space:
					dSpacePressed = 0;
					break;
				}
			default:
				break;
			}
		}

		if (dSpacePressed > 1)
		{
			dSpacePressed = 1;
			if (!bDeath)
			{
				if (!bPause)
				{
					txtMessage.setString("PAUSE");
					auto boundMessage = txtMessage.getLocalBounds();
					txtMessage.setOrigin(boundMessage.width * 0.5f, boundMessage.height * 0.5f);
					txtMessage.setFillColor(sf::Color::Yellow);
				}

				bPause = !bPause;
				scaleTime = (bPause) ? 0.f : 1.f;
			}
		}
		if (dEnterPressed > 1)
		{
			dEnterPressed = 1;
			if (bPause)
			{
				bPause = !bPause;
				scaleTime = (bPause) ? 0.f : 1.f;
				gamTime = 0;
				dScore = 0;

				if (bDeath)
				{
					sprPlayer.setTexture(texPlayer);
					bDeath = false;
					sidBranch[0] = Sides::None;
					for (int i = 1;i < cntBranch;++i)
					{
						sidBranch[i] = (Sides)(rand() % 3);
					}
					for (int i = 0;i < cntBranch;++i)
					{
						sprBranch[i].setScale((sidBranch[i] == Sides::Left) ? -1 : 1, 1);
					}
				}
			}
		}
		if (!bPause)
		{
			if (dArrowPressed > 1)
			{
				for (int i = 1;i < cntBranch;++i)
				{
					sidBranch[i - 1] = sidBranch[i];
				}
				sidBranch[cntBranch - 1] = (Sides)(rand() % 3);
				for (int i = 0;i < cntBranch;++i)
				{
					sprBranch[i].setScale((sidBranch[i] == Sides::Left) ? -1 : 1, 1);
				}

				sprPlayer.setScale((dArrowPressed > 2) ? 1 : -1, 1);

				if (sidBranch[0] != Sides::None
					&& sprPlayer.getScale().x * sprBranch[0].getScale().x > 0)
				{
					sprPlayer.setTexture(texRip);
					bPause = true;
					bDeath = true;
					txtMessage.setString("GAME OVER!\nPRESS ENTER TO RESTART!");
					auto boundMessage = txtMessage.getLocalBounds();
					txtMessage.setOrigin(boundMessage.width * 0.5f, boundMessage.height * 0.5f);
					txtMessage.setFillColor(sf::Color::Red);
				}
				else
				{
					++dScore;
					gamTime > 0.5f ? gamTime -= 0.5 : gamTime = 0;
				}
				dArrowPressed = 1;
			}

			sf::Vector2f size = timebar.getSize();
			size.x = sizeTimeBar.x - spdTimebar * gamTime;
			if (size.x < 0.f)
			{
				size.x = 0.f;
				bPause = true;
				bDeath = true;
				txtMessage.setString("TIME OVER!\nPRESS ENTER TO RESTART!");
				auto boundMessage = txtMessage.getLocalBounds();
				txtMessage.setOrigin(boundMessage.width * 0.5f, boundMessage.height * 0.5f);
				txtMessage.setFillColor(sf::Color::Red);
				scaleTime = 0.f;
			}

			timebar.setSize(size);

			txtScore.setString("SCORE: " + std::to_string(dScore));
		}

		for (int i = 0; i < cntCloud;++i)
		{
			auto posCurCloud = sprCloud[i].getPosition();
			auto bndCurCloud = sprCloud[i].getLocalBounds();

			if (dirRmnCloud[i]++ > 10)
			{
				angCloud[i] = randFloat(-acos(-1) * 0.5f, acos(-1) * 0.5f);
			}
			float speed = sclVector(velCloud[i]);

			float dirVelocity = 1;
			if (posCurCloud.y + bndCurCloud.height * 0.5f < 0)
			{
				angCloud[i] = abs(angCloud[i]);
			}
			else if (posCurCloud.y - bndCurCloud.height > rngGround.x)
			{
				angCloud[i] = -abs(angCloud[i]);
			}
			if (sprCloud[i].getScale().x > 0)
			{
				dirVelocity = -1;
			}
			velCloud[i].x += dirVelocity * speed * cos(angCloud[i]) * deltaTime;
			velCloud[i].y += speed * sin(angCloud[i]) * deltaTime;
			velCloud[i] *= speed / sclVector(velCloud[i]);
			posCurCloud.x += velCloud[i].x * deltaTime;
			posCurCloud.y += velCloud[i].y * deltaTime;

			sprCloud[i].setPosition(posCurCloud);

			if ((posCurCloud.x - bndCurCloud.width > rngScreenBound.y)
				|| (posCurCloud.x + bndCurCloud.width < rngScreenBound.x))
			{
				resetCloud(sprCloud[i], velCloud[i], rngGround.x, sizeWindow);
			}
		}

		for (int i = 0;i < cntBee;++i)
		{
			auto posCurBee = sprBee[i].getPosition();
			auto bndCurBee = sprBee[i].getLocalBounds();

			bool respawn = false;
			posCurBee.x += velBee[i].x * deltaTime;
			posCurBee.y += velBee[i].y * deltaTime * (sin((velBee[i].y / 25.4f) + envTime * acos(-1)));

			sprBee[i].setPosition(posCurBee);

			if ((posCurBee.x - bndCurBee.width > rngScreenBound.y)
				|| (posCurBee.x + bndCurBee.width < rngScreenBound.x))
			{
				resetBee(sprBee[i], velBee[i], rngGround.x, sizeWindow);
			}
		}

		// 업데이트


#pragma region DrawRegion
		// 그리기
		window.clear();

		window.draw(sprBackground);

		for (int i = 0;i < cntCloud;++i)
		{
			window.draw(sprCloud[i]);
		}
		for (int i = 0;i < cntBranch;++i)
		{
			if (sidBranch[i] != Sides::None)
			{
				window.draw(sprBranch[i]);
			}
		}

		window.draw(sprTree);
		window.draw(sprPlayer);
		for (int i = 0;i < cntBee;++i)
		{
			window.draw(sprBee[i]);
		}
		window.draw(txtScore);
		if (bPause || bDeath)
		{
			window.draw(txtMessage);
		}
		window.draw(timebar);

		//window.draw(sprAxe);
		window.display();
#pragma endregion DrawRegion
	}
	delete[] sprBee;
	delete[] velBee;
	delete[] sprBranch;
	delete[] sprCloud;
	delete[] velCloud;
	delete[] angCloud;
	delete[] dirRmnCloud;
	delete[] initposBee;
	delete[] sidBranch;
	return 0;
}

void resetBee(sf::Sprite& sprBee, sf::Vector2f& spdBee, int bndGround, sf::Vector2u& sizeWindow, bool first)
{
	bool dirLeft = randInt(0, 2);
	sf::FloatRect bndCurBee;

	float speedx = randInt(80, 160);
	float speedy = randInt(80, 160);
	float scalex = -randFloat(0.8f, 1.6f);
	float scaley = randFloat(0.8f, 1.6f);

	float spawnx = 0;
	float spawny = randInt(600, bndGround + 600);

	if (dirLeft)
	{
		spawnx = sizeWindow.x;
		speedx = -speedx;
		scalex = -scalex;
	}
	if (first)
	{
		spawnx = rand() % sizeWindow.x;
	}
	spdBee.x = speedx;
	spdBee.y = speedy;
	sprBee.setPosition(spawnx, spawny);
	sprBee.setScale(scalex, scaley);
}

void resetCloud(sf::Sprite& sprCloud, sf::Vector2f& spdCloud, int bndGround, sf::Vector2u& sizeWindow, bool first)
{
	bool dirLeft = randInt(0, 2);
	sf::FloatRect bndCurCloud;

	float speedx = randFloat(120.f, 200.f);
	float speedy = 0.f;
	float scalex = -(randFloat(0.8f, 1.8f));
	float scaley = randFloat(0.8f, 1.8f);
	float spawnx = 0;
	float spawny = 0;

	if (dirLeft)
	{
		scalex = -scalex;
		speedx = -speedx;
		spawnx = sizeWindow.x;
	}
	if (first)
	{
		spawnx = rand() % sizeWindow.x;
	}

	sprCloud.setScale(scalex, scaley);
	bndCurCloud = sprCloud.getLocalBounds();
	spawny = randInt(0, bndGround) - bndCurCloud.height;
	sprCloud.setPosition(spawnx, spawny);
	spdCloud.x = speedx;
	spdCloud.y = speedy;
}

float sclVector(sf::Vector2f vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float randFloat(const float& start, const float& end)
{
	return (rand() / (float)RAND_MAX) * (end - start) + start;
}

int randInt(const int& start, const int& end)
{
	return rand() % (end - start) + start;
}