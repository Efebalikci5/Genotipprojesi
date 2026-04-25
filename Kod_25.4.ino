// Birleştirilmiş Genetik Simülatör: Otosomal ve X/Y Bağlı Kalıtım
// Temel: Kod_24.04.ino'nun LED array yapısı ve basit loop.
// Eklenen: X_Bagli_Kalitim.ino'dan X-linked mantık, mod geçişi, cinsiyet LED'leri.
// Mod: false=otosomal (Kod_24.04.ino mantığı), true=X-linked (X_Bagli_Kalitim.ino mantığı).
// Pinler: Pin_Aciklama.txt'ye bak (karıştırmama için).
// Debounce: Her düğme için ayrı millis() kullan.
// Flash: Mod değişikliğinde tüm LED'ler 2 kez (otosomal) veya 3 kez (X-linked) yanıp söner.
// Cinsiyet LED'leri: Sadece X-linked modda çalışır, otosomal'de LOW.
// Serial: Mod'a göre çıktı (otosomal: AA/Aa/aa, X-linked: Türkçe + cinsiyet).

// Struct genişletmesi: isMale ekle (X-linked için).
struct Individual {
    char gene1; // A veya a (X-linked'de erkek için X alleli, kadın için X1).
    char gene2; // A, a, veya Y (X-linked'de erkek için Y, kadın için X2).
    bool isMale; // true=erkek, false=kadın (X-linked için).
};

Individual mother = {'A', 'a', false}; // Anne: kadın, başlangıç genler.
Individual father = {'A', 'a', true};  // Baba: erkek, başlangıç genler.
Individual child1; // Çocuk 1
Individual child2; // Çocuk 2

// LED array genişletmesi: 4 kişi, 5 LED (3 genotip + 1 cinsiyet).
// Cinsiyet LED'leri: Sadece X-linked'de kullanılır.
// Diyagrama göre düzeltilmiş pinler.
const int ledPins[4][5] = {
    {5, 6, 7, -1},    // Anne: 3 genotip, cinsiyet yok (-1 kullanılmaz).
    {11, 12, 13, -1}, // Baba: 3 genotip, cinsiyet yok.
    {8, 9, 10, 18},   // Çocuk 1: 3 genotip + cinsiyet LED.
    {14, 15, 16, 19}  // Çocuk 2: 3 genotip + cinsiyet LED.
};

// Düğme pinleri: Simülasyon (2), Mod geçişi (4).
const int buttonPin = 2; // Simülasyon düğmesi.
const int modeButtonPin = 4; // Mod geçiş düğmesi.

// Mod değişkeni: Başlangıç otosomal.
bool isXLinked = false;

// Debounce için ayrı zamanlayıcılar (karıştırmama için).
unsigned long lastSimPress = 0;
unsigned long lastModePress = 0;
unsigned long lastBlink = 0;
bool blinkState = false;

// Flash animasyonu için non-blocking değişkenler.
bool flashing = false;
int flashTimes = 0;
int currentFlash = 0;
unsigned long lastFlashToggle = 0;
bool flashState = false;

// On release debounce için.
bool simButtonWasPressed = false;
bool modeButtonWasPressed = false;

void setup() {
    Serial.begin(9600); // Seri iletişim başlat
    randomSeed(analogRead(A0) + analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A5)); // Rastgele sayı üreteci için karmaşık tohum değeri
    pinMode(buttonPin, INPUT_PULLUP); // Simülasyon düğmesi: giriş, pull-up aktif.
    pinMode(modeButtonPin, INPUT_PULLUP); // Mod düğmesi: giriş, pull-up aktif.

    // Tüm LED pinlerini çıkış olarak ayarla ve başlangıçta söndür
    for (int person = 0; person < 4; person++) {
        for (int color = 0; color < 3; color++) { // Genotip LED'leri.
            pinMode(ledPins[person][color], OUTPUT);
            digitalWrite(ledPins[person][color], LOW);
        }
        if (ledPins[person][3] != -1) { // Cinsiyet LED'leri (çocuklar için).
            pinMode(ledPins[person][3], OUTPUT);
            digitalWrite(ledPins[person][3], LOW);
        }
    }

    Serial.println("Genetik Simülatör Hazır - Mod: Otosomal");
    Serial.println("Mod geçişi için pin 4'e basın, simülasyon için pin 2'ye basın");

    // Başlangıç flash: Mod göstergesi.
    flashAllLEDs(isXLinked ? 3 : 2);

    // Başlangıçta anne ve babanın genotipini LED'lerde göster
    displayGenotype(0, mother);
    displayGenotype(1, father);

    // Çocukların LED'lerini başlangıçta söndür
    for (int color = 0; color < 3; color++) {
        digitalWrite(ledPins[2][color], LOW);
        digitalWrite(ledPins[3][color], LOW);
    }
    // Cinsiyet LED'leri LOW (mod'a göre sonra değişir).
}

