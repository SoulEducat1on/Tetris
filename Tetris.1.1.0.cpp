#define _CRT_SECURE_NO_WARNINGS  // ���sprintf��ȫ����
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <iostream>
using namespace std;

// ���峣���������С����Ϸ����ߴ�ȣ�
#define BLOCK_SIZE 20    // ÿ��С��������ش�С
#define WIDTH 10         // ��Ϸ�������������򷽿�����
#define HEIGHT 20        // ��Ϸ�������������򷽿�����
#define WIN_WIDTH BLOCK_SIZE * (WIDTH + 6)  // ���ڿ�ȣ����Ҳ���Ϣ����
#define WIN_HEIGHT BLOCK_SIZE * HEIGHT      // ���ڸ߶�
#define GRAY RGB(192, 192, 192)  // �����ɫ����

// ���巽����ɫ��8�֣���Ӧ��ͬ��״��
COLORREF blockColor[] = {
    RGB(0, 0, 0),      // 0-�հף���ɫ��
    RGB(255, 0, 0),    // 1-��ɫ
    RGB(0, 255, 0),    // 2-��ɫ
    RGB(0, 0, 255),    // 3-��ɫ
    RGB(255, 255, 0),  // 4-��ɫ
    RGB(255, 165, 0),  // 5-��ɫ
    RGB(128, 0, 128),  // 6-��ɫ
    RGB(255, 255, 255)   // 7-��ɫ
};

// ����7�ֶ���˹������״��4x4����
int blockShape[7][4][4][4] = {  // [��״][״̬][��][��]
    // I�ͣ�4����ת״̬����/����
    {
        {{1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0}},  // ״̬0������
        {{1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬1������
        {{1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0}},  // ״̬2��������ͬ״̬0��
        {{1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}   // ״̬3��������ͬ״̬1��
    },
    // L�ͣ�4����ת״̬���ҡ��¡����ϣ�
    {
        {{1,0,0,0}, {1,0,0,0}, {1,1,0,0}, {0,0,0,0}},  // ״̬0����L
        {{1,1,1,0}, {1,0,0,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬1����L
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // ״̬2����L
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}   // ״̬3����L
    },
    // ��L�ͣ�4����ת״̬�����¡��ҡ��ϣ�
    {
        {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}},  // ״̬0����L
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬1���·�L
        {{1,1,0,0}, {1,0,0,0}, {1,0,0,0}, {0,0,0,0}},  // ״̬2���ҷ�L
        {{1,1,1,0}, {0,0,1,0}, {0,0,0,0}, {0,0,0,0}}   // ״̬3���Ϸ�L
    },
    // ���Σ�4��״̬��ͬ����ת�󲻱䣩
    {
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬0
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬1
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬2
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}   // ״̬3
    },
    // T�ͣ�4����ת״̬���ϡ��ҡ��¡���
    {
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬0����T
        {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // ״̬1����T
        {{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}},  // ״̬2����T
        {{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}}   // ״̬3����T
    },
    // Z�ͣ�2����ת״̬���ҡ����ظ����Σ�
    {
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬0����Z
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}},  // ״̬1����Z
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬2����Z��ͬ״̬0��
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}   // ״̬3����Z��ͬ״̬1��
    },
    // ��Z�ͣ�2����ת״̬�����ң��ظ����Σ�
    {
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬0����Z
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // ״̬1���ҷ�Z
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // ״̬2����Z��ͬ״̬0��
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}   // ״̬3���ҷ�Z��ͬ״̬1��
    }
};


// ��Ϸ��������
int gameArea[HEIGHT][WIDTH] = { 0 };
// ��ǰ������Ϣ
int currShape, currRot, currX, currY;
// ��һ����������
int nextShape;
// ��Ϸ����
int score = 0;
int gameTime = 0;
time_t startTime;

// ��������
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
void currentTime();  // ��ǰ����
bool blockLocked = false;

