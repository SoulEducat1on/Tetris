#define _CRT_SECURE_NO_WARNINGS  // 解决sprintf安全警告
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <iostream>
using namespace std;

// 定义常量（方块大小、游戏区域尺寸等）
#define BLOCK_SIZE 20    // 每个小方块的像素大小
#define WIDTH 10         // 游戏区域列数（横向方块数）
#define HEIGHT 20        // 游戏区域行数（纵向方块数）
#define WIN_WIDTH BLOCK_SIZE * (WIDTH + 6)  // 窗口宽度（含右侧信息区）
#define WIN_HEIGHT BLOCK_SIZE * HEIGHT      // 窗口高度
#define GRAY RGB(192, 192, 192)  // 定义灰色常量

// 定义方块颜色（8种，对应不同形状）
COLORREF blockColor[] = {
    RGB(0, 0, 0),      // 0-空白（黑色）
    RGB(255, 0, 0),    // 1-红色
    RGB(0, 255, 0),    // 2-绿色
    RGB(0, 0, 255),    // 3-蓝色
    RGB(255, 255, 0),  // 4-黄色
    RGB(255, 165, 0),  // 5-橙色
    RGB(128, 0, 128),  // 6-紫色
    RGB(255, 255, 255)   // 7-青色
};

// 定义7种俄罗斯方块形状（4x4矩阵）
int blockShape[7][4][4][4] = {  // [形状][状态][行][列]
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


// 游戏区域数组
int gameArea[HEIGHT][WIDTH] = { 0 };
// 当前方块信息
int currShape, currRot, currX, currY;
// 下一个方块类型
int nextShape;
// 游戏数据
int score = 0;
int gameTime = 0;
time_t startTime;

// 函数声明
void initGame();
void createBlock(int& shape, int& rot, int& x, int& y);
void drawBlock(int shape, int rot, int x, int y);
void drawGameArea();
void drawInfo();
bool checkCollision(int shape, int rot, int x, int y);
void rotateBlock();
bool moveBlock(int dx, int dy);
void dropBlock();
void eliminateLines();
void updateTime();
bool isGameOver();
void gameOver();
void currentTime();  // 提前声明
bool blockLocked = false;

int main() {
    initgraph(WIN_WIDTH, WIN_HEIGHT);
    
    //开启双缓冲
    BeginBatchDraw();
   
    // 初始化游戏数据（生成初始方块、清空地图、初始化分数和时间）
    initGame();


    // 记录方块下落的上一次时间
    DWORD lastDropTime = GetTickCount();
    DWORD rotateCooldown = 150;      // 旋转冷却时间（ms）
    DWORD lastRotateTime = 0;

    bool running = true;
    bool isDropping = false;  // 快速落地标记

    while (running) {
        DWORD now = GetTickCount();

        // ------------------- 检测退出 -------------------
        if (GetAsyncKeyState('Q') & 0x8000) {
            running = false;
            break;
        }

        //GetAsyncKeyState 来替代 _kbhit() + _getch() 的方式检测按键
        // ------------------- 检测旋转 -------------------
        if ((GetAsyncKeyState('W') & 0x8000) && (now - lastRotateTime > rotateCooldown)) {
            int newRot = (currRot + 1) % 4;
            // 尝试旋转
            if (!checkCollision(currShape, newRot, currX, currY)) {
                currRot = newRot;
            }
            else if (!checkCollision(currShape, newRot, currX - 1, currY)) {
                currX--; currRot = newRot; // 左偏移旋转
            }
            else if (!checkCollision(currShape, newRot, currX + 1, currY)) {
                currX++; currRot = newRot; // 右偏移旋转
            }
            lastRotateTime = now;
        }

        if (GetAsyncKeyState('A') & 0x8000) moveBlock(-1, 0);
        if (GetAsyncKeyState('D') & 0x8000) moveBlock(1, 0);
        if (GetAsyncKeyState('S') & 0x8000) moveBlock(0, 1);


        // ------------------- 快速落地 -------------------
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            isDropping = true;
        }

        // ------------------- 自动下落 -------------------
        if (isDropping) {
            // 每帧尽可能下落直到碰撞
            if (!moveBlock(0, 1)) {  // 已经到底
                isDropping = false;
                // 固定方块

                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++)
                        if (blockShape[currShape][currRot][i][j]) {
                            int x = currX + j;
                            int y = currY + i;
                            if (y >= 0) gameArea[y][x] = currShape + 1;
                        }

                eliminateLines();

                if (!isGameOver()) {
                    gameOver(); 
                    break; 
                }

                cleardevice();
                drawGameArea();
                drawInfo();
                currentTime();
                FlushBatchDraw();

                currShape = nextShape;
                currRot = 0;
                currX = WIDTH / 2 - 2;
                currY = -4;
                nextShape = rand() % 7;
            }
        }
        else if (now - lastDropTime > 500) {  // 正常下落
            if (!moveBlock(0, 1)) {  // 到底
                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++)
                        if (blockShape[currShape][currRot][i][j]) {
                            int x = currX + j;
                            int y = currY + i;
                            if (y >= 0) gameArea[y][x] = currShape + 1;
                        }
                eliminateLines();

                if (!isGameOver()) { gameOver(); break; }
                currShape = nextShape;
                currRot = 0;
                currX = WIDTH / 2 - 2;
                currY = -4;
                nextShape = rand() % 7;
            }
            lastDropTime = now;
        }
        // ------------------- 更新时间 -------------------
        updateTime();


        // ------------------- 绘制画面 ------------------
        cleardevice();
        drawGameArea();
        drawBlock(currShape, currRot, currX, currY);
        drawInfo();
        currentTime();

        // 将双缓冲中的内容显示到屏幕上
        FlushBatchDraw();

        // ------------------- 控制循环频率 -------------------
        Sleep(20);  // 延时 20ms，降低 CPU 占用，同时保证按键检测流畅
    }
    closegraph();
    return 0;
}