void loop() {
    // Mod düğmesi: On release trigger.
    bool currentModeState = digitalRead(modeButtonPin) == LOW;
    if (currentModeState && !modeButtonWasPressed) {
        modeButtonWasPressed = true;
    }
    if (!currentModeState && modeButtonWasPressed && millis() - lastModePress > 200) {
        modeButtonWasPressed = false;
        lastModePress = millis();
        isXLinked = !isXLinked; // Mod değiştir.
        flashAllLEDs(isXLinked ? 3 : 2); // Gösterge.
        Serial.println(isXLinked ? "Mod: X/Y Bağlı" : "Mod: Otosomal");
    }

    // Simülasyon düğmesi: On release trigger.
    bool currentSimState = digitalRead(buttonPin) == LOW;
    if (currentSimState && !simButtonWasPressed) {
        simButtonWasPressed = true;
    }
    if (!currentSimState && simButtonWasPressed && millis() - lastSimPress > 200) {
        simButtonWasPressed = false;
        lastSimPress = millis();
        simulateGenetics(isXLinked); // Mod'a göre hesapla.
        updateLEDs(isXLinked); // Mod'a göre güncelle.
        printResults(isXLinked); // Mod'a göre çıktı.
    }

    // Cinsiyet LED güncellemesi: X-linked modda sürekli.
    if (isXLinked) {
        if (millis() - lastBlink > 500) {
            lastBlink = millis();
            blinkState = !blinkState;
        }
        digitalWrite(ledPins[2][3], child1.isMale ? HIGH : blinkState);
        digitalWrite(ledPins[3][3], child2.isMale ? HIGH : blinkState);
    } else {
        digitalWrite(ledPins[2][3], LOW);
        digitalWrite(ledPins[3][3], LOW);
    }

    // Flash animasyonu: Non-blocking.
    if (flashing) {
        if (millis() - lastFlashToggle > 500) {
            lastFlashToggle = millis();
            flashState = !flashState;
            for (int p = 0; p < 4; p++) {
                for (int c = 0; c < 3; c++) digitalWrite(ledPins[p][c], flashState);
                if (ledPins[p][3] != -1) digitalWrite(ledPins[p][3], flashState);
            }
            if (!flashState) {
                currentFlash++;
                if (currentFlash >= flashTimes) {
                    flashing = false;
                }
            }
        }
    }
}

// Yeni fonksiyonlar: Mod'a göre işlemler.

// Flash animasyonunu başlat (non-blocking).
void flashAllLEDs(int times) {
    flashing = true;
    flashTimes = times;
    currentFlash = 0;
    flashState = false;
    lastFlashToggle = millis();
}

// Genetik hesaplaması: Mod'a göre dallandır.
void simulateGenetics(bool isXLinked) {
    if (!isXLinked) {
        // Otosomal: Kod_24.04.ino mantığı.
        geneticCalculate(&mother);
        geneticCalculate(&father);
        geneticCalculate(&child1);
        geneticCalculate(&child2);
        // Cinsiyet yok, isMale kullanılmaz.
    } else {
        // X-linked: X_Bagli_Kalitim.ino mantığı.
        // Anne: rastgele XX.
        mother.gene1 = random(2) ? 'A' : 'a';
        mother.gene2 = random(2) ? 'A' : 'a';
        mother.isMale = false;
        // Baba: rastgele X, Y.
        father.gene1 = random(2) ? 'A' : 'a';
        father.gene2 = 'Y';
        father.isMale = true;
        // Çocuk 1: cinsiyet %50, miras.
        child1.isMale = random(2);
        if (child1.isMale) {
            child1.gene1 = random(2) ? mother.gene1 : mother.gene2;
            child1.gene2 = 'Y';
        } else {
            child1.gene1 = random(2) ? mother.gene1 : mother.gene2;
            child1.gene2 = father.gene1;
        }
        // Çocuk 2: aynı.
        child2.isMale = random(2);
        if (child2.isMale) {
            child2.gene1 = random(2) ? mother.gene1 : mother.gene2;
            child2.gene2 = 'Y';
        } else {
            child2.gene1 = random(2) ? mother.gene1 : mother.gene2;
            child2.gene2 = father.gene1;
        }
    }
}

// LED güncellemesi: Mod'a göre.
void updateLEDs(bool isXLinked) {
    displayGenotype(0, mother);
    displayGenotype(1, father);
    displayGenotype(2, child1);
    displayGenotype(3, child2);
    // Cinsiyet LED'leri: Loop'ta sürekli güncellenir.
}

