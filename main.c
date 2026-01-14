#include <stdio.h>


 //Ýsim SOYÝSÝM: [BATUHAN DEMÝREL]
 //Öðrenci Numarasý: [2420171009]
 //BTK Akademi Sertifika Baðlantýsý: [https://www.btkakademi.gov.tr/portal/certificate/validate?certificateId=1kZCeJqPdZ]
 


void kabarcikSirala(int dizi[], int n) {
    int i, j, gecici;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (dizi[j] > dizi[j + 1]) {
                
                gecici = dizi[j];
                dizi[j] = dizi[j + 1];
                dizi[j + 1] = gecici;
            }
        }
    }
}

/
int ikiliArama(int dizi[], int sol, int sag, int aranan) {
    while (sol <= sag) {
        int orta = sol + (sag - sol) / 2;

        
        if (dizi[orta] == aranan)
            return orta;

        
        if (dizi[orta] < aranan)
            sol = orta + 1;
       
        else
            sag = orta - 1;
    }
   
    return -1;
}

int main() {
    int n, i, aranan, sonuc;

    printf("Dizinin eleman sayisini giriniz: ");
    scanf("%d", &n);

    int dizi[n];
    printf("%d adet sayi giriniz:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &dizi[i]);
    }

  
    kabarcikSirala(dizi, n);

    printf("\nSiralanmis Dizi: ");
    for (i = 0; i < n; i++) {
        printf("%d ", dizi[i]);
    }

    printf("\n\nAramak istediginiz sayiyi giriniz: ");
    scanf("%d", &aranan);

  
    sonuc = ikiliArama(dizi, 0, n - 1, aranan);

    if (sonuc != -1) {
        printf("Sayi dizinin %d. indeksinde bulundu.\n", sonuc);
    } else {
        printf("Aranan sayi dizide bulunamadi.\n");
    }

    return 0;
}
