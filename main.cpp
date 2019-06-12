#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace sf;
using namespace std;

const int M = 30;
const int N = 14;
int playerscore = 0;
int colorNum;
float timer;
float delay;
int dx;

const int random (int N) {return rand()%7;}

int gamespace[M][N] = {0};

struct Point
	{
		int x, y;
	} tetrominofirst[4], tetrominosecond[4], tempa[4], tempb[4];

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

bool check()// границы
{
	for (int i = 0; i < 4; i++)
		if (tetrominofirst[i].x < 0 || tetrominofirst[i].x >= N || tetrominofirst[i].y >= M) return 0;
		else if(gamespace[tetrominofirst[i].y][tetrominofirst[i].x])return 0;
	return 1;
}

void Record(RenderWindow & window ,int playerscore)
{
		char buf[1];
		string buff;
		buff.resize(30);
		int Record = 0;
		fstream recordlist;
		ostringstream playerScoreRecord;
		Font fontrecord;
		fontrecord.loadFromFile("CyrilicOld.TTF");
		Text record("", fontrecord, 20);
		record.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст
		recordlist.open("Recordlist.txt", ios::in);
		recordlist >> buf[0];
		recordlist >> buff;
		recordlist.close();
		recordlist.open("Recordlist.txt");
		if (buf[0] < playerscore)
		{
			recordlist.close();
			recordlist.open("Recordlist.txt", ios::out);
			recordlist << playerscore;
			playerScoreRecord << playerscore;
			recordlist.close();
		}
		else {playerScoreRecord << buff; recordlist.close();}
		
		record.setString("Record:" + playerScoreRecord.str());
		record.setPosition(120,320);
		window.draw(record);
}

void Draw(RenderWindow & window, Sprite s)
{
		//
		// Отрисовка тетрамин лежащих на дне колодца
		//
			for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (gamespace[i][j] == 0) continue;
				s.setTextureRect(IntRect(gamespace[i][j]*0, 0, 18, 18));
				s.setPosition(j * 18, i * 18);//модулирование позиции на нужное место
				s.move(28,31);//выравнивание спрайтов 
				window.draw(s);
			}
			//
			//	Отрисовка падающих тетрамин
			//
		for (int i = 0; i < 4; i++)
		{	
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));//задание цвета
			s.setPosition(tetrominofirst[i].x * 18, tetrominofirst[i].y * 18);//нормализация движения
			s.move(28,31);//выравнивание спрайтов 
			window.draw(s);
		}
}


void Tick(RenderWindow & window, Sprite s)
{
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) 
				{
					tetrominosecond[i] = tetrominofirst[i];
					tetrominofirst[i].y += 1;
				}
			if (!check())
			{
				for (int i = 0; i < 4; i++)	gamespace[tetrominosecond[i].y][tetrominosecond[i].x] = colorNum;
				
				colorNum = 1 + rand()%7;
				int n = rand()%7;
				for (int i = 0; i < 4; i++)
				{
					tetrominofirst[i].x = figures[n][i] % 2;
					tetrominofirst[i].y = figures[n][i] / 2;
				}
			}
			timer = 0;
		}
}

void checklines(int scoreplus)//Проверка линий на заполнение
{
		int flag = 0;
			int k = M - 1;
			for (int i = M - 1; i > 0; i--)
			{
				int count = 0;
				for (int j = 0; j < N; j++)
				{
					if (gamespace[i][j]) count++;
					gamespace[k][j] = gamespace[i][j];
				}
				if (count < N) k--;
				if (count == N) playerscore += scoreplus;
			}
}


