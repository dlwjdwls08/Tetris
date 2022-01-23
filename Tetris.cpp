#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <future>
#include <random>
#include <time.h>
#include <conio.h>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

using namespace std;

//https://t1.daumcdn.net/cfile/tistory/113482444EBBB18E1A
//블록 회전

//https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.shutterstock.com%2Fko%2Fimage-vector%2Fpixel-font-214727482&psig=AOvVaw131rGiFjmKxLrHMUuigawC&ust=1642435690247000&source=images&cd=vfe&ved=0CAsQjRxqFwoTCMiCmvHTtvUCFQAAAAAdAAAAABAJ
//픽셀 알파벳 주소

//https://www.google.com/url?sa=i&url=https%3A%2F%2Fshaeod.tistory.com%2F228&psig=AOvVaw3rGMZS5b8CNgHhuGOavT6q&ust=1642436212383000&source=images&cd=vfe&ved=0CAsQjRxqFwoTCLiTpOvVtvUCFQAAAAAdAAAAABAD
//아스키 코드 주소

char blank = '0';
char wall = '8';
char player = 'O';
char brick = '@';

char Pblank[3] = " .";
char Pwall[3] = "OO";
char Pplayer[3] = "[]";
char Pbrick[3] = "[]";

char map[1000][1000];

int colormap[1000][1000] = { 0 };

long long int Time = 0;

int x = 1;
int y = 1;

int xdir = 0;
int ydir = 0;
double dropsec = 1;

int width = 20;//벽 미포함
int height = 20;//벽 미포함

int Oblock[1][4][2] = { {{-1,0},{0,0},{-1,1},{0,1} } };
int Iblock[2][4][2] = { {{-2,0},{-1,0},{0,0},{1,0}}, {{0,-1},{0,0},{0,1},{0,2}} };
int Sblock[2][4][2] = { {{0,0},{1,0},{-1,1},{0,1}}, {{-1,0},{0,0},{1,0},{1,1}} };
int Zblock[2][4][2] = { {{-1,0},{0,0},{0,1},{1,1}}, {{1,-1},{1,0},{0,0},{0,1}} };
int Lblock[4][4][2] = { {{-1,0},{0,0},{1,0},{-1,1}}, {{0,-1},{0,0},{0,1},{1,1}}, {{1,-1},{-1,0},{0,0},{1,0}}, {{-1,-1},{0,-1},{0,0},{0,1}} };
int Jblock[4][4][2] = { {{-1,0},{0,0},{1,0},{1,1}}, {{0,-1},{1,-1},{0,0},{0,1}}, {{-1,-1},{-1,0},{0,0},{1,0}}, {{0,-1},{0,0},{-1,1},{0,1}} };
int Tblock[4][4][2] = { {{-1,0},{0,0},{1,0},{0,1}}, {{0,-1},{0,0},{1,0},{0,1}}, {{0,-1},{-1,0},{0,0},{1,0}}, {{0,-1},{-1,0},{0,0},{0,1}} };

int _Blocks[7][4][4][2] = {
	{ {{-1,0},{0,0},{-1,1},{0,1} } },//Oblock
	{ {{-2,0},{-1,0},{0,0},{1,0}}, {{0,-1},{0,0},{0,1},{0,2}} },//Iblock
	{ {{0,0},{1,0},{-1,1},{0,1}}, {{0,-1},{0,0},{1,0},{1,1}} },//Sblock
	{ {{-1,0},{0,0},{0,1},{1,1}}, {{1,-1},{1,0},{0,0},{0,1}} },//Zblock
	{ { {-1,0},{0,0},{1,0},{-1,1}}, {{0,-1},{0,0},{0,1},{1,1}}, {{1,-1},{-1,0},{0,0},{1,0}}, {{-1,-1},{0,-1},{0,0},{0,1}} },//Lblock
	{ {{-1,0},{0,0},{1,0},{1,1}}, {{0,-1},{1,-1},{0,0},{0,1}}, {{-1,-1},{-1,0},{0,0},{1,0}}, {{0,-1},{0,0},{-1,1},{0,1}} },//Jblock
	{ {{-1,0},{0,0},{1,0},{0,1}}, {{0,-1},{0,0},{1,0},{0,1}}, {{0,-1},{-1,0},{0,0},{1,0}}, {{0,-1},{-1,0},{0,0},{0,1}} }//Tblock

};

int _BlockTypeCnt[7] = { 1,2,2,2,4,4,4 };

//https://tetris.wiki/Scoring Applied Sega Scoring System
int scoring[5] = { 0, 100, 200, 900, 2000 };

int score = 0;

int bestscore = 0;

int level = 1;

int removedlinecnt = 0;

int totalline = 0;

int mode = 0;

char modename[10][10] = { "Normal", "Endless" };

char _REMOVER[50][200] = {
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
	"                                                                                                                                                                                                   \n",
};

int GetLevelScore(void)
{
	if (level <= 7)
	{
		if (level <= 5)
		{
			if (level <= 3)
			{
				if (level <= 1)
					return 1;
				return 2;
			}
			return 2;
		}
		return 4;
	}
	return 5;
}

void MapInit(int width, int height)
{
	for (int i = 0; i < height + 2; i++)
	{
		for (int j = 0; j < width + 2; j++)
		{
			if (i == 0 || j == 0 || i == height + 1 || j == width + 1) map[i][j] = wall;
			else map[i][j] = blank;
		}
	}
}

void ClearScreen(void)
{
	printf("\x1B[H\x1B[3J");
	//for (int i = 0; i < 50; i++) printf("%s", _REMOVER[i]);
	//printf("\x1B[H");
}

enum {
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKYBLUE,
	DARK_RED,
	DARK_VOILET,
	DARK_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};

unsigned short color[7] = { YELLOW, SKYBLUE, GREEN, RED, DARK_YELLOW, BLUE, VIOLET };

void SetColor(unsigned short type)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), type);
}

