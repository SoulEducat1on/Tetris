#define _CRT_SECURE_NO_WARNINGS  // ���sprintf��ȫ����
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <iostream>
using namespace std;

// ���峣���������С����Ϸ����ߴ�ȣ�
#define BLOCK_SIZE 20                       // ÿ��С��������ش�С
#define WIDTH 10                            // ��Ϸ�������������򷽿�����
#define HEIGHT 20                           // ��Ϸ�������������򷽿�����
#define WIN_WIDTH BLOCK_SIZE * (WIDTH + 6)  // ���ڿ�ȣ����Ҳ���Ϣ����
#define WIN_HEIGHT BLOCK_SIZE * HEIGHT      // ���ڸ߶�
#define GRAY RGB(192, 192, 192)             // �����ɫ����

// ���巽����ɫ����Ӧ��ͬ��״��
// ����:���ı���ɫ����Ϊ��ɫ������3��,SetTextColor(hdc, blockColor[3]);
COLORREF blockColor[] = {
    RGB(0, 0, 0),      // 0-�հף���ɫ��
    RGB(255, 0, 0),    // 1-��ɫ
    RGB(0, 255, 0),    // 2-��ɫ
    RGB(0, 0, 255),    // 3-��ɫ
    RGB(255, 255, 0),  // 4-��ɫ
    RGB(255, 165, 0),  // 5-��ɫ
    RGB(128, 0, 128),  // 6-��ɫ
    RGB(0, 255, 255),   // 7-��ɫ
	RGB(225,192,203),   // 8-��ɫ
	RGB(255,255,255)   // 9-��ɫ
};

//************��Ϸ��ȫ�ֱ���****************//
// ����7�ֶ���˹������״��4x4����
int blockShape[7][4][4][4] = {
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
// ��Ϸ��������,0��ʾ�հף���0��ʾ�з���
int gameArea[HEIGHT][WIDTH] = { 0 };
// ��ǰ������Ϣ,currShape:��������,currRot:��ת״̬,currX:������,currY:������
int currShape, currRot, currX, currY;
// ��һ����������
int nextShape;
// ��Ϸ����
int score = 0;
int gameTime = 0;
time_t startTime;   // ��Ϸ��ʼʱ��

// ��������
void initGame();    //��ʼ����Ϸ
void createBlock(int& shape, int& rot, int& x, int& y); //�����·���
void drawBlock(int shape, int rot, int x, int y);   //���Ʒ���
void drawGameArea();    //������Ϸ����
void drawInfo();    //�����Ҳ���Ϣ��
bool checkCollision(int shape, int rot, int x, int y); //�����ײ
void rotateBlock(); //��ת����
bool moveBlock(int dx, int dy); //�ƶ�����
void dropBlock();   //�������
void eliminateLines();  //��������
void updateTime();  //  ����ʱ��
bool isGameOver(int shape, int rot, int x, int y);  //�����Ϸ����
bool checkAnyKeyPressed(); //����Ƿ��������λ
void gameOver();    //������Ϸ����
void handleInput(); //�������뺯��

int main() {
	initgraph(WIN_WIDTH, WIN_HEIGHT);   // ��ʼ��ͼ�δ���
    BeginBatchDraw();//����˫����
	initGame(); // ��ʼ����Ϸ���ݣ����ɳ�ʼ���顢��յ�ͼ����ʼ��������ʱ�䣩
    while (true) 
    {
		/*if (_kbhit()){ // ����������
			switch (_getch()) {// ��ȡ����
            case 'w': case 'W': rotateBlock(); break;
            case 'a': case 'A': moveBlock(-1, 0); break;
            case 'd': case 'D': moveBlock(1, 0); break;
            case 's': case 'S': moveBlock(0, 1); break;
			case ' ': dropBlock(); break;   // �ո���������
			case 'q': case 'Q': closegraph(); return 0;// �˳���Ϸ
            }
        }*/
        handleInput();
		static DWORD lastDropTime = 0;  // ��¼�ϴ��Զ�����ʱ��
        if (GetTickCount() - lastDropTime > 500) {
			if (!moveBlock(0, 1)) { // �޷����ƣ��̶�����
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
						if (blockShape[currShape][currRot][i][j]) { // �̶����鵽��Ϸ����
                            int x = currX + j;
                            int y = currY + i;
							if (y >= 0) gameArea[y][x] = currShape + 1; // ʹ�÷�0ֵ��ʾ�з���
                        }
                    }
                }
				eliminateLines();   // ��������

                if (isGameOver(currShape, currRot, currX, currY)) { // ��������ɵĵ�ǰ�����Ƿ���ײ����Ϸ�����ж���
                    gameOver();
                    break;  //������ѭ��
                }
				// �����·���
                currShape = nextShape;
				currRot = 0;               // �·����ʼ��ת״̬
				currX = WIDTH / 2 - 2;    // �·���X��ʼλ��
				currY = -4;              // �·���Y��ʼλ��
				createBlock(nextShape, currRot, currX, currY);  
            }
			lastDropTime = GetTickCount();  // ��������ʱ��
        }

		//������ʾ
        updateTime();
        cleardevice(); 
        drawGameArea(); 
        drawBlock(currShape, currRot, currX, currY); 
        drawInfo(); 

		FlushBatchDraw();   // ˢ�»�ͼ����˫�����е�������ʾ����Ļ��
		Sleep(20);   // ����CPUռ��
    }
	closegraph();   // �ر�ͼ�δ���
    return 0;
}

