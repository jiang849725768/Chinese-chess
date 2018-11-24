#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

void chessboard();
void chesskind(const int i,const int j,const int a);
void chesspiece(const int **chpiece);
void startpiece(int **chpiece);
int changepiece(const int *a,const int *b,const int m);
int inmove(const int *fclickplace,const int *sclickplace,int **chpiece);
int getclick(int *a);
int changecolor(const int *a,const int m);
int checkpiece(const int *a,const int **b,const int m);
int checkplace(int *a,const int *b);
int function(const int *a);
int gameend(const int **chpiece,const int count);
/*
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);        
HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE); 
*/
void gotoxy(int x, int y)    //光标移动到目标坐标
{
        COORD pos;
        pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void HideCursor()       //隐藏闪烁光标
{
    typedef struct
    {
        DWORD dwSize;
        BOOL bVisible; //为0时光标不可见
    }CONSOLE_CURSOR_INFO,  *PCONSOLE_CURSOR_INFO;
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};	//改为CONSOLE_CURSOR_INFO cursor_info = {1, 1}; 则光标可见
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void color(short x)	//自定义函根据参数改变颜色 
{
    if(x>=0 && x<=15)//参数在0-15的范围颜色
    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);	//只有一个参数，改变字体颜色 
    else//默认的颜色白色
    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
int main()                                                              //主程序
{
//	SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	int i,j,re,count = 1;
	int bf[2];
	int *fclickplace,*sclickplace,*fbf,*sbf;
	int **chpiece;

	HideCursor();
//	system("mode con cols=100 lines=40");

	chpiece = (int **)malloc(10*sizeof(int *));
	for(i = 0;i < 10;i++)
		chpiece[i] = (int *)malloc(9*sizeof(int)); 
	fclickplace = (int *)malloc(2*sizeof(int));
	sclickplace = (int *)malloc(2*sizeof(int));
	sbf = (int *)malloc(2*sizeof(int));
	fbf = (int *)malloc(2*sizeof(int));

	startpiece(chpiece);							//初始化
	chessboard();
	chesspiece(chpiece);
	gotoxy(18,32);
	color(12);
	printf("红方先行");
	gotoxy(28,32);
	printf("重新开始");
	gotoxy(8,32);
	printf("悔棋");
	color(7);

rs:	while(1)										//开始下棋
	{
		HideCursor();
		count = count%2;
		do{											//选择棋子
			getclick(fclickplace);
			if(re = function(fclickplace))
				break;
		}while (checkpiece(fclickplace,chpiece,count));
		if(re == 1)
		{
			startpiece(chpiece);
			chessboard();
			chesspiece(chpiece);
			count = 1;
			continue;
		}
		else if(re == 2)
		{
			i = fbf[1]/3 - 1;
			j = fbf[0]/6;
			chpiece[i][j] = bf[0];
			if(chpiece[i][j] > 7)
				color(12);
			else
				color(10);
			changepiece(sbf,fbf,bf[0]);
			i = sbf[1]/3 - 1;
			j = sbf[0]/6;
			chpiece[i][j] = bf[1];
			if(bf[1] != 0)
			{
				if(bf[1] > 7)
					color(12);
				else
					color(10);
				chesskind(i,j,bf[1]);
				color(7);
			}
			if(bf[0] > 7)
				color(12);
			else
				color(10);
			gotoxy(13,32);
			printf("落子无悔真君子");
			color(7);
			count++;
			continue;
		}
		i = fclickplace[1]/3 - 1;
		j = fclickplace[0]/6;
		changecolor(fclickplace,chpiece[i][j]);
		do{												//选择路径
			getclick(sclickplace);
			if(re = function(sclickplace))
				break;
		}while (checkplace(sclickplace,fclickplace));
		if(re == 1)
		{
			startpiece(chpiece);
			chessboard();
			chesspiece(chpiece);
			count = 1;
			continue;
		}
		else if(re == 2)
		{
			gotoxy(16,32);
			printf("晚了！    ");
		}
		if(fclickplace[0] != sclickplace[0] || fclickplace[1] != sclickplace[1])
			count++;
		if(inmove(fclickplace,sclickplace,chpiece))								//错误走法
		{
			i = fclickplace[1]/3 - 1;
			j = fclickplace[0]/6;
			if(chpiece[i][j] > 7)
				color(12);
			else
				color(10);
			chesskind(i,j,chpiece[i][j]);
			color(7);
			count--;
		}
		else																	//正确操作
		{
			i = fclickplace[1]/3 - 1;
			j = fclickplace[0]/6;
			bf[0] = chpiece[i][j];
			chpiece[i][j] = 0;
			i = sclickplace[1]/3 - 1;
			j = sclickplace[0]/6;
			bf[1] = chpiece[i][j];
			chpiece[i][j] = bf[0];
			if(bf[0] > 7)
				color(12);
			else
				color(10);
			changepiece(fclickplace,sclickplace,bf[0]);
			fbf[0] = fclickplace[0];
			fbf[1] = fclickplace[1];
			sbf[0] = sclickplace[0];
			sbf[1] = sclickplace[1];
		}
		if(gameend(chpiece,count%2))
			break;
	}
	gotoxy(8,32);
	printf("退出");
	while(1)
	{
		getclick(fclickplace);
		re = function(fclickplace);
		if(re == 1)
		{
			startpiece(chpiece);
			chessboard();
			chesspiece(chpiece);
			count = 1;
			goto rs;
		}
		else if(re == 2)
		break;
	}
	for(i = 0;i < 10;i++)
		free(chpiece[i]);
	free(chpiece);
	free(fclickplace);
	free(sclickplace);
	free(sbf);
	free(fbf);
}

void chessboard()//打印空棋盘
{
	int i,j;
	gotoxy(0,2);
	for(i = 0;i < 40;i++)
		printf(" ");
	printf("\n");
	for(i = 0;i < 28;i++)
	{
		if(i%3 == 0)
		{
			if(i == 0)
			{
				printf("   ┌ ");
				for(j = 0;j < 7;j++)
					printf(" — ┬ ");
			}
			else if(i == 27)
			{
				printf("   └ ");
				for(j = 0;j < 7;j++)
					printf(" — ┴ ");
			}
			else
			{
				printf("   ├ ");
				for(j = 0;j < 7;j++)
					printf(" — ┼ ");
			}
			
			if(i == 0)
				printf(" — ┐   \n");
			else if(i == 27)
				printf(" — ┘   \n");
			else
				printf(" — ┤   \n");
		}
		else if(i == 13)
			printf("   │                                               │   \n");
		else if(i == 14)
			printf("   │                                               │   \n");
		else
		{
			for(j = 0;j < 8;j++)
				printf("   │  ");
			printf("   │   \n");
		}
	}
	for(i = 0;i < 40;i++)
		printf(" ");
}
void chesskind(const int i,const int j,const int a)                                               //打印单个棋子
{
	gotoxy((1+6*j) ,(2+3*i));
	printf("┌ --┐ ");
	gotoxy((2+6*j) ,(3+3*i));
	printf(" ");
	switch(a){
			case 1: 
				printf("車");
				break;
			case 2:
				printf("马");
				break;
			case 3:
				printf("象");
				break;
			case 4:
				printf("士");
				break;
			case 5:
				printf("将");
				break;
			case 6:
				printf("炮");
				break;
			case 7:
				printf("卒");
				break;
			case 8:
				printf("車");
				break;
			case 9:
				printf("馬");
				break;
			case 10: 
				printf("相");
				break;
			case 11:
				printf("仕");
				break;
			case 12:
				printf("帥");
				break;
			case 13:
				printf("砲");
				break;
			case 14:
				printf("兵");
				break;
			}
	gotoxy((1+6*j) ,(4+3*i));
	printf("╰ —╯ ");
	return;
}
void chesspiece(const int **chpiece)                                  //在棋盘上打印全部棋子
{
	int i,j;
	for(i = 0;i < 10;i++)
	{
		for(j = 0;j < 9;j++)
		{
			if(chpiece[i][j] == 0)
				continue;
			if(chpiece[i][j] > 7)
				color(12);
			else
				color(10);
			chesskind(i,j,chpiece[i][j]);
			color(7);
		}
	}
}
void startpiece(int **chpiece)//初始化棋子位置
{
	int i,j;
	for(i = 0;i < 10;i++)
	{
		for(j = 0;j < 9;j++)
		{
			chpiece[i][j] = 0;
			switch(i){
			case 0:
				if(j < 5)
					chpiece[i][j] = j+1;
				else 
					chpiece[i][j] = 9-j;
				break;
			case 9:
				chpiece[i][j] = chpiece[0][j]+7;
				break;
			case 2:
				chpiece[i][1] = 6;
				chpiece[i][7] = 6;
				break;
			case 7:
				chpiece[i][1] = 13;
				chpiece[i][7] = 13;
				break;
			case 3:
				if(j%2 == 0)
					chpiece[i][j] = 7;
				break;
			case 6:
				if(j%2 == 0)
					chpiece[i][j] = 14;
				break;
			}
		}
	}
}
int changepiece(const int *a,const int *b,const int m)                                   //移动棋子
{
	int x1,x2,y1,y2;
	x1 = a[1]/3 - 1;
	x2 = b[1]/3 - 1;
	y1 = a[0]/6;
	y2 = b[0]/6;
	chesskind(x2,y2,m);
	if(x1 == x2 && y1 == y2)
		return 0;
	color(7);
	switch(x1){
	case 0:
		if(y1 == 0)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("      ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┌ ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("  │   ");
		}
		else if(y1 == 8)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("      ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┐ ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("  │   ");
		}
		else
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("      ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┬  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("  │   ");
		}
		break;
	case 4:
		if(y1 == 0)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("  │  ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ├  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("     ");
		}
		else if(y1 == 8)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("  │  ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┤  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("     ");
		}
		else
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("  │  ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┴  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("     ");
		}
		break;
	case 5:
		if(y1 == 0)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("     ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ├  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("  │  ");
		}
		else if(y1 == 8)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("     ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┤  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("  │  ");
		}
		else
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("     ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┬  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("  │  ");
		}
		break;
	case 9:
		if(y1 == 0)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("  │   ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" └  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("      ");
		}
		else if (y1 == 8)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("  │   ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┘  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("      ");
		}
		else
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("  │   ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┴  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("      ");
		}
		break;
	default:
		if(y1 == 0)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("  │  ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ├  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("  │  ");
		}
		else if(y1 == 8)
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("  │  ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┤  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("  │  ");
		}
		else
		{
			gotoxy((1+6*y1) ,(2+3*x1));
			printf("  │  ");
			gotoxy((2+6*y1) ,(3+3*x1));
			printf(" ┼  ");
			gotoxy((1+6*y1) ,(4+3*x1));
			printf("  │  ");
		}
		break;
	}
	return 0;
}
int getclick(int *a)  //获取鼠标输入
{  
	// 获取标准输入输出设备句柄  
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);        
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE); 
 
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	INPUT_RECORD	mouseRec;
	DWORD			res;
	COORD			crPos, crHome = {0, 3};
 
