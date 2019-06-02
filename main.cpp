#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
using namespace sf;
using namespace std;

const int M = 37;
const int N = 14;

const int random (int N) {return rand()%7;}

int field[M][N] = {0};
int tempfield[M][N] = {0};

struct Point
{int x, y;} a[4], b[4], tempa[4], tempb[4];

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


int tempfigures[7][4] = 
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
bool tempcheck()
{
	for (int i = 0; i < 4; i++)
		if (tempa[i].x < 0 || tempa[i].x >= N || tempa[i].y >= M) return 0;
		else if(field[tempa[i].y][tempa[i].x])return 0;
	return 1;
}



int main()
{
	srand(time(NULL));
	Texture tiles;
	Texture background;
	Texture frame;
	Texture tilesnext;

	frame.loadFromFile("images/frame5.png");
	background.loadFromFile("images/backgraund.jpg");
	tiles.loadFromFile("images/tiles.png");
	tilesnext.loadFromFile("images/tiles.png");

	Font font;
	font.loadFromFile("CyrilicOld.TTF");
	Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый

	Sprite c(frame);

	Sprite z(background);

	Sprite s(tiles);

	Sprite tn(tilesnext);
	// s.setTextureRect(IntRect(0,0,18,18));

	int dx = 0, colorNum = 1, dy = 0;
	int playerscore = 0;
	bool rotate = 0;
	float timer = 0, delay = 0.3;

	Clock clock;

	RenderWindow window(VideoMode(305, 700), "The TETRIS");


	while (window.isOpen() && check())

	{
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
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;	
				// s.setOrigin(-50, 50);


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
					// window.draw(s);

				}

			}
			timer = 0;
		}

		//
		//проверка линий на заполнение
		//
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
				if (count == N) playerscore += 100;
			}

//
			//
			//
			// Вывод счета игрока через стандартную библиотеку С++
			//
			//

		ostringstream playerScoreString;// объявили переменную
		playerScoreString << playerscore;//занесли в нее число очков, то есть формируем строку
		text.setString("Score:" + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		text.setPosition(143, 30);
		window.draw(text);





































////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








			
				// colorNum = 1 + rand()%7; //определение переменной сolorNumв теле цикла вызывает мерцание тетрамин




				int n = rand()%7;
				int shetrand = rand()%7; //прописать счетчик для рандомных чисел для отображения следующей тетраминки
				
				if (a[0].x == 0 && a[0].y == 0)
				{
					for (int i = 0; i < 4; i++)
					{
						// shetrand = n;
						a[i].x = figures[n][i] % 2;
						a[i].y = figures[n][i] / 2;

					}
				}		
				if (tempa[0].x == 0 && tempa[0].y == 0)
				{
					for (int i = 0; i < 4; i++)
					{
						// shetrand = n;
						// a[i].x = figures[n][i] % 2;
						// a[i].y = figures[n][i] / 2;
						tempa[i].x = a[i].x;
						tempa[i].y = a[i].y;
					}
				}	
				


	for (int i = 0; i < 4; i++)
		{
			tempb[i] = tempa[i];
			tempa[i].x += dx;
		}
		if (!tempcheck())for (int i = 0; i < 4; i++) {tempa[i] = tempb[i];}

		// Rotate

		if (rotate == true)
		{
			Point p = tempa[1]; // центр вращения фигуры
			for (int i = 0; i < 4; i++)// реализация поворота
			{
				int x = tempa[i].y - p.y;
				int y = tempa[i].x - p.x;
				tempa[i].x = p.x - x;
				tempa[i].y = p.y + y;
			}
			if (!tempcheck()) for (int i = 0; i < 4; i++) tempa[i] = tempb[i];
		}

		// Tick

		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) 
				{
					tempb[i] = tempa[i];
					tempa[i].y += 1;
				}
			if (!tempcheck())
			{
				for (int i = 0; i < 4; i++)	tempfield[tempb[i].y][tempb[i].x] = colorNum;
				
				colorNum = 1 + rand()%7;
				for (int i = 0; i < 4; i++)
				{
					tempa[i].x = figures[n][i] % 2;
					tempa[i].y = figures[n][i] / 2;
					// window.draw(s);

				}

			}
			timer = 0;
		}



		// int shetrand = rand()%7; //прописать счетчик для рандомных чисел для отображения следующей тетраминки
		// shetrand = n;
		// if (a[4].x == 0 && a[4].y == 0)
		// for (int i = 0; i < 4; i++)
		// {
		// 	a[i].x = figures[n][i] % 2;
		// 	a[i].y = figures[n][i] / 2;
		// 	tempa[i].x = a[i].x;
		// 	tempa[i].y = a[i].y;
		// 	a[i].x = tempfigures[shetrand][i] % 2;
		// 	a[i].y = tempfigures[shetrand][i] / 2;
		// 	tempb[i].x = a[i].x;
		// 	tempb[i].y = a[i].y;
		// 	// window.draw(s);
		// 	// s.setOrigin(-50, -50);
		// 	// s.setPosition(10,10);
		// }