// Sonuçları seri yazdır: Mod'a göre.
void printResults(bool isXLinked) {
    if (!isXLinked) {
        // Otosomal: Kod_24.04.ino çıktısı.
        Serial.println("Çaprazlama tamamlandı!");
        Serial.print("Anne: ");
        Serial.print(mother.gene1);
        Serial.println(mother.gene2);
        Serial.print("Baba: ");
        Serial.print(father.gene1);
        Serial.println(father.gene2);
        Serial.print("Çocuk 1: ");
        Serial.print(child1.gene1);
        Serial.println(child1.gene2);
        Serial.print("Çocuk 2: ");
        Serial.print(child2.gene1);
        Serial.println(child2.gene2);
    } else {
        // X-linked: Türkçe çıktı + cinsiyet.
        Serial.println("Çaprazlama tamamlandı!");
        Serial.print("Anne: Kadın - ");
        if (mother.gene1 == 'A' && mother.gene2 == 'A') Serial.println("Sağlıklı (XA XA)");
        else if (mother.gene1 == 'a' && mother.gene2 == 'a') Serial.println("Hasta (Xa Xa)");
        else Serial.println("Taşıyıcı (XA Xa)");
        Serial.print("Baba: Erkek - ");
        if (father.gene1 == 'A') Serial.println("Sağlıklı (XA Y)");
        else Serial.println("Hasta (Xa Y)");
        Serial.print("Çocuk 1: ");
        if (child1.isMale) {
            Serial.print("Erkek - ");
            if (child1.gene1 == 'A') Serial.println("Sağlıklı (XA Y)");
            else Serial.println("Hasta (Xa Y)");
        } else {
            Serial.print("Kız - ");
            if (child1.gene1 == 'A' && child1.gene2 == 'A') Serial.println("Sağlıklı (XA XA)");
            else if (child1.gene1 == 'a' && child1.gene2 == 'a') Serial.println("Hasta (Xa Xa)");
            else Serial.println("Taşıyıcı (XA Xa)");
        }
        Serial.print("Çocuk 2: ");
        if (child2.isMale) {
            Serial.print("Erkek - ");
            if (child2.gene1 == 'A') Serial.println("Sağlıklı (XA Y)");
            else Serial.println("Hasta (Xa Y)");
        } else {
            Serial.print("Kız - ");
            if (child2.gene1 == 'A' && child2.gene2 == 'A') Serial.println("Sağlıklı (XA XA)");
            else if (child2.gene1 == 'a' && child2.gene2 == 'a') Serial.println("Hasta (Xa Xa)");
            else Serial.println("Taşıyıcı (XA Xa)");
        }
    }
}

// Eski fonksiyonlar (otosomal için koru).
void geneticCalculate(Individual* child) {
    if (child == &mother || child == &father) {
        // Anne veya Baba için: her iki gen de rastgele A veya a olur
        child->gene1 = random(2) ? 'A' : 'a';
        child->gene2 = random(2) ? 'A' : 'a';
    } else {
        // Çocuk için: ebeveynlerden birer gen miras al
        child->gene1 = random(2) ? mother.gene1 : mother.gene2;
        child->gene2 = random(2) ? father.gene1 : father.gene2;
    }
}

// Genotipi LED'lerde görselleştir (otosomal için koru, X-linked'de erkekler için özel mantık).
void displayGenotype(int person, Individual individual) {
    // Önce tüm LED'leri söndür, sonra ilgili LED'i yak
    for (int color = 0; color < 3; color++) {
        digitalWrite(ledPins[person][color], LOW);
    }

    if (isXLinked && individual.isMale) {
        // Erkeklerde: gene1 == 'A' ise sağlıklı (yeşil), a ise hasta (kırmızı).
        if (individual.gene1 == 'A') {
            digitalWrite(ledPins[person][0], HIGH); // Yeşil
        } else {
            digitalWrite(ledPins[person][2], HIGH); // Kırmızı
        }
    } else {
        // Kadınlar ve otosomal: dominantCount mantığı.
        int dominantCount = 0;
        if (individual.gene1 == 'A') dominantCount++;
        if (individual.gene2 == 'A') dominantCount++;

        if (dominantCount == 2) {
            digitalWrite(ledPins[person][0], HIGH); // Tam dominant (AA) - yeşil
        } else if (dominantCount == 1) {
            digitalWrite(ledPins[person][1], HIGH); // Heterozigot (Aa) - sarı
        } else if (dominantCount == 0) {
            digitalWrite(ledPins[person][2], HIGH); // Recesif (aa) - kırmızı
        }
    }
}
