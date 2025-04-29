#include <stdio.h>
#include <stdlib.h>

#define MAKS_KULLANICI 1000

// --- Kırmızı-Siyah Ağaç Yapısı ---
typedef struct KSAdugum {
    int kullanici_id;
    char renk; // 'R' = Kırmızı, 'B' = Siyah
    struct KSAdugum *sol, *sag, *ebeveyn;
} KSAdugum;

KSAdugum *ks_kok = NULL;

KSAdugum* ks_dugum_olustur(int id) {
    KSAdugum* dugum = (KSAdugum*)malloc(sizeof(KSAdugum));
    dugum->kullanici_id = id;
    dugum->renk = 'R';
    dugum->sol = dugum->sag = dugum->ebeveyn = NULL;
    return dugum;
}

void ks_basit_ekle(int id) {
    KSAdugum* dugum = ks_dugum_olustur(id);
    if (ks_kok == NULL) {
        dugum->renk = 'B';
        ks_kok = dugum;
        return;
    }

    KSAdugum* mevcut = ks_kok;
    KSAdugum* ebeveyn = NULL;

    while (mevcut != NULL) {
        ebeveyn = mevcut;
        if (id < mevcut->kullanici_id)
            mevcut = mevcut->sol;
        else
            mevcut = mevcut->sag;
    }

    dugum->ebeveyn = ebeveyn;
    if (id < ebeveyn->kullanici_id)
        ebeveyn->sol = dugum;
    else
        ebeveyn->sag = dugum;
}

int ks_arama(int id) {
    KSAdugum* mevcut = ks_kok;
    while (mevcut) {
        if (id == mevcut->kullanici_id) return 1;
        mevcut = (id < mevcut->kullanici_id) ? mevcut->sol : mevcut->sag;
    }
    return 0;
}

// --- Kullanıcı Grafiği ---
typedef struct ArkadasDugum {
    int id;
    struct ArkadasDugum* sonraki;
} ArkadasDugum;

typedef struct {
    int kullanici_id;
    ArkadasDugum* arkadaslar;
} Kullanici;

Kullanici kullanicilar[MAKS_KULLANICI];
int kullanici_sayisi = 0;

int indeks_bul(int id) {
    for (int i = 0; i < kullanici_sayisi; i++)
        if (kullanicilar[i].kullanici_id == id) return i;
    return -1;
}

void kullanici_ekle(int id) {
    if (indeks_bul(id) != -1) return;
    kullanicilar[kullanici_sayisi].kullanici_id = id;
    kullanicilar[kullanici_sayisi].arkadaslar = NULL;
    kullanici_sayisi++;
    ks_basit_ekle(id);
}

void arkadaslik_ekle(int k1, int k2) {
    int i = indeks_bul(k1);
    int j = indeks_bul(k2);
    if (i == -1 || j == -1) return;

    ArkadasDugum* a1 = malloc(sizeof(ArkadasDugum));
    a1->id = k2; a1->sonraki = kullanicilar[i].arkadaslar; kullanicilar[i].arkadaslar = a1;

    ArkadasDugum* a2 = malloc(sizeof(ArkadasDugum));
    a2->id = k1; a2->sonraki = kullanicilar[j].arkadaslar; kullanicilar[j].arkadaslar = a2;
}

void kullanicilari_yazdir() {
    for (int i = 0; i < kullanici_sayisi; i++) {
        printf("Kullanici %d: ", kullanicilar[i].kullanici_id);
        for (ArkadasDugum* a = kullanicilar[i].arkadaslar; a; a = a->sonraki)
            printf("%d ", a->id);
        printf("\n");
    }
}

void derinlik_arama(int indeks, int hedef_derinlik, int mevcut_derinlik, int* ziyaret_edildi) {
    ziyaret_edildi[indeks] = 1;
    if (mevcut_derinlik == hedef_derinlik) {
        printf("%d ", kullanicilar[indeks].kullanici_id);
        return;
    }

    for (ArkadasDugum* a = kullanicilar[indeks].arkadaslar; a; a = a->sonraki) {
        int idx = indeks_bul(a->id);
        if (!ziyaret_edildi[idx])
            derinlik_arama(idx, hedef_derinlik, mevcut_derinlik + 1, ziyaret_edildi);
    }
}

