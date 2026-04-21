// Genetic Crossover Simulator - Arduino Uno

struct Individual {
    char gene1;
    char gene2;
};

Individual mother = {'A', 'a'};
Individual father = {'A', 'a'};
Individual child1;
Individual child2;

const int ledPins[4][3] = {
    {5, 6, 7},
    {8, 9, 10},
    {11, 12, 13},
    {14, 15, 16}
};

const int buttonPin = 2;
unsigned long lastPress = 0;

void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(A5));
    pinMode(buttonPin, INPUT_PULLUP);

    for (int person = 0; person < 4; person++) {
        for (int color = 0; color < 3; color++) {
            pinMode(ledPins[person][color], OUTPUT);
            digitalWrite(ledPins[person][color], LOW);
        }
    }

    Serial.println("Genetic Simulator Ready");
    Serial.println("Press button to start crossing");

    displayGenotype(0, mother);
    displayGenotype(1, father);

    for (int color = 0; color < 3; color++) {
        digitalWrite(ledPins[2][color], LOW);
        digitalWrite(ledPins[3][color], LOW);
    }
}

void loop() {
    if (digitalRead(buttonPin) == LOW && millis() - lastPress > 200) {
        lastPress = millis();

        calculateAndDisplayChildren();

        Serial.println("Crossing completed!");
        Serial.print("Child 1: ");
       Serial.print(child1.gene1);
        Serial.println(child1.gene2);
        Serial.print("Child 2: ");
        Serial.print(child2.gene1);
        Serial.println(child2.gene2);
    }
}

void calculateAndDisplayChildren() {
    geneticCalculate(&child1);
    displayGenotype(2, child1);
    geneticCalculate(&child2);
    displayGenotype(3, child2);
}

void geneticCalculate(Individual* child) {
    child->gene1 = random(2) ? mother.gene1 : mother.gene2;
    child->gene2 = random(2) ? father.gene1 : father.gene2;
}

void displayGenotype(int person, Individual individual) {
    for (int color = 0; color < 3; color++) {
        digitalWrite(ledPins[person][color], LOW);
    }

    int dominantCount = 0;
    if (individual.gene1 == 'A') dominantCount++;
    if (individual.gene2 == 'A') dominantCount++;

    if (dominantCount == 2) {
        digitalWrite(ledPins[person][0], HIGH);
    } else if (dominantCount == 1) {
        digitalWrite(ledPins[person][1], HIGH);
    } else if (dominantCount == 0) {
        digitalWrite(ledPins[person][2], HIGH);
    }
}