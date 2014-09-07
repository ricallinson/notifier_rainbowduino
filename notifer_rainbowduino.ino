//    (The MIT License)
//
//    Copyright (c) 2012 Richard S Allinson <rsa@mountainmansoftware.com>
//
//    Permission is hereby granted, free of charge, to any person obtaining
//    a copy of this software and associated documentation files (the
//    "Software"), to deal in the Software without restriction, including
//    without limitation the rights to use, copy, modify, merge, publish,
//    distribute, sublicense, and/or sell copies of the Software, and to
//    permit persons to whom the Software is furnished to do so, subject to
//    the following conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*
    Words.
*/

#include <Rainbowduino.h>

const int EMPTY = 0;
const int START = 7;
const int END = -6;

uint32_t color = (uint32_t)0x00FF00; // RRGGBB

char *WORDS[5] = {
    "one",
    "two",
    "three",
    "four",
    "five"
};

int queue[256] = {};
int lastAdded = 0;
int lastRead = 0;

/*
    Words.
*/

int buffer[2][2] = {
    {EMPTY, START}, // char, pos
    {EMPTY, START}
};

/*
    Set the pins to use for output.
*/

void pins() {
    Rb.init();
}

/*
    Words.
*/

void render() {
    
    if (buffer[0][0] == EMPTY) {
        buffer[0][0] = getNextFromQueue();
    } else if (buffer[1][0] == EMPTY && buffer[0][1] == 0) {
        buffer[1][0] = getNextFromQueue();
    }

    if (buffer[0][1] == END) {
        buffer[0][0] = EMPTY;
        buffer[0][1] = START;
    }

    if (buffer[0][0] > EMPTY) {
        Rb.drawChar(buffer[0][0], buffer[0][1], 0, color);
        buffer[0][1]--;
    }

    if (buffer[1][1] == END) {
        buffer[1][0] = EMPTY;
        buffer[1][1] = START;
    }

    if (buffer[1][0] > EMPTY) {
        Rb.drawChar(buffer[1][0], buffer[1][1], 0, color);
        buffer[1][1]--;
    }

    delay(160);

    Rb.blankDisplay();
}

/*
    Standard setup function.
*/

void setup() {
    pins();
    Serial.begin(9600);
    addStringToQueue("Hello");
}

int getNextFromQueue() {
    if (lastAdded == lastRead) {
        return 0;
    }
    lastRead++;
    if (lastRead >= 256) {
        lastRead = 0;
    }
    return queue[lastRead];
}

void addCharToQueue(char c) {
    lastAdded++;
    if (lastAdded >= 256) {
        lastAdded = 0;
    }
    queue[lastAdded] = c;
}

void addStringToQueue(char *string) {
    for(int i = 0; i < strlen(string); i++) {
        addCharToQueue(string[i]);
    }
}

/*
    Standard loop function.
*/

void loop() {
    if (Serial.available() > 0) {
        addCharToQueue(Serial.read());
    }
    if (false) {
        int r = rand() % 4;
        addStringToQueue(WORDS[r]);
    }
    render();
}
