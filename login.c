#include "klinika.h"
#include <string.h>

void KirishOynasi(int *screen, Vector2 mouse, int *userType) {
    static char l_login[30] = "";
    static char l_pass[30] = "";
    static int activeField = 1;
    static char xabar[64] = "";

    // 1. Sarlavhalar
    DrawText("TIZIMGA KIRISH", 280, 50, 30, DARKBLUE);

    char rolNom[20];
    if (*userType == 1) strcpy(rolNom, "(BEMOR)");
    else if (*userType == 2) strcpy(rolNom, "(SHIFOKOR)");
    else if (*userType == 3) strcpy(rolNom, "(KLINIKA)");
    else strcpy(rolNom, "");
    
    DrawText(rolNom, 350, 90, 18, GRAY);

    // 2. Maydonlar koordinatalari
    Rectangle rLogin = { 250, 150, 300, 45 };
    Rectangle rPass  = { 250, 230, 300, 45 };
    Rectangle btnKirish = { 250, 310, 300, 50 };
    Rectangle btnTiklash = { 250, 380, 300, 40 };

    // 3. Login maydoni
    DrawText("Login:", 250, 130, 16, DARKGRAY);
    DrawRectangleRec(rLogin, activeField == 1 ? LIGHTGRAY : WHITE);
    DrawRectangleLinesEx(rLogin, 2, activeField == 1 ? BLUE : GRAY);
    DrawText(l_login, rLogin.x + 10, rLogin.y + 12, 20, BLACK);

    // 4. Parol maydoni
    DrawText("Parol:", 250, 210, 16, DARKGRAY);
    DrawRectangleRec(rPass, activeField == 2 ? LIGHTGRAY : WHITE);
    DrawRectangleLinesEx(rPass, 2, activeField == 2 ? BLUE : GRAY);
    
    char hiddenPass[32] = "";
    int pLen = (int)strlen(l_pass);
    for(int i=0; i < pLen; i++) hiddenPass[i] = '*';
    hiddenPass[pLen] = '\0';
    DrawText(hiddenPass, rPass.x + 10, rPass.y + 12, 20, BLACK);

    // 5. KIRISH tugmasi
    bool hoverKirish = CheckCollisionPointRec(mouse, btnKirish);
    DrawRectangleRec(btnKirish, hoverKirish ? SKYBLUE : BLUE);
    DrawText("KIRISH", btnKirish.x + 110, btnKirish.y + 15, 22, WHITE);

    // 6. PAROLNI TIKLASH
    bool hoverTiklash = CheckCollisionPointRec(mouse, btnTiklash);
    DrawRectangleRec(btnTiklash, hoverTiklash ? LIGHTGRAY : WHITE);
    DrawRectangleLinesEx(btnTiklash, 1, hoverTiklash ? RED : DARKGRAY);
    DrawText("Parolni unutdingizmi?", btnTiklash.x + 55, btnTiklash.y + 12, 16, hoverTiklash ? RED : DARKBLUE);

    // 7. Xabarlar
    DrawText(xabar, 250, 440, 18, RED);

    // --- MANTIQ ---
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, rLogin)) activeField = 1;
        else if (CheckCollisionPointRec(mouse, rPass)) activeField = 2;

        if (hoverTiklash) {
            *screen = 15; // Parol tiklash oynasi
            strcpy(xabar, "");
        }

        if (hoverKirish) {
            // TIZIMGA KIRISHNI TEKSHIRISH
            if (user_tekshir(l_login, l_pass, *userType)) {
                *screen = 10; // Asosiy ishchi oyna (Dashboard)
                // Kirgandan keyin maydonlarni tozalash
                strcpy(l_login, "");
                strcpy(l_pass, "");
                strcpy(xabar, "");
            } else {
                strcpy(xabar, "Login yoki parol xato!");
            }
        }
    }

    // Klaviatura kiritishi
    int key = GetCharPressed();
    while (key > 0) {
        if (activeField == 1 && strlen(l_login) < 25) {
            int len = strlen(l_login);
            l_login[len] = (char)key;
            l_login[len+1] = '\0';
        }
        else if (activeField == 2 && strlen(l_pass) < 25) {
            int len = strlen(l_pass);
            l_pass[len] = (char)key;
            l_pass[len+1] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (activeField == 1 && strlen(l_login) > 0) l_login[strlen(l_login)-1] = '\0';
        else if (activeField == 2 && strlen(l_pass) > 0) l_pass[strlen(l_pass)-1] = '\0';
    }

    // ENTER bosilganda ham kirishga harakat qilish
    if (IsKeyPressed(KEY_ENTER)) {
        if (user_tekshir(l_login, l_pass, *userType)) {
            *screen = 10;
            strcpy(l_login, "");
            strcpy(l_pass, "");
            strcpy(xabar, "");
        } else {
            strcpy(xabar, "Login yoki parol xato!");
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        *screen = 0;
        strcpy(xabar, "");
    }
}
