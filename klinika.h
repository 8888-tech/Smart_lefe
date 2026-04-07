#ifndef KLINIKA_H
#define KLINIKA_H

#include "raylib.h"

// UI Funksiyalari
void IkkinchiOyna(int *screen, Vector2 mouse, int *userType, int isReg);
void BemorDoktorOynasi(int *screen, Vector2 mouse, int userType, char *id, int *letterCount);
void KlinikaRegOynasi(int *screen, Vector2 mouse, char *stir, char *lits, char *nomi, int *step);
void KirishOynasi(int *screen, Vector2 mouse, int *userType);
void ParolTiklashOynasi(int *screen, Vector2 mouse);

// Baza mantiqi (TARTIB MUHIM!)
int user_tekshir(const char* login, const char* pass, int rol);
void saqlash_bazaga(const char* login, const char* pass, int rol, const char* id, const char* tel);
int malumot_bazada_bormi(const char* id, const char* login);
int ssv_tasdiqlash(const char* stir, const char* lits, char* nomi);
int parol_yangila(const char* id, const char* yangi_parol);
// Boshqa funksiyalar qatoriga qo'shing:
int malumotni_top_id_bilan(const char* id, char* tel, char* login);
#endif
