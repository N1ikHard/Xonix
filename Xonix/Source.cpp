#include<SFML/Graphics.hpp>
#include<time.h>
using namespace sf;


//Игра Xionix. Размером поля игры по высоте 450 пикселей и ширине 720 пикселей. 
//Размер плитки 18 пикселей. Для игровой механики используется двумерный массив-координаты со следующими значениями
// 
// -1-противник(используется , чтобы не заставило плиткой)
//  0- свободный участок поля
//  1-синияя плитка
//  2-позиция игрока
//


const int M = 25;
const int N = 40;

int grid[M][N] = {0};		//Поле игры
int ts = 18;				//Размер плитки в пискелях 


//Противник
struct Enemy 
{
	//Координаты и скорость
	int x, y, dx, dy;
	
	Enemy() {
		x = y = 300;
		dy=dx = 4 - rand() % 8;
		
	}
	void move() {
		x += dx;
		//Смена направления при ударе о плитку
		if (grid[y / ts][x / ts] == 1) 
		{
			dx = -dx; x += dx;
		}
		y += dy;
		if (grid[y / ts][x / ts] == 1)
		{
			dy = -dy; y += dy;
		}
	}
};
//Этот метод используется для противников, если противник находится на коодинате , 
// то она принимает значение -1 ,чтобы не быть заставленной плиткой
void drop(int y, int x)
{
	if (grid[y][x] == 0) grid[y][x] = -1;
	if (grid[y - 1][x] == 0) drop(y - 1, x);
	if (grid[y + 1][x] == 0) drop(y + 1, x);
	if (grid[y][x - 1] == 0) drop(y, x - 1);
	if (grid[y][x + 1] == 0) drop(y, x + 1);
}

int main()
{

	#pragma region Инициализация
	srand(time(0));
	RenderWindow app(VideoMode(N*ts,M*ts), "Xonix");
	app.setFramerateLimit(60);

	Texture t1 , t2 , t3;
	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/gameover.png");
	t3.loadFromFile("images/enemy.png");
	Sprite sTile(t1), sGameOver(t2) , sEnemy(t3);
	sEnemy.setOrigin(20, 20);

	int enemyCount(4);
	Enemy a[10];

	sGameOver.setPosition(100, 100);
	bool Game = true;

	int x = 0, y = 0, dx = 0, dy = 0;
	float timer = 0, delay = 0.07;
	Clock clock;
	#pragma endregion

	//Установка рамки на поле игры
	for(int i=0;i<M;i++)
		for(int j=0;j<N;j++)
			if(i==0||j==0 ||i==M-1||j==N-1)
			{
				grid[i][j] = 1;
			}

	while (app.isOpen()) {

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		
		while (app.pollEvent(e))
			if (e.type == Event::Closed) 
				app.close();

		#pragma region Управление
		if(Keyboard::isKeyPressed(Keyboard::Left))
		{
			dx = -1; dy = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			dx = 1; dy = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) 
		{
			dx = 0; dy = -1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) 
		{
			dy = 1; dx = 0;
		}
		#pragma endregion
		
		if (!Game)continue;

		//Смена значений за период времени
		if (timer > delay) 
		{
			//Перемещение игрока
			x += dx; 
			y += dy;

			//Запрет выход за поля
			if (x < 0)x = 0;
			if (x > N - 1) x = N - 1;
			if (y < 0) y = 0;
			if (y > M - 1)y = M  -1;

			//Если игрок задевает свою линию
			if (grid[y][x] == 2) Game = false;

			//Если клектка-координата пустая , то ставим координату игрока
			if (grid[y][x] == 0)grid[y][x] = 2;

			timer = 0;
		}

	   //Передвижение противника
		for (int i = 0; i < enemyCount; i++)
			a[i].move();

		 //Если игрок находится в укрытии , то автоматическое перемещение выкл.
		if (grid[y][x] == 1) 
		 {
			 dx = dy = 0;
	
		//Не заставляем координаты плиткой ,где находится враг
		for (int i = 0; i < enemyCount; i++)
			drop(a[i].y/ts, a[i].x/ts);
	
		//Если на этой координате был противник , то вдальнейшем мы можем ее заставить плиткой
		//Если там была пустота , то заставлем плиткой
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
				if (grid[i][j] == -1)
					grid[i][j] = 0;
				else grid[i][j] = 1;
	
	
		}


		//Проверка на соприкосновение с противником и окончание игры	
		for (int i = 0; i < enemyCount; i++)
			if (grid[a[i].y / ts][a[i].x / ts] == 2)
				Game = false;
		
		app.clear();

		//Окрас плитки
		for(int i=0;i<M;i++)
			for (int j = 0; j < N; j++)
			{
				if (grid[i][j] == 0) continue;
				
				//Выбор синий плитки цвет стены
				if (grid[i][j] == 1)
					sTile.setTextureRect(IntRect(0, 0, ts, ts));
				//Выбор красной плитки - цвет игрока 
				if (grid[i][j] == 2)
					sTile.setTextureRect(IntRect(54, 0, ts, ts));

				sTile.setPosition(j * ts, i * ts);
				app.draw(sTile);
			}

		//Рисовка пути игрока зеленой плиткой
		sTile.setTextureRect(IntRect(36,0,ts,ts));
		sTile.setPosition(x * ts, y * ts);
		app.draw(sTile);

		//Скорость вращения вргаов
		sEnemy.rotate(10);

		//Рисовка врагов
		for (int i = 0; i < enemyCount; i++)
		{
			sEnemy.setPosition(a[i].x, a[i].y);
			app.draw(sEnemy);
		}
		
		//Вывод надписи
		if (!Game)
			app.draw(sGameOver);
		app.display();
			
	}



	
	return 1;

}