class Block {
private:
	int type;
	int nowform = 0;
	int nowpos[2] = { 0 };
	int isbrick = 0;
public:
	Block() {}
	void BlockSet(int type, int x, int y)
	{
		this->type = type;
		this->nowpos[0] = x, this->nowpos[1] = y;
		if (this->Movable(0, 0)) this->Draw(player);
	}
	void Draw(char drawThing)
	{
		for (int i = 0; i < 4; i++)
		{
			int X = this->nowpos[0] + _Blocks[this->type][this->nowform][i][0];
			int Y = this->nowpos[1] + _Blocks[this->type][this->nowform][i][1];
			map[Y][X] = drawThing;
		}
	}
	int Rotatable(int rot)
	{
		int nextform = this->nowform + rot;
		if (nextform >= _BlockTypeCnt[this->type]) nextform = 0;
		else if (nextform < 0) nextform = _BlockTypeCnt[this->type] - 1;
		//0
		int pass = 1;
		for (int i = 0; i < 4; i++)
		{
			int X = this->nowpos[0] + _Blocks[this->type][nextform][i][0];
			int Y = this->nowpos[1] + _Blocks[this->type][nextform][i][1];
			if (!(X <= 0 || Y <= 0 || X >= width + 1 || Y >= height + 1 || (map[Y][X] != blank && map[Y][X] != player)))
			{
				continue;
			}
			else
			{
				pass = 0;
				break;
			}
		}
		if (pass) return 0;
		//1
		this->nowpos[0]++;
		pass = 1;
		for (int i = 0; i < 4; i++)
		{
			int X = this->nowpos[0] + _Blocks[this->type][nextform][i][0];
			int Y = this->nowpos[1] + _Blocks[this->type][nextform][i][1];
			if (!(X <= 0 || Y <= 0 || X >= width + 1 || Y >= height + 1 || (map[Y][X] != blank && map[Y][X] != player)))
			{
				continue;
			}
			else
			{
				pass = 0;
				break;
			}
		}
		if (pass)
		{
			this->nowpos[0] -= 1;
			return 1;
		}
		//2
		this->nowpos[0]++;
		pass = 1;
		for (int i = 0; i < 4; i++)
		{
			int X = this->nowpos[0] + _Blocks[this->type][nextform][i][0];
			int Y = this->nowpos[1] + _Blocks[this->type][nextform][i][1];
			if (!(X <= 0 || Y <= 0 || X >= width + 1 || Y >= height + 1 || (map[Y][X] != blank && map[Y][X] != player)))
			{
				continue;
			}
			else
			{
				pass = 0;
				break;
			}
		}
		if (pass)
		{
			this->nowpos[0] -= 2;
			return 2;
		}
		//-1
		this->nowpos[0] -= 3;
		pass = 1;
		for (int i = 0; i < 4; i++)
		{
			int X = this->nowpos[0] + _Blocks[this->type][nextform][i][0];
			int Y = this->nowpos[1] + _Blocks[this->type][nextform][i][1];
			if (!(X <= 0 || Y <= 0 || X >= width + 1 || Y >= height + 1 || (map[Y][X] != blank && map[Y][X] != player)))
			{
				continue;
			}
			else
			{
				pass = 0;
				break;
			}
		}
		if (pass)
		{
			this->nowpos[0] -= -1;
			return -1;
		}
		//-2
		this->nowpos[0]--;
		pass = 1;
		for (int i = 0; i < 4; i++)
		{
			int X = this->nowpos[0] + _Blocks[this->type][nextform][i][0];
			int Y = this->nowpos[1] + _Blocks[this->type][nextform][i][1];
			if (!(X <= 0 || Y <= 0 || X >= width + 1 || Y >= height + 1 || (map[Y][X] != blank && map[Y][X] != player)))
			{
				continue;
			}
			else
			{
				pass = 0;
				break;
			}
		}
		if (pass)
		{
			this->nowpos[0] -= -2;
			return -2;
		}

		this->nowpos[0] += 2;
		return -10;
	}
	void Rotate(int rot)
	{
		int value = this->Rotatable(rot);
		if (value == -10) return;
		//this->nowform++;
		this->Draw(blank);
		this->nowpos[0] += value;
		this->nowform = (this->nowform + rot);
		if (this->nowform >= _BlockTypeCnt[this->type]) this->nowform = 0;
		else if (this->nowform < 0) this->nowform = _BlockTypeCnt[this->type] - 1;
		this->Draw(player);
	}
	int Movable(int xdir, int ydir)
	{
		for (int i = 0; i < 4; i++)
		{
			int X = this->nowpos[0] + _Blocks[this->type][this->nowform][i][0] + xdir;
			int Y = this->nowpos[1] + _Blocks[this->type][this->nowform][i][1] + ydir;
			if (X <= 0 || Y <= 0 || X >= width + 1 || Y >= height + 1 || (map[Y][X] != blank && map[Y][X] != player))
			{
				return 0;
			}
		}
		return 1;
	}
	int Move(int xdir, int ydir)
	{
		if (!this->Movable(xdir, ydir)) return 0;
		this->Draw(blank);
		nowpos[0] += xdir;
		nowpos[1] += ydir;
		this->Draw(player);
		return 1;
	}
	void BeBrick(void)
	{
		this->Draw(brick);
		this->~Block();
	}
	int IsBrick(void) { return this->isbrick; }
	int GetType(void) { return this->type; }
	int* GetPos(void) { return this->nowpos; }
	~Block() { this->isbrick = 1; }
};

//int abs(int p) { return p < 0 ? -p : p; }

int testvalue = 0;

Block b = Block();

int nextblock = -1;

char NextBlockBox[10][20] = {
	"--------------",
	"|000000|",
	"|000000|",
	"|000000|",
	"|000000|",
	"--------------"
};

char _TETRIS[20][100] = {
	"          *                  *                      *                        \n",
	"                   *                      *                   *              \n",
	"                                                                             \n",
	"        @@@@@@@    @@@@@@@    @@@@@@@    @@@@@@    @@@@@     @@@@@   *       \n",
	"  *        @       @             @       @     @     @      @                \n",
	"           @       @@@@@@@       @       @@@@@@      @       @@@@@           \n",
	"           @       @             @       @     @     @            @    *     \n",
	"           @       @             @       @     @     @      @     @          \n",
	"      *    @       @@@@@@@       @       @     @   @@@@@     @@@@@           \n",
	"                                                                             \n",
	"                                  *                                          \n",
	"                                                         *                   \n",
	"                      *                                                 *    \n",
	"             *                *              *                               \n"
};