//Следующая тетрамина
			//
			// отрисовка следующей тетрамины
			//
// for (int i = 0, j = 0; i < 4; i++)
// {
// 			// for (int i = 0; i < M; i++)
// 			// for (int j = 0; j < N; j++)
// 			// {
// 				// if (field[i][j] == 0) continue;
// 				// s.setTextureRect(IntRect(field[i][j]*0, 0, 18, 18));//изменение цвета спрайта
// 				// s.setPosition(j * 18, i * 18);//модулирование позиции на нужное место

// 				s.setPosition(tempa[i].x * 10, tempa[i].y * 10);//обнуление позиции на нужное место
// 				s.move(28,31);//выравнивание спрайтов 
// 				// s.setPosition(j, i);//модулирование позиции на нужное место
// 				// viewtetraminno(n);
// 				// s.setPosition(4 , 4 );//задаем позицию текста, центр камеры
// 				// window.draw(s);//рисую этот текст 
// 				// window.draw(tn);
// 			// }




// 		// viewtetraminno(n);
// 		s.setPosition(tempb[i].x * 18, tempb[i].y * 18);//обнуление позиции на нужное место // рисует фигуру но только один раз // показывает следующую тетравину только один раз
// 		window.draw(s);
// }

		window.display();
		dx = 0;
		rotate = 0;
		delay = 0.5;
		if (playerscore >= 1000)// level 2
		{
			delay = 0.3;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.06;
		}

		if (playerscore >= 5000) // level 3
		{
			colorNum = 1 + rand()%7; //определение переменной сolorNumв теле цикла вызывает мерцание тетрамин
			delay = 0.1;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.01;
		}

		if (playerscore >= 10000 && playerscore <= 11000) // level 4
		{
			delay = 0.05;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.005;
		}
		if (playerscore >= 12000 && playerscore <= 15000) //bonus level 5
		{
			delay = 0.1;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.01;
		}


		// window.clear(Color::White);
		// window.draw(z);
		// window.draw(c);

			// //
			// // отрисовка следующей тетрамины
			// //
			// for (int i = 0; i < M; i++)
			// for (int j = 0; j < N; j++)
			// {
			// 	if (tempfield[i][j] == 0) continue;
			// 	tn.setTextureRect(IntRect(tempfield[i][j]*0, 0, 18, 18));//изменение цвета спрайта
			// 	tn.setPosition(j, i);//модулирование позиции на нужное место
			// 	// viewtetraminno(n);
			// 	tn.setPosition(4 , 4 );//задаем позицию текста, центр камеры
			// 	window.draw(tn);//рисую этот текст 
			// 	// window.draw(tn);
			// }









//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


























































		window.clear(Color::White);
		window.draw(z);
		window.draw(c);






		//
		// Отрисовка тетрамин лежащих на дне колодца
		//
						// s.setPosition(10,10);

			for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				// s.setOrigin(-50, 50);
				s.setTextureRect(IntRect(field[i][j]*0, 0, 18, 18));//изменение цвета спрайта
				s.setPosition(j * 18, i * 18);//модулирование позиции на нужное место
				window.draw(s);
				// s.setPosition(105, 70);
				s.move(100,100);//выравнивание спрайтов 
				s.setPosition(j * 18, i * 18);//модулирование позиции на нужное место

				s.setTextureRect(IntRect(tempfield[i][j]*0, 0, 18, 18));//изменение цвета спрайта

				s.setTextureRect(IntRect(colorNum*18,0,18,18)); //изменение цвета спрайта

				s.setPosition(tempa[i].x * 18, tempa[i].y * 18);//обнуление позиции на нужное место
				// s.setOrigin(-50, -50);
				window.draw(s);
			}

			//
			//	Отрисовка падающих тетрамин
			//
		for (int i = 0; i < 4; i++)
		{
			// s.setOrigin(-50, 50);
			s.setTextureRect(IntRect(colorNum*18,0,18,18)); //изменение цвета спрайта
			s.setPosition(a[i].x * 18, a[i].y * 18);//обнуление позиции на нужное место
			// s.setOrigin(0,0);
			window.draw(s);
			// s.setPosition((a[1].x * 18) + 50, (a[1].y * 18) + 50);//обнуление позиции на нужное место
			// s.setPosition(105, 70);
			s.move(0,400);//выравнивание спрайтов 
			// s.setPosition(105, 70);
			s.setTextureRect(IntRect(colorNum*18,0,18,18)); //изменение цвета спрайта

			s.setPosition(tempa[i].x * 18, tempa[i].y * 18);//обнуление позиции на нужное место
			window.draw(s);
			// s.setPosition(50, 70);
			// window.draw(s);
		}
		// Вывод на экран
		// window.display();

	}
int n;
	cout << "GAME OVER!" << endl;
	cout << "You score: " << playerscore << endl;
	// system("PAUSE");
	std::cin >> n;

	return 0;
}