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



void Record(int playerscore)
{
		char buff[1];
		int a = 0;
		fstream recordlist;
		recordlist.open("Recordlist.txt", ios::app);
		recordlist >> buff[0];
		a = buff[0];
		recordlist.close();
		recordlist.open("Recordlist.txt");
		if (buff[0] < playerscore)
		{
			recordlist.close();
			recordlist.open("Recordlist.txt", ios::out);
			recordlist << playerscore;
			recordlist.close();
		}
		else {recordlist.close();}
}




void Play(RenderWindow & window)
{

	Texture tiles;
	Texture background;
	Texture frame;
	Texture tilesnext;

	frame.loadFromFile("images/frame9ps.png");
	background.loadFromFile("images/backgraund.jpg");
	tiles.loadFromFile("images/tilesnew.png");

	Font font;
	font.loadFromFile("CyrilicOld.TTF");
	Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	Text level("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)

	// text.setColor();//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	level.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый

	Sprite c(frame);

	Sprite z(background);

	Sprite s(tiles);

	Sprite tn(tilesnext);
	// s.setTextureRect(IntRect(0,0,18,18));
	// menu(window);//вызов меню
	int dx = 5, colorNum = 1, dy = 0;
	// int playerscore = 0;
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
			level.setString("Level 1");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
			level.setPosition(160, 450);
			window.draw(level);
			delay = 0.4;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.04;
		}

		if (playerscore >= 2000) // level 3
		{
			colorNum = 1 + rand()%7; //определение переменной сolorNumв теле цикла вызывает мерцание тетрамин
			delay = 0.3;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.03;
		}

		if (playerscore >= 3000) // level 4
		{
			delay = 0.2;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.02;
		}
		if (playerscore >= 4000) //bonus level 5
		{
			delay = 0.1;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.01;
		}
		if (playerscore >= 5000) //bonus level 6
		{
			delay = 0.09;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.009;
		}
		if (playerscore >= 6000) //bonus level 7
		{
			delay = 0.08;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.008;
		}
		if (playerscore >= 7000) //bonus level 8
		{
			delay = 0.07;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.007;
		}
		if (playerscore >= 8000) //bonus level 9
		{
			delay = 0.06;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.006;
		}		
		if (playerscore >= 9000) //bonus level 10
		{
			delay = 0.05;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.007;
		}
		if (playerscore >= 10000) //bonus level 11
		{
			delay = 0.04;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.006;
		}
		if (playerscore >= 11000) //bonus level 12
		{
			delay = 0.03;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.005;
		}
		if (playerscore >= 12000) //bonus level 13
		{
			delay = 0.02;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.004;
		}
		if (playerscore >= 13000) //bonus level 14
		{
			delay = 0.01;
			if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.003;
		}				
		window.clear(Color::White);
		window.draw(z);
		window.draw(c);










//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 	for (int j = 0; j < 4; ++j)
// 		{
// 		int n = rand()% 7;
// 			int shetrand = rand()%7; //прописать счетчик для рандомных чисел для отображения следующей тетраминки
// 				if (tempb[0].x == 0 && tempb[0].y == 0)
// 				{
// 					for (int i = 0; i < 4; i++)
// 					{
// 						// shetrand = n;
// 						a[i].x = figures[n][i] % 2;
// 						a[i].y = figures[n][i] / 2;
// 						tempa[i].x = a[i].x;
// 						tempa[i].y = a[i].y;
// 					}
// 					n = shetrand;
// 					for (int i = 0; i < 4; i++)
// 					{
// 						a[i].x = figures[n][i] % 2;
// 						a[i].y = figures[n][i] / 2;
// 						tempb[i].x = a[i].x;
// 						tempb[i].y = a[i].x;
// 					}
// 				}
// 		for (int i = 0; i < 4; i++)
// 		{
// 			s.setTextureRect(IntRect(colorNum*18,0,18,18));
// 			s.setPosition(tempa[i].x * 18, tempa[i].y * 18);//обнуление позиции на нужное место
// 			window.draw(s);
// 			s.move(28,31);//выравнивание спрайтов 
// 			window.draw(s);
// 		}
// }




		//
		// Отрисовка тетрамин лежащих на дне колодца
		//
						// s.setPosition(10,10);

			for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				s.setTextureRect(IntRect(field[i][j]*0, 0, 18, 18));
				s.setPosition(j * 18, i * 18);//модулирование позиции на нужное место
				s.move(28,31);//выравнивание спрайтов 
				window.draw(s);
			}

			//
			//	Отрисовка падающих тетрамин
			//
		for (int i = 0; i < 4; i++)
		{

			s.setTextureRect(IntRect(colorNum*18,0,18,18));
			s.setPosition(a[i].x * 18, a[i].y * 18);//обнуление позиции на нужное место
			s.move(28,31);//выравнивание спрайтов 
			window.draw(s);
		}

		if (window.isOpen() && !check()) //вывод окончания игры и вывод очков игрока
		{
			Font fontgameover;
			fontgameover.loadFromFile("CyrilicOld.TTF");
			Text gameover("", fontgameover, 20);
			Text score("", fontgameover, 20);
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
			window.display();
			Record(playerscore);
		}


	}
	int n;
	cout << "GAME OVER!" << endl;
	cout << "You score: " << playerscore << endl;
	// system("PAUSE");
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
	// settings.loadFromFile("images/buttonmenu.png");
	license.loadFromFile("images/buttonmenu.png");
	shotdown.loadFromFile("images/buttonmenu.png");
	menuBackground.loadFromFile("images/frame9ps.png");
	viewlevel.loadFromFile("images/metatiles.png");

	Font fontplay;
	fontplay.loadFromFile("CyrilicOld.TTF");
	Text textplay("", fontplay, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	// Text textsettings("", fontplay, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	Text textlicense("", fontplay, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	Text textshotdown("", fontplay, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	// textplay.setColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	textplay.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	// textsettings.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	textlicense.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	textshotdown.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый



	Sprite p(play); 
	// Sprite sett(settings); 
	Sprite lic(license); 
	Sprite shot(shotdown); 
	Sprite menuBg(menuBackground);
	Sprite lev(viewlevel);
	
	bool isMenu = 1;
	int menuNum = 0;

		// ostringstream play;// объявили переменную
		// playerScoreString << playerscore;//занесли в нее число очков, то есть формируем строку
		textplay.setString("Play");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		textshotdown.setString("Exit");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		textlicense.setString("License");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		textplay.setPosition(140, 187);
		textlicense.setPosition(120, 257);
		textshotdown.setPosition(140, 327);
		window.draw(textplay);


	p.setPosition(5, 30);
	shot.setPosition(5, 100);
	lic.setPosition(5, 170);
	menuBg.setPosition(0, 0);
	lev.setPosition(5, 50);
 
	//////////////////MENU///////////////////
	while (isMenu)
	{
		p.setColor(Color::White);
		shot.setColor(Color::White);
		lic.setColor(Color::White);
		menuNum = 0;
		window.clear();
 
		if (IntRect(0, 200, 500, 20).contains(Mouse::getPosition(window))) {p.setColor(Color::Black); menuNum = 1; }
		if (IntRect(0, 270, 500, 20).contains(Mouse::getPosition(window))) {shot.setColor(Color::Black); menuNum = 2; }
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
		window.draw(lic);
		window.draw(shot);
		window.draw(textplay);
		window.draw(textlicense);
		window.draw(textshotdown);
		
		window.display();
	}
}




void License(RenderWindow & window)
{
	Texture backgroundsettings;
	backgroundsettings.loadFromFile("images/backgroundsettings.png");

}

// void Record()
// {
// 	char buff[50];
// 	fstream recordlist;
// 	recordlist.open("Recordlist.txt", ios::out);
// 	for (int i = 0; i < 50; i++)
// 	if (buff[i] == ' ' )
// 	{
// 		cin.getline(buff, 50);
// 		recordlist << playerscore;
// 		recordlist.close();
// 	}
// 	else
// 	{
// 		cin.getline(buff, 50);
// 		recordlist << ' ';
// 		recordlist << playerscore;
// 		recordlist.close();
// 	}
// }




int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(320, 575), "The TETRIS");
	menu(window);

	return 0;
}