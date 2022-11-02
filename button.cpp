#include "TXLib.h"


struct Button
{
    int x;
    int y;
    const char* text;
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


