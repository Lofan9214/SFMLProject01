#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <iostream>

//지평선 높이 650 언저리

int main()
{
	srand(time(NULL));

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

	sf::Sprite spriteBackground;
	sf::Sprite spriteCloud[3];
	sf::Sprite spriteBee;
	sf::Sprite spriteTree;
	sf::Vector2u sizeWindow = window.getSize();
	sf::Vector2u posCenter = (sf::Vector2u)(sizeWindow / 2u);
	sf::Vector2u sizeTree;
	sf::Vector2f posBackground(sizeWindow.x / 2, sizeWindow.y / 2);
	sf::Vector2f posBee(0, 900); // v = d / t
	sf::Vector2f spdBee(100.f, 100.f); // v = d / t

	sf::Vector2f posCloud[3]{};
	sf::Vector2f scaCloud[3]{};
	sf::Vector2f spdCloud[3];
	bool rightcloud[3];

	texCloud.loadFromFile("graphics/cloud.png");
	texBackground.loadFromFile("graphics/background.png");
	texBee.loadFromFile("graphics/bee.png");
	texTree.loadFromFile("graphics/tree.png");

	sizeTree = texTree.getSize();
	spriteBackground.setTexture(texBackground);
	spriteBee.setTexture(texBee);
	spriteBee.scale(-1.f, 1.f);
	spriteTree.setTexture(texTree);
	for (int i = 0;i < _countof(spriteCloud);++i)
	{
		spriteCloud[i].setTexture(texCloud);
	}

	spriteBee.setPosition(posBee);
	spriteBackground.setPosition(posBackground);
	spriteTree.setPosition(posCenter.x, 0);
	for (int i = 0;i < _countof(spriteCloud);++i)
	{
		rightcloud[i] = rand() % 2;
		scaCloud[i].x = rand() % 100 / 100.f + 1;
		scaCloud[i].y = rand() % 100 / 100.f + 1;
		if (rightcloud[i])
		{
			scaCloud[i].x = -scaCloud[i].x;
		}

		spriteCloud[i].scale(scaCloud[i]);

		float hei = spriteCloud[i].getGlobalBounds().height;
		float wid = spriteCloud[i].getGlobalBounds().width;

		posCloud[i].y = rand() % 400 - hei;
		if (rightcloud[i])
		{
			posCloud[i].x = 0;
		}
		else
		{
			posCloud[i].x = sizeWindow.x;
		}
		spriteCloud[i].setPosition(posCloud[i]);
	}
	spriteTree.setOrigin(texTree.getSize().x / 2, 0);
	spriteBackground.setOrigin(posCenter.x, posCenter.y);

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
			default:
				break;
			}
		}

		for (int i = 0; i < _countof(posCloud);++i)
		{
			spdCloud[i].x = rand() % 100 + 50;
			spdCloud[i].y = rand() % 400 - 200;
			float hei = spriteCloud[i].getGlobalBounds().height;
			float wid = spriteCloud[i].getGlobalBounds().width;
			if ((rightcloud[i] && (posCloud[i].x - wid > sizeWindow.x)) || (!rightcloud[i] && (posCloud[i].x + wid < 0)))
			{
				rightcloud[i] = rand() % 2;
				scaCloud[i].x = rand() % 100 / 100.f + 1;
				scaCloud[i].y = rand() % 100 / 100.f + 1;
				if (rightcloud[i])
				{
					scaCloud[i].x = -scaCloud[i].x;
				}
				spriteCloud[i].setScale(scaCloud[i]);

				posCloud[i].y = rand() % 400 - hei;
				if (rightcloud[i])
				{
					posCloud[i].x = 0;
				}
				else
				{
					posCloud[i].x = sizeWindow.x;
				}
			}

			if (rightcloud[i])
			{
				posCloud[i].x += abs(spdCloud[i].x * deltaTime);
			}
			else
			{
				posCloud[i].x -= abs(spdCloud[i].x * deltaTime);
			}

			if (posCloud[i].y + spriteCloud[i].getGlobalBounds().height/2 < 0)
			{
				posCloud[i].y += abs(spdCloud[i].y * deltaTime);
			}
			else if (posCloud[i].y - spriteCloud[i].getGlobalBounds().height > 400)
			{
				posCloud[i].y -= abs(spdCloud[i].y * deltaTime);
			}
			else
			{
				posCloud[i].y += spdCloud[i].y * deltaTime;
			}

			spriteCloud[i].setPosition(posCloud[i]);
		}



		posBee = spriteBee.getPosition();
		posBee.x += spdBee.x * deltaTime;
		if ((posBee.x - spriteBee.getGlobalBounds().width > sizeWindow.x) || (posBee.x + spriteBee.getGlobalBounds().width < 0))
		{
			auto scabee = spriteBee.getScale();
			spriteBee.setScale(-scabee.x, scabee.y);
			spdBee.x = -spdBee.x;
		}
		posBee.y += spdBee.y * deltaTime * (std::sin(sumTime * acos(-1)));

		spriteBee.setPosition(posBee);

		// 업데이트

		window.clear();

		// 그리기

		window.draw(spriteBackground);
		for (int i = 0;i < _countof(spriteCloud);++i)
		{
			window.draw(spriteCloud[i]);
		}
		window.draw(spriteTree);
		window.draw(spriteBee);

		window.display();
	}

	return 0;
}