//	printf("[Cursor Position] X: %2lu  Y: %2lu\n", 0, 0);	// 初始状态

	while(1)
	{
		HideCursor();
		ReadConsoleInput(hIn, &mouseRec, 1, &res);
		if (mouseRec.EventType == MOUSE_EVENT)
		{
			crPos = mouseRec.Event.MouseEvent.dwMousePosition;
			GetConsoleScreenBufferInfo(hOut, &bInfo);
			SetConsoleCursorPosition(hOut, crHome);
//			printf("[Cursor Position] X: %2lu  Y: %2lu", crPos.X, crPos.Y);//crPos.X为横坐标，crPos.Y为纵坐标
			SetConsoleCursorPosition(hOut, bInfo.dwCursorPosition);
 
			switch (mouseRec.Event.MouseEvent.dwButtonState)
			{
			case FROM_LEFT_1ST_BUTTON_PRESSED:			// 左键
//				FillConsoleOutputCharacter(hOut, 'A', 1, crPos, &res);
				a[0] = (int)crPos.X, a[1] = (int)crPos.Y;//获取点击位置
				return 0;
				break;		// 如果使用printf输出，则之前需要先设置光标的位置
 
			case RIGHTMOST_BUTTON_PRESSED:				// 右键 
//				FillConsoleOutputCharacter(hOut, 'a', 1, crPos, &res);
				a[0] = -1,a[1] = -1;//取消上一次的左键点击
				return 0;
				break;
			default:
				break;
			}
		}
	}
	CloseHandle(hOut);  // 关闭标准输出设备句柄  
	CloseHandle(hIn);   // 关闭标准输入设备句柄  
	return 0;  
} 
int checkpiece(const int *a,const int **b,const int m)//检查鼠标输入是否为走方棋子
{
	int i,j;

	if(a[0] < 0)
		return 1;
	if(a[1] > 30 || a[0] > 52)
		return 1;
	if(a[1]%3 == 0 &&((a[0]-3)%6 == 0 || (a[0]-4)%6 == 0))
	{
		j = a[0]/6 ;
		i = a[1]/3 - 1;
		if(b[i][j] == 0)
			return 1;
		else if(b[i][j] > m*7 && b[i][j] <= (m+1)*7)
			return 0;
		else
			return 1;
	}
	else
		return 1;
}
int checkplace(int *a,const int *b)//检查鼠标输入是否有效
{
	if(a[0] == -1)
	{
		a[0] = b[0];
		a[1] = b[1];
		return 0;
	}
	if(a[1] > 30 || a[0] > 52)
		return 1;
	if(a[1]%3 == 0 &&((a[0]-3)%6 == 0 || (a[0]-4)%6 == 0))
		return 0;
	else
		return 1;
}
int changecolor(const int *a,const int m)//改变被选中棋子颜色；
{
	int x,y;
	x = a[1]/3 - 1;
	y = a[0]/6;
	color(3);
	chesskind(x,y,m);
	color(7);
	return 0;
}
int inmove(const int *fclickplace,const int *sclickplace,int **chpiece)//象棋规则
{
	int i,count;
	int piece,place;
	int x1,y1,x2,y2;

	x1 = fclickplace[1]/3 - 1;
	x2 = sclickplace[1]/3 - 1;
	y1 = fclickplace[0]/6;
	y2 = sclickplace[0]/6;
	piece = chpiece[x1][y1];
	place = chpiece[x2][y2];

	if(x1 == x2 && y1 == y2)
		return 0;
	if(place != 0)
		if((piece-1)/7 == (place-1)/7)
			return 1;
	switch(piece%7){
	case 1:                                         //܇車
		if(x1 == x2)
		{
			if(y1 < y2)
			{
				for(i = y1+1;i < y2;i++)
					if(chpiece[x1][i] != 0)
						return 1;
				return 0;
			}
			else
			{
				for(i = y2+1;i < y1;i++)
					if(chpiece[x1][i] != 0)
						return 1;
				return 0;
			}
		}
		else if(y1 == y2)
		{
			if(x1 < x2)
			{
				for(i = x1+1;i < x2;i++)
					if(chpiece[i][y1] != 0)
						return 1;
				return 0;
			}
			else
			{
				for(i = x2+1;i < x1;i++)
					if(chpiece[i][y1] != 0)
						return 1;
				return 0;
			}
		}
		else
			return 1;
		break;
	case 2:                                           //马
		if (abs(x1-x2) == 1 && abs(y1-y2) == 2)
		{
			if(chpiece[x1][y1+(y2-y1)/2] == 0)
			{
				return 0;
			}
			else
				return 1;
		}
		else if (abs(x1-x2) == 2 && abs(y1-y2) == 1)
		{
			if(chpiece[x1+(x2-x1)/2][y1] == 0)
			{
				return 0;
			}
			else
				return 1;
		}
		break;
	case 3:                                            //象
		if(piece > 7)
			if(x2 < 5)
				return 1;
		if(piece < 7)
			if(x2 > 4)
				return 1;
		if(abs(x1-x2) == 2 && abs(y1-y2) == 2)
		{
			if(chpiece[(x1+x2)/2][(y1+y2)/2] != 0)
				return 1;
			
			
			return 0;
		}
		else
			return 1;
		break;
	case 4:                                            //士
		if(y2 < 3 || y2 > 5)
			return 1;
		if(piece > 7)
			if(x2 < 7)
				return 1;
		if(piece < 7)
			if(x2 > 2)
				return 1;
		if(abs(x1-x2) == 1 && abs(y1-y2) == 1)
		{
			return 0;
		}
		else
			return 1;
		break;
	case 5:                                            //将
		if(abs(chpiece[x1][y1] - chpiece[x2][y2]) == 7)
		{
			if(y1 != y2)
				return 1;
			if(chpiece[x1][y1] > chpiece[x2][y2])
			{
				for(i = x2+1;i < x1;i++)
					if(chpiece[i][y1] != 0)
						return 1;
			}
			else
			{
				for(i = x1+1;i < x2;i++)
					if(chpiece[i][y1] != 0)
						return 1;
			}
			return 0;
		}
		if(y2 < 3 || y2 > 5)
			return 1;
		if(piece > 7)
			if(x2 < 7)
				return 1;
		if(piece < 7)
			if(x2 > 2)
				return 1;
		if((abs(x1-x2)+abs(y1-y2)) == 1)
		{
			return 0;
		}
		else
			return 1;
		break;
	case 6:                                            //炮
		if(x1 == x2)
		{
			if(y1 < y2)
			{
				for(i = y1+1,count = 0;i < y2;i++)
					if(chpiece[x1][i] != 0)
						count++;
				if(count == 1 && chpiece[x2][y2] != 0)
				{
					return 0;
				}
				else if(count == 0 && chpiece[x2][y2] == 0)
				{
					return 0;
				}
				else
					return 1;
				
			}
			else
			{
				for(i = y2+1,count = 0;i < y1;i++)
					if(chpiece[x1][i] != 0)
						count++;
				if(count == 1 && chpiece[x2][y2] != 0)
				{
					return 0;
				}
				else if(count == 0 && chpiece[x2][y2] == 0)
				{
					return 0;
				}
				else
					return 1;
			}
		}
		else if(y1 == y2)
		{
			if(x1 < x2)
			{
				for(i = x1+1,count = 0;i < x2;i++)
					if(chpiece[i][y1] != 0)
						count++;
				if(count == 1 && chpiece[x2][y2] != 0)
				{
					return 0;
				}
				else if(count == 0 && chpiece[x2][y2] == 0)
				{
					return 0;
				}
				else
					return 1;
			}
			else
			{
				for(i = x2+1,count = 0;i < x1;i++)
					if(chpiece[i][y1] != 0)
						count++;
				if(count == 1 && chpiece[x2][y2] != 0)
				{
					return 0;
				}
				else if(count == 0 && chpiece[x2][y2] == 0)
				{
					return 0;
				}
				else
					return 1;
			}
		}
		else
			return 1;
		break;
	case 0:                                            //卒
		if(chpiece[x1][y1] == 7)
		{
			if(x1 > 4)
			{
				if(abs(y2-y1)== 1 && x2 == x1)
				{
					return 0;
				}
				else if(x2-x1 == 1 && y2 == y1)
				{
					return 0;
				}
				else
					return 1;
			}
			else
			{
				if(x2-x1 != 1 || y2 !=y1)
					return 1;
				else
				{
					return 0;
				}
			}
		}
		if(chpiece[x1][y1] == 14)
		{
			if(x1 < 5)
			{
				if(abs(y2-y1)== 1 && x2 == x1)
				{
					return 0;
				}
				else if(x1-x2 == 1 && y2 == y1)
				{
					return 0;
				}
				else
					return 1;
			}
			else
			{
				if(x1-x2 != 1 || y2 !=y1)
					return 1;
				else
				{
					return 0;
				}
			}
		}
		else
			return 1;
		break;
	default:
		break;
	}
}
int function(const int *a)                                               //功能选择
{
	if(a[0] > 27 && a[0] < 36 && a[1] == 32)
		return 1;
	else if(a[0] > 7 && a[0] < 12 && a[1] == 32)
		return 2;
	else
		return 0;
}
int gameend(const int **chpiece,const int count)						//判断游戏结束
{
	int i,j;
	for(i = 0;i < 10;i++)
		for(j = 0;j < 9;j++)
			if(chpiece[i][j] == 5+count*7)
				return 0;
	if(count)
	{
		color(10);
		gotoxy(16,32);
		printf("   绿方赢  ");
	}
	else
	{
		color(12);
		gotoxy(16,32);
		printf("   红方胜  ");
	}
	color(7);
	return 1;
	
}
