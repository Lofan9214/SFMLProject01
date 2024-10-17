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

void resetBee(sf::Sprite& sprBee, sf::Vector2f& spdBee, float& ang, int bndGround, sf::Vector2u& sizeWindow, bool first = false);
void resetCloud(sf::Sprite& sprCloud, sf::Vector2f& spdCloud, int bndGround, sf::Vector2u& sizeWindow, bool first = false);

int main()
{
	srand(time(NULL));

	int cntCloud = rand() % 8 + 5;
	int cntBee = rand() % 3 + 5;
	int cntBranch = 6;
	int bndGround = 400;
	int dScore = 0;

	bool bPause = true;
	bool bDeath = false;
	bool bLeftDown = false;
	bool bRightDown = false;
	bool bLeftKey = false;
	bool bRightKey = false;

	float scaleTime = 1.f;
	float widTimebar = 400.f;
	float heiTimebar = 80.f;
	float durTimebar = 3.f;
	float rmnTimebar = durTimebar;
	float spdTimebar = widTimebar / durTimebar;
	float* startyBee;

	sf::VideoMode vm(1920, 1080);
	sf::RenderWindow window(vm, "Timber!!!", sf::Style::Default);

	sf::Clock clock;
	sf::Time dt;
	float deltaTime;
	float sumTime = 0;
	float timerTime = 0;

	sf::Texture texBackground;
	sf::Texture texCloud;
	sf::Texture texBee;
	sf::Texture texTree;
	sf::Texture texBranch;
	sf::Texture texPlayer;
	sf::Texture texRip;

	//sf::Texture texAxe;
	sf::Font font;

	sf::Sprite sprBackground;
	sf::Sprite sprTree;
	sf::Sprite sprPlayer;
	//sf::Sprite sprAxe;
	sf::Sprite* sprCloud;
	sf::Sprite* sprBee;
	sf::Sprite* sprBranch;

	sf::Vector2u sizeWindow = window.getSize();
	sf::Vector2u posCenter = sizeWindow / 2u;
	sf::Vector2u sizeTree;

	sf::Text txtScore;
	sf::Text txtMessage;

	sf::RectangleShape timebar;

	sf::Vector2f orgBranch;
	sf::Vector2f orgPlayer;
	sf::Vector2f posBackground(sizeWindow.x * 0.5f, sizeWindow.y * 0.5f);
	sf::Vector2f posBee;
	sf::Vector2f posBranch;
	sf::Vector2f posPlayer;
	sf::Vector2f* spdBee; // v = d / t
	sf::Vector2f* initposBee;

	sf::Vector2f posCloud;
	sf::Vector2f scaCloud;
	sf::Vector2f* spdCloud;

	Sides* sidBranch;

	texBackground.loadFromFile("graphics/background.png");
	texTree.loadFromFile("graphics/tree.png");
	texCloud.loadFromFile("graphics/cloud.png");
	texBee.loadFromFile("graphics/bee.png");
	texBranch.loadFromFile("graphics/branch.png");
	texPlayer.loadFromFile("graphics/player.png");
	texRip.loadFromFile("graphics/rip.png");
	//texAxe.loadFromFile("graphics/axe1.png");

	font.loadFromFile("fonts/KOMIKAP_.ttf");

	sprBackground.setTexture(texBackground);
	sprBackground.setOrigin(posCenter.x, posCenter.y);
	sprBackground.setPosition(posBackground);

	sizeTree = texTree.getSize();
	sprTree.setTexture(texTree);
	sprTree.setOrigin(texTree.getSize().x * 0.5f, 0);
	sprTree.setPosition(posCenter.x, 0);

	sprBee = new sf::Sprite[cntBee];
	spdBee = new sf::Vector2f[cntBee];
	startyBee = new float[cntBee];
	initposBee = new sf::Vector2f[cntBee];

	//sprAxe.setTexture(texAxe);
	//sprAxe.setOrigin(texAxe.getSize().x / 2, texAxe.getSize().y / 2);
	//sprAxe.setPosition(posCenter.x, posCenter.y * 1.5);

	for (int i = 0; i < cntBee;++i)
	{
		sprBee[i].setTexture(texBee);
		resetBee(sprBee[i], spdBee[i], startyBee[i], bndGround, sizeWindow, true);
	}

	sidBranch = new Sides[cntBranch];
	sprBranch = new sf::Sprite[cntBranch];
	orgBranch.x = -(texTree.getSize().x * 0.5f);
	orgBranch.y = texBranch.getSize().y * 0.5f;
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

	sprPlayer.setTexture(texPlayer);
	orgPlayer.x = -(texTree.getSize().x * 1.f);
	orgPlayer.y = texPlayer.getSize().y;
	sprPlayer.setOrigin(orgPlayer);
	posPlayer.x = posCenter.x;
	posPlayer.y = texTree.getSize().y;
	sprPlayer.setPosition(posPlayer);

	sprCloud = new sf::Sprite[cntCloud];
	spdCloud = new sf::Vector2f[cntCloud];

	sf::Vector2f spdrngCloud(100.f, 200.f); //좌표를 최소값, 최대값으로 사용
	sf::Vector2f yrngCloud(0.f, 400.f);

	float leftX = 0 - 200; // 경계 검사, 왼쪽 소환
	float rightX = sizeWindow.x + 200;

	txtScore.setFont(font);
	txtScore.setString("Score = " + std::to_string(dScore));
	txtScore.setCharacterSize(100);
	txtScore.setFillColor(sf::Color::White);
	txtScore.setPosition(10.f, 10.f);

	txtMessage.setFont(font);
	txtMessage.setString("PRESS ENTER TO START!");
	txtMessage.setCharacterSize(100);
	txtMessage.setFillColor(sf::Color::White);
	auto bndMessage = txtMessage.getLocalBounds();

	timebar.setSize({ widTimebar,heiTimebar });
	timebar.setFillColor(sf::Color::Red);
	timebar.setPosition(posCenter.x - widTimebar * 0.5f, 950);

	txtMessage.setOrigin(bndMessage.width * 0.5f, bndMessage.height * 0.5f);
	txtMessage.setPosition(posCenter.x, posCenter.y);

	for (int i = 0;i < cntCloud;++i)
	{
		sprCloud[i].setTexture(texCloud);
		resetCloud(sprCloud[i], spdCloud[i], bndGround, sizeWindow, true);
	}

	while (window.isOpen())
	{
		dt = clock.restart();
		deltaTime = dt.asSeconds();
		deltaTime *= scaleTime;
		sumTime += deltaTime;
		timerTime += deltaTime;
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
					scaleTime = (scaleTime == 0.f) ? 1.f : 0.f;
					break;
				case sf::Keyboard::Return:
					if (bPause)
					{
						bPause = !bPause;
						scaleTime = (bPause) ? 0.f : 1.f;
						sumTime = 0;
						timerTime = 0;
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
					break;
				case sf::Keyboard::Num1:
					dScore += 10;
					break;
				case sf::Keyboard::Num2:
					dScore -= 10;
					break;
				case sf::Keyboard::Left:
					if (!bPause)
					{
						if (bLeftDown)
						{
							break;
						}
						bLeftKey = true;
						bLeftDown = true;
					}
					break;
				case sf::Keyboard::Right:
					if (!bPause)
					{
						if (bRightDown)
						{
							break;
						}
						bRightKey = true;
						bRightDown = true;
					}
					break;
				}
				break;
			case sf::Event::KeyReleased:
				switch (ev.key.code)
				{
				case sf::Keyboard::Left:
					bLeftKey = false;
					bLeftDown = false;
					break;
				case sf::Keyboard::Right:
					bRightKey = false;
					bRightDown = false;
					break;
				}
			case sf::Event::MouseMoved:
				//float angmouse;
				//angmouse = 180 * atan2(ev.mouseMove.y - sprAxe.getPosition().y, ev.mouseMove.x - sprAxe.getPosition().x) / 3.14 + 90;
				//
				//sprAxe.setRotation(angmouse);
				break;

			default:
				break;
			}
		}

		if (!bPause)
		{
			if ((bLeftDown && bLeftKey) || (bRightDown && bRightKey))
			{
				bLeftKey = false;
				bRightKey = false;
				for (int i = 1;i < cntBranch;++i)
				{
					sidBranch[i - 1] = sidBranch[i];
				}
				sidBranch[cntBranch - 1] = (Sides)(rand() % 3);
				for (int i = 0;i < cntBranch;++i)
				{
					sprBranch[i].setScale((sidBranch[i] == Sides::Left) ? -1 : 1, 1);
				}

				sprPlayer.setScale((ev.key.code == sf::Keyboard::Left) ? -1 : 1, 1);

				if (sidBranch[0] != Sides::None
					&& sprPlayer.getScale().x * sprBranch[0].getScale().x > 0)
				{
					std::cout << "Game Over!" << std::endl;

					sprPlayer.setTexture(texRip);
					bPause = true;
					bDeath = true;
				}
				else
				{
					++dScore;
					(timerTime > 0.5f) ? timerTime -= 0.5 : timerTime = 0;
				}
			}

			for (int i = 0; i < cntCloud;++i)
			{
				auto posCurCloud = sprCloud[i].getPosition();
				auto bndCurCloud = sprCloud[i].getLocalBounds();

				bool dirRight;
				if (sprCloud[i].getScale().x < 0)
				{
					dirRight = true;
					spdCloud[i].x += rand() % 11 - 5;
				}
				else
				{
					dirRight = false;
					spdCloud[i].x -= rand() % 11 - 5;
				}
				spdCloud[i].y = rand() % 301 - 150;

				if (dirRight)
				{
					posCurCloud.x += abs(spdCloud[i].x * deltaTime);
				}
				else
				{
					posCurCloud.x -= abs(spdCloud[i].x * deltaTime);
				}

				if (posCurCloud.y + sprCloud[i].getLocalBounds().height * 0.5f < 0)
				{
					posCurCloud.y += abs(spdCloud[i].y * deltaTime);
				}
				else if (posCurCloud.y - sprCloud[i].getLocalBounds().height > bndGround)
				{
					posCurCloud.y -= abs(spdCloud[i].y * deltaTime);
				}
				else
				{
					posCurCloud.y += spdCloud[i].y * deltaTime;
				}

				sprCloud[i].setPosition(posCurCloud);

				if ((dirRight && (posCurCloud.x - bndCurCloud.width > rightX))
					|| (!dirRight && (posCurCloud.x + bndCurCloud.width < leftX)))
				{
					resetCloud(sprCloud[i], spdCloud[i], bndGround, sizeWindow);
				}
			}

			for (int i = 0;i < cntBee;++i)
			{
				auto posCurBee = sprBee[i].getPosition();
				auto bndCurBee = sprBee[i].getLocalBounds();

				bool dirRight;
				bool respawn = false;
				if (sprBee[i].getScale().x < 0)
				{
					dirRight = true;
				}
				else
				{
					dirRight = false;
				}
				posCurBee.x += spdBee[i].x * deltaTime;
				posCurBee.y += spdBee[i].y * deltaTime * (sin(startyBee[i] + sumTime * acos(-1)));

				sprBee[i].setPosition(posCurBee);

				if ((dirRight && posCurBee.x - bndCurBee.width > rightX)
					|| (!dirRight && posCurBee.x + bndCurBee.width < leftX))
				{
					resetBee(sprBee[i], spdBee[i], startyBee[i], bndGround, sizeWindow);
				}
			}

			sf::Vector2f size = timebar.getSize();
			size.x = widTimebar - spdTimebar * timerTime;
			if (size.x < 0.f)
			{
				size.x = 0.f;
				// 게임 오버
				std::cout << "Time Out!" << std::endl;
			}

			timebar.setSize(size);

			txtScore.setString("SCORE: " + std::to_string(dScore));

		}
		// 업데이트

		window.clear();

		// 그리기

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
		if (bPause)
		{
			window.draw(txtMessage);
		}
		window.draw(timebar);

		//window.draw(sprAxe);
		window.display();
	}
	delete[] sprBee;
	delete[] sprBranch;
	delete[] sprCloud;
	delete[] spdBee;
	delete[] spdCloud;
	delete[] startyBee;
	delete[] initposBee;
	delete[] sidBranch;
	return 0;
}

void resetBee(sf::Sprite& sprBee, sf::Vector2f& spdBee, float& ang, int bndGround, sf::Vector2u& sizeWindow, bool first)
{
	ang = (rand() % 315) / 100.f;
	bool dirLeft = rand() % 2;
	sf::FloatRect bndCurBee;

	float speedx = rand() % 81 + 80.f;
	float speedy = rand() % 81 + 80.f;
	float scalex = -(rand() % 41 + 80) / 100.f;
	float scaley = (rand() % 41 + 80) / 100.f;

	float spawnx = 0;
	float spawny = 600 + rand() % bndGround;

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
	bool dirLeft = rand() % 2;
	sf::FloatRect bndCurCloud;

	float speedx = rand() % 100 + 100;
	float speedy = 0.f;
	float scalex = -(rand() % 41 + 80) / 100.f;
	float scaley = (rand() % 41 + 80) / 100.f;

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
	bndCurCloud = sprCloud.getGlobalBounds();
	spawny = (rand() % bndGround) - bndCurCloud.height;
	sprCloud.setPosition(spawnx, spawny);
	spdCloud.x = speedx;
	spdCloud.y = speedy;
}