void Play(RenderWindow & window)
{
	Texture tiles;
	Texture frame;
	Texture tilesnext;
	Font font;

	frame.loadFromFile("images/frame9ps.png");
	tiles.loadFromFile("images/tilesnew.png");
	font.loadFromFile("CyrilicOld.TTF");

	Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	Text level("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)

	text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	level.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый

	Sprite c(frame);

	Sprite s(tiles);

	Sprite tn(tilesnext);

	colorNum = 1;
	float fastplayerscore = 0;
	int shetlevel = 1;
	int tetrominofull = 0;
	bool rotate = 0;
	dx = 5;
	timer = 0, 
	delay = 0.3;
	int scoreplus = 100;

	Clock clock;
		
	while (window.isOpen() && check())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		window.setVerticalSyncEnabled(true);
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

		// Движение

		for (int i = 0; i < 4; i++)
		{
			tetrominosecond[i] = tetrominofirst[i];
			tetrominofirst[i].x += dx;
		}
		if (!check())for (int i = 0; i < 4; i++) {tetrominofirst[i] = tetrominosecond[i];}//запрет выходв за левую и правую границы

		// Вращение

		if (rotate == true)
		{
			Point p = tetrominofirst[1]; // центр вращения фигуры
			for (int i = 0; i < 4; i++)// реализация поворота
			{
				int x = tetrominofirst[i].y - p.y;
				int y = tetrominofirst[i].x - p.x;
				tetrominofirst[i].x = p.x - x;
				tetrominofirst[i].y = p.y + y;
			}
			if (!check()) for (int i = 0; i < 4; i++) tetrominofirst[i] = tetrominosecond[i];//запрет на выход за границу колодца при вращении
		}

		Tick(window, s);

		checklines(scoreplus);
			//
			// Вывод счета игрока через стандартную библиотеку С++
			//
			//

		ostringstream playerScoreString;// объявили переменную
		playerScoreString << playerscore;//занесли в нее число очков, то есть формируем строку
		ostringstream playerLevelString;// объявили переменную
		playerLevelString << shetlevel;//занесли в нее число очков, то есть формируем строку
		text.setString("Score:" + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		text.setPosition(115, 18);
		window.draw(text);
		level.setString("Level " + playerLevelString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		level.setPosition(118, 42);
		window.draw(level);

//////////////////////////////////////////////////////////////////////////////////////////////////////


				int n = rand()%7;
				int shetrand = rand()%7; //прописать счетчик для рандомных чисел для отображения следующей тетраминки
				if (tempb[0].x == 0 && tempb[0].y == 0)
				{
					for (int i = 0; i < 4; i++)
					{
						// shetrand = n;
						tetrominofirst[i].x = figures[n][i] % 2;
						tetrominofirst[i].y = figures[n][i] / 2;
						tempa[i].x = tetrominofirst[i].x;
						tempa[i].y = tetrominofirst[i].y;
					}
					n = shetrand;
					for (int i = 0; i < 4; i++)
					{
						tetrominofirst[i].x = figures[shetrand][i] % 2;
						tetrominofirst[i].y = figures[shetrand][i] / 2;
						tempb[i].x = tetrominofirst[i].x;
						tempb[i].y = tetrominofirst[i].x;
					}
				}

		window.display();
		dx = 0;
		rotate = 0;
		delay = 0.5;

		//система уровней
		if (playerscore >= 1000)// level 2
		{
			delay = 0.4;
			if (Keyboard::isKeyPressed(Keyboard::Down)) {delay = 0.04;}
			shetlevel = 2;
			scoreplus = 120;
		}

		if (playerscore >= 2100) // level 3
		{
			delay = 0.3;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.03;
			shetlevel = 3;
			scoreplus = 150;
		}

		if (playerscore >= 3500) // level 4
		{
			shetlevel = 4;
			delay = 0.2;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.02;
			scoreplus = 200;
		}
		if (playerscore >= 5000) //bonus level 5
		{
			shetlevel = 5;
			delay = 0.1;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.01;
			scoreplus = 250;
		}
		if (playerscore >= 6000) //bonus level 6
		{
			shetlevel = 6;
			colorNum = 1 + rand()%7; //определение переменной сolorNumв в теле цикла вызывает мерцание тетрамин
			delay = 0.09;
			level.setFillColor(Color(255,255,128,128));
			level.setOutlineColor(Color::Red);
			level.setString("GOLD LEVEL");
			window.draw(level);
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.009;
			scoreplus = 1000;
		}
		if (playerscore >= 8500) //bonus level 7
		{
			shetlevel = 7;
			delay = 0.08;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.01;
			scoreplus = 300;
		}
		if (playerscore >= 10000) //bonus level 8
		{
			shetlevel = 8;
			delay = 0.07;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.009;
			scoreplus = 400;
		}
		if (playerscore >= 12000) //bonus level 9
		{
			shetlevel = 9;
			delay = 0.06;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.008;
			scoreplus = 500;
		}		
		if (playerscore >= 15000) //bonus level 10
		{
			shetlevel = 10;
			delay = 0.05;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.007;
			scoreplus = 600;
		}
		if (playerscore >= 17000) //bonus level 11
		{
			shetlevel = 11;
			delay = 0.04;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.006;
			scoreplus = 700;
		}
		if (playerscore >= 20000) //bonus level 12
		{
			shetlevel = 12;
			delay = 0.03;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.005;
			scoreplus = 800;
		}
		if (playerscore >= 25000) //bonus level 13
		{
			shetlevel = 13;
			delay = 0.02;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.004;
			scoreplus = 900;
		}
		if (playerscore >= 30000) //bonus level 14
		{
			shetlevel = 14;
			delay = 0.01;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.003;
			scoreplus = 1000;
		}				
		window.clear(Color::White);
		window.draw(c);

/////////////////////////////////////////////////////////////////////////////////////

		Draw(window, s);

		if (window.isOpen() && !check()) //вывод окончания игры и вывод очков игрока
		{
			Font fontgameover;
			fontgameover.loadFromFile("CyrilicOld.TTF");
			Text gameover("", fontgameover, 20);
			Text score("", fontgameover, 20);
			Text record("", fontgameover, 20);
			score.setStyle(sf::Text::Bold | sf::Text::Underlined);
			ostringstream playerScoreString;// объявили переменную
			playerScoreString << playerscore;//занесли в нее число очков, то есть формируем строку
			score.setString("Score:" + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
			gameover.setStyle(sf::Text::Bold | sf::Text::Underlined);
			gameover.setString("GAME OVER");
			gameover.setPosition(90,250);
			score.setPosition(120,285);
			window.draw(score);
			window.draw(gameover);
			Record(window, playerscore);
			window.display();
		}
	}
	int n;
	cout << "GAME OVER!" << endl;
	cout << "You score: " << playerscore << endl;
	std::cin >> n;
}

void menu(RenderWindow & window) {

	Texture play;
	Texture settings; 
	Texture license; 
	Texture shotdown; 
	Texture menuBackground;
	Texture viewlevel;

	play.loadFromFile("images/buttonmenu.png");
	shotdown.loadFromFile("images/buttonmenu.png");
	menuBackground.loadFromFile("images/frame9ps.png");

	Font fontplay;
	fontplay.loadFromFile("CyrilicOld.TTF");
	Text textplay("", fontplay, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	Text textlicense("", fontplay, 20);
	Text textshotdown("", fontplay, 20);
	textplay.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	textlicense.setStyle(sf::Text::Bold | sf::Text::Underlined);
	textshotdown.setStyle(sf::Text::Bold | sf::Text::Underlined);



	Sprite p(play); 
	Sprite shot(shotdown); 
	Sprite menuBg(menuBackground);
	Sprite lev(viewlevel);
	
	bool isMenu = 1;
	int menuNum = 0;

		textplay.setString("Play");//задаем строку тексту и вызываем строку методом .str() 
		textshotdown.setString("Exit");
		textplay.setPosition(140, 187);
		textlicense.setPosition(120, 257);
		textshotdown.setPosition(140, 327);
		window.draw(textplay);


	p.setPosition(5, 30);
	shot.setPosition(5, 170);
	menuBg.setPosition(0, 0);
	lev.setPosition(5, 50);
 
	//////////////////MENU///////////////////
	while (isMenu)
	{
		p.setColor(Color::White);
		shot.setColor(Color::White);
		menuNum = 0;
		window.clear();
 
		if (IntRect(0, 200, 500, 20).contains(Mouse::getPosition(window))) {p.setColor(Color::Black); menuNum = 1; }
		if (IntRect(0, 340, 500, 20).contains(Mouse::getPosition(window))) {shot.setColor(Color::Black); menuNum = 3; }
 
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) { 
				window.draw(lev); 
				window.display(); 
				Play(window); 
				isMenu = false; 
			}
			if (menuNum == 3)  { 
				window.close(); 
				isMenu = false; 
			}
		}
 
		window.draw(menuBg);
		window.draw(p);
		window.draw(shot);
		window.draw(textplay);
		window.draw(textlicense);
		window.draw(textshotdown);
		
		window.display();
	}
}


int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(320, 575), "The TETRIS");
	menu(window);
	return 0;
}