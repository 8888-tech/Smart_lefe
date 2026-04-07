#include "klinika.h"
#include <string.h>

void ParolTiklashOynasi(int *screen, Vector2 mouse) {
    static char stir[20] = "";
    static char smsKod[10] = "";
    static char yangiParol[30] = "";
    static char tasdiqParol[30] = "";
    static char topilganLogin[30] = "umidjon_01"; // Test uchun
    
    static int regStep = 1;      // 1: STIR, 2: SMS, 3: Parol o'rnatish
    static int activeField = 1;  
    static char xabar[64] = "";

    DrawText("PAROLNI TIKLASH", 280, 40, 25, DARKBLUE);

    if (regStep == 1) { // 1-bosqich: STIR kiritish
        DrawText("STIR yoki Pasport ID kiriting:", 250, 120, 16, DARKGRAY);
        Rectangle rStir = { 250, 150, 300, 45 };
        DrawRectangleRec(rStir, WHITE);
        DrawRectangleLinesEx(rStir, 2, activeField == 1 ? BLUE : GRAY);
        DrawText(stir, rStir.x + 10, rStir.y + 12, 20, BLACK);

        Rectangle btnSms = { 250, 220, 300, 50 };
        if (CheckCollisionPointRec(mouse, btnSms) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (strlen(stir) > 4) {
                regStep = 2; activeField = 2;
                strcpy(xabar, "SMS yuborildi: 1234");
            } else strcpy(xabar, "STIRni to'liq kiriting!");
        }
        DrawRectangleRec(btnSms, BLUE);
        DrawText("SMS YUBORISH", 335, 235, 20, WHITE);
    } 
    else if (regStep == 2) { // 2-bosqich: SMS kod
        DrawText("Kod (1234):", 250, 120, 16, DARKGRAY);
        Rectangle rSms = { 250, 150, 300, 45 };
        DrawRectangleRec(rSms, WHITE);
        DrawRectangleLinesEx(rSms, 2, activeField == 2 ? BLUE : GRAY);
        DrawText(smsKod, rSms.x + 10, rSms.y + 12, 20, BLACK);

        Rectangle btnVerify = { 250, 220, 300, 50 };
        if (CheckCollisionPointRec(mouse, btnVerify) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (strcmp(smsKod, "1234") == 0) {
                regStep = 3; activeField = 3;
                strcpy(xabar, "Kod tasdiqlandi!");
            } else strcpy(xabar, "SMS kod xato!");
        }
        DrawRectangleRec(btnVerify, GREEN);
        DrawText("TASDIQLASH", 345, 235, 20, WHITE);
    } 
    else if (regStep == 3) { // 3-bosqich: Parol yangilash
        DrawText("Sizning loginingiz:", 250, 90, 15, DARKGRAY);
        DrawText(topilganLogin, 250, 110, 22, RED);

        Rectangle rP1 = { 250, 170, 300, 40 };
        Rectangle rP2 = { 250, 240, 300, 40 };
        DrawRectangleRec(rP1, WHITE);
        DrawRectangleLinesEx(rP1, 2, activeField == 3 ? BLUE : GRAY);
        DrawText(yangiParol, rP1.x + 10, rP1.y + 10, 20, BLACK);
        DrawText("Yangi parol:", 250, 150, 14, DARKGRAY);

        DrawRectangleRec(rP2, WHITE);
        DrawRectangleLinesEx(rP2, 2, activeField == 4 ? BLUE : GRAY);
        DrawText(tasdiqParol, rP2.x + 10, rP2.y + 10, 20, BLACK);
        DrawText("Tasdiqlash:", 250, 220, 14, DARKGRAY);

        Rectangle btnSave = { 250, 310, 300, 50 };
        if (CheckCollisionPointRec(mouse, btnSave) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (strcmp(yangiParol, tasdiqParol) == 0 && strlen(yangiParol) >= 4) {
                if (parol_yangila(stir, yangiParol)) strcpy(xabar, "Muvaffaqiyatli! ESC bosing.");
                else strcpy(xabar, "Xatolik yuz berdi!");
            } else strcpy(xabar, "Parollar mos emas!");
        }
        DrawRectangleRec(btnSave, ORANGE);
        DrawText("SAQLASH", 355, 325, 20, WHITE);
    }

    DrawText(xabar, 250, 380, 16, MAROON);

    // Klaviatura va ESC mantiqi
    if (IsKeyPressed(KEY_ESCAPE)) { regStep = 1; *screen = 5; }
    int key = GetCharPressed();
    while (key > 0) {
        if (regStep == 1 && strlen(stir) < 15) { int l=strlen(stir); stir[l]=(char)key; stir[l+1]='\0'; }
        else if (regStep == 2 && strlen(smsKod) < 4) { int l=strlen(smsKod); smsKod[l]=(char)key; smsKod[l+1]='\0'; }
        else if (activeField == 3 && strlen(yangiParol) < 20) { int l=strlen(yangiParol); yangiParol[l]=(char)key; yangiParol[l+1]='\0'; }
        else if (activeField == 4 && strlen(tasdiqParol) < 20) { int l=strlen(tasdiqParol); tasdiqParol[l]=(char)key; tasdiqParol[l+1]='\0'; }
        key = GetCharPressed();
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (regStep == 3) {
            if (CheckCollisionPointRec(mouse, (Rectangle){250,170,300,40})) activeField = 3;
            if (CheckCollisionPointRec(mouse, (Rectangle){250,240,300,40})) activeField = 4;
        }
    }
}
