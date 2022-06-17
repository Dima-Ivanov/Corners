#include <iostream>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <algorithm>
#include <vector>

using namespace std;


enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LighrRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

COORD dot;

int pole[8][8]; // Массив для запоминания нахождения шашек
int mask[8][8]; // Массив маски для клеток, куда можно сходиться
double score[8][8];

struct coordi
{
	int x, y, xk, yk;
};

coordi kletk[8][8], shask[8][8];

int prevmask[8][8]; // Массив для рисования предыдущий
int xv = -1, yv = -1; // Координаты шашки, которой сходились через другую 
int curhod = 0; // Текущего хода
int choicegame = 0; // Выбор игры против ПК или другого игрока
int cur;
int x = 0, y = 0; // Переменные хода
bool flag = 0; // Есть ли выбранная шашка на поле или нет
int schet1, schet2;
bool podzk = 0; // Подсказки включены или нет

HWND black1, white1, red1, blue1, gray1, darkblue1, darkred1, cyan1;
HDC black, white, red, blue, gray, darkblue, darkred, cyan;

int xn1, yn1, xk1, yk1; // Нужно добавить очков дальним шашкам + что то придумать для заблокированных ходов
double maxs;
int width, height; // ширина высота окна


// Поиск хода
void poisk3ch(int yn, int xn, int yk, int xk, int yv, int xv, vector <pair<int, int>> v, int mas[8][8], double sum)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (yv > 1 && mas[yv - 1][xv] != 0 && mas[yv - 2][xv] == 0)
	{
		bool find = 0;

		for (int i = 0; i < v.size(); i++) if (v[i] == make_pair(yv - 2, xv))
		{
			find = 1;
			break;
		}

		if (find == 0)
		{
			double sc = score[yv - 2][xv] - score[yv][xv];
			if (sum + sc > maxs)
			{
				maxs = sum + sc;
				xn1 = xn;
				yn1 = yn;
				xk1 = xk;
				yk1 = yk;
			}

			swap(mas[yv][xv], mas[yv - 2][xv]);
			v.push_back(make_pair(yv - 2, xv));
			poisk3ch(yn, xn, yk, xk, yv - 2, xv, v, mas, sum + sc);
			swap(mas[yv][xv], mas[yv - 2][xv]);
			v.pop_back();
		}

	}

	if (yv < 6 && mas[yv + 1][xv] != 0 && mas[yv + 2][xv] == 0)
	{
		bool find = 0;

		for (int i = 0; i < v.size(); i++) if (v[i] == make_pair(yv + 2, xv))
		{
			find = 1;
			break;
		}

		if (find == 0)
		{
			double sc = score[yv + 2][xv] - score[yv][xv];
			if (sum + sc > maxs)
			{
				maxs = sum + sc;
				xn1 = xn;
				yn1 = yn;
				xk1 = xk;
				yk1 = yk;
			}

			swap(mas[yv][xv], mas[yv + 2][xv]);
			v.push_back(make_pair(yv + 2, xv));
			poisk3ch(yn, xn, yk, xk, yv + 2, xv, v, mas, sum + sc);
			swap(mas[yv][xv], mas[yv + 2][xv]);
			v.pop_back();
		}
	}

	if (xv > 1 && mas[yv][xv - 1] != 0 && mas[yv][xv - 2] == 0)
	{
		bool find = 0;

		for (int i = 0; i < v.size(); i++) if (v[i] == make_pair(yv, xv - 2))
		{
			find = 1;
			break;
		}

		if (find == 0)
		{
			double sc = score[yv][xv - 2] - score[yv][xv];
			if (sum + sc > maxs)
			{
				maxs = sum + sc;
				xn1 = xn;
				yn1 = yn;
				xk1 = xk;
				yk1 = yk;
			}

			swap(mas[yv][xv], mas[yv][xv - 2]);
			v.push_back(make_pair(yv, xv - 2));
			poisk3ch(yn, xn, yk, xk, yv, xv - 2, v, mas, sum + sc);
			swap(mas[yv][xv], mas[yv][xv - 2]);
			v.pop_back();
		}
	}

	if (xv < 6 && mas[yv][xv + 1] != 0 && mas[yv][xv + 2] == 0)
	{
		bool find = 0;

		for (int i = 0; i < v.size(); i++) if (v[i] == make_pair(yv, xv + 2))
		{
			find = 1;
			break;
		}

		if (find == 0)
		{
			double sc = score[yv][xv + 2] - score[yv][xv];
			if (sum + sc > maxs)
			{
				maxs = sum + sc;
				xn1 = xn;
				yn1 = yn;
				xk1 = xk;
				yk1 = yk;
			}

			swap(mas[yv][xv], mas[yv][xv + 2]);
			v.push_back(make_pair(yv, xv + 2));
			poisk3ch(yn, xn, yk, xk, yv, xv + 2, v, mas, sum + sc);
			swap(mas[yv][xv], mas[yv][xv + 2]);
			v.pop_back();
		}
	}
}

void poisk3(int yn, int xn, int yk, int xk, int mas[8][8], double sum)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (mas[i][j] == 2)
			{
				if (i > 0 && mas[i - 1][j] == 0)
				{
					double sc = score[i - 1][j] - score[i][j];
					if (sum + sc > maxs)
					{
						maxs = sum + sc;
						xn1 = xn;
						yn1 = yn;
						xk1 = xk;
						yk1 = yk;
					}
				}
				else if (i > 1 && mas[i - 2][j] == 0) // через клетку
				{
					double sc = score[i - 2][j] - score[i][j];

					if (sum + sc > maxs)
					{
						maxs = sum + sc;
						xn1 = xn;
						yn1 = yn;
						xk1 = xk;
						yk1 = yk;
					}

					vector <pair<int, int>> n;
					n.push_back(make_pair(i, j));
					n.push_back(make_pair(i - 2, j));

					swap(mas[i][j], mas[i - 2][j]);
					poisk3ch(yn, xn, yk, xk, i - 2, j, n, mas, sum + sc);
					swap(mas[i][j], mas[i - 2][j]);
				}

				if (i < 7 && mas[i + 1][j] == 0)
				{
					double sc = score[i + 1][j] - score[i][j];
					if (sum + sc > maxs)
					{
						maxs = sum + sc;
						xn1 = xn;
						yn1 = yn;
						xk1 = xk;
						yk1 = yk;
					}
				}
				else if (i < 6 && mas[i + 2][j] == 0) // через клетку
				{
					double sc = score[i + 2][j] - score[i][j];

					if (sum + sc > maxs)
					{
						maxs = sum + sc;
						xn1 = xn;
						yn1 = yn;
						xk1 = xk;
						yk1 = yk;
					}

					vector <pair<int, int>> n;
					n.push_back(make_pair(i, j));
					n.push_back(make_pair(i + 2, j));

					swap(mas[i][j], mas[i + 2][j]);
					poisk3ch(yn, xn, yk, xk, i + 2, j, n, mas, sum + sc);
					swap(mas[i][j], mas[i + 2][j]);
				}

				if (j > 0 && mas[i][j - 1] == 0)
				{
					double sc = score[i][j - 1] - score[i][j];
					if (sum + sc > maxs)
					{
						maxs = sum + sc;
						xn1 = xn;
						yn1 = yn;
						xk1 = xk;
						yk1 = yk;
					}
				}
				else if (j > 1 && mas[i][j - 2] == 0) // через клетку
				{
					double sc = score[i][j - 2] - score[i][j];

					if (sum + sc > maxs)
					{
						maxs = sum + sc;
						xn1 = xn;
						yn1 = yn;
						xk1 = xk;
						yk1 = yk;
					}

					vector <pair<int, int>> n;
					n.push_back(make_pair(i, j));
					n.push_back(make_pair(i, j - 2));

					swap(mas[i][j], mas[i][j - 2]);
					poisk3ch(yn, xn, yk, xk, i, j - 2, n, mas, sum + sc);
					swap(mas[i][j], mas[i][j - 2]);
				}

				if (j < 7 && mas[i][j + 1] == 0)
				{
					double sc = score[i][j + 1] - score[i][j];
					if (sum + sc > maxs)
					{
						maxs = sum + sc;
						xn1 = xn;
						yn1 = yn;
						xk1 = xk;
						yk1 = yk;
					}
				}
				else if (j < 6 && mas[i][j + 2] == 0) // через клетку
				{
					double sc = score[i][j + 2] - score[i][j];

					if (sum + sc > maxs)
					{
						maxs = sum + sc;
						xn1 = xn;
						yn1 = yn;
						xk1 = xk;
						yk1 = yk;
					}

					vector <pair<int, int>> n;
					n.push_back(make_pair(i, j));
					n.push_back(make_pair(i, j + 2));

					swap(mas[i][j], mas[i][j + 2]);
					poisk3ch(yn, xn, yk, xk, i, j + 2, n, mas, sum + sc);
					swap(mas[i][j], mas[i][j + 2]);
				}
			}
		}
	}
}

