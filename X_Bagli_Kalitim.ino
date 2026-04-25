// X-Bagli Kalitim Simulator

// Pin definitions
const int buttonPin = 2;

// Mother LEDs
const int motherGreen = 3;  // Saglikli (XA XA)
const int motherYellow = 4; // Tasiyici (XA Xa)
const int motherRed = 5;    // Hasta (Xa Xa)

// Father LEDs
const int fatherGreen = 6;  // Saglikli (XA Y)
const int fatherRed = 7;    // Hasta (Xa Y)

// Child 1 LEDs
const int child1Green = 8;
const int child1Yellow = 9;
const int child1Red = 10;
const int child1Blue = 11;  // Erkek
const int child1Pink = 12;  // Kiz

// Child 2 LEDs
const int child2Green = 13;
const int child2Yellow = 14;
const int child2Red = 15;
const int child2Blue = 16;  // Erkek
const int child2Pink = 17;  // Kiz

// Struct for individuals
struct Individual {
    char gene1; // 'A' or 'a'
    char gene2; // 'A', 'a', or 'Y'
    bool isMale;
};

Individual mother, father, child1, child2;

// Button debounce variables
unsigned long lastPressTime = 0;
const unsigned long debounceDelay = 200;

void setup() {
    // Initialize serial
    Serial.begin(9600);

    // Set button pin
    pinMode(buttonPin, INPUT_PULLUP);

    // Set all LED pins as output
    pinMode(motherGreen, OUTPUT);
    pinMode(motherYellow, OUTPUT);
    pinMode(motherRed, OUTPUT);
    pinMode(fatherGreen, OUTPUT);
    pinMode(fatherRed, OUTPUT);
    pinMode(child1Green, OUTPUT);
    pinMode(child1Yellow, OUTPUT);
    pinMode(child1Red, OUTPUT);
    pinMode(child1Blue, OUTPUT);
    pinMode(child1Pink, OUTPUT);
    pinMode(child2Green, OUTPUT);
    pinMode(child2Yellow, OUTPUT);
    pinMode(child2Red, OUTPUT);
    pinMode(child2Blue, OUTPUT);
    pinMode(child2Pink, OUTPUT);

    // Initial simulation
    simulateGenetics();
    updateLEDs();
    printResults();
}

void loop() {
    if (digitalRead(buttonPin) == LOW) {
        unsigned long currentTime = millis();
        if (currentTime - lastPressTime > debounceDelay) {
            lastPressTime = currentTime;
            simulateGenetics();
            updateLEDs();
            printResults();
        }
    }
}

void simulateGenetics() {
    // Mother: two random genes
    mother.gene1 = random(2) ? 'A' : 'a';
    mother.gene2 = random(2) ? 'A' : 'a';
    mother.isMale = false;

    // Father: one random X, Y
    father.gene1 = random(2) ? 'A' : 'a';
    father.gene2 = 'Y';
    father.isMale = true;

    // Child 1
    child1.isMale = random(2); // 0 female, 1 male
    if (child1.isMale) {
        // Boy: X from mother random, Y from father
        int randMother = random(2);
        child1.gene1 = (randMother == 0) ? mother.gene1 : mother.gene2;
        child1.gene2 = 'Y';
    } else {
        // Girl: one X from mother random, one from father
        int randMother = random(2);
        child1.gene1 = (randMother == 0) ? mother.gene1 : mother.gene2;
        child1.gene2 = father.gene1; // father's X
    }

    // Child 2
    child2.isMale = random(2);
    if (child2.isMale) {
        int randMother = random(2);
        child2.gene1 = (randMother == 0) ? mother.gene1 : mother.gene2;
        child2.gene2 = 'Y';
    } else {
        int randMother = random(2);
        child2.gene1 = (randMother == 0) ? mother.gene1 : mother.gene2;
        child2.gene2 = father.gene1;
    }
}

