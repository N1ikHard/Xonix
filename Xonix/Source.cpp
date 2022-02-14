#include<SFML/Graphics.hpp>
#include<time.h>
using namespace sf;


//���� Xionix. �������� ���� ���� �� ������ 450 �������� � ������ 720 ��������. 
//������ ������ 18 ��������. ��� ������� �������� ������������ ��������� ������-���������� �� ���������� ����������
// 
// -1-���������(������������ , ����� �� ��������� �������)
//  0- ��������� ������� ����
//  1-������ ������
//  2-������� ������
//


const int M = 25;
const int N = 40;

int grid[M][N] = {0};		//���� ����
int ts = 18;				//������ ������ � �������� 


//���������
struct Enemy 
{
	//���������� � ��������
	int x, y, dx, dy;
	
	Enemy() {
		x = y = 300;
		dy=dx = 4 - rand() % 8;
		
	}
	void move() {
		x += dx;
		//����� ����������� ��� ����� � ������
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
//���� ����� ������������ ��� �����������, ���� ��������� ��������� �� ��������� , 
// �� ��� ��������� �������� -1 ,����� �� ���� ������������ �������
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

	#pragma region �������������
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

	//��������� ����� �� ���� ����
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

		#pragma region ����������
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

		//����� �������� �� ������ �������
		if (timer > delay) 
		{
			//����������� ������
			x += dx; 
			y += dy;

			//������ ����� �� ����
			if (x < 0)x = 0;
			if (x > N - 1) x = N - 1;
			if (y < 0) y = 0;
			if (y > M - 1)y = M  -1;

			//���� ����� �������� ���� �����
			if (grid[y][x] == 2) Game = false;

			//���� �������-���������� ������ , �� ������ ���������� ������
			if (grid[y][x] == 0)grid[y][x] = 2;

			timer = 0;
		}

	   //������������ ����������
		for (int i = 0; i < enemyCount; i++)
			a[i].move();

		 //���� ����� ��������� � ������� , �� �������������� ����������� ����.
		if (grid[y][x] == 1) 
		 {
			 dx = dy = 0;
	
		//�� ���������� ���������� ������� ,��� ��������� ����
		for (int i = 0; i < enemyCount; i++)
			drop(a[i].y/ts, a[i].x/ts);
	
		//���� �� ���� ���������� ��� ��������� , �� ����������� �� ����� �� ��������� �������
		//���� ��� ���� ������� , �� ��������� �������
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
				if (grid[i][j] == -1)
					grid[i][j] = 0;
				else grid[i][j] = 1;
	
	
		}


		//�������� �� ��������������� � ����������� � ��������� ����	
		for (int i = 0; i < enemyCount; i++)
			if (grid[a[i].y / ts][a[i].x / ts] == 2)
				Game = false;
		
		app.clear();

		//����� ������
		for(int i=0;i<M;i++)
			for (int j = 0; j < N; j++)
			{
				if (grid[i][j] == 0) continue;
				
				//����� ����� ������ ���� �����
				if (grid[i][j] == 1)
					sTile.setTextureRect(IntRect(0, 0, ts, ts));
				//����� ������� ������ - ���� ������ 
				if (grid[i][j] == 2)
					sTile.setTextureRect(IntRect(54, 0, ts, ts));

				sTile.setPosition(j * ts, i * ts);
				app.draw(sTile);
			}

		//������� ���� ������ ������� �������
		sTile.setTextureRect(IntRect(36,0,ts,ts));
		sTile.setPosition(x * ts, y * ts);
		app.draw(sTile);

		//�������� �������� ������
		sEnemy.rotate(10);

		//������� ������
		for (int i = 0; i < enemyCount; i++)
		{
			sEnemy.setPosition(a[i].x, a[i].y);
			app.draw(sEnemy);
		}
		
		//����� �������
		if (!Game)
			app.draw(sGameOver);
		app.display();
			
	}



	
	return 1;

}