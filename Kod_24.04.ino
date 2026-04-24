// Genetic Crossover Simulator - Arduino Uno
// Genetik Çaprazlama Simülatörü

struct Individual {
    char gene1; // Birinci allel (A veya a)
    char gene2; // İkinci allel (A veya a)
};

Individual mother = {'A', 'a'}; // Anne birey
Individual father = {'A', 'a'}; // Baba birey
Individual child1; // Çocuk 1
Individual child2; // Çocuk 2

// LED pinleri: her birey için 3 LED (dominant, heterozigot, recesif)
// 0: Anne, 1: Baba, 2: Çocuk 1, 3: Çocuk 2
// Çocuk 2 için analog pinler kullanıldı: A0=14, A1=15, A2=16
const int ledPins[4][3] = {
    {5, 6, 7},    // Anne için LED pinleri (5=Yeşil/AA, 6=Sarı/Aa, 7=Kırmızı/aa)
    {8, 9, 10},   // Baba için LED pinleri (8=Yeşil, 9=Sarı, 10=Kırmızı)
    {11, 12, 13}, // Çocuk 1 için LED pinleri (11=Yeşil, 12=Sarı, 13=Kırmızı)
    {14, 15, 16}  // Çocuk 2 için LED pinleri (14=A0=Yeşil, 15=A1=Sarı, 16=A2=Kırmızı)
};

const int buttonPin = 2; // Çalıştırma butonu pini (Arduino Uno pin 2)
unsigned long lastPress = 0; // Son buton basışının zamanı (debounce için)

void setup() {
    Serial.begin(9600); // Seri iletişim başlat
    randomSeed(analogRead(A5)); // Rastgele sayı üreteci için tohum değeri
    pinMode(buttonPin, INPUT_PULLUP); // Buton pini: giriş, iç pull-up direnci aktif

    // Tüm LED pinlerini çıkış olarak ayarla ve başlangıçta söndür
    for (int person = 0; person < 4; person++) {
        for (int color = 0; color < 3; color++) {
            pinMode(ledPins[person][color], OUTPUT);
            digitalWrite(ledPins[person][color], LOW);
        }
    }

    Serial.println("Genetik Simülatör Hazır");
    Serial.println("Çaprazlama için butona basın");

    // Başlangıçta anne ve babanın genotipini LED'lerde göster
    displayGenotype(0, mother);
    displayGenotype(1, father);

    // Çocukların LED'lerini başlangıçta söndür
    for (int color = 0; color < 3; color++) {
        digitalWrite(ledPins[2][color], LOW);
        digitalWrite(ledPins[3][color], LOW);
    }
}

void loop() {
    // Butona basıldığında ve 200ms geçmişse (debounce)
    if (digitalRead(buttonPin) == LOW && millis() - lastPress > 200) {
        lastPress = millis();

        // 1. Anne ve Baba için yeni genler hesapla ve LED'lerini yak
        geneticCalculate(&mother);
        geneticCalculate(&father);
        displayGenotype(0, mother);
        displayGenotype(1, father);

        // 2. 1-1.5 saniye bekle
        delay(random(1000, 1501)); // 1000-1500 ms arası rastgele bekle

        // 3. Çocukları hesapla ve LED'lerini yak
        geneticCalculate(&child1);
        geneticCalculate(&child2);
        displayGenotype(2, child1);
        displayGenotype(3, child2);

        // 4. Sonuçları seri monitöre yazdır
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
    }
}

// Genetik çaprazlama hesaplaması
// Eğer anne veya Baba için hesaplanıyorsa: kendi genleri rastgele değişir
// Eğer çocuk için hesaplanıyorsa: ebeveynlerden genler miras alır
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

// Genotipi LED'lerde görselleştir
void displayGenotype(int person, Individual individual) {
    int dominantCount = 0;
    if (individual.gene1 == 'A') dominantCount++;
    if (individual.gene2 == 'A') dominantCount++;

    // Önce tüm LED'leri söndür, sonra ilgili LED'i yak
    for (int color = 0; color < 3; color++) {
        digitalWrite(ledPins[person][color], LOW);
    }

    if (dominantCount == 2) {
        digitalWrite(ledPins[person][0], HIGH); // Tam dominant (AA) - yeşil
    } else if (dominantCount == 1) {
        digitalWrite(ledPins[person][1], HIGH); // Heterozigot (Aa) - sarı
    } else if (dominantCount == 0) {
        digitalWrite(ledPins[person][2], HIGH); // Recesif (aa) - kırmızı
    }
}