void poisk1ch(int yn, int xn, int yv, int xv, vector <pair<int, int>> v, int mas[8][8], double sum)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (yv > 1 && mas[yv - 1][xv] != 0 && mas[yv - 2][xv] == 0)
	{
		bool find = 0;

		for (int i = 0; i < v.size(); i++) if (v[i] == make_pair(yv - 2, xv))
		{
			find = 1;
			break;
		}

		if (find == 0)
		{
			double sc = score[yv - 2][xv] - score[yv][xv];
			if (sum + sc >= maxs)
			{
				maxs = sum + sc;
				yn1 = yn;
				xn1 = xn;
				yk1 = yv - 2;
				xk1 = xv;
			}

			swap(mas[yv][xv], mas[yv - 2][xv]);
			v.push_back(make_pair(yv - 2, xv));
			poisk3(yn, xn, yv - 2, xv, mas, sum + sc);
			poisk1ch(yn, xn, yv - 2, xv, v, mas, sum + sc);
			swap(mas[yv][xv], mas[yv - 2][xv]);
			v.pop_back();
		}

	}

	if (yv < 6 && mas[yv + 1][xv] != 0 && mas[yv + 2][xv] == 0)
	{
		bool find = 0;

		for (int i = 0; i < v.size(); i++) if (v[i] == make_pair(yv + 2, xv))
		{
			find = 1;
			break;
		}

		if (find == 0)
		{
			double sc = score[yv + 2][xv] - score[yv][xv];
			if (sum + sc >= maxs)
			{
				maxs = sum + sc;
				yn1 = yn;
				xn1 = xn;
				yk1 = yv + 2;
				xk1 = xv;
			}

			swap(mas[yv][xv], mas[yv + 2][xv]);
			v.push_back(make_pair(yv + 2, xv));
			poisk3(yn, xn, yv + 2, xv, mas, sum + sc);
			poisk1ch(yn, xn, yv + 2, xv, v, mas, sum + sc);
			swap(mas[yv][xv], mas[yv + 2][xv]);
			v.pop_back();
		}
	}

	if (xv > 1 && mas[yv][xv - 1] != 0 && mas[yv][xv - 2] == 0)
	{
		bool find = 0;

		for (int i = 0; i < v.size(); i++) if (v[i] == make_pair(yv, xv - 2))
		{
			find = 1;
			break;
		}

		if (find == 0)
		{
			double sc = score[yv][xv - 2] - score[yv][xv];
			if (sum + sc >= maxs)
			{
				maxs = sum + sc;
				yn1 = yn;
				xn1 = xn;
				yk1 = yv;
				xk1 = xv - 2;
			}

			swap(mas[yv][xv], mas[yv][xv - 2]);
			v.push_back(make_pair(yv, xv - 2));
			poisk3(yn, xn, yv, xv - 2, mas, sum + sc);
			poisk1ch(yn, xn, yv, xv - 2, v, mas, sum + sc);
			swap(mas[yv][xv], mas[yv][xv - 2]);
			v.pop_back();
		}
	}

	if (xv < 6 && mas[yv][xv + 1] != 0 && mas[yv][xv + 2] == 0)
	{
		bool find = 0;

		for (int i = 0; i < v.size(); i++) if (v[i] == make_pair(yv, xv + 2))
		{
			find = 1;
			break;
		}

		if (find == 0)
		{
			double sc = score[yv][xv + 2] - score[yv][xv];
			if (sum + sc >= maxs)
			{
				maxs = sum + sc;
				yn1 = yn;
				xn1 = xn;
				yk1 = yv;
				xk1 = xv + 2;
			}

			swap(mas[yv][xv], mas[yv][xv + 2]);
			v.push_back(make_pair(yv, xv + 2));
			poisk3(yn, xn, yv, xv + 2, mas, sum + sc);
			poisk1ch(yn, xn, yv, xv + 2, v, mas, sum + sc);
			swap(mas[yv][xv], mas[yv][xv + 2]);
			v.pop_back();
		}
	}
}

void poisk1(int mas[8][8])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (mas[i][j] == 2)
			{
				if (i > 0 && mas[i - 1][j] == 0)
				{
					double sc = score[i - 1][j] - score[i][j];
					if (sc >= maxs)
					{
						maxs = sc;
						yn1 = i;
						xn1 = j;
						yk1 = i - 1;
						xk1 = j;
					}

					swap(mas[i][j], mas[i - 1][j]);
					poisk3(i, j, i - 1, j, mas, sc);
					swap(mas[i][j], mas[i - 1][j]);
				}
				else if (i > 1 && mas[i - 2][j] == 0) // через клетку
				{
					vector <pair<int, int>> n;
					n.push_back(make_pair(i, j));
					n.push_back(make_pair(i - 2, j));

					double sc = score[i - 2][j] - score[i][j];
					if (sc >= maxs)
					{
						maxs = sc;
						yn1 = i;
						xn1 = j;
						yk1 = i - 2;
						xk1 = j;
					}

					swap(mas[i][j], mas[i - 2][j]);
					poisk3(i, j, i - 2, j, mas, sc);
					poisk1ch(i, j, i - 2, j, n, mas, sc);
					swap(mas[i][j], mas[i - 2][j]);
				}

				if (i < 7 && mas[i + 1][j] == 0)
				{
					double sc = score[i + 1][j] - score[i][j];
					if (sc >= maxs)
					{
						maxs = sc;
						yn1 = i;
						xn1 = j;
						yk1 = i + 1;
						xk1 = j;
					}

					swap(mas[i][j], mas[i + 1][j]);
					poisk3(i, j, i + 1, j, mas, sc);
					swap(mas[i][j], mas[i + 1][j]);
				}
				else if (i < 6 && mas[i + 2][j] == 0) // через клетку
				{
					vector <pair<int, int>> n;
					n.push_back(make_pair(i, j));
					n.push_back(make_pair(i + 2, j));

					double sc = score[i + 2][j] - score[i][j];
					if (sc >= maxs)
					{
						maxs = sc;
						yn1 = i;
						xn1 = j;
						yk1 = i + 2;
						xk1 = j;
					}

					swap(mas[i][j], mas[i + 2][j]);
					poisk3(i, j, i + 2, j, mas, sc);
					poisk1ch(i, j, i + 2, j, n, mas, sc);
					swap(mas[i][j], mas[i + 2][j]);
				}

				if (j > 0 && mas[i][j - 1] == 0)
				{
					double sc = score[i][j - 1] - score[i][j];
					if (sc >= maxs)
					{
						maxs = sc;
						yn1 = i;
						xn1 = j;
						yk1 = i;
						xk1 = j - 1;
					}

					swap(mas[i][j], mas[i][j - 1]);
					poisk3(i, j, i, j - 1, mas, sc);
					swap(mas[i][j], mas[i][j - 1]);
				}
				else if (j > 1 && mas[i][j - 2] == 0) // через клетку
				{
					vector <pair<int, int>> n;
					n.push_back(make_pair(i, j));
					n.push_back(make_pair(i, j - 2));

					double sc = score[i][j - 2] - score[i][j];
					if (sc >= maxs)
					{
						maxs = sc;
						yn1 = i;
						xn1 = j;
						yk1 = i;
						xk1 = j - 2;
					}

					swap(mas[i][j], mas[i][j - 2]);
					poisk3(i, j, i, j - 2, mas, sc);
					poisk1ch(i, j, i, j - 2, n, mas, sc);
					swap(mas[i][j], mas[i][j - 2]);
				}

				if (j < 7 && mas[i][j + 1] == 0)
				{
					double sc = score[i][j + 1] - score[i][j];
					if (sc >= maxs)
					{
						maxs = sc;
						yn1 = i;
						xn1 = j;
						yk1 = i;
						xk1 = j + 1;
					}

					swap(mas[i][j], mas[i][j + 1]);
					poisk3(i, j, i, j + 1, mas, sc);
					swap(mas[i][j], mas[i][j + 1]);
				}
				else if (j < 6 && mas[i][j + 2] == 0) // через клетку
				{
					vector <pair<int, int>> n;
					n.push_back(make_pair(i, j));
					n.push_back(make_pair(i, j + 2));

					double sc = score[i][j + 2] - score[i][j];
					if (sc >= maxs)
					{
						maxs = sc;
						yn1 = i;
						xn1 = j;
						yk1 = i;
						xk1 = j + 2;
					}
					swap(mas[i][j], mas[i][j + 2]);
					poisk3(i, j, i, j + 2, mas, sc);
					poisk1ch(i, j, i, j + 2, n, mas, sc);
					swap(mas[i][j], mas[i][j + 2]);
				}
			}
		}
	}
}
// Поиск хода

