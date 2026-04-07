#include "klinika.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void KlinikaRegOynasi(int *screen, Vector2 mouse, char *stir, char *lits, char *nomi, int *step) {
    static char tel[20] = "+998";
    static char smsInput[10] = "";
    static char login[30] = "";
    static char pass[30] = "";
    static char confirmPass[30] = "";
    static bool showPass = false;
    static int smsKod = 0;
    static int activeField = 1;
    static int errorType = 0;
    // errorType: 1-SSVda yo'q, 2-Login band, 3-TEL BAND, 4-Parol xato, 5-STIR band, 6-Raqam xato

    DrawText("KLINIKA REGISTRATSIYASI", 220, 30, 28, DARKBLUE);

    // --- 1-BOSQICH: STIR VA LITSENZIYA ---
    if (*step == 1) {
        Rectangle b1 = { 250, 145, 300, 40 };
        Rectangle b2 = { 250, 225, 300, 40 };

        DrawText("INN (STIR):", 250, 120, 18, DARKGRAY);
        DrawRectangleRec(b1, activeField == 1 ? LIGHTGRAY : WHITE);
        DrawRectangleLinesEx(b1, 2, activeField == 1 ? BLUE : GRAY);
        DrawText(stir, b1.x + 10, b1.y + 10, 20, BLACK);

        DrawText("Litsenziya seriya va raqami:", 250, 200, 18, DARKGRAY);
        DrawRectangleRec(b2, activeField == 2 ? LIGHTGRAY : WHITE);
        DrawRectangleLinesEx(b2, 2, activeField == 2 ? BLUE : GRAY);
        DrawText(lits, b2.x + 10, b2.y + 10, 20, BLACK);

        if (errorType == 1) DrawText("XATO: SSV bazasida yo'q!", 250, 280, 16, RED);
        if (errorType == 5) DrawText("XATO: Bu STIR allaqachon band!", 250, 280, 16, RED);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, b1)) activeField = 1;
            else if (CheckCollisionPointRec(mouse, b2)) activeField = 2;
        }

        int key = GetCharPressed();
        while (key > 0) {
            if (activeField == 1 && strlen(stir) < 15) { stir[strlen(stir)] = (char)key; stir[strlen(stir)+1] = '\0'; }
            if (activeField == 2 && strlen(lits) < 15) { lits[strlen(lits)] = (char)key; lits[strlen(lits)+1] = '\0'; }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (activeField == 1 && strlen(stir) > 0) stir[strlen(stir)-1] = '\0';
            if (activeField == 2 && strlen(lits) > 0) lits[strlen(lits)-1] = '\0';
        }

        Rectangle btn = { 250, 320, 300, 50 };
        DrawRectangleRec(btn, BLUE);
        DrawText("TEKSHIRISH", 340, 335, 20, WHITE);

        if (CheckCollisionPointRec(mouse, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (ssv_tasdiqlash(stir, lits, nomi)) {
                if (malumot_bazada_bormi("ID", stir)) errorType = 5;
                else { *step = 2; activeField = 3; errorType = 0; }
            } else errorType = 1;
        }
    }

    // --- 2-BOSQICH: TELEFON RAQAM ---
    else if (*step == 2) {
        DrawText("2. TELEFON RAQAMNI KIRITING", 250, 80, 20, ORANGE);
        DrawText(nomi, 250, 150, 22, DARKBLUE);

        Rectangle b3 = { 250, 230, 300, 40 };
        DrawRectangleRec(b3, WHITE);
        DrawRectangleLinesEx(b3, 2, BLUE);
        DrawText(tel, b3.x + 10, b3.y + 10, 20, BLACK);

        if (errorType == 3) DrawText("XATO: Bu raqam allaqachon band!", 250, 280, 16, RED);
        if (errorType == 6) DrawText("Raqamni oxirigacha kiriting!", 250, 280, 16, ORANGE);

        int key = GetCharPressed();
        while (key > 0) {
            if (key >= '0' && key <= '9' && strlen(tel) < 13) {
                int len = strlen(tel);
                tel[len] = (char)key; tel[len + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(tel) > 4) tel[strlen(tel)-1] = '\0';

        Rectangle btn = { 250, 320, 300, 50 };
        DrawRectangleRec(btn, DARKGREEN);
        DrawText("SMS YUBORISH", 335, 335, 18, WHITE);

        if (CheckCollisionPointRec(mouse, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (malumot_bazada_bormi("TEL", tel)) {
                errorType = 3;
            } else if (strlen(tel) < 13) {
                errorType = 6;
            } else {
                smsKod = 1000 + rand() % 9000;
                printf("\n[SMS] Tasdiqlash kodi: %d\n", smsKod);
                *step = 3;
                errorType = 0;
            }
        }
    }

    // --- 3-BOSQICH: SMS ---
    else if (*step == 3) {
        DrawText("SMS KODNI KIRITING:", 310, 160, 18, DARKGRAY);
        Rectangle b4 = { 325, 200, 150, 50 };
        DrawRectangleRec(b4, LIGHTGRAY);

        int key = GetCharPressed();
        while (key > 0) {
            if (key >= '0' && key <= '9' && strlen(smsInput) < 4) { 
                smsInput[strlen(smsInput)] = (char)key; 
                smsInput[strlen(smsInput)+1] = '\0'; 
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(smsInput) > 0) smsInput[strlen(smsInput)-1] = '\0';
        DrawText(smsInput, b4.x + 50, b4.y + 12, 25, BLACK);

        if (atoi(smsInput) == smsKod && strlen(smsInput) == 4) {
            Rectangle btn = { 250, 320, 300, 50 };
            DrawRectangleRec(btn, BLUE);
            DrawText("DAVOM ETISH", 340, 335, 20, WHITE);
            if (CheckCollisionPointRec(mouse, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                *step = 4;
                activeField = 5;
            }
        }
    }

    // --- 4-BOSQICH: LOGIN/PAROL ---
    else if (*step == 4) {
        Rectangle bl = { 250, 130, 300, 40 };
        Rectangle bp = { 250, 200, 300, 40 };
        Rectangle bc = { 250, 270, 300, 40 };
        Rectangle eye = { 560, 205, 30, 30 };

        DrawText("Login:", 250, 110, 16, DARKGRAY);
        DrawRectangleRec(bl, activeField == 5 ? LIGHTGRAY : WHITE);
        DrawText(login, bl.x + 10, bl.y + 10, 20, BLACK);

        DrawText("Parol:", 250, 180, 16, DARKGRAY);
        DrawRectangleRec(bp, activeField == 6 ? LIGHTGRAY : WHITE);
        
        char hP[31] = ""; 
        for(int i=0; i<(int)strlen(pass); i++) hP[i] = '*';
        DrawText(showPass ? pass : hP, bp.x + 10, bp.y + 10, 20, BLACK);
        DrawRectangleRec(eye, showPass ? BLUE : GRAY);

        DrawText("Tasdiqlash:", 250, 250, 16, DARKGRAY);
        DrawRectangleRec(bc, activeField == 7 ? LIGHTGRAY : WHITE);
        char hC[31] = ""; 
        for(int i=0; i<(int)strlen(confirmPass); i++) hC[i] = '*';
        DrawText(showPass ? confirmPass : hC, bc.x + 10, bc.y + 10, 20, BLACK);

        if (errorType == 2) DrawText("XATO: Login band!", 250, 315, 16, RED);
        if (errorType == 4) DrawText("XATO: Parollar mos kelmadi!", 250, 315, 16, RED);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, bl)) activeField = 5;
            else if (CheckCollisionPointRec(mouse, bp)) activeField = 6;
            else if (CheckCollisionPointRec(mouse, bc)) activeField = 7;
            if (CheckCollisionPointRec(mouse, eye)) showPass = !showPass;
        }

        int key = GetCharPressed();
        while (key > 0) {
            if (activeField == 5 && strlen(login) < 25) { login[strlen(login)] = (char)key; login[strlen(login)+1] = '\0'; }
            if (activeField == 6 && strlen(pass) < 25) { pass[strlen(pass)] = (char)key; pass[strlen(pass)+1] = '\0'; }
            if (activeField == 7 && strlen(confirmPass) < 25) { confirmPass[strlen(confirmPass)] = (char)key; confirmPass[strlen(confirmPass)+1] = '\0'; }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (activeField == 5 && strlen(login) > 0) login[strlen(login)-1] = '\0';
            if (activeField == 6 && strlen(pass) > 0) pass[strlen(pass)-1] = '\0';
            if (activeField == 7 && strlen(confirmPass) > 0) confirmPass[strlen(confirmPass)-1] = '\0';
        }

        Rectangle btn = { 250, 340, 300, 50 };
        DrawRectangleRec(btn, DARKGREEN);
        DrawText("YAKUNLASH", 345, 355, 20, WHITE);

        if (CheckCollisionPointRec(mouse, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (strcmp(pass, confirmPass) != 0) errorType = 4;
            else if (malumot_bazada_bormi("LOGIN", login)) errorType = 2;
            else {
                // TARTIB: Login, Pass, Rol(3), ID(stir), Tel(tel)
                saqlash_bazaga(login, pass, 3, stir, tel);
                *screen = 0;
                // Ma'lumotlarni tozalash
                strcpy(stir, ""); strcpy(lits, ""); strcpy(tel, "+998");
                strcpy(login, ""); strcpy(pass, ""); strcpy(confirmPass, "");
                *step = 1; // Registratsiyani boshiga qaytarish
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) *screen = 0;
}
