#include "klinika.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int regStep = 1;
static char smsKod[10] = "";
static char smsInput[10] = "";
static char telInput[20] = "+998";
static char loginInput[30] = "";
static char passInput[30] = "";
static char confirmPass[30] = "";
static bool showPass = false;
static int activeField = 0;
static char xato_xabar[50] = "";

void BemorDoktorOynasi(int *screen, Vector2 mouse, int userType, char *idInput, int *letterCount) {
    ClearBackground(RAYWHITE);
    DrawText(userType == 1 ? "BEMOR RO'YXATDAN O'TISH" : "DOKTOR RO'YXATDAN O'TISH", 200, 40, 25, DARKBLUE);

    // --- 1-BOSQICH: ID VA TEL ---
    if (regStep == 1) {
        Rectangle r1 = { 250, 155, 300, 40 };
        Rectangle r2 = { 250, 235, 300, 40 };

        DrawText("ID raqam (Passport):", 250, 130, 18, DARKGRAY);
        DrawRectangleRec(r1, activeField == 0 ? LIGHTGRAY : WHITE);
        DrawRectangleLinesEx(r1, 2, activeField == 0 ? BLUE : GRAY);
        DrawText(idInput, r1.x + 10, r1.y + 10, 20, BLACK);

        DrawText("Telefon raqam:", 250, 210, 18, DARKGRAY);
        DrawRectangleRec(r2, activeField == 1 ? LIGHTGRAY : WHITE);
        DrawRectangleLinesEx(r2, 2, activeField == 1 ? BLUE : GRAY);
        DrawText(telInput, r2.x + 10, r2.y + 10, 20, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, r1)) activeField = 0;
            else if (CheckCollisionPointRec(mouse, r2)) activeField = 1;
        }

        int key = GetCharPressed();
        while (key > 0) {
            if (activeField == 0 && strlen(idInput) < 15) {
                int len = strlen(idInput); idInput[len] = (char)key; idInput[len+1] = '\0';
            }
            if (activeField == 1 && key >= '0' && key <= '9' && strlen(telInput) < 13) {
                int len = strlen(telInput); telInput[len] = (char)key; telInput[len+1] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (activeField == 0 && strlen(idInput) > 0) idInput[strlen(idInput)-1] = '\0';
            if (activeField == 1 && strlen(telInput) > 4) telInput[strlen(telInput)-1] = '\0';
        }

        Rectangle btnNext = { 250, 320, 300, 50 };
        DrawRectangleRec(btnNext, BLUE);
        DrawText("SMS KOD YUBORISH", 315, 335, 20, WHITE);

        if (CheckCollisionPointRec(mouse, btnNext) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (malumot_bazada_bormi("ID", idInput)) {
                strcpy(xato_xabar, "Bu ID allaqachon mavjud!");
            } else if (malumot_bazada_bormi("TEL", telInput)) {
                strcpy(xato_xabar, "Bu Tel allaqachon mavjud!");
            } else if (strlen(idInput) > 4 && strlen(telInput) == 13) {
                srand(time(NULL));
                sprintf(smsKod, "%04d", rand() % 10000);
                printf("\n\033[1;32m[SMS SERVIS] TEL: %s | KOD: %s\033[0m\n", telInput, smsKod);
                regStep = 2;
                strcpy(xato_xabar, "");
                strcpy(smsInput, "");
            } else {
                strcpy(xato_xabar, "Ma'lumotlar to'liq emas!");
            }
        }
        DrawText(xato_xabar, 250, 285, 18, RED);
    }

    // --- 2-BOSQICH: SMS ---
    else if (regStep == 2) {
        Rectangle rSms = { 250, 200, 300, 50 };
        DrawText("Terminaldagi kodni kiriting:", 250, 160, 18, DARKGRAY);
        DrawRectangleRec(rSms, WHITE);
        DrawRectangleLinesEx(rSms, 2, ORANGE);
        DrawText(smsInput, rSms.x + 115, rSms.y + 12, 25, BLACK);

        int key = GetCharPressed();
        while (key > 0) {
            if (key >= '0' && key <= '9' && strlen(smsInput) < 4) {
                int len = strlen(smsInput); smsInput[len] = (char)key; smsInput[len+1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(smsInput) > 0) smsInput[strlen(smsInput)-1] = '\0';

        Rectangle btnVerify = { 250, 280, 300, 50 };
        DrawRectangleRec(btnVerify, ORANGE);
        DrawText("TASDIQLASH", 345, 295, 20, WHITE);

        if (CheckCollisionPointRec(mouse, btnVerify) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (strcmp(smsInput, smsKod) == 0) {
                regStep = 3; activeField = 2; strcpy(xato_xabar, "");
            } else {
                strcpy(xato_xabar, "Kod noto'g'ri!");
            }
        }
        DrawText(xato_xabar, 250, 255, 18, RED);
    }

    // --- 3-BOSQICH: LOGIN VA PAROL ---
    else if (regStep == 3) {
        Rectangle r3 = { 250, 140, 300, 40 };
        Rectangle r4 = { 250, 210, 300, 40 };
        Rectangle r5 = { 250, 280, 300, 40 };
        Rectangle eye = { 560, 215, 30, 30 };

        DrawText("Login yarating:", 250, 120, 16, DARKGRAY);
        DrawRectangleRec(r3, activeField == 2 ? LIGHTGRAY : WHITE);
        DrawRectangleLinesEx(r3, 1, BLUE);
        DrawText(loginInput, r3.x + 10, r3.y + 10, 20, BLACK);

        DrawText("Parol:", 250, 190, 16, DARKGRAY);
        DrawRectangleRec(r4, activeField == 3 ? LIGHTGRAY : WHITE);
        DrawRectangleLinesEx(r4, 1, BLUE);
        
        char hP[31] = ""; 
        for(int i=0; i < (int)strlen(passInput); i++) hP[i] = '*';
        DrawText(showPass ? passInput : hP, r4.x + 10, r4.y + 10, 20, BLACK);
        DrawRectangleRec(eye, showPass ? BLUE : GRAY); 
        DrawText("O", eye.x+8, eye.y+5, 20, WHITE);

        DrawText("Tasdiqlash:", 250, 260, 16, DARKGRAY);
        DrawRectangleRec(r5, activeField == 4 ? LIGHTGRAY : WHITE);
        DrawRectangleLinesEx(r5, 1, BLUE);
        
        char hC[31] = ""; 
        for(int i=0; i < (int)strlen(confirmPass); i++) hC[i] = '*';
        DrawText(showPass ? confirmPass : hC, r5.x + 10, r5.y + 10, 20, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, r3)) activeField = 2;
            else if (CheckCollisionPointRec(mouse, r4)) activeField = 3;
            else if (CheckCollisionPointRec(mouse, r5)) activeField = 4;
            if (CheckCollisionPointRec(mouse, eye)) showPass = !showPass;
        }

        int key = GetCharPressed();
        while (key > 0) {
            if (activeField == 2 && strlen(loginInput) < 20) { loginInput[strlen(loginInput)] = (char)key; loginInput[strlen(loginInput)+1] = '\0'; }
            if (activeField == 3 && strlen(passInput) < 20) { passInput[strlen(passInput)] = (char)key; passInput[strlen(passInput)+1] = '\0'; }
            if (activeField == 4 && strlen(confirmPass) < 20) { confirmPass[strlen(confirmPass)] = (char)key; confirmPass[strlen(confirmPass)+1] = '\0'; }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (activeField == 2 && strlen(loginInput) > 0) loginInput[strlen(loginInput)-1] = '\0';
            if (activeField == 3 && strlen(passInput) > 0) passInput[strlen(passInput)-1] = '\0';
            if (activeField == 4 && strlen(confirmPass) > 0) confirmPass[strlen(confirmPass)-1] = '\0';
        }

        Rectangle btnFinal = { 250, 350, 300, 50 };
        DrawRectangleRec(btnFinal, DARKGREEN);
        DrawText("YAKUNLASH", 345, 365, 18, WHITE);

        if (CheckCollisionPointRec(mouse, btnFinal) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (strcmp(passInput, confirmPass) != 0) {
                strcpy(xato_xabar, "Parollar mos emas!");
            } else if (malumot_bazada_bormi("LOGIN", loginInput)) {
                strcpy(xato_xabar, "Bu login band!");
            } else if (strlen(loginInput) > 2) {
                // TARTIB: Login, Pass, Rol (userType), ID (idInput), Tel (telInput)
                saqlash_bazaga(loginInput, passInput, userType, idInput, telInput);
                *screen = 0; regStep = 1;
                strcpy(idInput, ""); strcpy(telInput, "+998"); strcpy(loginInput, ""); strcpy(passInput, ""); strcpy(confirmPass, "");
            } else {
                strcpy(xato_xabar, "Login juda qisqa!");
            }
        }
        DrawText(xato_xabar, 250, 325, 16, RED);
    }
}
