#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
using namespace sf;
using namespace std;

const int M = 30;
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








void Play(RenderWindow & window)
{

	Texture tiles;
	Texture background;
	Texture frame;
	Texture tilesnext;

	frame.loadFromFile("images/frame9ps.png");
	background.loadFromFile("images/backgraund.jpg");
	tiles.loadFromFile("images/tiles.png");

	Font font;
	font.loadFromFile("CyrilicOld.TTF");
	Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	// text.setColor();//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый

	Sprite c(frame);

	Sprite z(background);

	Sprite s(tiles);

	Sprite tn(tilesnext);
	// s.setTextureRect(IntRect(0,0,18,18));
	// menu(window);//вызов меню
	int dx = 5, colorNum = 1, dy = 0;
	int playerscore = 0;
	int tetrominofull = 0;
	bool rotate = 0;
	float timer = 0, delay = 0.3;
	Clock clock;
		
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
				tetrominofull += count;
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
		text.setPosition(115, 30);
		window.draw(text);







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


				int n = rand()%7;
				int shetrand = rand()%7; //прописать счетчик для рандомных чисел для отображения следующей тетраминки
				if (tempb[0].x == 0 && tempb[0].y == 0)
				{
					for (int i = 0; i < 4; i++)
					{
						// shetrand = n;
						a[i].x = figures[n][i] % 2;
						a[i].y = figures[n][i] / 2;
						tempa[i].x = a[i].x;
						tempa[i].y = a[i].y;
					}
					n = shetrand;
					for (int i = 0; i < 4; i++)
					{
						a[i].x = figures[shetrand][i] % 2;
						a[i].y = figures[shetrand][i] / 2;
						tempb[i].x = a[i].x;
						tempb[i].y = a[i].x;
					}
				}

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

		if (playerscore >= 10000 && playerscore <= 12000) // level 4
		{
			delay = 0.05;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.005;
		}
		if (playerscore >= 12000 && playerscore <= 15000) //bonus level 5
		{
			delay = 0.1;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.01;
		}


		window.clear(Color::White);
		window.draw(z);
		window.draw(c);










//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








		//
		// Отрисовка тетрамин лежащих на дне колодца
		//
						// s.setPosition(10,10);

			for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				s.setTextureRect(IntRect(field[i][j]*0, 0, 18, 18));//изменение цвета спрайта
				s.setPosition(j * 18, i * 18);//модулирование позиции на нужное место
				s.move(28,31);//выравнивание спрайтов 
				// s.setOrigin(-50, -50);
				window.draw(s);
			}

			//
			//	Отрисовка падающих тетрамин
			//
		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum*18,0,18,18)); //изменение цвета спрайта
			s.setPosition(a[i].x * 18, a[i].y * 18);//обнуление позиции на нужное место
			s.move(28,31);//выравнивание спрайтов 
			window.draw(s);
		}
	}


}



void menu(RenderWindow & window) {

	Texture play;
	Texture settings; 
	Texture license; 
	Texture shotdown; 
	Texture menuBackground;
	Texture viewlevel;

	play.loadFromFile("images/buttonmenu.png");
	settings.loadFromFile("images/buttonmenu.png");
	license.loadFromFile("images/buttonmenu.png");
	shotdown.loadFromFile("images/buttonmenu.png");
	menuBackground.loadFromFile("images/frame9ps.png");
	viewlevel.loadFromFile("images/metatiles.png");

	Font fontplay;
	fontplay.loadFromFile("CyrilicOld.TTF");
	Text textplay("", fontplay, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	Text textsettings("", fontplay, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	Text textlicense("", fontplay, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	// textplay.setColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	textplay.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	textsettings.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	textlicense.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый



	Sprite p(play); 
	Sprite sett(settings); 
	Sprite lic(license); 
	Sprite shot(shotdown); 
	Sprite menuBg(menuBackground);
	Sprite lev(viewlevel);
	
	bool isMenu = 1;
	int menuNum = 0;

		// ostringstream play;// объявили переменную
		// playerScoreString << playerscore;//занесли в нее число очков, то есть формируем строку
		textplay.setString("Play");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		textsettings.setString("Settings");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		textlicense.setString("License");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		textplay.setPosition(140, 187);
		textsettings.setPosition(120, 257);
		textlicense.setPosition(130, 327);
		window.draw(textplay);


	p.setPosition(5, 30);
	sett.setPosition(5, 100);
	lic.setPosition(5, 170);
	menuBg.setPosition(0, 0);
	lev.setPosition(5, 50);
 
	//////////////////MENU///////////////////
	while (isMenu)
	{
		p.setColor(Color::White);
		sett.setColor(Color::White);
		lic.setColor(Color::White);
		menuNum = 0;
		window.clear();
 
		if (IntRect(0, 200, 500, 20).contains(Mouse::getPosition(window))) {p.setColor(Color::Black); menuNum = 1; }
		if (IntRect(0, 270, 500, 20).contains(Mouse::getPosition(window))) {sett.setColor(Color::Black); menuNum = 2; }
		if (IntRect(0, 340, 500, 20).contains(Mouse::getPosition(window))) {lic.setColor(Color::Black); menuNum = 3; }
 
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) { 
				// window.clear();
				window.draw(lev); 
				window.display(); 
				Play(window); 
				isMenu = false; 
				// while (!Keyboard::isKeyPressed(Keyboard::Escape));
			}
			if (menuNum == 2) { 
				window.close(); 
				isMenu = false; 
			}
			if (menuNum == 3)  { 
				window.close(); 
				isMenu = false; 
			}
			if (menuNum == 4)  { 
				window.close(); 
				isMenu = false; 
			}
 
		}
 
		window.draw(menuBg);
		window.draw(p);
		window.draw(sett);
		window.draw(lic);
		window.draw(textplay);
		window.draw(textsettings);
		window.draw(textlicense);
		
		window.display();
	}
}






int main()
{
	srand(time(NULL));

	RenderWindow window(VideoMode(320, 575), "The TETRIS");
	menu(window);
	// Play(window);
	int n;
	cout << "GAME OVER!" << endl;
	// cout << "You score: " << playerscore << endl;
	// system("PAUSE");
	std::cin >> n;
	return 0;
}