int _TETRISLEN = 20;

char _GAMEOVER[20][120] = {
	"              *                                         *          *                                     \n",
	" *                                               *                                               *       \n",
	"                           *                                                                             \n",
	"         @@@@@@     @@@@@     @     @    @@@@@@@          @@@@@     @     @    @@@@@@@    @@@@@@         \n",
	"   *    @          @     @    @ @ @ @    @               @     @    @     @    @          @     @        \n",
	"        @   @@@    @     @    @  @  @    @@@@@@@         @     @    @     @    @@@@@@@    @@@@@@      *  \n",
	"        @     @    @@@@@@@    @     @    @               @     @     @   @     @          @     @        \n",
	"    *   @     @    @     @    @     @    @               @     @      @ @      @          @     @        \n",
	"         @@@@@@    @     @    @     @    @@@@@@@          @@@@@        @       @@@@@@@    @     @        \n",
	"                                                                                                         \n",
	"             *                                     *                                                     \n",
	"                                                                                    *                    \n",
	"                            *                                                                      *     \n",
	"    *                                       *                      *                                     \n"
};

int _GAMEOVERLEN = 20;

char _CONTROLHINT[20][100] = {
	"   R / L Arrow  : Move Left and Right\n",
	"    Up Arrow    : Spin Block\n",
	"   Down  Arrow  : Soft Drop(Drop one time)\n",
	"    Space Key   : Hard Drop(Drop to bottom at once)\n\n",
	"   Bonus Points for Hard Drops and Multiple Removes!\n"
};

int _CONTROLHINTLEN = 20;

char _INGAMECONTROL[20][100] = {
	"      Q key     : Quit Game\n",
	"      P key     : Pause Game\n",
};

int _INGAMECONTROLLEN = 5;

char _SETTINGS[20][120] = {
	"                      *                             *                    *                        \n",
	"         *                            *                                                     *     \n",
	"               *                                           *                                      \n",
	"         @@@@@     @@@@@@@    @@@@@@@    @@@@@@@    @@@@@    @      @     @@@@@@     @@@@@         \n",
	" *      @          @             @          @         @      @ @    @    @          @              \n",
	"         @@@@@     @@@@@@@       @          @         @      @  @   @    @   @@@     @@@@@    *    \n",
	"              @    @             @          @         @      @   @  @    @     @          @        \n",
	"        @     @    @             @          @         @      @    @ @    @     @    @     @        \n",
	"    *    @@@@@     @@@@@@@       @          @       @@@@@    @      @     @@@@@@     @@@@@         \n",
	"                                    *                                                             \n",
	"                                                    *                                     *       \n",
	"            *                              *                         *                            \n",
	"                          *                              *                      *                 \n",
};

int _SETTINGSLEN = 20;

char _SETTINGSHINT[20][120] = {
	" Up / Down Arrow : Change Slot\n",
	"   R / L Arrow   : Change Value\n",
};

int _SETTINGSHINTLEN = 5;

char _PAUSE[20][120] = {
	"           *                *                       *              \n",
	"              *                          *                  *      \n",
	"     *                              *                              \n",
	"        @@@@@@      @@@@@     @     @     @@@@@     @@@@@@@        \n",
	"        @     @    @     @    @     @    @          @            * \n",
	"        @     @    @     @    @     @     @@@@@     @@@@@@@        \n",
	" *      @@@@@@     @@@@@@@    @     @          @    @              \n",
	"        @          @     @    @     @    @     @    @         *    \n",
	"        @          @     @     @@@@@      @@@@@     @@@@@@@        \n",
	"    *          *                                                   \n",
	"                                *                        *         \n",
	"                     *                        *                    \n",
	"     *                                    *           *            \n",
};

int _PAUSELEN = 20;

char _LEVELUP[20][120] = {
	"                                  *               *                                 *              \n",
	"              *                       *                                *                     *     \n",
	"                                                             *                                     \n",
	"  *     @          @@@@@@@    @     @    @@@@@@@    @               @     @    @@@@@@     @        \n",
	"        @          @          @     @    @          @               @     @    @     @    @        \n",
	"        @          @@@@@@@    @     @    @@@@@@@    @               @     @    @     @    @        \n",
	"        @          @           @   @     @          @               @     @    @@@@@@     @     *  \n",
	"        @          @            @ @      @          @               @     @    @                   \n",
	"     *  @@@@@@@    @@@@@@@       @       @@@@@@@    @@@@@@@          @@@@@     @          @        \n",
	"                             *                                                                     \n",
	"   *  s                                           *                    *                 *          \n",
	"                                     *                       *                                     \n",
	"                 *                           *                            *                        \n"
};

int _LEVELUPLEN = 20;

char _OPTION[20][120] = {
	"                                 *                                  *                 * \n",
	"    *                                                *                                  \n",
	"                            *                                                    *      \n",
	"         @@@@@     @@@@@@     @@@@@@@    @@@@@     @@@@@     @      @     @@@@@         \n",
	" *      @     @    @     @       @         @      @     @    @ @    @    @              \n",
	"        @     @    @     @       @         @      @     @    @  @   @     @@@@@      *  \n",
	"        @     @    @@@@@@        @         @      @     @    @   @  @          @        \n",
	"        @     @    @             @         @      @     @    @    @ @    @     @        \n",
	"     *   @@@@@     @             @       @@@@@     @@@@@     @      @     @@@@@         \n",
	"                                                                                        \n",
	"                                              *                 *               *       \n",
	"                      *                               *                                 \n",
	"          *                             *                               *               \n",
};

int _OPTIONLEN = 20;