//��ʼ����Ϸ
void initGame() {
    srand((unsigned int)time(NULL));
    startTime = time(NULL);

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            gameArea[i][j] = 0;
	int tempX, tempY;  // ��ʱ����,��ֹcreateBlock�޸���һ��currX,currY
    tempX = currX;  
    tempY = currY;
	createBlock(currShape, currRot, tempX,tempY);  // ������ǰ����
	createBlock(nextShape, currRot, currX, currY);  // ������һ������
}

//�����·���
void createBlock(int& shape,int& rot,int& x,int& y) {    
	shape = rand() % 7;   // ���ѡ�񷽿���״
	rot = 0;             // ��ʼ��ת״̬
	x = WIDTH/2-2;      // ��ʼλ���ڶ�������
	y = -4;            // ��ʼλ���ڶ�����
}

//���Ʒ���
void drawBlock(int shape, int rot, int x, int y) {  // x,yΪ�������Ͻ�����Ϸ�����е�����,rotΪ��ת״̬,shapeΪ������״
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[shape][rot][i][j] && (y + i >= 0)) {
                int drawX = (x + j) * BLOCK_SIZE;
                int drawY = (y + i) * BLOCK_SIZE;
				setfillcolor(blockColor[shape + 1]);                    // shape��0��ʼ����ɫ��1��ʼ
				solidrectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);// ���Ʒ���
				setlinecolor(blockColor[0]);                // ���ñ߿���ɫ
				rectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);    // ���Ʊ߿�
            }
        }
    }
}

//������Ϸ����
void drawGameArea() {
	setlinecolor(RED);  // ���ñ߿���ɫ
	rectangle(0, 0, WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE);   // ���Ʊ߿�

	// ����������ѹ̶��ķ���
	for (int i = 0; i < HEIGHT; i++) { 
        for (int j = 0; j < WIDTH; j++) {
            int x = j * BLOCK_SIZE;
            int y = i * BLOCK_SIZE;
			setlinecolor(LIGHTGRAY);    // ������������ɫ
			line(x, 0, x, HEIGHT * BLOCK_SIZE);     // ��������,line(x1, y1, x2, y2),x1=x2ʱΪ����,y1=y2ʱΪ����
			line(0, y, WIDTH * BLOCK_SIZE, y);      // ���ƺ���,x1��ʾ��������,y1��ʾ���������,x2��ʾ�յ������,y2��ʾ�յ�������

            if (gameArea[i][j] != 0) {
				setfillcolor(blockColor[gameArea[i][j]]);                      // ���÷�����ɫ
				solidrectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1); // ���Ʒ���
				setlinecolor(BLACK);                                         // ���ñ߿���ɫ
				rectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1);    // ���Ʊ߿�
            }
        }
    }
}

