// Genetik Çaprazlama Simülatörü - Arduino Uno

// Yapı Tanımı: Her bireyin genlerini tutmak için
struct Birey {
    char gen1; // İlk gen: 'A' veya 'a'
    char gen2; // İkinci gen: 'A' veya 'a'
};

// Hiyerarşi: Anne, Baba ve 2 Çocuk
Birey anne = {'A', 'a'}; // Anne genotipi: Heterozigot
Birey baba = {'A', 'a'}; // Baba genotipi: Heterozigot
Birey cocuk1;
Birey cocuk2;

// Pin Düzeni
const int ledPins[4][3] = {
    {5, 6, 7},   // Anne LEDleri: Yeşil(5), Sarı(6), Kırmızı(7)
    {8, 9, 10},  // Baba LEDleri: Yeşil(8), Sarı(9), Kırmızı(10)
    {11, 12, 13}, // Çocuk1 LEDleri: Yeşil(11), Sarı(12), Kırmızı(13)
    {A0, A1, A2} // Çocuk2 LEDleri: Yeşil(A0), Sarı(A1), Kırmızı(A2)
};

// Girişler: Anne ve Baba durumunu değiştirmek için 2 ve 3 numaralı pinler
const int anneButon = 2;
const int babaButon = 3;

// Debounce için
unsigned long lastPress[2] = {0, 0};

void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(A5));
    pinMode(anneButon, INPUT_PULLUP);
    pinMode(babaButon, INPUT_PULLUP);
    // Tüm LED pinlerini çıkış olarak ayarla
    for (int kisi = 0; kisi < 4; kisi++) {
        for (int renk = 0; renk < 3; renk++) {
            pinMode(ledPins[kisi][renk], OUTPUT);
            digitalWrite(ledPins[kisi][renk], LOW);
        }
    }
    Serial.println("Genetik Simülatör Hazır");
    // Başlangıçta Anne ve Baba'nın LED'lerini yak
    gosterGenotip(0, anne);
    gosterGenotip(1, baba);
    // Çocuk LED'lerini söndür (henüz hesaplanmadı)
    for (int renk = 0; renk < 3; renk++) {
        digitalWrite(ledPins[2][renk], LOW);
        digitalWrite(ledPins[3][renk], LOW);
    }
}

void loop() {
    // Anne butonu kontrolü
    if (digitalRead(anneButon) == LOW && millis() - lastPress[0] > 1000) {
        lastPress[0] = millis();
        // Kalıtımı gerçekleştir
        hesaplaVeGosterCocuklar();
        Serial.println("Kalıtım gerçekleştirildi (Anne)");
    }
    // Baba butonu kontrolü
    if (digitalRead(babaButon) == LOW && millis() - lastPress[1] > 1000) {
        lastPress[1] = millis();
        // Kalıtımı gerçekleştir
        hesaplaVeGosterCocuklar();
        Serial.println("Kalıtım gerçekleştirildi (Baba)");
    }
}

// Çocukların genotiplerini hesapla ve göster
void hesaplaVeGosterCocuklar() {
    genetikHesapla(&cocuk1);
    gosterGenotip(2, cocuk1);
    genetikHesapla(&cocuk2);
    gosterGenotip(3, cocuk2);
}

// Genetik hesaplama: Anneden ve babadan rastgele gen seç
void genetikHesapla(Birey* cocuk) {
    cocuk->gen1 = random(2) ? anne.gen1 : anne.gen2;
    cocuk->gen2 = random(2) ? baba.gen1 : baba.gen2;
}

// Genotipi LED'lerde göster
void gosterGenotip(int kisi, Birey birey) {
    // Önce tüm renkleri söndür
    for (int renk = 0; renk < 3; renk++) {
        digitalWrite(ledPins[kisi][renk], LOW);
    }
    // Dominant gen sayısına göre LED'i yak
    int dominantSayisi = 0;
    if (birey.gen1 == 'A') dominantSayisi++;
    if (birey.gen2 == 'A') dominantSayisi++;
    if (dominantSayisi == 2) {
        digitalWrite(ledPins[kisi][0], HIGH); // 2 gen: Yeşil
    } else if (dominantSayisi == 1) {
        digitalWrite(ledPins[kisi][1], HIGH); // 1 gen: Sarı
    } else if (dominantSayisi == 0) {
        digitalWrite(ledPins[kisi][2], HIGH); // 0 gen: Kırmızı
    }
}