// PREVMASK: 1 - положение курсора (серая клетка вокруг шашки), 2 - выбранная шашка (красная/синяя клетка вокруг шашки), 3 - возможные ходы (красная синяя клетка)
// 4 - возможный ход через шашку, 5 - когда наводишься курсором на возможный ход (ярко красная/синяя) клетка, 6 - 5, но когда через шашку
// 7 - наведение на выделенную шашку, 8 - выбранная шашка, которую нельзя убрать, 9 - наведение на выбранную шашку, которую нельзя убрать

void redraw() // Обновление экрана
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (prevmask[i][j] != mask[i][j])
			{
				if (mask[i][j] == 1) // Клетку в серый
				{
					if (flag != 0 && podzk == 1 && pole[i][j] == 0)
					{
						if (pole[yv][xv] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
						else if (pole[yv][xv] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

						RoundRect(cyan, shask[i][j].x + 25, shask[i][j].y + 25, shask[i][j].xk - 25, shask[i][j].yk - 25, 10, 10);
					}
					else
					{
						Rectangle(gray, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
						if (pole[i][j] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
						else if (pole[i][j] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
					}
				}
				else if (mask[i][j] == 2) // Выбранная шашка
				{
					if ((i + j) % 2 == 0) Rectangle(white, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
					else Rectangle(black, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);

					if (pole[i][j] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
					else if (pole[i][j] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

					RoundRect(cyan, shask[i][j].x + 20, shask[i][j].y + 20, shask[i][j].xk - 20, shask[i][j].yk - 20, 20, 20);
				}
				else if ((mask[i][j] == 3 || mask[i][j] == 4) && podzk == 0) // Возможный ход
				{
					if (pole[yv][xv] == 1) RoundRect(darkred, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
					else if (pole[yv][xv] == 2) RoundRect(darkblue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

					RoundRect(cyan, shask[i][j].x + 25, shask[i][j].y + 25, shask[i][j].xk - 25, shask[i][j].yk - 25, 10, 10);
				}
				else if ((mask[i][j] == 5 || mask[i][j] == 6) && podzk == 0) // Наводишься на возможжный ход
				{
					if (pole[yv][xv] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
					else if (pole[yv][xv] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

					RoundRect(cyan, shask[i][j].x + 23, shask[i][j].y + 23, shask[i][j].xk - 23, shask[i][j].yk - 23, 14, 14);
				}
				else if ((mask[i][j] == 3 || mask[i][j] == 4) && podzk == 1) // Возможный ход не показывается
				{
					if ((i + j) % 2 == 0) Rectangle(white, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
					else if ((i + j) % 2 == 1) Rectangle(black, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
				}
				else if ((mask[i][j] == 5 || mask[i][j] == 6) && podzk == 1) // Наводишься на возможжный ход не показывается
				{
					if (pole[yv][xv] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
					else if (pole[yv][xv] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

					RoundRect(cyan, shask[i][j].x + 25, shask[i][j].y + 25, shask[i][j].xk - 25, shask[i][j].yk - 25, 10, 10);
				}
				else if (mask[i][j] == 0)
				{
					if ((i + j) % 2 == 0) Rectangle(white, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
					else if ((i + j) % 2 == 1) Rectangle(black, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);

					if (pole[i][j] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
					else if (pole[i][j] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
				}
				else if (mask[i][j] == 7 || mask[i][j] == 8) // Навелись курсором на выделенную шашку
				{
					Rectangle(gray, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
					if (pole[i][j] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
					else if (pole[i][j] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

					RoundRect(cyan, shask[i][j].x + 20, shask[i][j].y + 20, shask[i][j].xk - 20, shask[i][j].yk - 20, 20, 20);
				}
			}
			// При смене шашек местами перерисовка в нижнем цикле
		}
	}
	dot.X = 54;
	dot.Y = 0;
	SetConsoleCursorPosition(hConsole, dot);
}
// Обновление поля

void draw() // Клетки белые/черные, шашки светло красные/синие, когда их выбираешь красные/синие, обводка поля коричневая, возможные ходы  ?
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	Rectangle(cyan, 0, 50, 720, 770); // Рисовка обводки поля
	for (int i = 0; i < 8; i++) // Рисовка поля
	{
		for (int j = 0; j < 8; j++)
		{
			if ((i + j) % 2 == 0) Rectangle(white, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
			else Rectangle(black, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);

			if (pole[i][j] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
			else if (pole[i][j] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
			Sleep(25);
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (mask[i][j] == 1) // Клетку в серый
			{
				if (flag != 0 && podzk == 1 && pole[i][j] == 0)
				{
					if (pole[yv][xv] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
					else if (pole[yv][xv] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

					RoundRect(cyan, shask[i][j].x + 25, shask[i][j].y + 25, shask[i][j].xk - 25, shask[i][j].yk - 25, 10, 10);
				}
				else
				{
					Rectangle(gray, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
					if (pole[i][j] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
					else if (pole[i][j] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
				}
			}
			else if (mask[i][j] == 2) // Выбранная шашка
			{
				if ((i + j) % 2 == 0) Rectangle(white, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
				else Rectangle(black, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);

				if (pole[i][j] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
				else if (pole[i][j] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

				RoundRect(cyan, shask[i][j].x + 20, shask[i][j].y + 20, shask[i][j].xk - 20, shask[i][j].yk - 20, 20, 20);
			}
			else if ((mask[i][j] == 3 || mask[i][j] == 4) && podzk == 0) // Возможный ход
			{
				if (pole[yv][xv] == 1) RoundRect(darkred, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
				else if (pole[yv][xv] == 2) RoundRect(darkblue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

				RoundRect(cyan, shask[i][j].x + 25, shask[i][j].y + 25, shask[i][j].xk - 25, shask[i][j].yk - 25, 10, 10);
			}
			else if ((mask[i][j] == 5 || mask[i][j] == 6) && podzk == 0) // Наводишься на возможжный ход
			{
				if (pole[yv][xv] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
				else if (pole[yv][xv] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

				RoundRect(cyan, shask[i][j].x + 23, shask[i][j].y + 23, shask[i][j].xk - 23, shask[i][j].yk - 23, 14, 14);
			}
			else if ((mask[i][j] == 3 || mask[i][j] == 4) && podzk == 1) // Возможный ход не показывается
			{
				if ((i + j) % 2 == 0) Rectangle(white, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
				else if ((i + j) % 2 == 1) Rectangle(black, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
			}
			else if ((mask[i][j] == 5 || mask[i][j] == 6) && podzk == 1) // Наводишься на возможжный ход не показывается
			{
				if (pole[yv][xv] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
				else if (pole[yv][xv] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

				RoundRect(cyan, shask[i][j].x + 25, shask[i][j].y + 25, shask[i][j].xk - 25, shask[i][j].yk - 25, 10, 10);
			}
			else if (mask[i][j] == 0)
			{
				if ((i + j) % 2 == 0) Rectangle(white, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
				else if ((i + j) % 2 == 1) Rectangle(black, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);

				if (pole[i][j] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
				else if (pole[i][j] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
			}
			else if (mask[i][j] == 7 || mask[i][j] == 8) // Навелись курсором на выделенную шашку
			{
				Rectangle(gray, kletk[i][j].x, kletk[i][j].y, kletk[i][j].xk, kletk[i][j].yk);
				if (pole[i][j] == 1) RoundRect(red, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);
				else if (pole[i][j] == 2) RoundRect(blue, shask[i][j].x, shask[i][j].y, shask[i][j].xk, shask[i][j].yk, 60, 60);

				RoundRect(cyan, shask[i][j].x + 20, shask[i][j].y + 20, shask[i][j].xk - 20, shask[i][j].yk - 20, 20, 20);
			}
		}
		// При смене шашек местами перерисовка в нижнем цикле
	}
	dot.X = 54;
	dot.Y = 0;
	SetConsoleCursorPosition(hConsole, dot);
}
// Рисовка всего поля

void endgame()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	system("color F0");
	Rectangle(white, 0, 0, 720, 770);
	dot.X = 37;
	dot.Y = 24;
	SetConsoleCursorPosition(hConsole, dot);
	cout << "Спасибо за игру!\n";
	cin.ignore();
}
// Конец игры

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	{
		HWND hcon = GetConsoleWindow();
		CONSOLE_FONT_INFO cfi;
		GetCurrentConsoleFont(hConsole, false, &cfi);
		COORD fs = GetConsoleFontSize(hConsole, cfi.nFont);
		PCONSOLE_FONT_INFOEX ccf = new CONSOLE_FONT_INFOEX;
		(*ccf).dwFontSize.X = 8;
		(*ccf).dwFontSize.Y = 16;
		(*ccf).nFont = 11;
		(*ccf).cbSize = sizeof(CONSOLE_FONT_INFOEX);
		ccf->FontWeight = 400;
		lstrcpyW((*ccf).FaceName, L"Consolas");
		(*ccf).FontFamily = FF_DONTCARE;
		bool b = SetCurrentConsoleFontEx(hConsole, false, ccf);
		fs = GetConsoleFontSize(hConsole, cfi.nFont);

		width = 90;
		height = 48;
		HWND hWindowConsole = GetConsoleWindow();
		RECT r;
		GetWindowRect(hWindowConsole, &r);
		MoveWindow(hWindowConsole, r.left, r.top, 753, 808, TRUE);

		black1 = GetConsoleWindow();
		white1 = GetConsoleWindow();
		gray1 = GetConsoleWindow();
		red1 = GetConsoleWindow();
		blue1 = GetConsoleWindow();
		gray1 = GetConsoleWindow();
		darkblue1 = GetConsoleWindow();
		darkred1 = GetConsoleWindow();
		cyan1 = GetConsoleWindow();

		black = GetDC(black1);
		white = GetDC(white1);
		gray = GetDC(gray1);
		red = GetDC(red1);
		blue = GetDC(blue1);
		darkblue = GetDC(darkblue1);
		darkred = GetDC(darkred1);
		cyan = GetDC(cyan1);

		HBRUSH red2 = CreateSolidBrush(RGB(220, 0, 0));
		HBRUSH blue2 = CreateSolidBrush(RGB(0, 0, 220));
		HBRUSH darkred2 = CreateSolidBrush(RGB(120, 0, 0));
		HBRUSH darkblue2 = CreateSolidBrush(RGB(0, 0, 120));
		HBRUSH cyan2 = CreateSolidBrush(RGB(0, 170, 170));

		SelectObject(black, GetStockObject(BLACK_BRUSH));
		SelectObject(white, GetStockObject(WHITE_BRUSH));
		SelectObject(gray, GetStockObject(GRAY_BRUSH));
		SelectObject(red, red2);
		SelectObject(blue, blue2);
		SelectObject(darkred, darkred2);
		SelectObject(darkblue, darkblue2);
		SelectObject(cyan, cyan2);

		dot.X = 0;
		dot.Y = 2;
		system("color 0F");
		srand(time(NULL));
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);

		CONSOLE_CURSOR_INFO structCursorInfo;
		GetConsoleCursorInfo(hConsole, &structCursorInfo);
		structCursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(hConsole, &structCursorInfo);
	}
	// Настройки экрана

	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				kletk[i][j].x = 40 + j * 80;
				kletk[i][j].y = 90 + i * 80;
				kletk[i][j].xk = 40 + (j + 1) * 80;
				kletk[i][j].yk = 90 + (i + 1) * 80;
				shask[i][j].x = kletk[i][j].x + 10;
				shask[i][j].y = kletk[i][j].y + 10;
				shask[i][j].xk = kletk[i][j].xk - 10;
				shask[i][j].yk = kletk[i][j].yk - 10;
			}
		}

		score[0][0] = 16; score[0][1] = 15; score[0][2] = 14; score[0][3] = 13; score[0][4] = 8; score[0][5] = 7; score[0][6] = 6; score[0][7] = 5;
		score[1][0] = 15; score[1][1] = 14; score[1][2] = 13.5; score[1][3] = 12; score[1][4] = 8; score[1][5] = 7; score[1][6] = 6; score[1][7] = 5;
		score[2][0] = 14; score[2][1] = 13; score[2][2] = 12; score[2][3] = 10; score[2][4] = 8; score[2][5] = 7; score[2][6] = 6; score[2][7] = 5;
		score[3][0] = 8; score[3][1] = 8; score[3][2] = 8; score[3][3] = 8; score[3][4] = 5.5; score[3][5] = 5; score[3][6] = 5; score[3][7] = 4;
		score[4][0] = 7; score[4][1] = 7; score[4][2] = 7; score[4][3] = 7; score[4][4] = 4.5; score[4][5] = 4; score[4][6] = 4; score[4][7] = 3;
		score[5][0] = 6; score[5][1] = 6; score[5][2] = 6; score[5][3] = 6; score[5][4] = 4; score[5][5] = 3.5; score[5][6] = 3; score[5][7] = 2;
		score[6][0] = 5; score[6][1] = 5; score[6][2] = 5; score[6][3] = 5; score[6][4] = 4; score[6][5] = 3; score[6][6] = 2; score[6][7] = 1;
		score[7][0] = 4; score[7][1] = 4; score[7][2] = 4; score[7][3] = 4; score[7][4] = 3; score[7][5] = 2; score[7][6] = 1; score[7][7] = 0;

		//for (int i = 0; i < 3; i++) for (int j = 0; j < 4; j++) score[i][j] = (score[i][j] + 2) * 3;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pole[i][j] = 1;
				pole[7 - i][7 - j] = 2;
			}
		}

	}
	// Заполнение массивов

	{
		system("color F0");
		dot.Y = (height - 4) / 2; dot.X = (width - 15) / 2; // 48 на 90
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Игра <<Уголки>>"; // 15
		SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Red));
		dot.Y += 2; dot.X = (width - 17) / 2;
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Сыграть против ПК"; // 17
		dot.Y++; dot.X = (width - 29) / 2;
		SetConsoleCursorPosition(hConsole, dot);
		SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
		cout << "Сыграть против другого игрока"; // 29
		dot.Y++; dot.X = (width - 4) / 2;
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Инфо"; // 4
		dot.Y = (height - 4) / 2 + 2; dot.X = (width - 17) / 2;
	}
	while (1) // ВНИЗ - 80, ВВЕРХ - 72, ВПРАВО - 77, ВЛЕВО - 75, ENTER - 13, ESC - 27. ВЫВОД 2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		SetConsoleCursorPosition(hConsole, dot);
		SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Red));
		if (choicegame == 0) cout << "Сыграть против ПК";
		else if (choicegame == 1) cout << "Сыграть против другого игрока";
		else if (choicegame == 2) cout << "Инфо";
		cur = _getch();

		if (cur == 72 && choicegame > 0)
		{
			SetConsoleCursorPosition(hConsole, dot);
			SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
			if (choicegame == 0) cout << "Сыграть против ПК";
			else if (choicegame == 1) cout << "Сыграть против другого игрока";
			else if (choicegame == 2) cout << "Инфо";
			dot.Y--;
			choicegame--;
			if (choicegame == 0) dot.X = (width - 17) / 2;
			else if (choicegame == 1) dot.X = (width - 29) / 2;
		}
		else if (cur == 80 && choicegame < 2)
		{
			SetConsoleCursorPosition(hConsole, dot);
			SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
			if (choicegame == 0) cout << "Сыграть против ПК";
			else if (choicegame == 1) cout << "Сыграть против другого игрока";
			else if (choicegame == 2) cout << "Инфо";
			dot.Y++;
			choicegame++;
			if (choicegame == 1) dot.X = (width - 29) / 2;
			else if (choicegame == 2) dot.X = (width - 4) / 2;
		}
		else if (cur == 13)
		{
			if (choicegame == 2)
			{
				system("cls");
				system("color F0");
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				cout << "Игра <<Уголки>>\n\n";
				cout << "Управление курсором осуществляется с помощью стрелочек;\n";
				cout << "Чтобы выбрать шашку и сделать ход нажмите ENTER;\n";
				cout << "Если вы изменили размер окна и картинка сбилась, нажмите 1;\n";
				cout << "Пропустить ход можно с помощью клавиши 0;\n";
				cout << "Отключить и включить подсказки можно с помощью клавиши 2;\n";
				cout << "Вы в любой момент можете выйти из игры, нажав Esc;\n\n";
				cout << "Правила игры:\n";
				cout << "Цель игры - быстрее соперника провести шашки в противоположный\n";
				cout << "по диагонали угол;\n";
				cout << "Можно ходить на одну клетку или прыжками через шашки;\n";
				cout << "Если противник вывел все шашки из своего поля, то и вы\n";
				cout << "обязаны вывести все свои шашки;\n";
				cout << "Нельзя полностью блокировать ход противнику;\n\n";
				cout << "Слева сверху отображается строка, сообщающая чей сейчас ход;\n";
				cout << "Далее отображается счет и подсказки;\n\n";
				cout << "Иванов Дмитрий 1-41хх 2021г.\n\n";
				while (1)
				{
					cur = _getch();
					if (cur == 13)
					{
						system("cls");
						system("color F0");
						dot.Y = (height - 4) / 2; dot.X = (width - 15) / 2; // 48 на 90
						SetConsoleCursorPosition(hConsole, dot);
						cout << "Игра <<Уголки>>"; // 15
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						dot.Y += 2; dot.X = (width - 17) / 2;
						SetConsoleCursorPosition(hConsole, dot);
						cout << "Сыграть против ПК"; // 17
						dot.Y++; dot.X = (width - 29) / 2;
						SetConsoleCursorPosition(hConsole, dot);
						cout << "Сыграть против другого игрока"; // 29
						dot.Y++; dot.X = (width - 4) / 2;
						SetConsoleCursorPosition(hConsole, dot);
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Red));
						cout << "Инфо"; // 4
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						dot.Y = (height - 4) / 2 + 4; dot.X = (width - 4) / 2;
						break;
					}
					else if (cur == 27)
					{
						endgame();
						return 0;
					}
				}

			}
			else if (choicegame == 0 || choicegame == 1) break;
		}
		else if (cur == 27)
		{
			endgame();
			return 0;
		}
	}
	// Вывод выбора соперника

	// choicegame 0 - против ПК, choicegame 1 - против другого игрока
	if (choicegame == 0) // ВЫВОД 3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		system("cls");
		system("color F0");
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
		dot.Y = (height - 4) / 2; dot.X = (width - 15) / 2; // 48 на 90
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Игра <<Уголки>>"; // 15
		dot.Y += 2; dot.X = (width - 20) / 2; // 35
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Первым ходится игрок"; // 20
		dot.Y++; dot.X = (width - 17) / 2; // 36
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Первым ходится ПК"; // 17
		dot.Y++; dot.X = (width - 31) / 2;
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Выбрать начинающего игру наугад"; // 31
		dot.Y = (height - 4) / 2 + 2;
		while (1) // ВНИЗ - 80, ВВЕРХ - 72, ВПРАВО - 77, ВЛЕВО - 75, ENTER - 13, ESC - 27.
		{
			if (curhod == 0) dot.X = (width - 20) / 2;
			else if (curhod == 1) dot.X = (width - 17) / 2;
			else if (curhod == 2) dot.X = (width - 31) / 2;

			SetConsoleCursorPosition(hConsole, dot);
			SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Red));
			if (curhod == 0) cout << "Первым ходится игрок";
			else if (curhod == 1) cout << "Первым ходится ПК";
			else if (curhod == 2) cout << "Выбрать начинающего игру наугад";
			cur = _getch();

			if (curhod == 0) dot.X = (width - 20) / 2;
			else if (curhod == 1) dot.X = (width - 17) / 2;
			else if (curhod == 2) dot.X = (width - 31) / 2;
			if (cur == 72 && curhod > 0)
			{
				SetConsoleCursorPosition(hConsole, dot);
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				if (curhod == 0) cout << "Первым ходится игрок";
				else if (curhod == 1) cout << "Первым ходится ПК";
				else if (curhod == 2) cout << "Выбрать начинающего игру наугад";
				dot.Y--;
				curhod--;
			}
			else if (cur == 80 && curhod < 2)
			{
				SetConsoleCursorPosition(hConsole, dot);
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				if (curhod == 0) cout << "Первым ходится игрок";
				else if (curhod == 1) cout << "Первым ходится ПК";
				else if (curhod == 2) cout << "Выбрать начинающего игру наугад";
				dot.Y++;
				curhod++;
			}
			else if (cur == 13) break;
			else if (cur == 27)
			{
				endgame();
				return 0;
			}
		}
	}
	else if (choicegame == 1) // ВНИЗ - 80, ВВЕРХ - 72, ВПРАВО - 77, ВЛЕВО - 75, ENTER - 13, ESC - 27.
	{
		system("cls");
		system("color F0");
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
		dot.Y = (height - 4) / 2; dot.X = (width - 15) / 2; // 48 на 90
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Игра <<Уголки>>"; // 15
		dot.Y += 2; dot.X = (width - 23) / 2;
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Игрок №1 ходится первым"; // 23
		dot.Y++; dot.X = (width - 23) / 2;
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Игрок №2 ходится первым"; // 23
		dot.Y++; dot.X = (width - 31) / 2;
		SetConsoleCursorPosition(hConsole, dot);
		cout << "Выбрать начинающего игру наугад"; // 31
		dot.Y = (height - 4) / 2 + 2;
		while (1)
		{
			if (curhod == 0) dot.X = (width - 23) / 2;
			else if (curhod == 1) dot.X = (width - 23) / 2;
			else if (curhod == 2) dot.X = (width - 31) / 2;

			SetConsoleCursorPosition(hConsole, dot);
			SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Red));
			if (curhod == 0) cout << "Игрок №1 ходится первым";
			else if (curhod == 1) cout << "Игрок №2 ходится первым";
			else if (curhod == 2) cout << "Выбрать начинающего игру наугад";
			cur = _getch();

			if (curhod == 0) dot.X = (width - 23) / 2;
			else if (curhod == 1) dot.X = (width - 23) / 2;
			else if (curhod == 2) dot.X = (width - 31) / 2;
			if (cur == 72 && curhod > 0)
			{
				SetConsoleCursorPosition(hConsole, dot);
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				if (curhod == 0) cout << "Игрок №1 ходится первым";
				else if (curhod == 1) cout << "Игрок №2 ходится первым";
				else if (curhod == 2) cout << "Выбрать начинающего игру наугад";
				dot.Y--;
				curhod--;
			}
			else if (cur == 80 && curhod < 2)
			{
				SetConsoleCursorPosition(hConsole, dot);
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				if (curhod == 0) cout << "Игрок №1 ходится первым";
				else if (curhod == 1) cout << "Игрок №2 ходится первым";
				else if (curhod == 2) cout << "Выбрать начинающего игру наугад";
				dot.Y++;
				curhod++;
			}
			else if (cur == 13) break;
			else if (cur == 27)
			{
				endgame();
				return 0;
			}
		}
	}
	// Вывод выбора хода

	if (curhod > 1) curhod = rand() % 2;

	{
		system("cls");
		system("color F0");
		SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
		cout << "                         Инфо - 3, выйти из игры - Esc\n\n";

		dot.X = 0;
		dot.Y = 0;
		SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
		SetConsoleCursorPosition(hConsole, dot);
		if (choicegame == 0)
		{
			if (curhod == 0)
			{
				cout << "Ваш ход";
			}
			else if (curhod == 1)
			{
				cout << "Ход ПК ";
			}
		}
		else if (choicegame == 1)
		{
			if (curhod == 0)
			{
				cout << "Ход игрока №1";
			}
			else if (curhod == 1)
			{
				cout << "Ход игрока №2";
			}
		}

		dot.X = 16;
		dot.Y = 0;
		SetConsoleCursorPosition(hConsole, dot);
		cout << schet1 << " : " << schet2;


		mask[x][y] = 1;
		draw();
	}
	// Вывод верхней строки главного вывода игры

	while (1)
	{                                                                         // ВНИЗ - 80, ВВЕРХ - 72, ВПРАВО - 77, ВЛЕВО - 75, ENTER - 13, ESC - 27, NUM0 - 48
		if (choicegame == 0 && curhod == 0 || choicegame == 1)
		{
			cur = _getch();
			if (cur == 72 && y > 0)
			{
				if (prevmask[y][x] == 1) mask[y][x] = 0;
				else if (prevmask[y][x] == 5) mask[y][x] = 3;
				else if (prevmask[y][x] == 6) mask[y][x] = 4;
				else if (prevmask[y][x] == 7) mask[y][x] = 2;
				else if (prevmask[y][x] == 9) mask[y][x] = 8;
				y--;
				if (prevmask[y][x] == 0) mask[y][x] = 1;
				else if (prevmask[y][x] == 3) mask[y][x] = 5;
				else if (prevmask[y][x] == 4) mask[y][x] = 6;
				else if (prevmask[y][x] == 2) mask[y][x] = 7;
				else if (prevmask[y][x] == 8) mask[y][x] = 9;
			}
			else if (cur == 80 && y < 7)
			{
				if (prevmask[y][x] == 1) mask[y][x] = 0;
				else if (prevmask[y][x] == 5) mask[y][x] = 3;
				else if (prevmask[y][x] == 6) mask[y][x] = 4;
				else if (prevmask[y][x] == 7) mask[y][x] = 2;
				else if (prevmask[y][x] == 9) mask[y][x] = 8;
				y++;
				if (prevmask[y][x] == 0) mask[y][x] = 1;
				else if (prevmask[y][x] == 3) mask[y][x] = 5;
				else if (prevmask[y][x] == 4) mask[y][x] = 6;
				else if (prevmask[y][x] == 2) mask[y][x] = 7;
				else if (prevmask[y][x] == 8) mask[y][x] = 9;
			}
			else if (cur == 77 && x < 7)
			{
				if (prevmask[y][x] == 1) mask[y][x] = 0;
				else if (prevmask[y][x] == 5) mask[y][x] = 3;
				else if (prevmask[y][x] == 6) mask[y][x] = 4;
				else if (prevmask[y][x] == 7) mask[y][x] = 2;
				else if (prevmask[y][x] == 9) mask[y][x] = 8;
				x++;
				if (prevmask[y][x] == 0) mask[y][x] = 1;
				else if (prevmask[y][x] == 3) mask[y][x] = 5;
				else if (prevmask[y][x] == 4) mask[y][x] = 6;
				else if (prevmask[y][x] == 2) mask[y][x] = 7;
				else if (prevmask[y][x] == 8) mask[y][x] = 9;
			}
			else if (cur == 75 && x > 0)
			{
				if (prevmask[y][x] == 1) mask[y][x] = 0;
				else if (prevmask[y][x] == 5) mask[y][x] = 3;
				else if (prevmask[y][x] == 6) mask[y][x] = 4;
				else if (prevmask[y][x] == 7) mask[y][x] = 2;
				else if (prevmask[y][x] == 9) mask[y][x] = 8;
				x--;
				if (prevmask[y][x] == 0) mask[y][x] = 1;
				else if (prevmask[y][x] == 3) mask[y][x] = 5;
				else if (prevmask[y][x] == 4) mask[y][x] = 6;
				else if (prevmask[y][x] == 2) mask[y][x] = 7;
				else if (prevmask[y][x] == 8) mask[y][x] = 9;
			}
			else if (cur == 13)
			{
				if (pole[y][x] == 1 && curhod == 0 && flag == 0) // Выделение шашки
				{
					mask[y][x] = 7;
					xv = x; yv = y;
					flag = 1;
					// Возможные ходы
					if (y > 0)
					{
						if (pole[y - 1][x] == 0) mask[y - 1][x] = 3;
					}

					if (y > 1 && pole[y - 1][x] != 0 && pole[y - 2][x] == 0) mask[y - 2][x] = 4;

					if (y < 7)
					{
						if (pole[y + 1][x] == 0) mask[y + 1][x] = 3;
					}

					if (y < 6 && pole[y + 1][x] != 0 && pole[y + 2][x] == 0) mask[y + 2][x] = 4;

					if (x > 0 && pole[y][x - 1] == 0) mask[y][x - 1] = 3;
					else if (x > 1 && pole[y][x - 2] == 0) mask[y][x - 2] = 4;

					if (x < 7 && pole[y][x + 1] == 0) mask[y][x + 1] = 3;
					else if (x < 6 && pole[y][x + 2] == 0) mask[y][x + 2] = 4;
				}
				else if (pole[y][x] == 2 && curhod == 1 && flag == 0) // Выделение шашки 2
				{
					mask[y][x] = 7;
					xv = x; yv = y;
					flag = 1;
					// Возможные ходы
					if (y > 0)
					{
						if (pole[y - 1][x] == 0) mask[y - 1][x] = 3;
					}

					if (y > 1 && pole[y - 1][x] != 0 && pole[y - 2][x] == 0) mask[y - 2][x] = 4;

					if (y < 7)
					{
						if (pole[y + 1][x] == 0) mask[y + 1][x] = 3;
					}

					if (y < 6 && pole[y + 1][x] != 0 && pole[y + 2][x] == 0) mask[y + 2][x] = 4;

					if (x > 0 && pole[y][x - 1] == 0) mask[y][x - 1] = 3;
					else if (x > 1 && pole[y][x - 2] == 0) mask[y][x - 2] = 4;

					if (x < 7 && pole[y][x + 1] == 0) mask[y][x + 1] = 3;
					else if (x < 6 && pole[y][x + 2] == 0) mask[y][x + 2] = 4;
				}
				else if (pole[y][x] == 1 && prevmask[y][x] == 7) // Снятие выделения шашки
				{
					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 8; j++) mask[i][j] = 0;
					}
					mask[y][x] = 1;
					xv = -1; yv = -1;
					flag = 0;
				}
				else if (pole[y][x] == 2 && prevmask[y][x] == 7) // Снятие выделения шашки 2
				{
					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 8; j++) mask[i][j] = 0;
					}
					mask[y][x] = 1;
					xv = -1; yv = -1;
					flag = 0;
				}
				else if (prevmask[y][x] == 5) // Сделали ход на 1 клетку
				{
					swap(pole[y][x], pole[yv][xv]);

					if ((yv + xv) % 2 == 0) Rectangle(white, kletk[yv][xv].x, kletk[yv][xv].y, kletk[yv][xv].xk, kletk[yv][xv].yk);
					else Rectangle(black, kletk[yv][xv].x, kletk[yv][xv].y, kletk[yv][xv].xk, kletk[yv][xv].yk);

					if (pole[y][x] == 1) RoundRect(red, shask[y][x].x, shask[y][x].y, shask[y][x].xk, shask[y][x].yk, 60, 60);
					else if (pole[y][x] == 2) RoundRect(blue, shask[y][x].x, shask[y][x].y, shask[y][x].xk, shask[y][x].yk, 60, 60);

					flag = 0;
					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 8; j++) mask[i][j] = 0;
					}
					mask[y][x] = 1;
					xv = -1; yv = -1;
					if (curhod == 0) curhod = 1;
					else curhod = 0;

					HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
					dot.X = 0;
					dot.Y = 0;
					SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
					SetConsoleCursorPosition(hConsole, dot);
					if (choicegame == 0)
					{
						if (curhod == 0)
						{
							cout << "Ваш ход";
						}
						else if (curhod == 1)
						{
							cout << "Ход ПК ";
						}
					}
					else if (choicegame == 1)
					{
						if (curhod == 0)
						{
							cout << "Ход игрока №1";
						}
						else if (curhod == 1)
						{
							cout << "Ход игрока №2";
						}
					}

					int sum1 = 0, sum2 = 0; // Проверка на победу
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							if (pole[i][j] == 2) sum2++;
							if (pole[7 - i][7 - j] == 1) sum1++;
						}
					}
					if (sum2 == 12)
					{
						schet2++;
						dot.X = 0;
						dot.Y = 1;
						SetConsoleCursorPosition(hConsole, dot);
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						if (choicegame == 0) cout << "ПК выйграл! ";
						else if (choicegame == 1) cout << "Игрок №2 выйграл! ";
						cout << "Чтобы начать новую игру нажмите ENTER!";
						while (1)
						{
							cur = _getch();
							if (cur == 13)
							{
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 8; j++)
									{
										pole[i][j] = 0;
										mask[i][j] = 0;
									}
								}
								for (int i = 0; i < 3; i++)
								{
									for (int j = 0; j < 4; j++)
									{
										pole[i][j] = 1;
										pole[7 - i][7 - j] = 2;
									}
								}
								mask[0][0] = 1;
								sum2 = 1;
								draw();
								x = 0;
								y = 0;
							}
							else if (cur == 27)
							{
								endgame();
								return 0;
							}
							if (sum2 == 1) break;
						}
						dot.X = 16;
						dot.Y = 0;
						SetConsoleCursorPosition(hConsole, dot);
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						cout << schet1 << " : " << schet2;
						dot.X = 0;
						dot.Y = 1;
						SetConsoleCursorPosition(hConsole, dot);
						cout << "                                                                    ";

					}
					else if (sum1 == 12)
					{
						schet1++;
						dot.X = 0;
						dot.Y = 1;
						SetConsoleCursorPosition(hConsole, dot);
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						if (choicegame == 0) cout << "Вы выйграли! ";
						else if (choicegame == 1) cout << "Игрок №1 выйграл! ";
						cout << "Чтобы начать новую игру нажмите ENTER!";
						while (1)
						{
							cur = _getch();
							if (cur == 13)
							{
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 8; j++)
									{
										pole[i][j] = 0;
										mask[i][j] = 0;
									}
								}
								for (int i = 0; i < 3; i++)
								{
									for (int j = 0; j < 4; j++)
									{
										pole[i][j] = 1;
										pole[7 - i][7 - j] = 2;
									}
								}
								mask[0][0] = 1;
								sum1 = 1;
								draw();
								x = 0;
								y = 0;
							}
							else if (cur == 27)
							{
								endgame();
								return 0;
							}
							if (sum1 == 1) break;
						}
						dot.X = 16;
						dot.Y = 0;
						SetConsoleCursorPosition(hConsole, dot);
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						cout << schet1 << " : " << schet2;
						dot.X = 0;
						dot.Y = 1;
						SetConsoleCursorPosition(hConsole, dot);
						cout << "                                                                    ";
						dot.X = 54;
						dot.Y = 0;
						SetConsoleCursorPosition(hConsole, dot);
					}
				}
				else if (prevmask[y][x] == 6) // Сделали ход через шашку
				{
					swap(pole[y][x], pole[yv][xv]);

					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 8; j++) mask[i][j] = 0;
					}

					if ((yv + xv) % 2 == 0) Rectangle(white, kletk[yv][xv].x, kletk[yv][xv].y, kletk[yv][xv].xk, kletk[yv][xv].yk);
					else Rectangle(black, kletk[yv][xv].x, kletk[yv][xv].y, kletk[yv][xv].xk, kletk[yv][xv].yk);

					if (pole[y][x] == 1) RoundRect(red, shask[y][x].x, shask[y][x].y, shask[y][x].xk, shask[y][x].yk, 60, 60);
					else if (pole[y][x] == 2) RoundRect(blue, shask[y][x].x, shask[y][x].y, shask[y][x].xk, shask[y][x].yk, 60, 60);

					xv = x; yv = y;

					mask[y][x] = 8; // Запоминаем шашку

					// Возможные ходы

					bool col = 0;

					if (y > 1 && pole[y - 1][x] != 0 && pole[y - 2][x] == 0 && prevmask[y - 2][x] != 2 && prevmask[y - 2][x] != 8)
					{
						mask[y - 2][x] = 4;
						col = 1;
					}

					if (y < 6 && pole[y + 1][x] != 0 && pole[y + 2][x] == 0 && prevmask[y + 2][x] != 2 && prevmask[y + 2][x] != 8)
					{
						mask[y + 2][x] = 4;
						col = 1;
					}

					if (x > 1 && pole[y][x - 1] != 0 && pole[y][x - 2] == 0 && prevmask[y][x - 2] != 2 && prevmask[y][x - 2] != 8)
					{
						mask[y][x - 2] = 4;
						col = 1;
					}

					if (x < 6 && pole[y][x + 1] != 0 && pole[y][x + 2] == 0 && prevmask[y][x + 2] != 2 && prevmask[y][x + 2] != 8)
					{
						mask[y][x + 2] = 4;
						col = 1;
					}

					if (col == 0)
					{
						for (int i = 0; i < 8; i++)
						{
							for (int j = 0; j < 8; j++) mask[i][j] = 0;
						}
						mask[y][x] = 1;
						xv = -1; yv = -1;
						flag = 0;

						if (curhod == 0) curhod = 1;
						else if (curhod == 1) curhod = 0;

						HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
						dot.X = 0;
						dot.Y = 0;
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						SetConsoleCursorPosition(hConsole, dot);
						if (choicegame == 0)
						{
							if (curhod == 0)
							{
								cout << "Ваш ход";
							}
							else if (curhod == 1)
							{
								cout << "Ход ПК ";
							}
						}
						else if (choicegame == 1)
						{
							if (curhod == 0)
							{
								cout << "Ход игрока №1";
							}
							else if (curhod == 1)
							{
								cout << "Ход игрока №2";
							}
						}
					}

					int sum1 = 0, sum2 = 0; // Проверка на победу
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							if (pole[i][j] == 2) sum2++;
							if (pole[7 - i][7 - j] == 1) sum1++;
						}
					}
					if (sum2 == 12)
					{
						schet2++;
						dot.X = 0;
						dot.Y = 1;
						SetConsoleCursorPosition(hConsole, dot);
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						if (choicegame == 0) cout << "ПК выйграл! ";
						else if (choicegame == 1) cout << "Игрок №2 выйграл! ";
						cout << "Чтобы начать новую игру нажмите ENTER!";
						while (1)
						{
							cur = _getch();
							if (cur == 13)
							{
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 8; j++)
									{
										pole[i][j] = 0;
										mask[i][j] = 0;
									}
								}
								for (int i = 0; i < 3; i++)
								{
									for (int j = 0; j < 4; j++)
									{
										pole[i][j] = 1;
										pole[7 - i][7 - j] = 2;
									}
								}
								mask[0][0] = 1;
								sum2 = 1;
								draw();
								x = 0;
								y = 0;
							}
							else if (cur == 27)
							{
								endgame();
								return 0;
							}
							if (sum2 == 1) break;
						}
						dot.X = 16;
						dot.Y = 0;
						SetConsoleCursorPosition(hConsole, dot);
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						cout << schet1 << " : " << schet2;
						dot.X = 0;
						dot.Y = 1;
						SetConsoleCursorPosition(hConsole, dot);
						cout << "                                                                    ";
					}
					else if (sum1 == 12)
					{
						schet1++;
						dot.X = 0;
						dot.Y = 1;
						SetConsoleCursorPosition(hConsole, dot);
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						if (choicegame == 0) cout << "Вы выйграли! ";
						else if (choicegame == 1) cout << "Игрок №1 выйграл! ";
						cout << "Чтобы начать новую игру нажмите ENTER!";
						while (1)
						{
							cur = _getch();
							if (cur == 13)
							{
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 8; j++)
									{
										pole[i][j] = 0;
										mask[i][j] = 0;
									}
								}
								for (int i = 0; i < 3; i++)
								{
									for (int j = 0; j < 4; j++)
									{
										pole[i][j] = 1;
										pole[7 - i][7 - j] = 2;
									}
								}
								mask[0][0] = 1;
								sum1 = 1;
								draw();
								x = 0;
								y = 0;
							}
							else if (cur == 27)
							{
								endgame();
								return 0;
							}
							if (sum1 == 1) break;
						}
						dot.X = 16;
						dot.Y = 0;
						SetConsoleCursorPosition(hConsole, dot);
						SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
						cout << schet1 << " : " << schet2;
						dot.X = 0;
						dot.Y = 1;
						SetConsoleCursorPosition(hConsole, dot);
						cout << "                                                                    ";
						dot.X = 54;
						dot.Y = 0;
						SetConsoleCursorPosition(hConsole, dot);
					}
				}
				else if (prevmask[y][x] == 8 || prevmask[y][x] == 9)
				{
					flag = 0;
					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 8; j++) mask[i][j] = 0;
					}
					mask[y][x] = 1;

					xv = -1; yv = -1;
					if (curhod == 0) curhod = 1;
					else curhod = 0;

					HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
					dot.X = 0;
					dot.Y = 0;
					SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
					SetConsoleCursorPosition(hConsole, dot);
					if (choicegame == 0)
					{
						if (curhod == 0)
						{
							cout << "Ваш ход";
						}
						else if (curhod == 1)
						{
							cout << "Ход ПК ";
						}
					}
					else if (choicegame == 1)
					{
						if (curhod == 0)
						{
							cout << "Ход игрока №1";
						}
						else if (curhod == 1)
						{
							cout << "Ход игрока №2";
						}
					}
				}
			}
			else if (cur == 27)
			{
				endgame();
				return 0;
			}
			else if (cur == 48) // Смена хода
			{
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++) mask[i][j] = 0;
				}
				mask[y][x] = 1;
				xv = -1; yv = -1;
				flag = 0;
				if (curhod == 0) curhod = 1;
				else curhod = 0;

				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				dot.X = 0;
				dot.Y = 0;
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				SetConsoleCursorPosition(hConsole, dot);
				if (choicegame == 0)
				{
					if (curhod == 0)
					{
						cout << "Ваш ход";
					}
					else if (curhod == 1)
					{
						cout << "Ход ПК ";
					}
				}
				else if (choicegame == 1)
				{
					if (curhod == 0)
					{
						cout << "Ход игрока №1";
					}
					else if (curhod == 1)
					{
						cout << "Ход игрока №2";
					}
				}

				dot.X = 54;
				dot.Y = 0;
				SetConsoleCursorPosition(hConsole, dot);
			}
			else if (cur == 49) // Перерисовка экрана
			{
				system("color F0");
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				draw();
			}
			else if (cur == 50) // Включение/отключение подсказок
			{
				if (podzk == 0) podzk = 1;
				else podzk = 0;
				dot.X = 0;
				dot.Y = 2;
				SetConsoleCursorPosition(hConsole, dot);
				draw();
			}
			else if (cur == 51) // Инфо
			{
				system("cls");
				system("color F0");
				Rectangle(white, 0, 0, 720, 770);
				cout << "Игра <<Уголки>>\n\n";
				cout << "Управление курсором осуществляется с помощью стрелочек;\n";
				cout << "Чтобы выбрать шашку и сделать ход нажмите ENTER;\n";
				cout << "Если вы изменили размер окна и картинка сбилась, нажмите 1;\n";
				cout << "Пропустить ход можно с помощью клавиши 0;\n";
				cout << "Отключить и включить подсказки можно с помощью клавиши 2;\n";
				cout << "Вы в любой момент можете выйти из игры, нажав Esc;\n\n";
				cout << "Правила игры:\n";
				cout << "Цель игры - быстрее соперника провести шашки в противоположный\n";
				cout << "по диагонали угол;\n";
				cout << "Можно ходить на одну клетку или прыжками через шашки;\n";
				cout << "Если противник вывел все шашки из своего поля, то и вы\n";
				cout << "обязаны вывести все свои шашки;\n";
				cout << "Нельзя полностью блокировать ход противнику;\n\n";
				cout << "Слева сверху отображается строка, сообщающая чей сейчас ход;\n";
				cout << "Далее отображается счет и подсказки;\n\n";
				cout << "Иванов Дмитрий 1-41хх 2021г.\n\n";
				system("pause");
				dot.X = 0;
				dot.Y = 0;
				SetConsoleCursorPosition(hConsole, dot);
				system("cls");
				system("color F0");
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				cout << "                         Инфо - 3, выйти из игры - Esc\n\n";

				dot.X = 0;
				dot.Y = 0;
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				SetConsoleCursorPosition(hConsole, dot);
				if (choicegame == 0)
				{
					if (curhod == 0)
					{
						cout << "Ваш ход";
					}
					else if (curhod == 1)
					{
						cout << "Ход ПК ";
					}
				}
				else if (choicegame == 1)
				{
					if (curhod == 0)
					{
						cout << "Ход игрока №1";
					}
					else if (curhod == 1)
					{
						cout << "Ход игрока №2";
					}
				}

				dot.X = 16;
				dot.Y = 0;
				SetConsoleCursorPosition(hConsole, dot);
				cout << schet1 << " : " << schet2;
				SetConsoleCursorPosition(hConsole, dot);
				draw();
			}

			redraw();

			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					prevmask[i][j] = mask[i][j];
				}
			}
		}
		else
		{
			// ХОД ПК
			maxs = -100;

			poisk1(pole);

			swap(pole[yn1][xn1], pole[yk1][xk1]);
			dot.X = 0;
			dot.Y = 1;
			SetConsoleCursorPosition(hConsole, dot);
			//cout << yn1 + 1 << " " << xn1 + 1 << "  " << yk1 + 1 << " " << xk1 + 1 << "  " << maxs << "    ";


			if ((yn1 + xn1) % 2 == 0) Rectangle(white, kletk[yn1][xn1].x, kletk[yn1][xn1].y, kletk[yn1][xn1].xk, kletk[yn1][xn1].yk);
			else Rectangle(black, kletk[yn1][xn1].x, kletk[yn1][xn1].y, kletk[yn1][xn1].xk, kletk[yn1][xn1].yk);

			RoundRect(blue, shask[yk1][xk1].x, shask[yk1][xk1].y, shask[yk1][xk1].xk, shask[yk1][xk1].yk, 60, 60);

			dot.X = 0;
			dot.Y = 0;
			SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
			SetConsoleCursorPosition(hConsole, dot);
			cout << "Ваш ход";
			curhod = 0;

			dot.X = 54;
			dot.Y = 0;
			SetConsoleCursorPosition(hConsole, dot);

			int sum2 = 0; // Проверка на победу
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (pole[i][j] == 2) sum2++;
				}
			}
			if (sum2 == 12)
			{
				schet2++;
				dot.X = 0;
				dot.Y = 1;
				SetConsoleCursorPosition(hConsole, dot);
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				cout << "ПК выйграл! ";
				cout << "Чтобы начать новую игру нажмите ENTER!";
				while (1)
				{
					cur = _getch();
					if (cur == 13)
					{
						for (int i = 0; i < 8; i++)
						{
							for (int j = 0; j < 8; j++)
							{
								pole[i][j] = 0;
								mask[i][j] = 0;
							}
						}
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 4; j++)
							{
								pole[i][j] = 1;
								pole[7 - i][7 - j] = 2;
							}
						}
						sum2 = 1;
						draw();
						x = 0;
						y = 0;
					}
					else if (cur == 27)
					{
						endgame();
						return 0;
					}
					if (sum2 == 1) break;
				}
				dot.X = 16;
				dot.Y = 0;
				SetConsoleCursorPosition(hConsole, dot);
				SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
				cout << schet1 << " : " << schet2;
				dot.X = 0;
				dot.Y = 1;
				SetConsoleCursorPosition(hConsole, dot);
				cout << "                                                                    ";
				dot.X = 54;
				dot.Y = 0;
				SetConsoleCursorPosition(hConsole, dot);
			}
		}
	}
	// Главный вывод
}