//�����Ҳ���Ϣ��
void drawInfo() {

	int infoX = WIDTH * BLOCK_SIZE + 10;  // �Ҳ���Ϣ����ʼX����
	settextcolor(blockColor[9]);          // �����ı���ɫ
	setbkmode(TRANSPARENT);     // �����ı�����͸��

    // ���Ʒ�����ʹ�ÿ��ַ������swprintf_s��
    WCHAR scoreText[20];  // ���ַ�����
    swprintf_s(scoreText, L"����: %d", score);  // ֱ�Ӹ�ʽ�����ַ���
	outtextxy(infoX, 20, scoreText);    // ���Ʒ���

    // ������Ϸʱ��
    WCHAR timeText[20];  // ���ַ�����
    swprintf_s(timeText, L"ʱ��: %ds", gameTime);  // ֱ�Ӹ�ʽ�����ַ���
	outtextxy(infoX, 60, timeText);     // ����ʱ��
     
    //������һ��������ʾ
    outtextxy(infoX, 100, L"��һ��:");  // ֱ��ʹ�ÿ��ַ���ǰ׺L

    // ������һ������
    int nextX = infoX + (BLOCK_SIZE * 4 - BLOCK_SIZE * 2) / 2;
    int nextY = 120;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[nextShape][0][i][j]) {
                int drawX = nextX+j*BLOCK_SIZE;
                int drawY = nextY+i*BLOCK_SIZE;
                setfillcolor(blockColor[nextShape + 1]);                            //������ɫ
				solidrectangle(drawX,drawY,drawX+BLOCK_SIZE-1,drawY+BLOCK_SIZE-1);//���Ʒ���
				setlinecolor(blockColor[0]);                                            //�߿���ɫ
				rectangle(drawX,drawY,drawX+BLOCK_SIZE-1,drawY+BLOCK_SIZE-1);//���Ʊ߿�
            }
        }
    }

	// ���Ʋ�����ʾ��ֱ��ʹ�ÿ��ַ���ǰ׺L),outtextxy(x, y, L"�ı�����")����֧�ֿ��ַ��������
    outtextxy(infoX, 220, L"����:");
    outtextxy(infoX, 240, L"W - ��ת");
    outtextxy(infoX, 260, L"A - ����");
    outtextxy(infoX, 280, L"D - ����");
    outtextxy(infoX, 300, L"S - ����");
    outtextxy(infoX, 320, L"�ո� - ���");
    outtextxy(infoX, 340, L"Q - �˳�");

    //���Ƶ�ǰ��Ϸʱ��
    time_t now = time(NULL);
    tm* localTime = localtime(&now);

 

    WCHAR timeText1[40];
    swprintf_s(timeText1, 40, L"%02d:%02d:%02d",
        localTime->tm_hour,
        localTime->tm_min,
        localTime->tm_sec);
    outtextxy(infoX, 360, timeText1);
}


//�����ײ
bool checkCollision(int shape, int rot, int x, int y) {
    // ����4x4��������ÿ�����ӣ�����˹�����׼��4x4����洢��״��
    for (int i = 0; i < 4; i++) {       // i�����������У���Ӧ��Ϸ�����Y����
        for (int j = 0; j < 4; j++) {   // j�����������У���Ӧ��Ϸ�����X����
            // ֻ������ʵ�ʴ��ڵĸ��ӣ�blockShapeΪ1�Ĳ��֣�
            if (blockShape[shape][rot][i][j] == 1) {
                // ���㵱ǰ��������Ϸ�����е�ʵ������
                int gridX = x + j;  // ת��Ϊ��Ϸ�����������
                int gridY = y + i;  // ת��Ϊ��Ϸ�����������

                // 1. ���ұ߽���ײ��������Ϸ�������ұ�Ե����ȫ��ֹ��
                if (gridX < 0 || gridX >= WIDTH) {
                    return true;  // ���Խ����ұ�Խ�磬��ײ
                }

                // 2. �ײ��߽���ײ��������Ϸ�����±�Ե������ж���
                if (gridY >= HEIGHT) {
                    return true;  // �ײ�Խ�磬��ײ���޷��������ƣ�
                }

                // 4. ���ѹ̶��������ײ����ǰ��������Ϸ�����ڣ�gridY >=0���������з���
                if (gameArea[gridY][gridX] != 0) {
                    return true;        // ���ѹ̶��ķ����ص�����ײ
                }
            }
        }
    }
    // ���и��Ӷ�����ײ
    return false;
}

//��ת����
void rotateBlock() {
	int newRot = (currRot + 1) % 4;                        // �����µ���ת״̬(ѭ��)
	if (!checkCollision(currShape, newRot, currX, currY)) // ����ײ��������ת״̬
        currRot = newRot;
}

