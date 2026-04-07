#include "klinika.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 1. TIZIMGA KIRISHNI TEKSHIRISH (Login oynasi uchun)
int user_tekshir(const char* login, const char* pass, int rol) {
    FILE *f = fopen("foydalanuvchilar.txt", "r");
    if (!f) return 0;

    char f_l[50], f_p[50], f_i[50], f_t[50];
    int f_r;
    int ruxsat = 0;

    // Fayldagi tartib: login parol rol id tel
    while (fscanf(f, "%s %s %d %s %s", f_l, f_p, &f_r, f_i, f_t) != EOF) {
        if (strcmp(f_l, login) == 0 && strcmp(f_p, pass) == 0 && f_r == rol) {
            ruxsat = 1;
            break;
        }
    }
    fclose(f);
    return ruxsat;
}

// 2. YANGI FOYDALANUVCHINI SAQLASH (Registratsiya uchun)
void saqlash_bazaga(const char* login, const char* pass, int rol, const char* id, const char* tel) {
    FILE *f = fopen("foydalanuvchilar.txt", "a");
    if (f) {
        fprintf(f, "%s %s %d %s %s\n", login, pass, rol, id, tel);
        fclose(f);
    }
}

// 3. ID ORQALI MA'LUMOTLARNI TOPISH (Parol tiklashning 1-bosqichi uchun)
// Bu funksiya ID kiritilganda unga tegishli TEL va LOGINni qaytaradi
int malumotni_top_id_bilan(const char* id, char* tel, char* login) {
    FILE *f = fopen("foydalanuvchilar.txt", "r");
    if (!f) return 0;

    char f_l[50], f_p[50], f_i[50], f_t[50];
    int f_r;
    int topildi = 0;

    while (fscanf(f, "%s %s %d %s %s", f_l, f_p, &f_r, f_i, f_t) != EOF) {
        if (strcmp(f_i, id) == 0) {
            if (tel != NULL) strcpy(tel, f_t);     // Telefonni recovery'ga beradi
            if (login != NULL) strcpy(login, f_l); // Loginni recovery'ga beradi
            topildi = 1;
            break;
        }
    }
    fclose(f);
    return topildi;
}

// 4. UNIVERSAL TEKSHIRUV (ID, Login yoki Tel bandligini tekshirish uchun)
int malumot_bazada_bormi(const char* key, const char* value) {
    FILE *f = fopen("foydalanuvchilar.txt", "r");
    if (!f) return 0;

    char f_l[50], f_p[50], f_i[50], f_t[50];
    int f_r;
    int bor = 0;

    while (fscanf(f, "%s %s %d %s %s", f_l, f_p, &f_r, f_i, f_t) != EOF) {
        if (strcmp(key, "ID") == 0 && strcmp(f_i, value) == 0) bor = 1;
        else if (strcmp(key, "LOGIN") == 0 && strcmp(f_l, value) == 0) bor = 1;
        else if (strcmp(key, "TEL") == 0 && strcmp(f_t, value) == 0) bor = 1;

        if (bor) break;
    }
    fclose(f);
    return bor;
}

// 5. SSV TASDIQLASH (Klinika uchun simulatsiya)
int ssv_tasdiqlash(const char* stir, const char* lits, char* nomi) {
    if (strlen(stir) >= 4 && strlen(lits) >= 3) {
        strcpy(nomi, "CITY MEDICAL CENTER");
        return 1;
    }
    return 0;
}

// 6. PAROLNI YANGILASH (Parol tiklashning yakuniy bosqichi uchun)
int parol_yangila(const char* id, const char* yangi_parol) {
    FILE *f = fopen("foydalanuvchilar.txt", "r");
    if (!f) return 0;

    FILE *t = fopen("temp.txt", "w");
    if (!t) { fclose(f); return 0; }

    char f_l[50], f_p[50], f_i[50], f_t[50];
    int f_r;
    int muvaffaqiyat = 0;

    while (fscanf(f, "%s %s %d %s %s", f_l, f_p, &f_r, f_i, f_t) != EOF) {
        if (strcmp(f_i, id) == 0) {
            // Faqat parolni yangilab, qolgan ma'lumotlarni o'z holicha saqlaymiz
            fprintf(t, "%s %s %d %s %s\n", f_l, yangi_parol, f_r, f_i, f_t);
            muvaffaqiyat = 1;
        } else {
            fprintf(t, "%s %s %d %s %s\n", f_l, f_p, f_r, f_i, f_t);
        }
    }
    fclose(f);
    fclose(t);

    remove("foydalanuvchilar.txt");
    rename("temp.txt", "foydalanuvchilar.txt");

    return muvaffaqiyat;
}
