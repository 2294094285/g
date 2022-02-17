#include <stdio.h>
#include <graphics.h>      // 引用图形库头文件
#include <conio.h>
#include <time.h>
#include <easyx.h>   

#define GAMEROW 20
#define GAMECOLUMN 30  
#define TIPSN 6       //提示区方格数
#define SPACE 20
#define PIXMAP 20     //方格像素大小
#define BOUNDARY 20
#define BORDER 4
DWORD g_oldtime, g_newtime;//滴答时间,通过GetTickCount()获取
char m_gameMap[GAMEROW][GAMECOLUMN] = {0};    //游戏界面数组

unsigned short block[7][4] = {
 {0x660, 0x660, 0x660, 0x660},    // 田
 {0x2222, 0xf0, 0x2222, 0xf0},  // 一 
 {0x446, 0x740, 0x622, 0x170},    // L
 {0x226, 0x470, 0x644, 0x710},  // 反L
 {0x630, 0x264, 0x630, 0x264},  // Z
 {0x360, 0x462, 0x360, 0x462},  // 反Z
 {0x720, 0x262, 0x270, 0x464}  // T
};

struct Point
{
 int x;
 int y;
};

struct BlockInfo
{
 int type;     //方块类型
 int dir;   //方块方向
 struct Point pos;   //游戏4*4整体方块的游戏区坐标
 struct Point basePos[4];  //4个小方块的4*4区域坐标
 struct Point offPos[4];   //4个小方块在游戏区的相对坐标
}g_curBlock, g_nextBlock;
void hin_win_cub(struct BlockInfo *np,struct BlockInfo *cp)//提示窗口图形
{
	int i,j=0;
	setorigin(GAMEROW*PIXMAP+2*PIXMAP,1*PIXMAP);
	setfillcolor(BLACK);
	solidrectangle(0,0,4*PIXMAP,4*PIXMAP);
	srand(time(0));
	np->type = rand()%7;
	np->dir = rand()%4;
	short num =block[np->type][np->dir];
	for(i = 15;i>=0,j<4;i-- )
	{
		if(num&0x8000)
		{
			
			np->basePos[j].x =i%4;
			np->basePos[j].y =i/4;
			j++;
		}
		num<<=1;
	}
	for(i=0;i<4;i++)
	{
		setfillcolor(CYAN);
		fillrectangle(np->basePos[i].x*PIXMAP,np->basePos[i].y*PIXMAP,np->basePos[i].x*PIXMAP+PIXMAP,np->basePos[i].y*PIXMAP+PIXMAP); 
	}
	setorigin(0,0);
}
void geme_win_cube( struct BlockInfo *np,int sink_n ,int move_g_n,int n, int var_g)//游戏窗口图形
{
	int i,j=0;
	if(n)
	for(i=0;i<4;i++)
				{
					setfillcolor(BLACK);
					solidrectangle(np->basePos[i].x*PIXMAP,np->basePos[i].y*PIXMAP,np->basePos[i].x*PIXMAP+PIXMAP,np->basePos[i].y*PIXMAP+PIXMAP); 
				}
	if(var_g)
	{
		np->dir = np->dir +1;
		if(np->dir >3)
			np->dir =0;
	}
	short num=block[np->type][np->dir];
	for(i = 15;i>=0,j<4;i-- )
	{
		if(num&0x8000)
		{
			
			np->basePos[j].x =i%4;
			np->basePos[j].y =i/4;
			j++;
		}
		num<<=1;
	}
	if(!n)
	{
		np->pos.x =(GAMEROW/2-2);
		np->pos .y = 1;
	}
	else
	{
		np->pos.x =np->pos.x+move_g_n;
	}
	setorigin((np->pos.x )*PIXMAP,(np->pos .y+sink_n)*PIXMAP);
	for(i=0;i<4;i++)
	{
		setfillcolor(CYAN);
		fillrectangle(np->basePos[i].x*PIXMAP,np->basePos[i].y*PIXMAP,np->basePos[i].x*PIXMAP+PIXMAP,np->basePos[i].y*PIXMAP+PIXMAP); 
	}

}
int cub_move(struct BlockInfo *np,int (*arr)[GAMEROW] )
{
	int snik=0;
	int as = 0;
	int i;
	setorigin(PIXMAP,PIXMAP);
	while(1) 
	{
		g_newtime = GetTickCount();
		int move_n=0;
		int a1 = 0,var = 0;
		if(g_newtime - g_oldtime >= 300) 
		{
			int n = 0;
			char ch;
			if(kbhit())
			{
				ch = getch();
				switch(ch)
				{
				case 'a':
					for(i = 0;i<4;i++)
					{
						if(np->basePos[i].x+np->pos.x<=1||arr[np->pos.y + np->basePos[i].y+snik][np->pos.x + np->basePos[i].x-1]==1)
						{
							move_n=0;
							break;
						}
						else
							move_n=-1;
					}
					break;
				case 's':
					 
					for(i = 0;i < 4;i++)
					{
						if(np->basePos [i].y +snik+2 >= GAMECOLUMN||arr[np->pos.y + np->basePos[i].y+snik+2][np->pos.x + np->basePos[i].x]==1)
						{
							n++;
							break;
						}
					}
					if(!n)
						snik++;
					break;
				case 'd':
					for(i = 0;i<4;i++)
					{
						if(np->basePos[i].x+np->pos.x>=GAMEROW||arr[np->pos.y + np->basePos[i].y+snik][np->pos.x + np->basePos[i].x+1]==1)
						{
							move_n=0;
							break;
						}
						else
							move_n=1;
					}
					break;
				case 'w':
					var++;
					break;
				default:
					move_n=0;
				}
			}
			int j;
			geme_win_cube(np,snik,move_n,as ,var);
			for(i = 0;i < 4;i++)
			{
				if(np->basePos [i].y +snik+1 >= GAMECOLUMN||arr[np->pos.y + np->basePos[i].y+snik+1][np->pos.x + np->basePos[i].x]==1)
				{
					for(j = 0;j<4;j++)
					{
						arr[np->pos.y + np->basePos[j].y+snik][np->pos.x + np->basePos[j].x]=1;
					}
					return 0;
				}
			}
			as++;
			snik++;
			g_oldtime = g_newtime;
		}
	}
}
void draw_cub(int (*p)[GAMEROW])
{
	setorigin(0,0);
	setfillcolor(BLACK);
	solidrectangle(1*PIXMAP,1*PIXMAP,GAMEROW*PIXMAP+1*PIXMAP,GAMECOLUMN*PIXMAP+1*PIXMAP);
	int i,j;
	for(i=0;i < (GAMECOLUMN+2);i++)
	{
		for(j = 0;j<GAMEROW+1;j++)
		{
			if(p[i][j] == 1)
			{
				setfillcolor(CYAN);
				fillrectangle(j*PIXMAP,i*PIXMAP,(j+1)*PIXMAP,(i+1)*PIXMAP);
			}
		}
	}
}
void disa_cub(int (*p)[GAMEROW])
{
	int i,j,k;
	for(i=0;i < (GAMECOLUMN+2);i++)
	{
		int n = 0;
		for(j = 0;j<GAMEROW;j++)
		{
			if(0 == p[i][j])
			{
				n++;
				break;
			}
		}
		if(!n)
		{
			for(k =i;k >0;k--)
			{
				for(j = 0;j <GAMEROW;j++ )
					p[k][j]=p[k-1][j];
			}
			draw_cub(p);
		}
	}
	 
}
void window()
{
	initgraph(GAMEROW*PIXMAP+TIPSN*PIXMAP+3*PIXMAP,GAMECOLUMN*PIXMAP+2*PIXMAP,SHOWCONSOLE);  
	setbkcolor(LIGHTGRAY);
	cleardevice();
	setfillcolor(BLACK);
	solidrectangle(1*PIXMAP,1*PIXMAP,GAMEROW*PIXMAP+1*PIXMAP,GAMECOLUMN*PIXMAP+1*PIXMAP); 
	setfillcolor(BLACK);
	solidrectangle(GAMEROW*PIXMAP+2*PIXMAP,1*PIXMAP,GAMEROW*PIXMAP+2*PIXMAP+TIPSN*PIXMAP,TIPSN*PIXMAP+1*PIXMAP); 
}
int main()
{ 
	int arr_game[GAMECOLUMN+2][GAMEROW]={0};
	int fra = 0,i;
	window();
	while(1){
		int n =0;
		g_curBlock.type = g_nextBlock.type ;
		g_curBlock.dir = g_nextBlock.dir ;
		hin_win_cub(&g_nextBlock,&g_curBlock);
		cub_move(&g_curBlock,arr_game);
		setorigin(0,0);
		disa_cub(arr_game);
		for( i = 0;i <GAMEROW;i++ )
		{
			if(arr_game[1][i] == 1)
			{
				n++;
				break;
			}
		}
		if(n)
		{
			settextcolor(RED);
			outtextxy(10*PIXMAP,10*PIXMAP,"game over!");
			break;
		}
	}
	 _getch();              // 按任意键继续
    closegraph();         
	return 0;
}