void initGame() {
    srand((unsigned int)time(NULL));
    startTime = time(NULL);

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            gameArea[i][j] = 0;

    createBlock(currShape, currRot, currX, currY);
    // 只生成下一个方块的形状，不需要坐标
    nextShape = rand() % 7;
}

void createBlock(int& shape, int& rot, int& x, int& y) {
    shape = rand() % 7;// 随机选择方块形状
    rot = 0;             // 初始旋转状态
    x = WIDTH / 2 - 2;      // 初始位置在顶部中央
    y = -4;            // 初始位置在顶部外
}

void drawBlock(int shape, int rot, int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[shape][rot][i][j] && (y + i >= 0)) {
                int drawX = (x + j) * BLOCK_SIZE;
                int drawY = (y + i) * BLOCK_SIZE;
                setfillcolor(blockColor[shape + 1]);
                solidrectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);
                setlinecolor(GREEN);
                rectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);
            }
        }
    }
}

void drawGameArea() {
    setlinecolor(RED);
    rectangle(0, 0, WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int x = j * BLOCK_SIZE;
            int y = i * BLOCK_SIZE;
            setlinecolor(LIGHTGRAY);
            line(x, 0, x, HEIGHT * BLOCK_SIZE);
            line(0, y, WIDTH * BLOCK_SIZE, y);

            if (gameArea[i][j] != 0) {
                setfillcolor(blockColor[gameArea[i][j]]);
                solidrectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1);
                setlinecolor(BLACK);
                rectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1);
            }
        }
    }
}

void drawInfo() {
    int infoX = WIDTH * BLOCK_SIZE + 10;
    settextcolor(RED);
    setbkmode(TRANSPARENT);

    // 绘制分数（使用宽字符数组和swprintf_s）
    WCHAR scoreText[20];  // 宽字符数组
    swprintf_s(scoreText, L"分数: %d", score);  // 直接格式化宽字符串
    outtextxy(infoX, 20, scoreText);

    // 绘制游戏时间
    WCHAR timeText[20];  // 宽字符数组
    swprintf_s(timeText, L"时间: %ds", gameTime);  // 直接格式化宽字符串
    outtextxy(infoX, 60, timeText);



    // 绘制下一个方块提示
    outtextxy(infoX, 100, L"下一个:");  // 直接使用宽字符串前缀L

    // 绘制下一个方块
    int nextX = infoX + (BLOCK_SIZE * 4 - BLOCK_SIZE * 2) / 2;
    int nextY = 120;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[nextShape][0][i][j]) {
                int drawX = nextX + j * BLOCK_SIZE;
                int drawY = nextY + i * BLOCK_SIZE;
                setfillcolor(blockColor[nextShape + 1]);
                solidrectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);
                setlinecolor(BLACK);
                rectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);
            }
        }
    }

    // 绘制操作提示（直接使用宽字符串前缀L）
    outtextxy(infoX, 220, L"操作:");
    outtextxy(infoX, 240, L"W - 旋转");
    outtextxy(infoX, 260, L"A - 左移");
    outtextxy(infoX, 280, L"D - 右移");
    outtextxy(infoX, 300, L"S - 下移");
    outtextxy(infoX, 320, L"空格 - 落地");
    outtextxy(infoX, 340, L"Q - 退出");
}

