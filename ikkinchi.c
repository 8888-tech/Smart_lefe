#include "klinika.h"
#include <stdbool.h> // bool ishlatish uchun kerak

void IkkinchiOyna(int *screen, Vector2 mouse, int *userType, int isReg) {
    DrawText("KIM SIZ?", 340, 100, 30, DARKGRAY);

    Rectangle b1 = {250, 180, 300, 50};
    Rectangle b2 = {250, 250, 300, 50};
    Rectangle b3 = {250, 320, 300, 50};

    // Tugmalarni chizish
    DrawRectangleRec(b1, BLUE); 
    DrawText("BEMOR", 360, 195, 20, WHITE);
    
    DrawRectangleRec(b2, DARKGREEN); 
    DrawText("SHIFOKOR", 350, 265, 20, WHITE);
    
    DrawRectangleRec(b3, ORANGE); 
    DrawText("ADMIN (KLINIKA)", 320, 335, 20, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        bool clicked = false;
        if (CheckCollisionPointRec(mouse, b1)) { *userType = 1; clicked = true; }
        else if (CheckCollisionPointRec(mouse, b2)) { *userType = 2; clicked = true; }
        else if (CheckCollisionPointRec(mouse, b3)) { *userType = 3; clicked = true; }

        if (clicked) {
            // isReg == 0 bo'lsa Kirish (Screen 5)
            // isReg == 1 bo'lsa Ro'yxatdan o'tish (Bemor/Doktor uchun 2, Klinika uchun 3)
            if (isReg == 0) {
                *screen = 5;
            } else {
                *screen = (*userType == 3) ? 3 : 2;
            }
        }
    }
    
    if (IsKeyPressed(KEY_ESCAPE)) *screen = 0;
}