char _CONGRATULATIONS[20][200] = {
	"                             *                                   *                                     *                                  *                                      *      \n",
	"     *                                       *            *                           *                         *           *          *                             *                  \n",
	"                 *                                                      *                          *                                                    *                       *       \n",
	"  *      @@@@@      @@@@@     @      @     @@@@@@    @@@@@@      @@@@@     @@@@@@@    @     @    @           @@@@@     @@@@@@@    @@@@@     @@@@@     @      @     @@@@@     @          \n",
	"        @     @    @     @    @ @    @    @          @     @    @     @       @       @     @    @          @     @       @         @      @     @    @ @    @    @          @          \n",
	"        @          @     @    @  @   @    @   @@@    @@@@@@     @     @       @       @     @    @          @     @       @         @      @     @    @  @   @     @@@@@     @      *   \n",
	"        @          @     @    @   @  @    @     @    @     @    @@@@@@@       @       @     @    @          @@@@@@@       @         @      @     @    @   @  @          @    @          \n",
	"    *   @     @    @     @    @    @ @    @     @    @     @    @     @       @       @     @    @          @     @       @         @      @     @    @    @ @    @     @               \n",
	"         @@@@@      @@@@@     @      @     @@@@@@    @     @    @     @       @        @@@@@     @@@@@@@    @     @       @       @@@@@     @@@@@     @      @     @@@@@     @  *       \n",
	"   *                       *                                             *                                                                         *                                    \n",
	"        *                                    *                                                    *                *                                            *                       \n",
	"                   *                  *                       *                     *                                     *               *                              *            * \n",
};

int _CONGRATULATIONSLEN = 20;

void BlockChange(void)
{
	b = Block();
	b.BlockSet(nextblock, width / 2 + 1, 1);
	nextblock = rand() % 7;
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 6; j++)
		{
			NextBlockBox[i][j] = blank;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		NextBlockBox[2 + _Blocks[nextblock][0][i][1]][4 + _Blocks[nextblock][0][i][0]] = player;
	}
}

int NowTextColorType = 0;//0 == 기본(흰색)  1 == 플레이어 색

int ColorOn = 1;

clock_t LvUptime = 0;

void PrintMap(void)
{
	ClearScreen();
	printf("\n\n\n");
	for (int i = 0; i < height + 2; i++)
	{
		printf("          ");
		//printf("%s", map[i]);
		for (int j = 0; j < width + 2; j++)
		{
			if (map[i][j] == player && !NowTextColorType && ColorOn)
			{
				SetColor(color[b.GetType()]);
				NowTextColorType = 1;
			}
			else if (map[i][j] != player && NowTextColorType && ColorOn)
			{
				SetColor(WHITE);
				NowTextColorType = 0;
			}
			if (map[i][j] == player) printf("%s", Pplayer);
			else if (map[i][j] == brick) printf("%s", Pbrick);
			else if (map[i][j] == blank) printf("%s", Pblank);
			else if (map[i][j] == wall) printf("%s", Pwall);
			else printf("%c", map[i][j]);
		}
		if (i == 0)
		{
			printf("          Next Block");
		}
		else if (i >= 1 && i <= 6)
		{
			//printf("          %s", NextBlockBox[i - 1]);
			printf("       ");
			for (int j = 0; j < 20; j++)
			{
				if (NextBlockBox[i - 1][j] == player && !NowTextColorType && ColorOn)
				{
					SetColor(color[nextblock]);
					NowTextColorType = 1;
				}
				else if (NextBlockBox[i - 1][j] != player && NowTextColorType && ColorOn)
				{
					SetColor(WHITE);
					NowTextColorType = 0;
				}
				//if (NextBlockBox[i - 1][j] == player) printf("%c%c", player);
				//else printf("%c", NextBlockBox[i - 1][j]);
				//printf("%c", NextBlockBox[i - 1][j]);
				if (NextBlockBox[i - 1][j] == player) printf("%s", Pplayer);
				else if (NextBlockBox[i - 1][j] == brick) printf("%s", Pbrick);
				else if (NextBlockBox[i - 1][j] == blank) printf("%s", Pblank);
				else if (NextBlockBox[i - 1][j] == wall) printf("%s", Pwall);
				else printf("%c", NextBlockBox[i - 1][j]);
			}
		}
		else if (i == 9)
		{
			printf("          SCORE : %d", score);
		}
		else if (i == 10)
		{
			printf("     BEST SCORE : %d", bestscore);
		}
		else if (i == 12)
		{
			printf("          LINES : %d", totalline);
		}
		else if (i == 14)
		{
			printf("          LEVEL : %d %s", level, mode == 0 ? "/ 20" : "");
		}
		else if (i == 15)
		{
			if (clock() - LvUptime <= 3000 && LvUptime) printf("       L E V E L   U P !");
			else printf("                        ");
		}
		printf("\n");

	}
	printf("\n\n");
	for (int i = 0; i < _CONTROLHINTLEN; i++) printf("%s", _CONTROLHINT[i]);
	for (int i = 0; i < _INGAMECONTROLLEN; i++) printf("%s", _INGAMECONTROL[i]);
	//printf("%d %d %d<-\nScore : %d", x, y, testvalue, score);
}

int Wait = 1;

void Destroy(void)
{
	int cnt = 0;
	for (int i = height; i >= 1; i--)
	{
		int n = 0;
		for (int j = 1; j <= width; j++)
		{
			if (map[i][j] == brick) n++;
		}
		if (n == 0) break;
		if (n == width)
		{
			cnt++;
			for (int j = i - 1; i >= 1; j--)
			{
				int m = 0;
				for (int k = 1; k <= width; k++)
				{
					if (map[j][k] == blank) m++;
					map[j + 1][k] = map[j][k];
				}
				if (m == width) break;
			}
			i++;
		}
	}
	int mul = 10;
	for (int i = 1; i <= width; i++)
	{
		if (map[height][i] == brick)
		{
			mul = 1;
			break;
		}
	}
	removedlinecnt += cnt;
	totalline += cnt;
	score += scoring[cnt] * GetLevelScore() * mul;
}

int lastcol = 0;
int lastline = 0;

void Resize(int col = lastcol, int line = lastline, const char* title = NULL)
{
	char buffer[100];
	lastcol = col;
	lastline = line;
	if (title == NULL)
	{
		sprintf(buffer, "mode con: cols=%d lines=%d", col, line);
	}
	else
	{
		sprintf(buffer, "mode con: cols=%d lines=%d | title %s", col, line, title);
	}
	system(buffer);
}