//�ƶ�����
bool moveBlock(int dx, int dy) {
    int newX = currX + dx;
    int newY = currY + dy;
	if (!checkCollision(currShape, currRot, newX, newY)) {  // ����ײ������λ��
        currX = newX;
        currY = newY;
        return true;
    }
    return false;
}

//�������
void dropBlock() {
    while (moveBlock(0, 1));
}

//��������
void eliminateLines() {
	int lineCount = 0;  // ��¼����������
	for (int i = HEIGHT - 1; i >= 0; i--) {     // �ӵײ���ʼ���ÿһ��
		bool isFull = true;                     // ����������ǰ���Ƿ���
		for (int j = 0; j < WIDTH; j++) {      //ÿ�м���Ƿ��з���
            if (gameArea[i][j] == 0) {
                isFull = false;
                break;
            }
        }

		if (isFull) {       // �����ǰ��������������
            lineCount++;
            for (int k = i;k>0;k--)
                for (int j = 0; j<WIDTH; j++)
                    gameArea[k][j] = gameArea[k-1][j];
            for (int j = 0; j<WIDTH;j++)
                gameArea[0][j] = 0;
            i++;
        }
    }

	switch (lineCount) {    // ���������������·���
    case 1: score += 10; break;
    case 2: score += 30; break;
    case 3: score += 50; break;
	case 4: score += 80; break; // ����һ�����������÷����
    }
}

// ����ʱ��
void updateTime() {
    gameTime = (int)(time(NULL) - startTime);
}

// ��ʾ��ǰʱ��   
void currentTime() {
	WCHAR timeText[20];  // ���ַ�����

	swprintf_s(timeText, L"��ǰʱ��: %02d:%02d:%02d",
		(gameTime / 3600) % 24, (gameTime / 60) % 60, gameTime % 60);

	outtextxy(WIN_WIDTH - 150, WIN_HEIGHT - 30, timeText);
}

//�����Ϸ����
bool isGameOver(int shape, int rot, int x, int y) {
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++)    
            if (blockShape[shape][rot][i][j] == 1) {
                int gridY = y + i;  // ת��Ϊ��Ϸ�����������
                if (gridY < 0) {
                    return true;
                }
            }
    return false;
}

//������Ϸ����
void gameOver() {
    cleardevice();  // ����
    while(1) {
        settextcolor(RED);  // �����ı���ɫ
        settextstyle(40, 0, L"����");  // ���ַ�����������������
        outtextxy(WIN_WIDTH / 2 - 120, WIN_HEIGHT / 2 - 60, L"��Ϸ������");

        WCHAR scoreText[20];  // ���ַ�����,������ʾ
        swprintf_s(scoreText, L"���շ���: %d", score);
        outtextxy(WIN_WIDTH / 2 - 100, WIN_HEIGHT / 2, scoreText);

        settextstyle(20, 0, L"����");  // ���ַ�������
        outtextxy(WIN_WIDTH / 2 - 80, WIN_HEIGHT / 2 + 40, L"��������˳�...");
        if (checkAnyKeyPressed())
            break;
        FlushBatchDraw();   // ˢ�»�ͼ����˫�����е�������ʾ����Ļ��
        Sleep(10);   // ����CPUռ��
    }
}

//���������������λ
bool checkAnyKeyPressed() {
    for (int key = 8; key <= 255; ++key) { //����һ���λ
        if (GetAsyncKeyState(key) & 0x8000) {
            return true;  // �������������
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

//��������
void handleInput() {

    //������λ�Ƿ񱻰��µ�boolֵ ��0x8000�ǰ���)
    bool isW = GetAsyncKeyState('W') & 0x8000;
    bool isA = GetAsyncKeyState('A') & 0x8000;
    bool isS = GetAsyncKeyState('S') & 0x8000;
    bool isD = GetAsyncKeyState('D') & 0x8000;
    bool isQ = GetAsyncKeyState('Q') & 0x8000;
    bool isVK_SPACE = GetAsyncKeyState(VK_SPACE) & 0x8000;


    // ֻ���ڱ�֡���£���һ֡û����ʱ����ִ��һ��
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

    

    // ������һ֡��״̬
    wasKeyPressedW = isW;
    wasKeyPressedA = isA;
    wasKeyPressedS = isS;
    wasKeyPressedD = isD;
}