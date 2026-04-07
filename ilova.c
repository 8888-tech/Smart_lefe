#include "klinika.h"
#include <string.h> // BU QATORNI QO'SHING

int main(void) {
    // Oyna o'lchami va nomi
    InitWindow(800, 600, "Smart Klinika Tizimi");
    SetTargetFPS(60);

    // Dastur holati o'zgaruvchilari
    int currentScreen = 0;
    int userType = 1;            // 1: Bemor, 2: Shifokor, 3: Klinika/Admin
    int isRegistration = 1;      // 0: Kirish, 1: Ro'yxatdan o'tish (int qildik)

    // Kiritish maydonlari uchun o'zgaruvchilar
    char idInput[32] = "";
    int letterCount = 0;
    char stirInput[20] = "", litsInput[20] = "", nomiInput[100] = "";
    int regStep = 1;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // --- ASOSIY MANTIQ (SCREENS) ---
        switch (currentScreen) {
            case 0: // ASOSIY MENYU
                DrawText("SMART KLINIKA TIZIMI", 210, 80, 35, DARKBLUE);
                Rectangle btnKirish = {250, 200, 300, 60};
                Rectangle btnReg = {250, 280, 300, 60};

                // Kirish tugmasi
                DrawRectangleRec(btnKirish, SKYBLUE);
                DrawText("KIRISH", 360, 215, 25, WHITE);
                if (CheckCollisionPointRec(mouse, btnKirish) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    isRegistration = 0; // Kirish rejimi
                    currentScreen = 1; 
                }

                // Ro'yxatdan o'tish tugmasi
                DrawRectangleRec(btnReg, DARKBLUE);
                DrawText("RO'YXATDAN O'TISH", 295, 295, 22, WHITE);
                if (CheckCollisionPointRec(mouse, btnReg) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    isRegistration = 1; // Registratsiya rejimi
                    currentScreen = 1;
                }
                break;

            case 1: // ROL TANLASH OYNASI
                IkkinchiOyna(&currentScreen, mouse, &userType, isRegistration);
                break;

            case 2: // BEMOR YOKI SHIFOKOR RO'YXATDAN O'TISH
                BemorDoktorOynasi(&currentScreen, mouse, userType, idInput, &letterCount);
                break;

            case 3: // KLINIKA RO'YXATDAN O'TISH
                KlinikaRegOynasi(&currentScreen, mouse, stirInput, litsInput, nomiInput, &regStep);
                break;

            case 5: // LOGIN (KIRISH) OYNASI
                KirishOynasi(&currentScreen, mouse, &userType);
                break;

            case 10: // SHAXSIY KABINET
                DrawText("XUSH KELIBSIZ!", 280, 150, 35, DARKGREEN);
                DrawText("Tizimga muvaffaqiyatli kirdingiz.", 260, 220, 18, GRAY);

                Rectangle btnChiqish = {300, 350, 200, 50};
                DrawRectangleRec(btnChiqish, RED);
                DrawText("CHIQISH", 360, 365, 20, WHITE);

                if ((CheckCollisionPointRec(mouse, btnChiqish) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_ESCAPE)) {
                    currentScreen = 0;
                    // Ma'lumotlarni tozalash (keyingi kirish uchun)
                    strcpy(idInput, "");
                    regStep = 1;
                }
                break;

            case 15: // PAROLNI TIKLASH OYNASI
                // Agar bu funksiya auth.c yoki boshqa yerda bo'lsa
                ParolTiklashOynasi(&currentScreen, mouse);
                break;

            default:
                currentScreen = 0;
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