void ortak_arkadaslar(int k1, int k2) {
    int i = indeks_bul(k1), j = indeks_bul(k2);
    if (i == -1 || j == -1) return;

    printf("%d ve %d ortak arkadaşları: ", k1, k2);
    for (ArkadasDugum* a1 = kullanicilar[i].arkadaslar; a1; a1 = a1->sonraki)
        for (ArkadasDugum* a2 = kullanicilar[j].arkadaslar; a2; a2 = a2->sonraki)
            if (a1->id == a2->id)
                printf("%d ", a1->id);
    printf("\n");
}

void bagli_bilesen_dfs(int indeks, int* ziyaret_edildi) {
    ziyaret_edildi[indeks] = 1;
    printf("%d ", kullanicilar[indeks].kullanici_id);
    for (ArkadasDugum* a = kullanicilar[indeks].arkadaslar; a; a = a->sonraki) {
        int idx = indeks_bul(a->id);
        if (!ziyaret_edildi[idx])
            bagli_bilesen_dfs(idx, ziyaret_edildi);
    }
}

void topluluklari_bul() {
    int ziyaret_edildi[MAKS_KULLANICI] = {0};
    for (int i = 0; i < kullanici_sayisi; i++) {
        if (!ziyaret_edildi[i]) {
            printf("Topluluk: ");
            bagli_bilesen_dfs(i, ziyaret_edildi);
            printf("\n");
        }
    }
}

int etki_alani_hesapla(int indeks, int* ziyaret_edildi) {
    ziyaret_edildi[indeks] = 1;
    int sayac = 1;
    for (ArkadasDugum* a = kullanicilar[indeks].arkadaslar; a; a = a->sonraki) {
        int idx = indeks_bul(a->id);
        if (!ziyaret_edildi[idx])
            sayac += etki_alani_hesapla(idx, ziyaret_edildi);
    }
    return sayac;
}

// --- Ana Program ---
int main() {
    int secim, id, k1, k2, mesafe;

    while (1) {
        printf("\n1. Kullanici Ekle\n2. Arkadaslik Ekle\n3. Kullanici Listesi\n4. Mesafeye Gore Arkadaslar\n5. Ortak Arkadaslar\n6. Topluluklari Bul\n7. Etki Alani Hesapla\n8. ID Ara (Kırmızı-Siyah Ağaç)\n0. Cikis\nSeciminiz: ");
        scanf("%d", &secim);
        if (secim == 0) break;

        switch (secim) {
            case 1:
                printf("Kullanici ID: ");
                scanf("%d", &id);
                kullanici_ekle(id);
                break;
            case 2:
                printf("1. Kullanici ID: "); scanf("%d", &k1);
                printf("2. Kullanici ID: "); scanf("%d", &k2);
                kullanici_ekle(k1); kullanici_ekle(k2);
                arkadaslik_ekle(k1, k2);
                break;
            case 3:
                kullanicilari_yazdir();
                break;
            case 4:
                printf("Kullanici ID: "); scanf("%d", &id);
                printf("Mesafe: "); scanf("%d", &mesafe);
                k1 = indeks_bul(id);
                if (k1 == -1) printf("Kullanici bulunamadi.\n");
                else {
                    int ziyaret_edildi[MAKS_KULLANICI] = {0};
                    printf("%d mesafedeki arkadaşlar: ", mesafe);
                    derinlik_arama(k1, mesafe, 0, ziyaret_edildi);
                    printf("\n");
                }
                break;
            case 5:
                printf("1. Kullanici ID: "); scanf("%d", &k1);
                printf("2. Kullanici ID: "); scanf("%d", &k2);
                ortak_arkadaslar(k1, k2);
                break;
            case 6:
                topluluklari_bul();
                break;
            case 7:
                printf("Kullanici ID: "); scanf("%d", &id);
                k1 = indeks_bul(id);
                if (k1 == -1) printf("Kullanici bulunamadi.\n");
                else {
                    int ziyaret_edildi[MAKS_KULLANICI] = {0};
                    int sayi = etki_alani_hesapla(k1, ziyaret_edildi);
                    printf("%d kisiyi etkiliyor.\n", sayi - 1);
                }
                break;
            case 8:
                printf("Kullanici ID girin: ");
                scanf("%d", &id);
                if (ks_arama(id)) printf("Kullanici mevcut (Kırmızı-Siyah Ağaç).\n");
                else printf("Kullanici bulunamadi (Kırmızı-Siyah Ağaç).\n");
                break;
            default:
                printf("Geçersiz seçim.\n");
        }
    }

    return 0;
}
