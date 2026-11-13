#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Rastgelelik cekirdegi
    srand(time(NULL));

    // Durum degiskenleri
    int saglik = 100;
    int enerji = 100;
    int oksijen = 100;
    int su = 0;
    int yedekParca = 0;
    int modulHasari = 0;
    int kilitMuhurlu = 0; // 0: Acik, 1: Muhurlu
    int gun = 1;
    int tur = 0;

    // Zorluk seviyesine gore degisen parametreler
    char zorluk;
    int gecerliZorluk = 0;
    int firtinaSuresi = 5;
    int firtinaHasarCarpani = 1;
    int kesifEsikBonus = 0;

    printf("==================================================\n");
    printf("   MARS KOLONISI YASAM DESTEK SIMULATORU\n");
    printf("==================================================\n");

    // 1. ZORLUK SEVIYESI SECIMI (SWITCH-CASE ve DO-WHILE)
    do {
        printf("\nZorluk Seviyesi Secin [K: Kolay | N: Normal | Z: Zor]: ");
        scanf(" %c", &zorluk);

        switch (zorluk) {
            case 'k':
            case 'K':
                saglik = 100; enerji = 100; oksijen = 100;
                su = 25; yedekParca = 5; modulHasari = 5;
                firtinaSuresi = 3;
                firtinaHasarCarpani = 1;
                kesifEsikBonus = 15;
                gecerliZorluk = 1;
                printf(">> Zorluk: KOLAY secildi. Baslangic kaynaklari yuksek.\n");
                break;

            case 'n':
            case 'N':
                saglik = 100; enerji = 90; oksijen = 90;
                su = 15; yedekParca = 3; modulHasari = 15;
                firtinaSuresi = 5;
                firtinaHasarCarpani = 1;
                kesifEsikBonus = 0;
                gecerliZorluk = 1;
                printf(">> Zorluk: NORMAL secildi. Dengeli sartlar aktif.\n");
                break;

            case 'z':
            case 'Z':
                saglik = 85; enerji = 80; oksijen = 80;
                su = 8; yedekParca = 1; modulHasari = 30;
                firtinaSuresi = 7;
                firtinaHasarCarpani = 2;
                kesifEsikBonus = -15;
                gecerliZorluk = 1;
                printf(">> Zorluk: ZOR secildi. Kaynaklar kit, firtinalar siddetli!\n");
                break;

            default:
                printf("[HATA] Gecersiz zorluk seviyesi! Lutfen K, N veya Z girin.\n");
                break;
        }
    } while (!gecerliZorluk);

    char komut;
    int oyunDevam = 1;

    // 2. ANA KOMUT DINLEME DONGUSU (DO-WHILE)
    do {
        int turIlerlesin = 0; // Sadece gecerli ve zaman harcayan komutlarda 1 olur

        printf("\n--------------------------------------------------\n");
        printf("GUN: %d / 15 | TUR: %d | Saglik: %d | Enerji: %d | Oksijen: %d\n", 
               gun, tur, saglik, enerji, oksijen);
        printf("Komutlar -> [G]Kesif [O]Oksijen [T]Tamir [D]Durum [U]Uyu [K]Firtina [H]HavaKilidi [Q]Cikis\n");
        printf("Komutunuz: ");
        scanf(" %c", &komut);

        switch (komut) {
            // ---------------------------------------------------------
            // G: KESIF GOREVI
            // ---------------------------------------------------------
            case 'g':
            case 'G': {
                if (enerji < 15 || oksijen < 10) {
                    printf("[YETERSIZ KAYNAK] Kesif icin en az 15 Enerji ve 10 Oksijen gerekli!\n");
                    break;
                }

                enerji -= 15;
                oksijen -= 10;
                turIlerlesin = 1;

                int zar = rand() % 100;
                // Enerji ve oksijen durumuna gore dinamik esik belirleme
                int basariEsigi = (enerji > 40 && oksijen > 30) ? (55 + kesifEsikBonus) : (35 + kesifEsikBonus);
                if (basariEsigi < 10) basariEsigi = 10;
                if (basariEsigi > 90) basariEsigi = 90;

                printf("[KESIF] Zar: %d / Esik: %d -> %s\n", 
                       zar, basariEsigi, (zar < basariEsigi) ? "BASARILI" : "BASARISIZ");

                // En az 3 farkli sonuc (Su bulma, Yedek parca bulma, Eli bos donme)
                if (zar < basariEsigi / 2) {
                    int bulunanSu = 6 + (rand() % 5);
                    su += bulunanSu;
                    printf(">> Yeralti buz tabakasi bulundu! +%d birim Su kazandiniz.\n", bulunanSu);
                } else if (zar < basariEsigi) {
                    int bulunanParca = 1 + (rand() % 2);
                    yedekParca += bulunanParca;
                    printf(">> Eski bir uydu enkazi bulundu! +%d Yedek Parca kazandiniz.\n", bulunanParca);
                } else {
                    printf(">> Bolge tarandi ancak yararli bir kaynak bulunamadi (Eli bos donuldu).\n");
                }

                // Yaralanma kontrolu (zar < 10 || modulHasari > 70)
                if (zar < 10 || modulHasari > 70) {
                    int yaralanmaHasari = 12;
                    saglik -= yaralanmaHasari;
                    printf("[TEHLIKE] Engebeli arazide kaza veya modul arizasi! -%d Saglik kaybettiniz.\n", yaralanmaHasari);
                }
                break;
            }

            // ---------------------------------------------------------
            // O: OKSIJEN URETIMI (MOD % OPERATORU KULLANIMI)
            // ---------------------------------------------------------
            case 'o':
            case 'O': {
                if (enerji < 10) {
                    printf("[YETERSIZ KAYNAK] Oksijen jeneratorunu calistirmak icin en az 10 Enerji gerekli!\n");
                    break;
                }
                if (su < 2) {
                    printf("[YETERSIZ KAYNAK] Oksijen uretimi icin en az 2 birim Su gerekli! (Mevcut: %d)\n", su);
                    break;
                }

                int harcanacakSu;
                printf("Oksijen uretimi icin kac birim su kullanilsin? (Mevcut Su: %d): ", su);
                scanf("%d", &harcanacakSu);

                if (harcanacakSu <= 0 || harcanacakSu > su) {
                    printf("[GECERSIZ MIKTAR] Mevcut sudan fazla veya sifir/negatif miktar girilemez!\n");
                    break;
                }
                if (harcanacakSu < 2) {
                    printf("[BILGI] Donusum icin en az 2 birim su gereklidir.\n");
                    break;
                }

                // Mod (%) operatoru ile tek/cift su miktari kontrolu
                int artikSu = harcanacakSu % 2;
                int islenenSu = harcanacakSu - artikSu;
                int uretilenOksijen = (islenenSu / 2) * 3;

                su -= islenenSu;
                enerji -= 10;
                oksijen += uretilenOksijen;
                turIlerlesin = 1;

                if (artikSu == 1) {
                    printf(">> %d birim su islendi (+%d Oksijen). 1 birim su tek kaldigi icin depoda birakildi.\n", 
                           islenenSu, uretilenOksijen);
                } else {
                    printf(">> Girilen %d birim suyun tamami islendi (+%d Oksijen).\n", 
                           islenenSu, uretilenOksijen);
                }
                break;
            }

            // ---------------------------------------------------------
            // T: TAMIR (IF - ELSE IF - ELSE ZINCIRI)
            // ---------------------------------------------------------
            case 't':
            case 'T': {
                if (modulHasari == 0) {
                    printf("[BILGI] Modul hasari zaten 0. Tamire gerek yok.\n");
                    break;
                }
                if (yedekParca <= 0) {
                    printf("[YETERSIZ KAYNAK] Tamir icin hic yedek parcaniz yok!\n");
                    break;
                }
                if (enerji < 10) {
                    printf("[YETERSIZ KAYNAK] Tamir islemi icin en az 10 Enerji gerekli!\n");
                    break;
                }

                // Hasar seviyesine gore 3 kademeli tamir
                if (modulHasari <= 30) {
                    // Hafif hasar: 1 parca harcar, 15 hasar onarir
                    yedekParca -= 1;
                    modulHasari -= 15;
                    enerji -= 10;
                    turIlerlesin = 1;
                    printf("[HAFIF HASAR TAMIRI] 1 Yedek Parca kullanildi. Modul hasari 15 birim azaltildi.\n");
                } else if (modulHasari <= 70) {
                    // Orta hasar: 2 parca harcar, 30 hasar onarir
                    if (yedekParca < 2) {
                        printf("[YETERSIZ KAYNAK] Orta seviye hasar tamiri icin 2 Yedek Parca gerekli! (Mevcut: %d)\n", yedekParca);
                        break;
                    }
                    yedekParca -= 2;
                    modulHasari -= 30;
                    enerji -= 10;
                    turIlerlesin = 1;
                    printf("[ORTA HASAR TAMIRI] 2 Yedek Parca kullanildi. Modul hasari 30 birim azaltildi.\n");
                } else {
                    // Agir hasar (71-100): 3 parca harcar, 45 hasar onarir
                    if (yedekParca < 3) {
                        printf("[YETERSIZ KAYNAK] Agir hasar tamiri icin 3 Yedek Parca gerekli! (Mevcut: %d)\n", yedekParca);
                        break;
                    }
                    yedekParca -= 3;
                    modulHasari -= 45;
                    enerji -= 10;
                    turIlerlesin = 1;
                    printf("[AGIR HASAR TAMIRI] 3 Yedek Parca kullanildi. Modul hasari 45 birim azaltildi.\n");
                }
                break;
            }

            // ---------------------------------------------------------
            // D: DURUM RAPORU (TUR HARCAMAZ)
            // ---------------------------------------------------------
            case 'd':
            case 'D': {
                printf("\n================ ISTASYON DURUM RAPORU ================\n");
                printf("Gun          : %d / 15 (Toplam Tur: %d)\n", gun, tur);
                printf("Saglik       : %d / 100\n", saglik);
                printf("Enerji       : %d / 100\n", enerji);
                printf("Oksijen      : %d / 100\n", oksijen);
                printf("Su Miktari   : %d birim\n", su);
                printf("Yedek Parca  : %d adet\n", yedekParca);
                printf("Modul Hasari : %d / 100\n", modulHasari);
                printf("Hava Kilidi  : %s\n", kilitMuhurlu ? "MUHURLU (Guvenli)" : "ACIK (Savunmasiz)");
                printf("-------------------------------------------------------\n");

                // Kritik uyarilar
                if (oksijen < 25) printf("[KRITIK UYARI] Oksijen seviyesi tehlikeli derecede dusuk!\n");
                if (saglik < 30)  printf("[KRITIK UYARI] Karakter sagligi kritik seviyede!\n");
                if (enerji < 20)  printf("[KRITIK UYARI] Enerji rezervleri tukenmek uzere!\n");
                if (modulHasari > 70) printf("[KRITIK UYARI] Modul hasari kritik! Yapisal cokme riski!\n");
                if (su < 4)       printf("[KRITIK UYARI] Su stogu kritik seviyede!\n");

                // Tek kosulla tum sistemlerin normal oldugunun kontrolu
                if (oksijen >= 25 && saglik >= 30 && enerji >= 20 && modulHasari <= 70 && su >= 4) {
                    printf(">> TUM SISTEMLER NORMAL <<\n");
                }
                printf("=======================================================\n");
                // turIlerlesin = 0 kalir (zaman harcamaz)
                break;
            }

            // ---------------------------------------------------------
            // U: UYU / DINLEN (IC ICE IF)
            // ---------------------------------------------------------
            case 'u':
            case 'U': {
                enerji += 25;
                oksijen -= 10;
                turIlerlesin = 1;

                // Oksijen 20'nin altindaysa saglik artmaz, azalir
                if (oksijen < 20) {
                    saglik -= 10;
                    printf("[UYARI] Dusuk oksijen altinda uyudunuz! Enerji artti (+25) ama hipoksi nedeniyle -10 Saglik kaybettiniz.\n");
                } else {
                    if (saglik < 100) {
                        saglik += 15;
                        printf(">> Dinlendiniz. +25 Enerji ve +15 Saglik kazandiniz (-10 Oksijen).\n");
                    } else {
                        printf(">> Dinlendiniz. +25 Enerji kazandiniz, sagliginiz zaten tam (-10 Oksijen).\n");
                    }
                }
                break;
            }

            // ---------------------------------------------------------
            // K: KUM FIRTINASI SIMULASYONU (FOR DONGUSU, CONTINUE, BREAK)
            // ---------------------------------------------------------
            case 'k':
            case 'K': {
                if (enerji < 10) {
                    printf("[YETERSIZ KAYNAK] Firtina kalkanlarini aktif tutmak icin en az 10 Enerji gerekli!\n");
                    break;
                }

                enerji -= 10;
                turIlerlesin = 1;

                int hasarAlinanTur = 0;
                int topKayipSaglik = 0;
                int topKayipOksijen = 0;
                int i; // C89 uyumlulugu icin dongu disinda tanimlandi

                printf("\n>>> SIDDETLI MARS KUM FIRTINASI BASLADI (%d Tur Surecek) <<<\n", firtinaSuresi);

                for (i = 1; i <= firtinaSuresi; i++) {
                    int zar = rand() % 100;
                    int esik = 60; // 60 alti olumsuz olay
                    int carpan = (i % 2 == 0) ? 2 : 1; // Cift numarali turlarda hasar 2 katina cikar

                    printf("Firtina Turu %d/%d %s | Zar: %d / Esik: %d -> ", 
                           i, firtinaSuresi, (carpan == 2 ? "[SIDDETLI RUZGAR x2]" : ""), zar, esik);

                    if (zar >= esik) {
                        printf("BASARILI (Korunuldu)\n");
                    } else {
                        printf("BASARISIZ -> ");
                        int olayTipi = zar % 3; // 0: Oksijen sizintisi, 1: Modul hasari, 2: Can kaybi

                        if (olayTipi == 0) {
                            // Hava kilidi muhurluyse oksijen sizintisi continue ile atlanir
                            if (kilitMuhurlu == 1) {
                                printf("Hava kilidi muhurlu oldugu icin oksijen sizintisi engellendi!\n");
                                continue;
                            }
                            int o2Kaybi = 6 * carpan * firtinaHasarCarpani;
                            oksijen -= o2Kaybi;
                            topKayipOksijen += o2Kaybi;
                            hasarAlinanTur++;
                            printf("Oksijen sizintisi! -%d Oksijen\n", o2Kaybi);
                        } else if (olayTipi == 1) {
                            int mHasar = 8 * carpan * firtinaHasarCarpani;
                            modulHasari += mHasar;
                            hasarAlinanTur++;
                            printf("Kaya parcalari istasyona carpti! +%d Modul Hasari\n", mHasar);
                        } else {
                            int sKaybi = 7 * carpan * firtinaHasarCarpani;
                            saglik -= sKaybi;
                            topKayipSaglik += sKaybi;
                            hasarAlinanTur++;
                            printf("Basinc dususu yaralanmaya yol acti! -%d Saglik\n", sKaybi);
                        }
                    }

                    // Saglik veya oksijen 0'a duserse dongu break ile derhal sonlandirilir
                    if (saglik <= 0 || oksijen <= 0) {
                        printf("[KRITIK DURUM] Yasam belirtileri tukendi! Firtina simulasyonu kesiliyor...\n");
                        break;
                    }
                }

                printf("--- FIRTINA OZETI ---\n");
                printf("Hasar Alinan Tur Sayisi : %d\n", hasarAlinanTur);
                printf("Toplam Kaybedilen Saglik: %d\n", topKayipSaglik);
                printf("Toplam Kaybedilen Oksijen: %d\n", topKayipOksijen);
                break;
            }

            // ---------------------------------------------------------
            // H: HAVA KILIDI SIFRESI (DO-WHILE DOGRULAMA DONGUSU)
            // ---------------------------------------------------------
            case 'h':
            case 'H': {
                if (kilitMuhurlu == 1) {
                    printf("[BILGI] Hava kilidi zaten muhurlu durumda!\n");
                    break;
                }
                if (enerji < 15) {
                    printf("[YETERSIZ KAYNAK] Kilit terminalini acmak icin en az 15 Enerji gerekli!\n");
                    break;
                }

                // O anki gun sayisindan aritmetik olarak turetilen sifre
                char dogruSifre = 'A' + (gun % 26);
                char tahmin;
                int deneme = 0;
                int maxDeneme = 3;

                printf("\n[HAVA KILIDI TERMINALI] Kilit sistemini muhurlemek icin sifreyi girin.\n");
                printf("IPUCU: Sifre, Ingiliz alfabesinde 'A' harfinden bugunun gun sayisi (%d) kadar ilerideki harftir.\n", gun);

                do {
                    printf("Deneme %d/%d - Tek karakter sifre girin (A-Z): ", deneme + 1, maxDeneme);
                    scanf(" %c", &tahmin);

                    // Kucuk harf girisini buyuk harfe cevirme (ASCII aritmetigi)
                    if (tahmin >= 'a' && tahmin <= 'z') {
                        tahmin = tahmin - ('a' - 'A');
                    }

                    deneme++;

                    if (tahmin != dogruSifre) {
                        enerji -= 5;
                        printf("[YANLIS SIFRE] -5 Enerji kaybettiniz. ");
                        if (tahmin < dogruSifre) {
                            printf("Ipucu: Alfabede ILERI git!\n");
                        } else {
                            printf("Ipucu: Alfabede GERI git!\n");
                        }
                    }
                } while (tahmin != dogruSifre && deneme < maxDeneme);

                turIlerlesin = 1;

                if (tahmin == dogruSifre) {
                    kilitMuhurlu = 1;
                    printf(">> [ERISIM ONAYLANDI] Hava kilidi basariyla muhurlendi!\n");
                } else {
                    modulHasari += 20;
                    printf(">> [ERISIM REDDEDILDI] 3 deneme basarisiz! Guvenlik kilitlenmesi nedeniyle +20 Modul Hasari olustu.\n");
                }
                break;
            }

            // ---------------------------------------------------------
            // Q: CIKIS
            // ---------------------------------------------------------
            case 'q':
            case 'Q':
                printf("\n[CIKIS] Gorev oyuncu tarafindan sonlandirildi.\n");
                oyunDevam = 0;
                break;

            // ---------------------------------------------------------
            // DEFAULT: TANIMSIZ KOMUT
            // ---------------------------------------------------------
            default:
                printf("[UYARI] Tanimsiz komut girdiniz! Bu islem tur harcamadi.\n");
                break;
        }

        // 3. SINIRLANDIRMA KONTROLLERI (0 - 100 ARALIGI)
        if (saglik < 0) saglik = 0;
        if (saglik > 100) saglik = 100;

        if (enerji < 0) enerji = 0;
        if (enerji > 100) enerji = 100;

        if (oksijen < 0) oksijen = 0;
        if (oksijen > 100) oksijen = 100;

        if (modulHasari < 0) modulHasari = 0;
        if (modulHasari > 100) modulHasari = 100;

        // 4. ZAMAN AKISI VE GUN GECISI (Gecerli komutlarda ve oyun bitmediyse)
        if (oyunDevam && turIlerlesin) {
            tur++;

            // Her 3 turda bir yeni gun baslar
            if (tur % 3 == 0) {
                gun++;
                if (gun <= 15) {
                    oksijen -= 8;
                    printf("\n*** YENI GUN BASLADI (GUN %d) ***\n", gun);
                    printf(">> Gunluk yasam destegi tuketimi: -8 Oksijen.\n");

                    if (modulHasari > 70) {
                        saglik -= 5;
                        printf(">> [UYARI] Modul hasari 70'in uzerinde oldugu icin -5 Saglik kaybettiniz!\n");
                    }

                    // Gun basi kesintileri sonrasi tekrar sinirlandirma
                    if (oksijen < 0) oksijen = 0;
                    if (saglik < 0) saglik = 0;
                }
            }
        }

        // 5. OYUN SONU KOSULLARI KONTROLU
        if (saglik <= 0 || oksijen <= 0) {
            printf("\n==================================================\n");
            printf("   OYUN BITTI - KAYBETTINIZ!\n");
            if (saglik <= 0) printf("   Sebep: Muhendisin sagligi tukendi.\n");
            else printf("   Sebep: Istasyondaki oksijen tamamen tukendi.\n");
            printf("==================================================\n");
            oyunDevam = 0;
        } else if (gun > 15) {
            gun = 15; // Raporlamada 15 gun tamamlandi olarak gorunmesi icin
            printf("\n==================================================\n");
            printf("   TEBRIKLER - KURTARMA EKIBI ULASTI!\n");
            printf("   15 gun boyunca Mars'ta hayatta kalmayi basardiniz!\n");
            printf("==================================================\n");
            oyunDevam = 0;
        }

    } while (oyunDevam);

    // 6. FINAL SONUC RAPORU VE PUAN HESAPLAMASI
    int puan = (gun * 100) + saglik + oksijen - modulHasari;

    printf("\n================ FINAL SONUC RAPORU ================\n");
    printf("Hayatta Kalinan Gun : %d\n", gun);
    printf("Toplam Gecen Tur    : %d\n", tur);
    printf("Son Saglik          : %d\n", saglik);
    printf("Son Enerji          : %d\n", enerji);
    printf("Son Oksijen         : %d\n", oksijen);
    printf("Son Su Miktari      : %d\n", su);
    printf("Son Yedek Parca     : %d\n", yedekParca);
    printf("Son Modul Hasari    : %d\n", modulHasari);
    printf("----------------------------------------------------\n");
    printf("TOPLAM PUAN         : %d\n", puan);
    printf("====================================================\n");

    return 0;
}
