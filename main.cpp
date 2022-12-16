#include "TXLib.h"
#include <iostream>
#include <fstream>
#include "dirent.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

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
}

int get_w(string adress)
{
    FILE *f1 = fopen(adress.c_str(), "rb");

    unsigned char headerinfo[54];
    fread(headerinfo, sizeof(unsigned char),54,f1);

    int w = *(int*)&headerinfo[18];

    return w;
}

int get_h(string adress)
{
    FILE *f1 = fopen(adress.c_str(), "rb");

    unsigned char headerinfo[54];
    fread(headerinfo, sizeof(unsigned char),54,f1);

    int h = *(int*)&headerinfo[22];

    return h;
}

int readFromDir(string adress, Pictures menuPicture[], int COUNT_PICTURES)
{
    DIR *dir;
    struct dirent *ent;
    int lastY = 100;
    if ((dir = opendir (adress.c_str())) != NULL)
    {
      while ((ent = readdir (dir)) != NULL)
      {
        if((string)ent->d_name != "." && (string)ent->d_name != "..")
        {
            menuPicture[COUNT_PICTURES].y = lastY;
            menuPicture[COUNT_PICTURES].adress = adress + (string)ent->d_name;
            COUNT_PICTURES ++;
            lastY +=100;
        }
      }
      closedir (dir);
    }
    return COUNT_PICTURES;
}


//������ �������� / ���������� �����
string runFileDialog(bool isSave)
{
    string fileName = "";

    OPENFILENAME ofn;       // common dialog box structure
    TCHAR szFile[260] = { 0 };       // if using TCHAR macros

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = txWindow();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = ("Text\0*.TXT\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (isSave)
    {
        if (GetSaveFileName(&ofn) == TRUE)
        {
            fileName = ofn.lpstrFile;

            if (fileName.find(".txt") > 1000)
            {
                fileName = fileName + ".txt";
            }
        }
    }
    else
    {
        if (GetOpenFileName(&ofn) == TRUE)
        {
            fileName = ofn.lpstrFile;
        }
    }

    return fileName;
}

const int COUNT_BTN = 7;
const int BTN_SAVE = COUNT_BTN - 2;
const int BTN_LOAD = COUNT_BTN - 1;


int main()
{
    txCreateWindow (1200, 800);
    txDisableAutoPause();
    txTextCursor (false);

    int COUNT_PICTURES = 0;
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
    btn[5] = {1000, 30, "���������", ""};
    btn[6] = {1000, 80, "���������", ""};


    //������ ��������-����
    Pictures menuPicture[100];

    //������ �������� � ������
    Pictures centralPicture[100];

    COUNT_PICTURES = readFromDir("Pictures/����/", menuPicture, COUNT_PICTURES);
    COUNT_PICTURES = readFromDir("Pictures/������/", menuPicture, COUNT_PICTURES);
    COUNT_PICTURES = readFromDir("Pictures/�����/", menuPicture, COUNT_PICTURES);
    COUNT_PICTURES = readFromDir("Pictures/����/", menuPicture, COUNT_PICTURES);
    COUNT_PICTURES = readFromDir("Pictures/����/", menuPicture, COUNT_PICTURES);


    for(int npic=0; npic < COUNT_PICTURES; npic++)
    {
        menuPicture[npic].x = 50;

        menuPicture[npic].image = txLoadImage (menuPicture[npic].adress.c_str());

        menuPicture[npic].w = get_w(menuPicture[npic].adress);
        menuPicture[npic].h = get_h(menuPicture[npic].adress);

        int pos_1 = menuPicture[npic].adress.find("/");
        int pos_2 = menuPicture[npic].adress.find("/", pos_1+1);
        menuPicture[npic].category = menuPicture[npic].adress.substr(pos_1+1, pos_2-pos_1-1);

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

        centralPicture[npic].adress = menuPicture[npic].adress;
        centralPicture[npic].image = menuPicture[npic].image;
        centralPicture[npic].w = menuPicture[npic].w;
        centralPicture[npic].h = menuPicture[npic].h;
        centralPicture[npic].w_scr = centralPicture[npic].w;
        centralPicture[npic].h_scr = centralPicture[npic].h;
        centralPicture[npic].visible = menuPicture[npic].visible;
        centralPicture[npic].category = menuPicture[npic].category;

        if(centralPicture[npic].category == "����" )
        {
            centralPicture[npic].x = 450;
            centralPicture[npic].y = 200;
        }

        if(centralPicture[npic].category == "������" )
        {
            centralPicture[npic].x = 490;
            centralPicture[npic].y = 150;
        }

        if(centralPicture[npic].category == "�����" )
        {
            centralPicture[npic].x = 570;
            centralPicture[npic].y = 400;
        }

        if(centralPicture[npic].category == "����" )
        {
            centralPicture[npic].x = 640;
            centralPicture[npic].y = 460;
        }

        if(centralPicture[npic].category == "����" )
        {
            centralPicture[npic].x = 620;
            centralPicture[npic].y = 550;
        }

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

        //���������
        if(Click(btn[BTN_SAVE]))
        {
            string fileName = runFileDialog(true);
            if (fileName != "")
            {
                ofstream fout; //������ ��� ���� ����������

                fout.open(fileName); //������� ���� ��� ������

                for (int npic = 0; npic < COUNT_PICTURES; npic++)
                {
                    if (centralPicture[npic].visible)
                    {
                        fout << centralPicture[npic].x << endl; //���-�� ��������
                        fout << centralPicture[npic].y << endl;
                        fout << centralPicture[npic].adress << endl;
                    }
                }
                fout.close();            //������� ����

                txMessageBox("���������", "�������", MB_ICONINFORMATION);
            }

        }


        //���������
        if (Click(btn[BTN_LOAD]))
        {
            string fileName = runFileDialog(false);
            if (fileName != "")
            {
                for (int npic = 0; npic < COUNT_PICTURES; npic++)
                {
                    centralPicture[npic].visible = false;
                }

                char buff[50];              // ���� ����� ��������� �����
                ifstream fin(fileName);      // ������� ���� ��� ������
                while (fin.good())
                {
                    fin.getline(buff, 50); // ������� ������ �� �����
                    int x = atoi(buff);
                    fin.getline(buff, 50); // ������� ������ �� �����
                    int y = atoi(buff);
                    fin.getline(buff, 50); // ������� ������ �� �����
                    string adress = (buff);

                    for (int npic = 0; npic < COUNT_PICTURES; npic++)
                    {
                        if (centralPicture[npic].adress == adress)
                        {
                            centralPicture[npic].x = x;
                            centralPicture[npic].y = y;
                            centralPicture[npic].visible = true;
                        }
                    }
                }
                fin.close();                //������� ����
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

