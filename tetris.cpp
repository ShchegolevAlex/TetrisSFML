#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace sf;

const int M = 24;
const int N = 10;

const int random (int N) {return rand()%7;}

int field[M][N] = {0};

struct Point
{int x, y;} a[4], b[4];

int figures[7][4] = 
{
	1,3,5,7,
	2,4,5,7,
	3,5,4,6,
	3,5,4,7,
	2,3,5,7,
	3,5,7,6,
	2,3,4,5,
};

bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
		else if(field[a[i].y][a[i].x])return 0;
	return 1;
}




int main()
{
	srand(time(NULL));
	Texture tiles;
	Texture background;
	Texture frame;
	frame.loadFromFile("images/frame4.png");
	background.loadFromFile("images/backgraund.jpg");
	tiles.loadFromFile("images/tiles.png");

	Sprite c(frame);

	Sprite z(background);

	Sprite s(tiles);
	// s.setTextureRect(IntRect(0,0,18,18));

	int dx = 5, colorNum = 1;
	bool rotate = 0;
	float timer = 0, delay = 0.3;

	Clock clock;

	RenderWindow window(VideoMode(320, 480), "The TETRIS");

	s.setOrigin(15, 5);


	while (window.isOpen() && check())
	{
		// window.draw(z);
		// dx = 5;
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up) rotate = true;
				else if (e.key.code == Keyboard::Left) dx -= 1;
				else if (e.key.code == Keyboard::Right) dx += 1;
				// else if (e.key.code == Keyboard::) window.close();
		}




		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;	

		// Move

		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		if (!check())for (int i = 0; i < 4; i++) {a[i] = b[i];}

		// Rotate

		if (rotate == true)
		{
			Point p = a[1]; // центр вращения фигуры
			for (int i = 0; i < 4; i++)// реализация поворота
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
		}

		// Tick

		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) 
				{
					b[i] = a[i];
					a[i].y += 1;
				}
			if (!check())
			{
				for (int i = 0; i < 4; i++)	field[b[i].y][b[i].x] = colorNum;
				
				colorNum = 1 + rand()%7;
				int n = rand()%7;
				// if (a[0].x == 0)
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
					window.draw(s);

				}

			}
			timer = 0;
		}

		//check lines
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) k--;

		}

		// Draw //

		// int n = 2;
		int n = rand()%7;

		if (a[4].x == 0 && a[4].y == 0)
		for (int i = 0; i < 4; i++)
		{
			a[i].x = figures[n][i] % 2;
			a[i].y = figures[n][i] / 2;
			window.draw(s);
		}
		dx = 0;
		// dx = 0;

		rotate = 0;
		delay = 0.3;

		window.clear(Color::White);
		window.draw(z);
		window.draw(c);
			for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				s.setTextureRect(IntRect(field[i][j]*0, 0, 18, 18));//изменение цвета спрайта
				s.setPosition(j * 18, i * 18);//обнуление позиции на нужное место
				s.move(28,31);//выравнивание спрайтов 
				window.draw(s);
			}


		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum*18,0,18,18)); //изменение цвета спрайта
			s.setPosition(a[i].x * 18, a[i].y * 18);//обнуление позиции на нужное место
			s.move(28,31);//выравнивание спрайтов 
			window.draw(s);
		}

		window.display();
	}
int n;
	std::cout << "GAME OVER!";
	// system("PAUSE");
	std::cin >> n;

	return 0;
}