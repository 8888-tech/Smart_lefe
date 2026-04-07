#include "klinika.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ParolTiklashOynasi(int *screen, Vector2 mouse) {
    // Statik o'zgaruvchilar ma'lumotlarni bosqichlararo saqlab turadi
    static char r_id[30] = "";
    static char r_tel[20] = "";
    static char r_login[30] = "";
    static char r_sms[10] = "";
    static char r_pass[30] = "";
    
    static int step = 1;      // 1: ID, 2: SMS, 3: Yangi Parol
    static int smsKod = 0;    // Tizim yaratgan tasodifiy kod
    static char xabar[64] = "";
    static int activeField = 1;

    DrawText("PAROLNI TIKLASH XIZMATI", 220, 50, 28, DARKBLUE);

    // --- 1-BOSQICH: ID/STIR KIRITISH ---
    if (step == 1) {
        DrawText("ID yoki STIR raqamingizni kiriting:", 250, 140, 18, DARKGRAY);
        Rectangle r1 = { 250, 170, 300, 45 };
        DrawRectangleRec(r1, WHITE);
        DrawRectangleLinesEx(r1, 2, BLUE);
        DrawText(r_id, r1.x + 10, r1.y + 12, 20, BLACK);

        Rectangle btn1 = { 250, 240, 300, 50 };
        DrawRectangleRec(btn1, DARKBLUE);
        DrawText("NOMERNI TOPISH", 320, 255, 18, WHITE);

        if (CheckCollisionPointRec(mouse, btn1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // auth.c dagi funksiya orqali bazadan qidiramiz
            if (malumotni_top_id_bilan(r_id, r_tel, r_login)) {
                smsKod = 1000 + rand() % 9000; // 4 xonali kod yaratish
                printf("\n[SMS] %s raqamiga yuborildi: %d\n", r_tel, smsKod);
                step = 2;
                strcpy(xabar, "");
            } else {
                strcpy(xabar, "Bunday ID bazada mavjud emas!");
            }
        }
    }
    // --- 2-BOSQICH: SMS TASDIQLASH ---
    else if (step == 2) {
        DrawText("Tizimdagi telefon raqamingiz:", 250, 120, 16, DARKGRAY);
        DrawText(r_tel, 250, 140, 22, DARKGREEN); // Bazadan topilgan nomer
        
        DrawText("SMS kodni kiriting:", 250, 190, 18, DARKGRAY);
        Rectangle r2 = { 325, 220, 150, 50 };
        DrawRectangleRec(r2, LIGHTGRAY);
        DrawText(r_sms, r2.x + 45, r2.y + 12, 25, BLACK);

        DrawText("Kodni kiritgach, tizim loginni ko'rsatadi", 240, 290, 16, GRAY);

        // Kod to'g'ri bo'lsa keyingi bosqichga o'tamiz
        if (strlen(r_sms) == 4) {
            if (atoi(r_sms) == smsKod) {
                step = 3;
                strcpy(r_sms, "");
            } else {
                DrawText("Kod xato!", 365, 275, 16, RED);
            }
        }
    }
    // --- 3-BOSQICH: LOGINNI KO'RSATISH VA PAROL YANGILASH ---
    else if (step == 3) {
        DrawText("Sizning loginigiz:", 250, 110, 16, DARKGRAY);
        DrawText(r_login, 250, 130, 24, BLUE); // Loginni foydalanuvchiga eslatamiz

        DrawText("Yangi parol o'rnating:", 250, 180, 18, DARKGRAY);
        Rectangle r3 = { 250, 210, 300, 45 };
        DrawRectangleRec(r3, WHITE);
        DrawRectangleLinesEx(r3, 2, ORANGE);
        DrawText(r_pass, r3.x + 10, r3.y + 12, 20, BLACK);

        Rectangle btnOk = { 250, 280, 300, 50 };
        DrawRectangleRec(btnOk, GREEN);
        DrawText("SAQLASH VA KIRISH", 310, 295, 18, WHITE);

        if (CheckCollisionPointRec(mouse, btnOk) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (strlen(r_pass) > 3) {
                if (parol_yangila(r_id, r_pass)) {
                    *screen = 5; // Login oynasiga qaytish
                    step = 1; // Ma'lumotlarni tozalash
                    strcpy(r_id, ""); strcpy(r_pass, "");
                }
            } else strcpy(xabar, "Parol juda qisqa!");
        }
    }

    // --- KLAVIATURA KIRITISH MANTIQI ---
    int key = GetCharPressed();
    while (key > 0) {
        if (step == 1 && strlen(r_id) < 25) { 
            int len = strlen(r_id); r_id[len] = (char)key; r_id[len+1] = '\0'; 
        }
        else if (step == 2 && strlen(r_sms) < 4) { 
            int len = strlen(r_sms); r_sms[len] = (char)key; r_sms[len+1] = '\0'; 
        }
        else if (step == 3 && strlen(r_pass) < 25) { 
            int len = strlen(r_pass); r_pass[len] = (char)key; r_pass[len+1] = '\0'; 
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (step == 1 && strlen(r_id) > 0) r_id[strlen(r_id)-1] = '\0';
        if (step == 2 && strlen(r_sms) > 0) r_sms[strlen(r_sms)-1] = '\0';
        if (step == 3 && strlen(r_pass) > 0) r_pass[strlen(r_pass)-1] = '\0';
    }

    DrawText(xabar, 250, 360, 18, RED);
    if (IsKeyPressed(KEY_ESCAPE)) { *screen = 5; step = 1; }
}
