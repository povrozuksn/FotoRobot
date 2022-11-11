#include "TXLib.h"

struct Button
{
    int x;
    int y;
    const char* text;
    string category;
};

void drawButton(Button btn)
{
    txSetColor (TX_GRAY);
    txSetFillColor (TX_GRAY);
    Win32::RoundRect (txDC(), btn.x+5, btn.y+5, btn.x+155, btn.y+45, 30, 30);
    txSetColor (TX_BLACK);
    txSetFillColor (TX_WHITE);
    Win32::RoundRect (txDC(), btn.x, btn.y, btn.x+150, btn.y+40, 30, 30);
    txSetColor (TX_BLACK);
    txSelectFont("Times New Roman", 32);
    txDrawText(btn.x, btn.y, btn.x+150, btn.y+40, btn.text);
}

bool Click(Button btn)
{
    return( txMouseButtons() == 1 &&
            txMouseX() >= btn.x &&
            txMouseX() <= btn.x+150 &&
            txMouseY() >= btn.y &&
            txMouseY() <= btn.y+40);
}

struct Pictures
{
    int x;
    int y;
    string adress;
    HDC image;
    int w;
    int h;
    int w_scr;
    int h_scr;
    string category;
    bool visible;
};

void drawPicture(Pictures pct)
{
    if(pct.visible)
    {
        Win32::TransparentBlt(txDC(), pct.x, pct.y, pct.w_scr, pct.h_scr, pct.image, 0, 0, pct.w, pct.h, TX_WHITE);
    }
};


