#define _CRT_SECURE_NO_WARNINGS  // 解决sprintf安全警告
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <iostream>
using namespace std;

// 定义常量（方块大小、游戏区域尺寸等）
#define BLOCK_SIZE 20                       // 每个小方块的像素大小
#define WIDTH 10                            // 游戏区域列数（横向方块数）
#define HEIGHT 20                           // 游戏区域行数（纵向方块数）
#define WIN_WIDTH BLOCK_SIZE * (WIDTH + 6)  // 窗口宽度（含右侧信息区）
#define WIN_HEIGHT BLOCK_SIZE * HEIGHT      // 窗口高度
#define GRAY RGB(192, 192, 192)             // 定义灰色常量

// 定义方块颜色（对应不同形状）
// 例子:将文本颜色设置为蓝色（索引3）,SetTextColor(hdc, blockColor[3]);
COLORREF blockColor[] = {
    RGB(0, 0, 0),      // 0-空白（黑色）
    RGB(255, 0, 0),    // 1-红色
    RGB(0, 255, 0),    // 2-绿色
    RGB(0, 0, 255),    // 3-蓝色
    RGB(255, 255, 0),  // 4-黄色
    RGB(255, 165, 0),  // 5-橙色
    RGB(128, 0, 128),  // 6-紫色
    RGB(0, 255, 255),   // 7-青色
	RGB(225,192,203),   // 8-粉色
	RGB(255,255,255)   // 9-白色
};

//************游戏的全局变量****************//
// 定义7种俄罗斯方块形状（4x4矩阵）
int blockShape[7][4][4][4] = {
    // I型：4种旋转状态（横/竖）
        {
            {{1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0}},  // 状态0：竖条
            {{1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态1：横条
            {{1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0}},  // 状态2：竖条（同状态0）
            {{1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}   // 状态3：横条（同状态1）
        },
    // L型：4种旋转状态（右、下、左、上）
    {
        {{1,0,0,0}, {1,0,0,0}, {1,1,0,0}, {0,0,0,0}},  // 状态0：右L
        {{1,1,1,0}, {1,0,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态1：下L
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // 状态2：左L
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}   // 状态3：上L
    },
    // 反L型：4种旋转状态（左、下、右、上）
    {
        {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}},  // 状态0：左反L
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // 状态1：下反L
        {{1,1,0,0}, {1,0,0,0}, {1,0,0,0}, {0,0,0,0}},  // 状态2：右反L
        {{1,1,1,0}, {0,0,1,0}, {0,0,0,0}, {0,0,0,0}}   // 状态3：上反L
    },
    // 方形：4种状态相同（旋转后不变）
    {
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态0
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态1
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态2
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}   // 状态3
    },
    // T型：4种旋转状态（上、右、下、左）
    {
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // 状态0：上T
        {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // 状态1：右T
        {{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}},  // 状态2：下T
        {{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}}   // 状态3：左T
    },
    // Z型：2种旋转状态（右、左，重复两次）
    {
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // 状态0：右Z
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}},  // 状态1：左Z
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // 状态2：右Z（同状态0）
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}   // 状态3：左Z（同状态1）
    },
    // 反Z型：2种旋转状态（左、右，重复两次）
    {
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态0：左反Z
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // 状态1：右反Z
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态2：左反Z（同状态0）
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}   // 状态3：右反Z（同状态1）
    }
};
// 游戏区域数组,0表示空白，非0表示有方块
int gameArea[HEIGHT][WIDTH] = { 0 };
// 当前方块信息,currShape:方块类型,currRot:旋转状态,currX:横坐标,currY:纵坐标
int currShape, currRot, currX, currY;
// 下一个方块类型
int nextShape;
// 游戏数据
int score = 0;
int gameTime = 0;
time_t startTime;   // 游戏开始时间

// 函数声明
void initGame();    //初始化游戏
void createBlock(int& shape, int& rot, int& x, int& y); //创建新方块
void drawBlock(int shape, int rot, int x, int y);   //绘制方块
void drawGameArea();    //绘制游戏区域
void drawInfo();    //绘制右侧信息区
bool checkCollision(int shape, int rot, int x, int y); //检测碰撞
void rotateBlock(); //旋转方块
bool moveBlock(int dx, int dy); //移动方块
void dropBlock();   //快速落地
void eliminateLines();  //消除满行
void updateTime();  //  更新时间
bool isGameOver(int shape, int rot, int x, int y);  //检查游戏结束
bool checkAnyKeyPressed(); //检测是否按下任意键位
void gameOver();    //处理游戏结束
void handleInput(); //键盘输入函数