void updateLEDs() {
    // Mother
    if (mother.gene1 == 'A' && mother.gene2 == 'A') {
        digitalWrite(motherGreen, HIGH);
        digitalWrite(motherYellow, LOW);
        digitalWrite(motherRed, LOW);
    } else if (mother.gene1 == 'a' && mother.gene2 == 'a') {
        digitalWrite(motherGreen, LOW);
        digitalWrite(motherYellow, LOW);
        digitalWrite(motherRed, HIGH);
    } else {
        digitalWrite(motherGreen, LOW);
        digitalWrite(motherYellow, HIGH);
        digitalWrite(motherRed, LOW);
    }

    // Father
    if (father.gene1 == 'A') {
        digitalWrite(fatherGreen, HIGH);
        digitalWrite(fatherRed, LOW);
    } else {
        digitalWrite(fatherGreen, LOW);
        digitalWrite(fatherRed, HIGH);
    }

    // Child1
    if (child1.isMale) {
        digitalWrite(child1Blue, HIGH);
        digitalWrite(child1Pink, LOW);
        // Phenotype
        if (child1.gene1 == 'A') {
            digitalWrite(child1Green, HIGH);
            digitalWrite(child1Yellow, LOW);
            digitalWrite(child1Red, LOW);
        } else {
            digitalWrite(child1Green, LOW);
            digitalWrite(child1Yellow, LOW);
            digitalWrite(child1Red, HIGH);
        }
    } else {
        digitalWrite(child1Blue, LOW);
        digitalWrite(child1Pink, HIGH);
        if (child1.gene1 == 'A' && child1.gene2 == 'A') {
            digitalWrite(child1Green, HIGH);
            digitalWrite(child1Yellow, LOW);
            digitalWrite(child1Red, LOW);
        } else if (child1.gene1 == 'a' && child1.gene2 == 'a') {
            digitalWrite(child1Green, LOW);
            digitalWrite(child1Yellow, LOW);
            digitalWrite(child1Red, HIGH);
        } else {
            digitalWrite(child1Green, LOW);
            digitalWrite(child1Yellow, HIGH);
            digitalWrite(child1Red, LOW);
        }
    }

    // Child2
    if (child2.isMale) {
        digitalWrite(child2Blue, HIGH);
        digitalWrite(child2Pink, LOW);
        if (child2.gene1 == 'A') {
            digitalWrite(child2Green, HIGH);
            digitalWrite(child2Yellow, LOW);
            digitalWrite(child2Red, LOW);
        } else {
            digitalWrite(child2Green, LOW);
            digitalWrite(child2Yellow, LOW);
            digitalWrite(child2Red, HIGH);
        }
    } else {
        digitalWrite(child2Blue, LOW);
        digitalWrite(child2Pink, HIGH);
        if (child2.gene1 == 'A' && child2.gene2 == 'A') {
            digitalWrite(child2Green, HIGH);
            digitalWrite(child2Yellow, LOW);
            digitalWrite(child2Red, LOW);
        } else if (child2.gene1 == 'a' && child2.gene2 == 'a') {
            digitalWrite(child2Green, LOW);
            digitalWrite(child2Yellow, LOW);
            digitalWrite(child2Red, HIGH);
        } else {
            digitalWrite(child2Green, LOW);
            digitalWrite(child2Yellow, HIGH);
            digitalWrite(child2Red, LOW);
        }
    }
}

void printResults() {
    Serial.print("Anne: Kiz - ");
    if (mother.gene1 == 'A' && mother.gene2 == 'A') Serial.println("Saglikli (XA XA)");
    else if (mother.gene1 == 'a' && mother.gene2 == 'a') Serial.println("Hasta (Xa Xa)");
    else Serial.println("Tasiyici (XA Xa)");

    Serial.print("Baba: Erkek - ");
    if (father.gene1 == 'A') Serial.println("Saglikli (XA Y)");
    else Serial.println("Hasta (Xa Y)");

    Serial.print("Cocuk 1: ");
    if (child1.isMale) {
        Serial.print("Erkek - ");
        if (child1.gene1 == 'A') Serial.println("Saglikli (XA Y)");
        else Serial.println("Hasta (Xa Y)");
    } else {
        Serial.print("Kiz - ");
        if (child1.gene1 == 'A' && child1.gene2 == 'A') Serial.println("Saglikli (XA XA)");
        else if (child1.gene1 == 'a' && child1.gene2 == 'a') Serial.println("Hasta (Xa Xa)");
        else Serial.println("Tasiyici (XA Xa)");
    }

    Serial.print("Cocuk 2: ");
    if (child2.isMale) {
        Serial.print("Erkek - ");
        if (child2.gene1 == 'A') Serial.println("Saglikli (XA Y)");
        else Serial.println("Hasta (Xa Y)");
    } else {
        Serial.print("Kiz - ");
        if (child2.gene1 == 'A' && child2.gene2 == 'A') Serial.println("Saglikli (XA XA)");
        else if (child2.gene1 == 'a' && child2.gene2 == 'a') Serial.println("Hasta (Xa Xa)");
        else Serial.println("Tasiyici (XA Xa)");
    }

    Serial.println();
}