int main()
{
    txCreateWindow (1200, 800);
    txDisableAutoPause();
    txTextCursor (false);

    int COUNT_BTN = 5;
    int COUNT_PICTURES = 16;
    int vybor = -1;
    bool mouse_free = false;
    char str[100];

    //������ ������
    Button btn[COUNT_BTN];
    btn[0] = {100, 30, "���� ����", "����"};
    btn[1] = {260, 30, "������", "������"};
    btn[2] = {420, 30, "�����", "�����"};
    btn[3] = {580, 30, "����", "����"};
    btn[4] = {740, 30, "����", "����"};


    //������ ��������-����
    Pictures menuPicture[COUNT_PICTURES];
    menuPicture[0] = {50, 100, "Pictures/����/����1.bmp",     NULL, 450, 450, NULL, NULL, "����"};
    menuPicture[1] = {50, 200, "Pictures/����/����2.bmp",     NULL, 450, 450, NULL, NULL, "����"};
    menuPicture[2] = {50, 300, "Pictures/����/����3.bmp",     NULL, 450, 450, NULL, NULL, "����"};
    menuPicture[3] = {50, 100, "Pictures/������/������1.bmp", NULL, 350, 350, NULL, NULL, "������"};
    menuPicture[4] = {50, 200, "Pictures/������/������2.bmp", NULL, 350, 350, NULL, NULL, "������"};
    menuPicture[5] = {50, 300, "Pictures/������/������3.bmp", NULL, 350, 350, NULL, NULL, "������"};
    menuPicture[6] = {50, 100, "Pictures/�����/�����1.bmp",   NULL, 200, 65, NULL, NULL, "�����"};
    menuPicture[7] = {50, 200, "Pictures/�����/�����2.bmp",   NULL, 200, 65, NULL, NULL, "�����"};
    menuPicture[8] = {50, 300, "Pictures/�����/�����3.bmp",   NULL, 200, 65, NULL, NULL, "�����"};
    menuPicture[9] = {50, 100, "Pictures/����/���1.bmp",      NULL, 50, 65, NULL, NULL, "����"};
    menuPicture[10] = {50, 200, "Pictures/����/���2.bmp",     NULL, 50, 65, NULL, NULL, "����"};
    menuPicture[11] = {50, 300, "Pictures/����/���3.bmp",     NULL, 50, 65, NULL, NULL, "����"};
    menuPicture[12] = {50, 100, "Pictures/����/����1.bmp",    NULL, 100, 40, NULL, NULL, "����"};
    menuPicture[13] = {50, 200, "Pictures/����/����2.bmp",    NULL, 100, 40, NULL, NULL, "����"};
    menuPicture[14] = {50, 300, "Pictures/����/����3.bmp",    NULL, 100, 40, NULL, NULL, "����"};
    menuPicture[15] = {50, 400, "Pictures/����/����4.bmp",    NULL, 100, 40, NULL, NULL, "����"};

    //������ �������� � ������
    Pictures centralPicture[COUNT_PICTURES];
    centralPicture[0] = {450, 200};
    centralPicture[1] = {450, 200};
    centralPicture[2] = {450, 200};
    centralPicture[3] = {490, 150};
    centralPicture[4] = {490, 150};
    centralPicture[5] = {490, 150};
    centralPicture[6] = {570, 400};
    centralPicture[7] = {570, 400};
    centralPicture[8] = {570, 400};
    centralPicture[9] = {640, 460};
    centralPicture[10] = {640, 460};
    centralPicture[11] = {640, 460};
    centralPicture[12] = {620, 550};
    centralPicture[13] = {620, 550};
    centralPicture[14] = {620, 550};
    centralPicture[15] = {620, 550};

    for(int npic=0; npic < COUNT_PICTURES; npic++)
    {
        menuPicture[npic].image = txLoadImage (menuPicture[npic].adress.c_str());

        if(menuPicture[npic].category == "����" || menuPicture[npic].category == "������")
        {
            menuPicture[npic].w_scr = 100;
            menuPicture[npic].h_scr = 100;
        }

        if(menuPicture[npic].category == "�����" )
        {
            menuPicture[npic].w_scr = 100;
            menuPicture[npic].h_scr = 35;
        }

        if(menuPicture[npic].category == "����" || menuPicture[npic].category == "����")
        {
            menuPicture[npic].w_scr = menuPicture[npic].w;
            menuPicture[npic].h_scr = menuPicture[npic].h;
        }


        menuPicture[npic].visible = false;

        centralPicture[npic].image = menuPicture[npic].image;
        centralPicture[npic].w = menuPicture[npic].w;
        centralPicture[npic].h = menuPicture[npic].h;
        centralPicture[npic].w_scr = centralPicture[npic].w;
        centralPicture[npic].h_scr = centralPicture[npic].h;
        centralPicture[npic].visible = menuPicture[npic].visible;
        centralPicture[npic].category = menuPicture[npic].category;
    }

    while(!GetAsyncKeyState(VK_ESCAPE))
    {
        txBegin();
        txSetColor (TX_WHITE);
        txSetFillColor (TX_YELLOW);
        txClear();

        //��������� ������
        for(int nk=0; nk<COUNT_BTN; nk++)
        {
            drawButton(btn[nk]);
        }

        //��������� ����-��������
        for(int npic=0; npic < COUNT_PICTURES; npic++)
        {
            drawPicture(menuPicture[npic]);
        }

        //��������� �������� � ������
        for(int npic=0; npic < COUNT_PICTURES; npic++)
        {
            drawPicture(centralPicture[npic]);
        }

        //��������� �������� � ������ �� ��������� ����-��������
        for(int npic=0; npic < COUNT_PICTURES; npic++)
        {
            if(txMouseButtons() == 1 &&
                menuPicture[npic].visible &&
                txMouseX() >= menuPicture[npic].x &&
                txMouseX() <= menuPicture[npic].x + menuPicture[npic].w_scr &&
                txMouseY() >= menuPicture[npic].y &&
                txMouseY() <= menuPicture[npic].y + menuPicture[npic].h_scr)
                {
                    for(int npic1=0; npic1 < COUNT_PICTURES; npic1++)
                    {
                        if(centralPicture[npic1].category == centralPicture[npic].category)
                        {
                            centralPicture[npic1].visible = false;
                        }
                    }
                    centralPicture[npic].visible = !centralPicture[npic].visible;
                    txSleep(100);
                }
         }

        //��������� ����-�������� �� ��������� ������
        for(int nk=0; nk<COUNT_BTN; nk++)
        {
            if(Click(btn[nk]))
            {
                for(int npic=0; npic < COUNT_PICTURES; npic++)
                {
                    menuPicture[npic].visible = false;
                    if(menuPicture[npic].category == btn[nk].category)
                    {
                        menuPicture[npic].visible = true;
                    }
                }
            }
        }


        //����� ����������� ��������
        for(int npic=0; npic < COUNT_PICTURES; npic++)
        {
            if(txMouseButtons() == 1 &&
                centralPicture[npic].visible &&
                txMouseX() >= centralPicture[npic].x &&
                txMouseX() <= centralPicture[npic].x + centralPicture[npic].w &&
                txMouseY() >= centralPicture[npic].y &&
                txMouseY() <= centralPicture[npic].y + centralPicture[npic].h)
                {
                    vybor=npic;
                    mouse_free = false;
                }
        }

        sprintf(str, "����� = %d", vybor);
        //txTextOut(0,0, str);

        //������������ �������� ����������� �������� ���������
        if(vybor>=0)
        {
            if(GetAsyncKeyState(VK_RIGHT)) centralPicture[vybor].x += 3;
            if(GetAsyncKeyState(VK_LEFT))  centralPicture[vybor].x -= 3;
            if(GetAsyncKeyState(VK_UP))    centralPicture[vybor].y -= 3;
            if(GetAsyncKeyState(VK_DOWN))  centralPicture[vybor].y += 3;

            if(GetAsyncKeyState(VK_OEM_PLUS))
            {
                centralPicture[vybor].w_scr = centralPicture[vybor].w_scr * 1.02;
                centralPicture[vybor].h_scr = centralPicture[vybor].h_scr * 1.02;;
            }

            if(GetAsyncKeyState(VK_OEM_MINUS))
            {
                centralPicture[vybor].w_scr = centralPicture[vybor].w_scr * 0.98;
                centralPicture[vybor].h_scr = centralPicture[vybor].h_scr * 0.988;
            }
        }


        //������������ �������� ����������� �������� ������
        if(vybor>=0)
        {
            if(txMouseButtons() == 1 && !mouse_free)
            {
                centralPicture[vybor].x = txMouseX() - centralPicture[vybor].w/2;
                centralPicture[vybor].y = txMouseY() - centralPicture[vybor].h/2;
            }
            else
            {
                if(txMouseButtons() != 1)
                {
                    mouse_free = true;
                }
            }
        }


        txSleep(50);
        txEnd();

    }

    for(int npic=0; npic < COUNT_PICTURES; npic++)
    {
        txDeleteDC(menuPicture[npic].image);
    }


    for(int npic=0; npic < COUNT_PICTURES; npic++)
    {
        txDeleteDC(centralPicture[npic].image);
    }



return 0;
}