void cursor(int n) // 커서 보이기 & 숨기기
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void PrintStartMenu(int p)
{
	ClearScreen();
	for (int i = 0; i < _TETRISLEN; i++) printf("  %s", _TETRIS[i]);
	printf("\n\n");
	printf("                                Press Enter To Start                             \n\n\n");
	printf("                                  Best Score : %d\n\n\n", bestscore);

	printf("                  ");

	if (p == 0) printf("> ");
	else printf("  ");
	printf("Start Game");
	if (p == 0) printf(" <");
	else printf("  ");

	printf("                   ");

	if (p == 1) printf("> ");
	else printf("  ");
	printf("Start Tutorial");
	if (p == 1) printf(" <");
	else printf("  ");

	printf("\n\n");

	printf("                                   ");

	if (p == 2) printf("> ");
	else printf("  ");
	printf("Quit Game");
	if (p == 2) printf(" <");
	else printf("  ");

	printf("\n\n\n");

	for (int i = 0; i < _CONTROLHINTLEN; i++) printf("                  %s", _CONTROLHINT[i]);
	printf("\n                                                                     made by Lee JJ");
}

int StartMenu(void)
{
	Resize(86, 40, "테트리스 게임");
	while (GetAsyncKeyState(VK_RETURN)) 1;
	int p = 0;
	int maxp = 2;
	int check[5] = { 0 };
	int beforep = 0;
	PrintStartMenu(p);
	while (1)
	{
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !check[0] && p != 1)
		{
			if (p == 2) p = 0;
			p += 1;
			check[0] = 1;
			PrintStartMenu(p);
		}
		else if (!GetAsyncKeyState(VK_RIGHT)) check[0] = 0;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !check[1] && p > 0)
		{
			if (p == 2) p = 1;
			p -= 1;
			check[1] = 1;
			PrintStartMenu(p);
		}
		else if (!GetAsyncKeyState(VK_LEFT)) check[1] = 0;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && !check[2] && p <= 1)
		{
			beforep = p;
			p = 2;
			check[2] = 1;
			PrintStartMenu(p);
		}
		else if (!GetAsyncKeyState(VK_DOWN)) check[2] = 0;
		if (GetAsyncKeyState(VK_UP) & 0x8000 && !check[3] && p == 2)
		{
			p = beforep;
			check[3] = 1;
			PrintStartMenu(p);
		}
		else if (!GetAsyncKeyState(VK_UP)) check[3] = 0;
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (score > bestscore)  bestscore = score;
			return p;
		}
	}
}

void PrintSettingMenu(int pointing)
{
	char interval[100] = "                                 ";
	ClearScreen();
	printf("a");
	Resize(100, 40);
	for (int i = 0; i < _SETTINGSLEN; i++) printf("%s", _SETTINGS[i]);
	printf("\n\n\n\n");
	//Width
	printf("%s     ", interval);
	if (pointing == 0) printf(">  ");
	else printf("   ");
	printf("Map Width");
	if (pointing == 0) printf("  <");
	else printf("   ");
	printf("\n");
	printf("%s         ", interval);
	if (width > 8) printf("< ");
	else printf("  ");
	printf("%d", width);
	if (width < 100) printf(" >");
	printf("\n\n\n\n");
	//Height
	printf("%s     ", interval);
	if (pointing == 1) printf(">  ");
	else printf("   ");
	printf("Map Height");
	if (pointing == 1) printf("  <");
	else printf("   ");
	printf("\n");
	printf("%s         ", interval);
	if (height > 15) printf("< ");
	else printf("  ");
	printf("%d", height);
	if (height < 50) printf(" >");
	printf("\n\n\n\n");
	//Duration
	printf("%s ", interval);
	if (pointing == 2) printf(">  ");
	else printf("   ");
	printf("Auto Drop Duration");
	if (pointing == 2) printf("  <");
	else printf("   ");
	printf("\n");
	printf("%s         ", interval);
	if (dropsec > 0.11) printf("< ");
	else printf("  ");
	printf("%.1lf", dropsec);
	printf(" >");
	printf("\n\n\n\n");
	//Color
	printf("%s     ", interval);
	if (pointing == 3) printf(">  ");
	else printf("   ");
	printf("Block Color");
	if (pointing == 3) printf("  <");
	else printf("   ");
	printf("\n");
	printf("%s         ", interval);
	if (!ColorOn) printf("< ");
	else printf("  ");
	printf("%s", ColorOn ? "On" : "Off");
	if (ColorOn) printf(" >");
	else printf("  ");
	printf("\n\n\n\n");
	for (int i = 0; i < _SETTINGSHINTLEN; i++) printf("%s%s", interval, _SETTINGSHINT[i]);
}

void SettingMenu(void)
{
	int pointing = 0;
	int maxpoint = 3;
	PrintSettingMenu(pointing);
	int check[10] = { 0 };
	while (1)
	{
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && pointing < maxpoint && !check[0])
		{
			pointing++;
			PrintSettingMenu(pointing);
			check[0] = 1;
		}
		else if (!GetAsyncKeyState(VK_DOWN)) check[0] = 0;
		if (GetAsyncKeyState(VK_UP) & 0x8000 && pointing > 0 && !check[1])
		{
			pointing--;
			PrintSettingMenu(pointing);
			check[1] = 1;
		}
		else if (!GetAsyncKeyState(VK_UP)) check[1] = 0;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !check[2])
		{
			if (pointing == 0 && width > 8)
			{
				width--;
				PrintSettingMenu(pointing);
			}
			else if (pointing == 1 && height > 15)
			{
				height--;
				PrintSettingMenu(pointing);
			}
			else if (pointing == 2 && dropsec > 0.11)
			{
				dropsec -= 0.1;
				PrintSettingMenu(pointing);
			}
			else if (pointing == 3 && !ColorOn)
			{
				ColorOn = 1;
				PrintSettingMenu(pointing);
			}
			check[2] = 1;
		}
		else if (!GetAsyncKeyState(VK_LEFT)) check[2] = 0;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !check[3])
		{
			if (pointing == 0 && width < 100)
			{
				width++;
				PrintSettingMenu(pointing);
			}
			else if (pointing == 1 && height < 50)
			{
				height++;
				PrintSettingMenu(pointing);
			}
			else if (pointing == 2)
			{
				dropsec += 0.1;
				PrintSettingMenu(pointing);
			}
			else if (pointing == 3 && ColorOn)
			{
				ColorOn = 0;
				PrintSettingMenu(pointing);
			}
			check[3] = 1;
		}
		else if (!GetAsyncKeyState(VK_RIGHT)) check[3] = 0;
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) break;
	}
}