int main() {
	initgraph(WIN_WIDTH, WIN_HEIGHT);   // 初始化图形窗口
    BeginBatchDraw();//开启双缓冲
	initGame(); // 初始化游戏数据（生成初始方块、清空地图、初始化分数和时间）
    while (true) 
    {
		/*if (_kbhit()){ // 检测键盘输入
			switch (_getch()) {// 获取按键
            case 'w': case 'W': rotateBlock(); break;
            case 'a': case 'A': moveBlock(-1, 0); break;
            case 'd': case 'D': moveBlock(1, 0); break;
            case 's': case 'S': moveBlock(0, 1); break;
			case ' ': dropBlock(); break;   // 空格键快速落地
			case 'q': case 'Q': closegraph(); return 0;// 退出游戏
            }
        }*/
        handleInput();
		static DWORD lastDropTime = 0;  // 记录上次自动下落时间
        if (GetTickCount() - lastDropTime > 500) {
			if (!moveBlock(0, 1)) { // 无法下移，固定方块
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
						if (blockShape[currShape][currRot][i][j]) { // 固定方块到游戏区域
                            int x = currX + j;
                            int y = currY + i;
							if (y >= 0) gameArea[y][x] = currShape + 1; // 使用非0值表示有方块
                        }
                    }
                }
				eliminateLines();   // 消除满行

                if (isGameOver(currShape, currRot, currX, currY)) { // 检测新生成的当前方块是否碰撞（游戏结束判定）
                    gameOver();
                    break;  //跳出大循环
                }
				// 生成新方块
                currShape = nextShape;
				currRot = 0;               // 新方块初始旋转状态
				currX = WIDTH / 2 - 2;    // 新方块X初始位置
				currY = -4;              // 新方块Y初始位置
				createBlock(nextShape, currRot, currX, currY);  
            }
			lastDropTime = GetTickCount();  // 更新下落时间
        }

		//更新显示
        updateTime();
        cleardevice(); 
        drawGameArea(); 
        drawBlock(currShape, currRot, currX, currY); 
        drawInfo(); 

		FlushBatchDraw();   // 刷新绘图，将双缓冲中的内容显示到屏幕上
		Sleep(20);   // 减少CPU占用
    }
	closegraph();   // 关闭图形窗口
    return 0;
}

//初始化游戏
void initGame() {
    srand((unsigned int)time(NULL));
    startTime = time(NULL);

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            gameArea[i][j] = 0;
	int tempX, tempY;  // 临时变量,防止createBlock修改下一个currX,currY
    tempX = currX;  
    tempY = currY;
	createBlock(currShape, currRot, tempX,tempY);  // 创建当前方块
	createBlock(nextShape, currRot, currX, currY);  // 创建下一个方块
}

//创建新方块
void createBlock(int& shape,int& rot,int& x,int& y) {    
	shape = rand() % 7;   // 随机选择方块形状
	rot = 0;             // 初始旋转状态
	x = WIDTH/2-2;      // 初始位置在顶部中央
	y = -4;            // 初始位置在顶部外
}

//绘制方块
void drawBlock(int shape, int rot, int x, int y) {  // x,y为方块左上角在游戏区域中的坐标,rot为旋转状态,shape为方块形状
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[shape][rot][i][j] && (y + i >= 0)) {
                int drawX = (x + j) * BLOCK_SIZE;
                int drawY = (y + i) * BLOCK_SIZE;
				setfillcolor(blockColor[shape + 1]);                    // shape从0开始，颜色从1开始
				solidrectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);// 绘制方块
				setlinecolor(blockColor[0]);                // 设置边框颜色
				rectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);    // 绘制边框
            }
        }
    }
}

//绘制游戏区域
void drawGameArea() {
	setlinecolor(RED);  // 设置边框颜色
	rectangle(0, 0, WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE);   // 绘制边框

	// 绘制网格和已固定的方块
	for (int i = 0; i < HEIGHT; i++) { 
        for (int j = 0; j < WIDTH; j++) {
            int x = j * BLOCK_SIZE;
            int y = i * BLOCK_SIZE;
			setlinecolor(LIGHTGRAY);    // 设置网格线颜色
			line(x, 0, x, HEIGHT * BLOCK_SIZE);     // 绘制竖线,line(x1, y1, x2, y2),x1=x2时为竖线,y1=y2时为横线
			line(0, y, WIDTH * BLOCK_SIZE, y);      // 绘制横线,x1表示起点横坐标,y1表示起点纵坐标,x2表示终点横坐标,y2表示终点纵坐标

            if (gameArea[i][j] != 0) {
				setfillcolor(blockColor[gameArea[i][j]]);                      // 设置方块颜色
				solidrectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1); // 绘制方块
				setlinecolor(BLACK);                                         // 设置边框颜色
				rectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1);    // 绘制边框
            }
        }
    }
}