int main() {
    initgraph(WIN_WIDTH, WIN_HEIGHT);
    
    //����˫����
    BeginBatchDraw();
   
    // ��ʼ����Ϸ���ݣ����ɳ�ʼ���顢��յ�ͼ����ʼ��������ʱ�䣩
    initGame();


    // ��¼�����������һ��ʱ��
    DWORD lastDropTime = GetTickCount();
    DWORD rotateCooldown = 150;      // ��ת��ȴʱ�䣨ms��
    DWORD lastRotateTime = 0;

    bool running = true;
    bool isDropping = false;  // ������ر��

    while (running) {
        DWORD now = GetTickCount();

        // ------------------- ����˳� -------------------
        if (GetAsyncKeyState('Q') & 0x8000) {
            running = false;
            break;
        }

        //GetAsyncKeyState ����� _kbhit() + _getch() �ķ�ʽ��ⰴ��
        // ------------------- �����ת -------------------
        if ((GetAsyncKeyState('W') & 0x8000) && (now - lastRotateTime > rotateCooldown)) {
            int newRot = (currRot + 1) % 4;
            // ������ת
            if (!checkCollision(currShape, newRot, currX, currY)) {
                currRot = newRot;
            }
            else if (!checkCollision(currShape, newRot, currX - 1, currY)) {
                currX--; currRot = newRot; // ��ƫ����ת
            }
            else if (!checkCollision(currShape, newRot, currX + 1, currY)) {
                currX++; currRot = newRot; // ��ƫ����ת
            }
            lastRotateTime = now;
        }

        if (GetAsyncKeyState('A') & 0x8000) moveBlock(-1, 0);
        if (GetAsyncKeyState('D') & 0x8000) moveBlock(1, 0);
        if (GetAsyncKeyState('S') & 0x8000) moveBlock(0, 1);


        // ------------------- ������� -------------------
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            isDropping = true;
        }

        // ------------------- �Զ����� -------------------
        if (isDropping) {
            // ÿ֡����������ֱ����ײ
            if (!moveBlock(0, 1)) {  // �Ѿ�����
                isDropping = false;
                // �̶�����

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
        else if (now - lastDropTime > 500) {  // ��������
            if (!moveBlock(0, 1)) {  // ����
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
        // ------------------- ����ʱ�� -------------------
        updateTime();


        // ------------------- ���ƻ��� ------------------
        cleardevice();
        drawGameArea();
        drawBlock(currShape, currRot, currX, currY);
        drawInfo();
        currentTime();

        // ��˫�����е�������ʾ����Ļ��
        FlushBatchDraw();

        // ------------------- ����ѭ��Ƶ�� -------------------
        Sleep(20);  // ��ʱ 20ms������ CPU ռ�ã�ͬʱ��֤�����������
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
    // ֻ������һ���������״������Ҫ����
    nextShape = rand() % 7;
}

void createBlock(int& shape, int& rot, int& x, int& y) {
    shape = rand() % 7;// ���ѡ�񷽿���״
    rot = 0;             // ��ʼ��ת״̬
    x = WIDTH / 2 - 2;      // ��ʼλ���ڶ�������
    y = -4;            // ��ʼλ���ڶ�����
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

    // ���Ʒ�����ʹ�ÿ��ַ������swprintf_s��
    WCHAR scoreText[20];  // ���ַ�����
    swprintf_s(scoreText, L"����: %d", score);  // ֱ�Ӹ�ʽ�����ַ���
    outtextxy(infoX, 20, scoreText);

    // ������Ϸʱ��
    WCHAR timeText[20];  // ���ַ�����
    swprintf_s(timeText, L"ʱ��: %ds", gameTime);  // ֱ�Ӹ�ʽ�����ַ���
    outtextxy(infoX, 60, timeText);



    // ������һ��������ʾ
    outtextxy(infoX, 100, L"��һ��:");  // ֱ��ʹ�ÿ��ַ���ǰ׺L

    // ������һ������
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

    // ���Ʋ�����ʾ��ֱ��ʹ�ÿ��ַ���ǰ׺L��
    outtextxy(infoX, 220, L"����:");
    outtextxy(infoX, 240, L"W - ��ת");
    outtextxy(infoX, 260, L"A - ����");
    outtextxy(infoX, 280, L"D - ����");
    outtextxy(infoX, 300, L"S - ����");
    outtextxy(infoX, 320, L"�ո� - ���");
    outtextxy(infoX, 340, L"Q - �˳�");
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



// ������أ���ÿ��ѭ��ֻ��һ�񣬱�֤��ѭ����Ȼִ��
void dropBlock() {
    while (moveBlock(0, 1)) {
        cleardevice();
        drawGameArea();
        drawBlock(currShape, currRot, currX, currY);
        drawInfo();
        currentTime();
        FlushBatchDraw();
        Sleep(10);  // ���������ٶȣ���Ҫ����
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
    WCHAR timeText[30];  // ���ַ�����
    swprintf_s(timeText, L"��ǰʱ��: %02d:%02d:%02d",
        (gameTime / 3600) % 24, (gameTime / 60) % 60, gameTime % 60);
    outtextxy(WIN_WIDTH - 150, WIN_HEIGHT - 30, timeText);
}




bool isGameOver() {
    // �����ǰ��������һ����������Ϸ�����ص�������Ϸ����
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[currShape][currRot][i][j]) {
                int x = currX + j;
                int y = currY + i;

                // �����������ɾ�ռ������Ч����y >= 0�����ҷ�����ײ
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
    settextstyle(40, 0, L"����");  // ���ַ�������
    outtextxy(WIN_WIDTH / 2 - 120, WIN_HEIGHT / 2 - 60, L"��Ϸ������");

    WCHAR scoreText[30];  // ���ַ�����
    swprintf_s(scoreText, L"���շ���: %d", score);
    outtextxy(WIN_WIDTH / 2 - 100, WIN_HEIGHT / 2, scoreText);

    settextstyle(20, 0, L"����");  // ���ַ�������
    outtextxy(WIN_WIDTH / 2 - 80, WIN_HEIGHT / 2 + 40, L"��������˳�...");
    _getch();
    closegraph();
}