void DefaultSetting(void)
{
	width = 10;
	height = 20;
	dropsec = 1;
}

void PauseMenu(void)
{
	ClearScreen();
	Resize(72, 25);
	for (int i = 0; i < _PAUSELEN; i++) printf("%s", _PAUSE[i]);
	printf("\n\n\n\n\n");
	printf("                 Press P key to return to the game");
	while (GetAsyncKeyState(0x50)) 1;
	while (!GetAsyncKeyState(0x50)) 1;
	Resize(width + 50, height + 20);
}

void LevelUpMenu(void)
{
	ClearScreen();
	Resize(100, 30);
	for (int t = 3; t >= 1; t--)
	{
		for (int i = 0; i < _LEVELUPLEN; i++) printf("%s", _LEVELUP[i]);
		printf("\n\n\n\n");
		printf("                                     Game starts after %d sec", t);
		Sleep(1000);
		ClearScreen();
	}
}

void PrintExitMenu(int pointing)
{
	ClearScreen();
	for (int i = 0; i < _TETRISLEN; i++)
		printf("      %s", _TETRIS[i]);
	printf("\n\n\n\n");
	printf("     ");
	if (pointing == 0) printf("> ");
	else printf("  ");
	printf("Back to the Game");
	if (pointing == 0) printf(" <");
	else printf("  ");

	printf("        ");

	if (pointing == 1) printf("> ");
	else printf("  ");
	printf("Back to the MainMenu");
	if (pointing == 1) printf(" <");
	else printf("  ");

	printf("        ");

	if (pointing == 2) printf("> ");
	else printf("  ");
	printf("Quit the Game");
	if (pointing == 2) printf(" <");
	else printf("  ");
}

int ExitMenu(void)
{
	ClearScreen();
	Resize(90, 35);
	int pointing = 0;
	PrintExitMenu(pointing);
	int check[5] = { 0 };
	int maxpoint = 2;
	while (1)
	{
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !check[0] && pointing < 2)
		{
			pointing += 1;
			check[0] = 1;
			PrintExitMenu(pointing);
		}
		else if (!GetAsyncKeyState(VK_RIGHT)) check[0] = 0;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !check[1] && pointing > 0)
		{
			pointing -= 1;
			check[1] = 1;
			PrintExitMenu(pointing);
		}
		else if (!GetAsyncKeyState(VK_LEFT)) check[1] = 0;
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			return pointing;
		}
	}
}

void TutorialMenu(void)
{
	Resize(width + 50, height + 20);
	ClearScreen();
	printf("         Press Left Key to move left\n\n");
	PrintMap();
	while (1)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) break;
	}
	b.Move(-1, 0);
	ClearScreen();
	printf("         Press Down Key to move down once\n\n");
	PrintMap();
	while (1)
	{
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) break;
	}
	b.Move(0, 1);
	ClearScreen();
	printf("         Press Up Key to rotate block     \n\n");
	PrintMap();
	while (1)
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000) break;
	}
	b.Rotate(1);
	ClearScreen();
	printf("         Press Space Key to move to bottom at once");
	PrintMap();
	while (1)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) break;
	};
	while (b.Move(0, 1)) 1;
	ClearScreen();
	printf("         Perferct!                                 \n\n");
	PrintMap();
	Sleep(2000);
	ClearScreen();
	printf("         P key and Q key is pause and quit each\n");
	printf("         Press Enter to exit tutorial\n\n");
	PrintMap();
	while (1)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) break;
	}
}

void PrintOptionMenu(int p)
{
	ClearScreen();
	for (int i = 0; i < _OPTIONLEN; i++) printf("%s", _OPTION[i]);
	char interval[100] = "                                    ";
	printf("\n\n\n");

	printf("%s   ", interval);
	if (p == 0) printf("> ");
	else printf("  ");
	printf("Level");
	if (p == 0) printf(" <");
	else printf("  ");
	printf("\n");
	printf("%s    ", interval);
	if (level > 1) printf("< ");
	else printf("  ");
	printf("%2d", level);
	if (level < 10) printf(" >");
	else printf("  ");

	printf("\n\n\n");

	printf("%s", interval);
	if (p == 1) printf("> ");
	else printf("  ");
	printf("Block Color");
	if (p == 1) printf(" <");
	else printf("  ");
	printf("\n");
	printf("%s    ", interval);
	if (!ColorOn) printf("< ");
	else printf("  ");
	printf("%s", ColorOn ? "YES" : "NO ");
	if (ColorOn) printf(" >");
	else printf("    ");

	printf("\n\n\n");

	printf("%s   ", interval);
	if (p == 2) printf("> ");
	else printf("  ");
	printf("Mode");
	if (p == 2) printf(" <");
	else printf("  ");
	printf("\n");
	printf("%s  ", interval);
	if (mode > 0) printf("< ");
	else printf("  ");
	printf("%s", modename[mode]);
	if (mode < 1) printf(" >");
	else printf("  ");
}