//绘制右侧信息区
void drawInfo() {

	int infoX = WIDTH * BLOCK_SIZE + 10;  // 右侧信息区起始X坐标
	settextcolor(blockColor[9]);          // 设置文本颜色
	setbkmode(TRANSPARENT);     // 设置文本背景透明

    // 绘制分数（使用宽字符数组和swprintf_s）
    WCHAR scoreText[20];  // 宽字符数组
    swprintf_s(scoreText, L"分数: %d", score);  // 直接格式化宽字符串
	outtextxy(infoX, 20, scoreText);    // 绘制分数

    // 绘制游戏时间
    WCHAR timeText[20];  // 宽字符数组
    swprintf_s(timeText, L"时间: %ds", gameTime);  // 直接格式化宽字符串
	outtextxy(infoX, 60, timeText);     // 绘制时间
     
    //绘制下一个方块提示
    outtextxy(infoX, 100, L"下一个:");  // 直接使用宽字符串前缀L

    // 绘制下一个方块
    int nextX = infoX + (BLOCK_SIZE * 4 - BLOCK_SIZE * 2) / 2;
    int nextY = 120;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[nextShape][0][i][j]) {
                int drawX = nextX+j*BLOCK_SIZE;
                int drawY = nextY+i*BLOCK_SIZE;
                setfillcolor(blockColor[nextShape + 1]);                            //方块颜色
				solidrectangle(drawX,drawY,drawX+BLOCK_SIZE-1,drawY+BLOCK_SIZE-1);//绘制方块
				setlinecolor(blockColor[0]);                                            //边框颜色
				rectangle(drawX,drawY,drawX+BLOCK_SIZE-1,drawY+BLOCK_SIZE-1);//绘制边框
            }
        }
    }

	// 绘制操作提示（直接使用宽字符串前缀L),outtextxy(x, y, L"文本内容")函数支持宽字符串的输出
    outtextxy(infoX, 220, L"操作:");
    outtextxy(infoX, 240, L"W - 旋转");
    outtextxy(infoX, 260, L"A - 左移");
    outtextxy(infoX, 280, L"D - 右移");
    outtextxy(infoX, 300, L"S - 下移");
    outtextxy(infoX, 320, L"空格 - 落地");
    outtextxy(infoX, 340, L"Q - 退出");

    //绘制当前游戏时间
    time_t now = time(NULL);
    tm* localTime = localtime(&now);

 

    WCHAR timeText1[40];
    swprintf_s(timeText1, 40, L"%02d:%02d:%02d",
        localTime->tm_hour,
        localTime->tm_min,
        localTime->tm_sec);
    outtextxy(infoX, 360, timeText1);
}


//检测碰撞
bool checkCollision(int shape, int rot, int x, int y) {
    // 遍历4x4方块矩阵的每个格子（俄罗斯方块标准用4x4矩阵存储形状）
    for (int i = 0; i < 4; i++) {       // i：方块矩阵的行（对应游戏区域的Y方向）
        for (int j = 0; j < 4; j++) {   // j：方块矩阵的列（对应游戏区域的X方向）
            // 只处理方块实际存在的格子（blockShape为1的部分）
            if (blockShape[shape][rot][i][j] == 1) {
                // 计算当前格子在游戏区域中的实际坐标
                int gridX = x + j;  // 转换为游戏区域的列坐标
                int gridY = y + i;  // 转换为游戏区域的行坐标

                // 1. 左右边界碰撞：超出游戏区域左右边缘（完全禁止）
                if (gridX < 0 || gridX >= WIDTH) {
                    return true;  // 左边越界或右边越界，碰撞
                }

                // 2. 底部边界碰撞：超出游戏区域下边缘（落地判定）
                if (gridY >= HEIGHT) {
                    return true;  // 底部越界，碰撞（无法继续下移）
                }

                // 4. 与已固定方块的碰撞：当前格子在游戏区域内（gridY >=0），且已有方块
                if (gameArea[gridY][gridX] != 0) {
                    return true;        // 与已固定的方块重叠，碰撞
                }
            }
        }
    }
    // 所有格子都无碰撞
    return false;
}

//旋转方块
void rotateBlock() {
	int newRot = (currRot + 1) % 4;                        // 计算新的旋转状态(循环)
	if (!checkCollision(currShape, newRot, currX, currY)) // 无碰撞，更新旋转状态
        currRot = newRot;
}

//移动方块
bool moveBlock(int dx, int dy) {
    int newX = currX + dx;
    int newY = currY + dy;
	if (!checkCollision(currShape, currRot, newX, newY)) {  // 无碰撞，更新位置
        currX = newX;
        currY = newY;
        return true;
    }
    return false;
}

