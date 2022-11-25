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

    //Массив кнопок
    Button btn[COUNT_BTN];
    btn[0] = {100, 30, "Овал лица", "Лицо"};
    btn[1] = {260, 30, "Волосы", "Волосы"};
    btn[2] = {420, 30, "Глаза", "Глаза"};
    btn[3] = {580, 30, "Носы", "Носы"};
    btn[4] = {740, 30, "Губы", "Губы"};


    //Массив картинок-меню
    Pictures menuPicture[COUNT_PICTURES];
    menuPicture[0] = {NULL, 100, "Pictures/Лицо/Овал1.bmp"};
    menuPicture[1] = {NULL, 200, "Pictures/Лицо/Овал2.bmp"};
    menuPicture[2] = {NULL, 300, "Pictures/Лицо/Овал3.bmp"};
    menuPicture[3] = {NULL, 100, "Pictures/Волосы/Волосы1.bmp"};
    menuPicture[4] = {NULL, 200, "Pictures/Волосы/Волосы2.bmp"};
    menuPicture[5] = {NULL, 300, "Pictures/Волосы/Волосы3.bmp"};
    menuPicture[6] = {NULL, 100, "Pictures/Глаза/Глаза1.bmp"};
    menuPicture[7] = {NULL, 200, "Pictures/Глаза/Глаза2.bmp"};
    menuPicture[8] = {NULL, 300, "Pictures/Глаза/Глаза3.bmp"};
    menuPicture[9] = {NULL, 100, "Pictures/Носы/нос1.bmp"};
    menuPicture[10] = {NULL, 200, "Pictures/Носы/нос2.bmp"};
    menuPicture[11] = {NULL, 300, "Pictures/Носы/нос3.bmp"};
    menuPicture[12] = {NULL, 100, "Pictures/Губы/Губы1.bmp"};
    menuPicture[13] = {NULL, 200, "Pictures/Губы/Губы2.bmp"};
    menuPicture[14] = {NULL, 300, "Pictures/Губы/Губы3.bmp"};
    menuPicture[15] = {NULL, 400, "Pictures/Губы/Губы4.bmp"};

    //Массив картинок в центре
    Pictures centralPicture[COUNT_PICTURES];


    for(int npic=0; npic < COUNT_PICTURES; npic++)
    {
        menuPicture[npic].x = 50;

        menuPicture[npic].image = txLoadImage (menuPicture[npic].adress.c_str());

        menuPicture[npic].w = get_w(menuPicture[npic].adress);
        menuPicture[npic].h = get_h(menuPicture[npic].adress);

        int pos_1 = menuPicture[npic].adress.find("/");
        int pos_2 = menuPicture[npic].adress.find("/", pos_1+1);
        menuPicture[npic].category = menuPicture[npic].adress.substr(pos_1+1, pos_2-pos_1-1);

        if(menuPicture[npic].category == "Лицо" || menuPicture[npic].category == "Волосы")
        {
            menuPicture[npic].w_scr = 100;
            menuPicture[npic].h_scr = 100;
        }

        if(menuPicture[npic].category == "Глаза" )
        {
            menuPicture[npic].w_scr = 100;
            menuPicture[npic].h_scr = 35;
        }

        if(menuPicture[npic].category == "Носы" || menuPicture[npic].category == "Губы")
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

        if(centralPicture[npic].category == "Лицо" )
        {
            centralPicture[npic].x = 450;
            centralPicture[npic].y = 200;
        }

        if(centralPicture[npic].category == "Волосы" )
        {
            centralPicture[npic].x = 490;
            centralPicture[npic].y = 150;
        }

        if(centralPicture[npic].category == "Глаза" )
        {
            centralPicture[npic].x = 570;
            centralPicture[npic].y = 400;
        }

        if(centralPicture[npic].category == "Носы" )
        {
            centralPicture[npic].x = 640;
            centralPicture[npic].y = 460;
        }

        if(centralPicture[npic].category == "Губы" )
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

        //Рисование кнопок
        for(int nk=0; nk<COUNT_BTN; nk++)
        {
            drawButton(btn[nk]);
        }

        //Рисование меню-картинок
        for(int npic=0; npic < COUNT_PICTURES; npic++)
        {
            drawPicture(menuPicture[npic]);
        }

        //Рисование картинок в центре
        for(int npic=0; npic < COUNT_PICTURES; npic++)
        {
            drawPicture(centralPicture[npic]);
        }

        //Видимость картинок в центре по категории меню-картинок
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

        //Видимость меню-картинок по категории кнопки
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


        //Выбор центральной картинки
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

        sprintf(str, "выбор = %d", vybor);
        //txTextOut(0,0, str);

        //Передвижение выбраной центральной кортинки клавишами
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


        //Передвижение выбраной центральной кортинки мышкой
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