void OptionMenu(void)
{
	ClearScreen();
	Resize(90, 40);
	int p = 0;
	int maxp = 2;
	int check[5] = { 0 };
	PrintOptionMenu(p);
	while (GetAsyncKeyState(VK_RETURN)) 1;
	while (1)
	{
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && !check[0] && p < maxp)
		{
			check[0] = 1;
			p++;
			PrintOptionMenu(p);
		}
		else if (!GetAsyncKeyState(VK_DOWN)) check[0] = 0;
		if (GetAsyncKeyState(VK_UP) & 0x8000 && !check[1] && p > 0)
		{
			check[1] = 1;
			p--;
			PrintOptionMenu(p);
		}
		else if (!GetAsyncKeyState(VK_UP)) check[1] = 0;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !check[2])
		{
			check[2] = 1;
			if (p == 0 && level > 1)
			{
				level--;
			}
			else if (p == 1 && !ColorOn)
			{
				ColorOn = !ColorOn;
			}
			else if (p == 2 && mode > 0)
			{
				mode--;
			}
			PrintOptionMenu(p);
		}
		else if (!GetAsyncKeyState(VK_LEFT)) check[2] = 0;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !check[3])
		{
			check[3] = 1;
			if (p == 0 && level < 10)
			{
				level++;
			}
			else if (p == 1 && ColorOn)
			{
				ColorOn = !ColorOn;
			}
			else if (p == 2 && mode < 1)
			{
				mode++;
			}
			PrintOptionMenu(p);
		}
		else if (!GetAsyncKeyState(VK_RIGHT)) check[3] = 0;
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			dropsec -= 0.05 * (level - 1);
			if (dropsec < 0.1) dropsec = 0.1;
			return;
		}
	}
}

void PrintGameOverMenu(int p)
{
	ClearScreen();
	for (int i = 0; i < _GAMEOVERLEN; i++) printf("%s", _GAMEOVER[i]);
	printf("\n\n\n\n");

	printf("                                       ");
	printf("Best Score : %d\n\n", bestscore);
	printf("                                       ");
	printf("Your Score : %d", score);
	if (score > bestscore) printf("    New Record!!!");
	printf("\n\n\n\n\n\n\n");
	printf("                       ");

	if (p == 0) printf("> ");
	else printf("  ");
	printf("Back to the MainMenu");
	if (p == 0) printf(" <");
	else printf("  ");

	printf("              ");

	if (p == 1) printf("> ");
	else printf("  ");
	printf("Quit the Game");
	if (p == 1) printf(" <");
	else printf("  ");
}

int GameOverMenu(void)
{
	ClearScreen();
	Resize(110, 35, "테트리스 게임");
	int p = 0;
	int maxp = 1;
	int check[5] = { 0 };
	if (score > bestscore)
	{
		FILE* fp = fopen("./tetris.bin", "wb");
		char buffer[10] = { 0 };
		sprintf(buffer, "%d", score);
		fwrite(buffer, sizeof(buffer), 1, fp);
		fclose(fp);
		//char str[1000] = { 0 };
		//_getcwd(str, 1000);
		//printf("Succes\n%s", str);
		//Sleep(5000);
	}
	PrintGameOverMenu(p);
	while (1)
	{
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !check[0] && p < maxp)
		{
			p += 1;
			check[0] = 1;
			PrintGameOverMenu(p);
		}
		else if (!GetAsyncKeyState(VK_RIGHT)) check[0] = 0;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !check[1] && p > 0)
		{
			p -= 1;
			check[1] = 1;
			PrintGameOverMenu(p);
		}
		else if (!GetAsyncKeyState(VK_LEFT)) check[1] = 0;
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (score > bestscore)  bestscore = score;
			return p;
		}
	}
}

void PrintEndingMenu(int p)
{
	ClearScreen();
	for (int i = 0; i < _CONGRATULATIONSLEN; i++) printf("%s", _CONGRATULATIONS[i]);
	printf("\n\n\n\n");

	printf("                                                                                    ");
	printf("Best Score : %d\n\n", bestscore);
	printf("                                                                                    ");
	printf("Your Score : %d", score);
	if (score > bestscore) printf("    New Record!!!");
	printf("\n\n\n\n\n\n\n");
	printf("                                                              ");

	if (p == 0) printf("> ");
	else printf("  ");
	printf("Back to the MainMenu");
	if (p == 0) printf(" <");
	else printf("  ");

	printf("              ");

	if (p == 1) printf("> ");
	else printf("  ");
	printf("Quit the Game");
	if (p == 1) printf(" <");
	else printf("  ");
}

int EndingMenu(void)
{
	ClearScreen();
	Resize(185, 35, "테트리스 게임");
	int p = 0;
	int maxp = 1;
	int check[5] = { 0 };
	if (score > bestscore)
	{
		FILE* fp = fopen("./tetris.bin", "wb");
		char buffer[10] = { 0 };
		sprintf(buffer, "%d", score);
		fwrite(buffer, sizeof(buffer), 1, fp);
		fclose(fp);
	}
	PrintEndingMenu(p);
	while (1)
	{
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !check[0] && p < maxp)
		{
			p += 1;
			check[0] = 1;
			PrintEndingMenu(p);
		}
		else if (!GetAsyncKeyState(VK_RIGHT)) check[0] = 0;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !check[1] && p > 0)
		{
			p -= 1;
			check[1] = 1;
			PrintEndingMenu(p);
		}
		else if (!GetAsyncKeyState(VK_LEFT)) check[1] = 0;
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (score > bestscore)  bestscore = score;
			return p;
		}
	}
}

int CheckGamOver(void)
{
	for (int i = 1; i <= width; i++)
	{
		if (map[1][i] == brick) return 1;
	}
	return 0;
}

