#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <iostream>

//지평선 높이 650 언저리

int main()
{
	srand(time(NULL));

	int cntCloud = rand() % 8 + 3;
	int cntBee = rand() % 8 + 3;
	int bndGround = 400;

	sf::VideoMode vm(1920, 1080);
	sf::RenderWindow window(vm, "Timber!!!", sf::Style::Default);

	sf::Clock clock;
	sf::Time dt;
	float deltaTime;
	float sumTime = 0;

	sf::Texture texBackground;
	sf::Texture texCloud;
	sf::Texture texBee;
	sf::Texture texTree;

	sf::Sprite sprBackground;
	sf::Sprite sprTree;
	sf::Sprite* sprCloud;
	sf::Sprite* sprBee;
	sf::Vector2u sizeWindow = window.getSize();
	sf::Vector2u posCenter = (sf::Vector2u)(sizeWindow / 2u);
	sf::Vector2u sizeTree;
	sf::Vector2f posBackground(sizeWindow.x / 2, sizeWindow.y / 2);

	sf::Vector2f posBee; // v = d / t
	sf::Vector2f* spdBee; // v = d / t
	float* angBee;

	sf::Vector2f posCloud;
	sf::Vector2f scaCloud;
	sf::Vector2f* spdCloud;

	texBackground.loadFromFile("graphics/background.png");
	texTree.loadFromFile("graphics/tree.png");
	texCloud.loadFromFile("graphics/cloud.png");
	texBee.loadFromFile("graphics/bee.png");

	sprBackground.setTexture(texBackground);
	sprBackground.setOrigin(posCenter.x, posCenter.y);
	sprBackground.setPosition(posBackground);

	sizeTree = texTree.getSize();
	sprTree.setTexture(texTree);
	sprTree.setOrigin(texTree.getSize().x / 2, 0);
	sprTree.setPosition(posCenter.x, 0);

	sprBee = new sf::Sprite[cntBee];
	spdBee = new sf::Vector2f[cntBee];
	angBee = new float[cntBee];

	bool dirRight;
	for (int i = 0; i < cntBee;++i)
	{
		sprBee[i].setTexture(texBee);
		angBee[i] = (rand() % 315) / 100.f;
		dirRight = rand() % 2;
		if (dirRight)
		{
			sprBee[i].setScale(-(rand() % 41 + 80) / 100.f, (rand() % 41 + 80) / 100.f);
			sprBee[i].setPosition(0, 600 + rand() % bndGround);
			spdBee[i].x = rand() % 81 + 40.f;
			spdBee[i].y = rand() % 81 + 40.f;
		}
		else
		{
			sprBee[i].setScale((rand() % 41 + 80 )/ 100.f, (rand() % 41 + 80) / 100.f);
			sprBee[i].setPosition(sizeWindow.x, 600 + rand() % bndGround);
			spdBee[i].x = -(rand() % 81 + 40.f);
			spdBee[i].y = rand() % 81 + 40.f;
		}

	}

	sprCloud = new sf::Sprite[cntCloud];
	spdCloud = new sf::Vector2f[cntCloud];

	for (int i = 0;i < cntCloud;++i)
	{
		sprCloud[i].setTexture(texCloud);
		dirRight = rand() % 2;
		if (dirRight)
		{
			sprCloud[i].setScale(-(rand() % 41 + 80) / 100.f, (rand() % 41 + 80) / 100.f);
			sprCloud[i].setPosition(0, (rand() % bndGround) - sprCloud[i].getGlobalBounds().height);
			spdCloud[i].x = rand() % 101 + 50.f;
			spdCloud[i].y = 0.f;
		}
		else
		{
			sprCloud[i].setScale((rand() % 41 + 80) / 100.f, (rand() % 41 + 80) / 100.f);
			sprCloud[i].setPosition(sizeWindow.x, (rand() % bndGround) - sprCloud[i].getGlobalBounds().height);
			spdCloud[i].x = -(rand() % 101 + 50.f);
			spdCloud[i].y = 0.f;
		}
	}

	while (window.isOpen())
	{
		dt = clock.restart();
		deltaTime = dt.asSeconds();
		sumTime += deltaTime;
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
					for (int i = 0;i < 10;++i)
					{
						int number = rand() % 10 - 5;
						std::cout << number << std::endl;
					}
					break;
				}
				break;
			case sf::Event::MouseMoved:
				ev.mouseMove.x;
				break;

			default:
				break;
			}
		}

		for (int i = 0; i < cntCloud;++i)
		{
			auto posCurCloud = sprCloud[i].getPosition();
			auto bndCurCloud = sprCloud[i].getGlobalBounds();

			bool dirRight;
			if (spdCloud[i].x > 0)
			{
				dirRight = true;
				spdCloud[i].x = rand() % 201;
				spdCloud[i].y = rand() % 101 - 50;
			}
			else
			{
				dirRight = false;
				spdCloud[i].x = -(rand() % 201);
				spdCloud[i].y = rand() % 101 - 50;
			}



			if (dirRight)
			{
				posCurCloud.x += abs(spdCloud[i].x * deltaTime);
			}
			else
			{
				posCurCloud.x -= abs(spdCloud[i].x * deltaTime);
			}

			if (posCurCloud.y + sprCloud[i].getGlobalBounds().height / 2 < 0)
			{
				posCurCloud.y += abs(spdCloud[i].y * deltaTime);
			}
			else if (posCurCloud.y - sprCloud[i].getGlobalBounds().height > bndGround)
			{
				posCurCloud.y -= abs(spdCloud[i].y * deltaTime);
			}
			else
			{
				posCurCloud.y += spdCloud[i].y * deltaTime;
			}

			sprCloud[i].setPosition(posCurCloud);

			if ((dirRight && (posCurCloud.x - bndCurCloud.width > sizeWindow.x))
				|| (!dirRight && (posCurCloud.x + bndCurCloud.width < 0)))
			{
				dirRight = rand() % 2;
				if (dirRight)
				{
					sprCloud[i].setScale(-(rand() % 41 + 80) / 100.f, (rand() % 41 + 80) / 100.f);
					bndCurCloud = sprCloud[i].getGlobalBounds();
					spdCloud[i].x = rand() % 201;
					spdCloud[i].y = 0.f;
					sprCloud[i].setPosition(0, (rand() % bndGround) - bndCurCloud.height);
				}
				else
				{
					sprCloud[i].setScale((rand() % 41 + 80) / 100.f, (rand() % 41 + 80) / 100.f);
					bndCurCloud = sprCloud[i].getGlobalBounds();
					spdCloud[i].x = -(rand() % 201);
					spdCloud[i].y = 0.f;
					sprCloud[i].setPosition(sizeWindow.x, (rand() % bndGround) - bndCurCloud.height);
				}
			}
		}

		for (int i = 0;i < cntBee;++i)
		{
			auto posCurBee = sprBee[i].getPosition();
			auto bndCurBee = sprBee[i].getGlobalBounds();

			bool dirRight;
			bool respawn = false;
			if (spdBee[i].x > 0)
			{
				dirRight = true;
			}
			else
			{
				dirRight = false;
			}
			posCurBee.x += spdBee[i].x * deltaTime;
			posCurBee.y += spdBee[i].y * deltaTime * (std::sin(angBee[i]+sumTime * acos(-1)));


			sprBee[i].setPosition(posCurBee);

			if (posCurBee.x + bndCurBee.width<0
				|| posCurBee.x - bndCurBee.width>sizeWindow.x)
			{
				angBee[i] = (rand() % 315) / 100.f;
				dirRight = rand() % 2;
				if (dirRight)
				{
					sprBee[i].setScale(-(rand() % 41 + 80) / 100.f, (rand() % 41 + 80) / 100.f);
					bndCurBee = sprBee[i].getGlobalBounds();
					sprBee[i].setPosition(0, 600 + rand() % bndGround);
					spdBee[i].x = rand() % 81 + 40.f;
					spdBee[i].y = rand() % 81 + 40.f;
				}
				else
				{
					sprBee[i].setScale((rand() % 41 + 80) / 100.f, (rand() % 41 + 80) / 100.f);
					bndCurBee = sprBee[i].getGlobalBounds();
					sprBee[i].setPosition(sizeWindow.x, 600 + rand() % bndGround);
					spdBee[i].x = -(rand() % 81 + 40.f);
					spdBee[i].y = rand() % 81 + 40.f;
				}
			}

		}

		// 업데이트

		window.clear();

		// 그리기

		window.draw(sprBackground);
		for (int i = 0;i < cntCloud;++i)
		{
			window.draw(sprCloud[i]);
		}
		window.draw(sprTree);
		for (int i = 0;i < cntBee;++i)
		{
			window.draw(sprBee[i]);
		}

		window.display();
	}
	delete[] sprBee;
	delete[] spdBee;
	delete[] sprCloud;
	delete[] spdCloud;
	return 0;
}