bool checkCollision(int shape, int rot, int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[shape][rot][i][j]) {
                int newX = x + j;
                int newY = y + i;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT)
                    return true;
                if (newY >= 0 && gameArea[newY][newX] != 0)
                    return true;
            }
        }
    }
    return false;
}



void rotateBlock() {
    int newRot = (currRot + 1) % 4;
    if (!checkCollision(currShape, newRot, currX, currY))
        currRot = newRot;
}



bool moveBlock(int dx, int dy) {
    int newX = currX + dx;
    int newY = currY + dy;
    if (!checkCollision(currShape, currRot, newX, newY)) {
        currX = newX;
        currY = newY;
        return true;
    }
    return false;
}



// 快速落地，但每次循环只落一格，保证主循环仍然执行
void dropBlock() {
    while (moveBlock(0, 1)) {
        cleardevice();
        drawGameArea();
        drawBlock(currShape, currRot, currX, currY);
        drawInfo();
        currentTime();
        FlushBatchDraw();
        Sleep(10);  // 控制下落速度，不要卡死
    }
}



void eliminateLines() {
    int lineCount = 0;
    for (int i = HEIGHT - 1; i >= 0; i--) {
        bool isFull = true;
        for (int j = 0; j < WIDTH; j++) {
            if (gameArea[i][j] == 0) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            lineCount++;
            for (int k = i; k > 0; k--)
                for (int j = 0; j < WIDTH; j++)
                    gameArea[k][j] = gameArea[k - 1][j];
            for (int j = 0; j < WIDTH; j++)
                gameArea[0][j] = 0;
            i++;
        }
    }

    switch (lineCount) {
    case 1: score += 100; break;
    case 2: score += 300; break;
    case 3: score += 500; break;
    case 4: score += 800; break;
    }
}




void updateTime() {
    gameTime = (int)(time(NULL) - startTime);
}



void currentTime() {
    WCHAR timeText[30];  // 宽字符数组
    swprintf_s(timeText, L"当前时间: %02d:%02d:%02d",
        (gameTime / 3600) % 24, (gameTime / 60) % 60, gameTime % 60);
    outtextxy(WIN_WIDTH - 150, WIN_HEIGHT - 30, timeText);
}




bool isGameOver() {
    // 如果当前方块任意一个方格与游戏区域重叠，则游戏结束
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[currShape][currRot][i][j]) {
                int x = currX + j;
                int y = currY + i;

                // 如果方块刚生成就占据了有效区域（y >= 0）并且发生碰撞
                if (y >= 0 && gameArea[y][x] != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}






void gameOver() {
    cleardevice();
    settextcolor(RED);
    settextstyle(40, 0, L"宋体");  // 宽字符字体名
    outtextxy(WIN_WIDTH / 2 - 120, WIN_HEIGHT / 2 - 60, L"游戏结束！");

    WCHAR scoreText[30];  // 宽字符数组
    swprintf_s(scoreText, L"最终分数: %d", score);
    outtextxy(WIN_WIDTH / 2 - 100, WIN_HEIGHT / 2, scoreText);

    settextstyle(20, 0, L"宋体");  // 宽字符字体名
    outtextxy(WIN_WIDTH / 2 - 80, WIN_HEIGHT / 2 + 40, L"按任意键退出...");
    _getch();
    closegraph();
}