//快速落地
void dropBlock() {
    while (moveBlock(0, 1));
}

//消除满行
void eliminateLines() {
	int lineCount = 0;  // 记录消除的行数
	for (int i = HEIGHT - 1; i >= 0; i--) {     // 从底部开始检查每一行
		bool isFull = true;                     // 检查变量，当前行是否满
		for (int j = 0; j < WIDTH; j++) {      //每列检查是否都有方块
            if (gameArea[i][j] == 0) {
                isFull = false;
                break;
            }
        }

		if (isFull) {       // 如果当前行满，消除该行
            lineCount++;
            for (int k = i;k>0;k--)
                for (int j = 0; j<WIDTH; j++)
                    gameArea[k][j] = gameArea[k-1][j];
            for (int j = 0; j<WIDTH;j++)
                gameArea[0][j] = 0;
            i++;
        }
    }

	switch (lineCount) {    // 根据消除行数更新分数
    case 1: score += 10; break;
    case 2: score += 30; break;
    case 3: score += 50; break;
	case 4: score += 80; break; // 四行一次性消除，得分最高
    }
}

// 更新时间
void updateTime() {
    gameTime = (int)(time(NULL) - startTime);
}

// 显示当前时间   
void currentTime() {
	WCHAR timeText[20];  // 宽字符数组

	swprintf_s(timeText, L"当前时间: %02d:%02d:%02d",
		(gameTime / 3600) % 24, (gameTime / 60) % 60, gameTime % 60);

	outtextxy(WIN_WIDTH - 150, WIN_HEIGHT - 30, timeText);
}

//检查游戏结束
bool isGameOver(int shape, int rot, int x, int y) {
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++)    
            if (blockShape[shape][rot][i][j] == 1) {
                int gridY = y + i;  // 转换为游戏区域的行坐标
                if (gridY < 0) {
                    return true;
                }
            }
    return false;
}

//处理游戏结束
void gameOver() {
    cleardevice();  // 清屏
    while(1) {
        settextcolor(RED);  // 设置文本颜色
        settextstyle(40, 0, L"宋体");  // 宽字符字体名，结束标题
        outtextxy(WIN_WIDTH / 2 - 120, WIN_HEIGHT / 2 - 60, L"游戏结束！");

        WCHAR scoreText[20];  // 宽字符数组,分数显示
        swprintf_s(scoreText, L"最终分数: %d", score);
        outtextxy(WIN_WIDTH / 2 - 100, WIN_HEIGHT / 2, scoreText);

        settextstyle(20, 0, L"宋体");  // 宽字符字体名
        outtextxy(WIN_WIDTH / 2 - 80, WIN_HEIGHT / 2 + 40, L"按任意键退出...");
        if (checkAnyKeyPressed())
            break;
        FlushBatchDraw();   // 刷新绘图，将双缓冲中的内容显示到屏幕上
        Sleep(10);   // 减少CPU占用
    }
}

//检测键盘输入任意键位
bool checkAnyKeyPressed() {
    for (int key = 8; key <= 255; ++key) { //遍历一遍键位
        if (GetAsyncKeyState(key) & 0x8000) {
            return true;  // 有任意键被按下
        }
    }
    return false;
}

bool wasKeyPressedW = false;
bool wasKeyPressedA = false;
bool wasKeyPressedS = false;
bool wasKeyPressedD = false;
bool wasKeyPressedQ = false;
bool wasKeyPressedVK_SPACE = false;

//键盘输入
void handleInput() {

    //各个键位是否被按下的bool值 （0x8000是按下)
    bool isW = GetAsyncKeyState('W') & 0x8000;
    bool isA = GetAsyncKeyState('A') & 0x8000;
    bool isS = GetAsyncKeyState('S') & 0x8000;
    bool isD = GetAsyncKeyState('D') & 0x8000;
    bool isQ = GetAsyncKeyState('Q') & 0x8000;
    bool isVK_SPACE = GetAsyncKeyState(VK_SPACE) & 0x8000;


    // 只有在本帧按下，上一帧没按下时，才执行一次
    if (isW && !wasKeyPressedW) rotateBlock();
    if (isA && !wasKeyPressedA)moveBlock(-1, 0);
    if (isS && !wasKeyPressedS)moveBlock( 0 , 1);
    if (isD && !wasKeyPressedD)moveBlock( 1 , 0);
    if (isQ && !wasKeyPressedQ) {
        closegraph();  
    }
    if (isVK_SPACE && !wasKeyPressedVK_SPACE) {
        dropBlock();
    }

    

    // 更新上一帧的状态
    wasKeyPressedW = isW;
    wasKeyPressedA = isA;
    wasKeyPressedS = isS;
    wasKeyPressedD = isD;
}