int main()
{
	double a = time(NULL);
	cursor(0);
	srand(time(NULL));
	FILE* fp = fopen("./tetris.bin", "rb");
	if (fp != NULL)
	{
		char buffer[10] = { 0 };
		fread(buffer, sizeof(buffer), 1, fp);
		bestscore = atoi(buffer);
		fclose(fp);
	}
	int StartValue = StartMenu();
	if (StartValue == 2) return 0;
	DefaultSetting();
	Resize(width + 50, height + 20, "테트리스 게임");
	MapInit(width, height);
	nextblock = rand() % 7;
	BlockChange();

	while (StartValue == 1)
	{
		TutorialMenu();
		StartValue = StartMenu();
		if (StartValue == 2) return 0;
		DefaultSetting();
		Resize(width + 50, height + 20, "테트리스 게임");
		MapInit(width, height);
		nextblock = rand() % 7;
		BlockChange();
	}
	OptionMenu();
	ClearScreen();
	Resize(width + 50, height + 20, "테트리스 게임");
	PrintMap();
	int rot = 0;
	int check[10] = { 0 };
	int drop = 0;
	int pause = 0;
	int exit = 0;
	auto before = clock();
	while (1)
	{
		auto now = clock();
		if (removedlinecnt >= 10)
		{
			removedlinecnt -= 10;
			if (mode == 0 && level == 20)
			{
				int value = EndingMenu();
			}
			else
			{
				level++;
				LvUptime = clock();
				if (dropsec > 0.11) dropsec -= 0.05;
				PrintMap();
			}
		}
		if ((double)now - before >= dropsec * 1000)
		{
			before = now;
			if (!b.Move(0, 1))
			{
				b.BeBrick();
				Destroy();
			}
			PrintMap();
		}
		//KeyCheck
		if (1)
		{
			if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !check[0])
			{
				xdir = -1;
				check[0] = 1;//A or Left
			}
			else if (!GetAsyncKeyState(VK_LEFT)) check[0] = 0;
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !check[1])
			{
				xdir = 1;
				check[1] = 1;//D or Right
			}
			else if (!GetAsyncKeyState(VK_RIGHT)) check[1] = 0;
			if (GetAsyncKeyState(VK_DOWN) & 0x8000 && !check[3])
			{
				ydir = 1;
				check[3] = 1;//S or Soft Drop
			}
			else if (!GetAsyncKeyState(VK_DOWN)) check[3] = 0;
			if (GetAsyncKeyState(VK_UP) & 0x8000 && !check[5])
			{
				rot = 1;
				check[5] = 1;//L or turn fore
			}
			else if (!GetAsyncKeyState(VK_UP)) check[5] = 0;
			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !check[6])
			{
				drop = 1;
				check[6] = 1;//Space or Hard Drop
			}
			else if (!GetAsyncKeyState(VK_SPACE)) check[6] = 0;
			if (GetAsyncKeyState(0x50) & 0x8000 && !check[7])
			{
				pause = 1;
				check[7] = 1;//pause
			}
			else if (!GetAsyncKeyState(0x50)) check[7] = 0;
			if (GetAsyncKeyState(0x51) & 0x8000 && !check[8])
			{
				exit = 1;
				check[8] = 1;//exit
			}
			else if (!GetAsyncKeyState(0x51)) check[8] = 0;
			/*if (GetAsyncKeyState(VK_TAB) & 0x8000 && !check[9])
			{
				check[9] = 1;
				removedlinecnt += 1;
				totalline += 1;
			}
			else if (!GetAsyncKeyState(VK_TAB)) check[9] = 0;*/
		}
		//KeyExcute
		if (1)
		{
			if (xdir && abs(xdir) <= 1)//Move L/R
			{
				b.Move(xdir, ydir);
				xdir = 0;
				PrintMap();
			}
			if (ydir)//Move Down
			{
				score += GetLevelScore();
				if (!b.Move(xdir, ydir))
				{
					b.BeBrick();
					Destroy();
				}
				ydir = 0;
				PrintMap();
			}
			if (rot && abs(rot) <= 1)//Rotate
			{
				b.Rotate(rot);
				rot = 0;
				PrintMap();
			}
			if (drop)//Hard Drop
			{
				while (b.Move(0, 1)) score += GetLevelScore();
				b.BeBrick();
				Destroy();
				drop = 0;
				PrintMap();
			}
			if (exit)
			{
				int value = ExitMenu();
				if (value == 1)
				{
					//SettingMenu();
					int StartValue = StartMenu();
					if (StartValue == 2) return 0;
					DefaultSetting();
					Resize(width + 50, height + 20, "테트리스 게임");
					MapInit(width, height);
					nextblock = rand() % 7;
					BlockChange();
					while (StartValue == 1)
					{
						TutorialMenu();
						StartValue = StartMenu();
						if (StartValue == 2) return 0;
						DefaultSetting();
						Resize(width + 50, height + 20, "테트리스 게임");
						MapInit(width, height);
						nextblock = rand() % 7;
						BlockChange();
					}
					OptionMenu();
					ClearScreen();
					Resize(width + 50, height + 20, "테트리스 게임");
					PrintMap();
					rot = 0;
					drop = 0;
					pause = 0;
					exit = 0;
					level = 1;
					score = 0;
					removedlinecnt = 0;
					totalline = 0;
					before = clock();
					continue;
				}
				else if (value == 2)
				{
					return 0;
				}
				else if (value == 0)
				{
					ClearScreen();
					Resize(width + 50, height + 20);
					PrintMap();
					exit = 0;
					before = clock() - now + before;
				}
			}
			if (pause)
			{
				PauseMenu();
				Resize();
				PrintMap();
				pause = 0;
				before = clock() - now + before;
			}
			if (b.IsBrick())
			{
				if (CheckGamOver())
				{
					for (int i = 1; i <= height; i++)
					{
						Sleep(2000 / height);
						for (int j = 1; j <= width; j++)
						{
							map[i][j] = brick;
						}
						PrintMap();
					}
					PrintMap();
					int value = GameOverMenu();
					if (value == 0)
					{
						int StartValue = StartMenu();
						if (StartValue == 2) return 0;
						DefaultSetting();
						Resize(width + 50, height + 20, "테트리스 게임");
						MapInit(width, height);
						nextblock = rand() % 7;
						BlockChange();
						while (StartValue == 1)
						{
							TutorialMenu();
							StartValue = StartMenu();
							if (StartValue == 2) return 0;
							DefaultSetting();
							Resize(width + 50, height + 20, "테트리스 게임");
							MapInit(width, height);
							nextblock = rand() % 7;
							BlockChange();
						}
						OptionMenu();
						ClearScreen();
						Resize(width + 50, height + 20, "테트리스 게임");
						PrintMap();
						rot = 0;
						drop = 0;
						pause = 0;
						exit = 0;
						level = 1;
						score = 0;
						removedlinecnt = 0;
						totalline = 0;
						before = clock();
						continue;
					}
					else if (value == 1)
					{
						return 0;
					}
				}
				BlockChange();
				PrintMap();
			}
		}
	}
}