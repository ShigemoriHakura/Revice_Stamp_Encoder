#include <IRremote.h>
#include <FlexiTimer2.h>

#define IR_SEND_PIN 3

int lastStatus = 0;
int isSending = 0;

long lastMS = 0L;
long lastLength = 0L;

String comdata = "";

void setup() {
  Serial.begin(115200);

  Serial.println("Checking Timer");
  FlexiTimer2::set(1, 1.0 / 1000, processPulse);
  FlexiTimer2::start();

  Serial.println("Started");
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);
}

int ATimer = 300;
int ATimerNow = 0;


void processPulse() {
  FlexiTimer2::stop();
  ATimerNow++;
  if (isSending == 0) {
    if (lastStatus == 2 || lastStatus == 5) {
      unsigned long us = micros();
      //Check if meets lastLength
      if (long(lastMS + lastLength * 1000 - us) < 0) {
        if (ATimerNow <= 200) {
          //lastMS += 100L;
          //IrSender.space(400);
          //Serial.println("To Next Turn");
        } else {
          Serial.print("Loop Sound ");
          switch (lastStatus) {
            case 2:
              sendUnwaitCode(164);
              Serial.println("2");
              break;
            case 5:
              sendUnwaitCode(167);
              Serial.println("5");
              break;
          }
          lastMS = micros();
          Serial.println("Clean ATimerNow");
          ATimerNow = 0;
        }
      }
    }
    if (ATimerNow >= ATimer) {
      ATimerNow = 0;
      //Serial.println("Sent Pulse");
      if (isSending == 0) {
        sendUnwaitCode(150);
      }
    }
  }
  FlexiTimer2::start();
}

void delayRemoveSending() {
  delay(10);
  isSending = 0;
  ATimerNow = 0;
}

void loop() {
  comdata = "";
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
    delay(2);
  }

  if (comdata !=  "") {
    //Serial.println(comdata);
    if (comdata == "0\n" || comdata == "0\r\n") {
      Serial.println("Set Status 0");
      isSending = 1;
      lastStatus = 0;
      sendCode(161);
      delayRemoveSending();
    }
    if (comdata == "1\n" || comdata == "1\r\n") {
      Serial.println("Set Status 1");
      isSending = 1;
      lastStatus = 1;
      sendCode(162);
      delayRemoveSending();
    }
    if (comdata == "2\n" || comdata == "2\r\n") {
      Serial.println("Set Status 2");
      isSending = 1;
      lastStatus = 2;
      sendCode(164);
      lastMS = micros();
      lastLength = 2700;
      delayRemoveSending();
    }
    if (comdata == "3\n" || comdata == "3\r\n") {
      Serial.println("Set Status 3");
      isSending = 1;
      lastStatus = 3;
      sendCode(165);
      delayRemoveSending();
    }
    if (comdata == "4\n" || comdata == "4\r\n") {
      Serial.println("Set Status 4");
      isSending = 1;
      lastStatus = 4;
      sendCode(166);
      delayRemoveSending();
    }
    if (comdata == "5\n" || comdata == "5\r\n") {
      Serial.println("Set Status 5");
      isSending = 1;
      lastStatus = 5;
      sendCode(167);
      lastMS = micros();
      lastLength = 3100;
      delayRemoveSending();
    }
    if (comdata == "6\n" || comdata == "6\r\n") {
      Serial.println("Set Status 6");
      isSending = 1;
      lastStatus = 6;
      sendCode(168);
      delayRemoveSending();
    }
    if (comdata == "7\n" || comdata == "7\r\n") {
      Serial.println("Set Status 7");
      isSending = 1;
      lastStatus = 7;
      sendCode(169);
      delayRemoveSending();
    }
    if (comdata == "8\n" || comdata == "8\r\n") {
      Serial.println("Set Status 8");
      isSending = 1;
      lastStatus = 8;
      sendCode(170);
      delayRemoveSending();
    }
    if (comdata == "9\n" || comdata == "9\r\n") {
      Serial.println("Set Status 9");
      isSending = 1;
      lastStatus = 9;
      sendCode(160);
      delayRemoveSending();
    }

    if (comdata == "A1\n" || comdata == "A1\r\n") {
      Serial.println("Set Status A1 - Press and wait");
      isSending = 1;
      lastStatus = 0;
      sendCode(161);
      delayRemoveSending();
      Serial.println("1");
      delay(2500);

      isSending = 1;
      sendCode(162);
      delayRemoveSending();
      Serial.println("2");
      delay(1400);

      isSending = 1;
      lastStatus = 2;
      Serial.println("3");
      sendCode(164);
      lastMS = micros();
      lastLength = 2700;
      delayRemoveSending();
    }

    if (comdata == "A2\n" || comdata == "A2\r\n") {
      Serial.println("Set Status A2 - Henshin");
      isSending = 1;
      lastStatus = 3;
      sendCode(165);
      delayRemoveSending();
      Serial.println("1");
      delay(2500);

      isSending = 1;
      sendCode(166);
      delayRemoveSending();
      Serial.println("2");
    }

    if (comdata == "A3\n" || comdata == "A3\r\n") {
      Serial.println("Set Status A3 - Hissatsu");
      isSending = 1;
      lastStatus = 5;
      sendCode(167);
      lastMS = micros();
      lastLength = 3100;
      delayRemoveSending();
    }

    if (comdata == "A4\n" || comdata == "A4\r\n") {
      Serial.println("Set Status A4 - Hissatsu Finish");
      isSending = 1;
      lastStatus = 0;
      sendCode(168);
      delayRemoveSending();
      Serial.println("1");
      delay(1200);

      isSending = 1;
      sendCode(169);
      delayRemoveSending();
      Serial.println("2");
      delay(2000);

      isSending = 1;
      sendCode(170);
      delayRemoveSending();
      Serial.println("3");
    }
  }
}

void sendUnwaitCode(int code) {
  int ccode[30] = {};
  getCode(code, ccode);
  IrSender.sendRaw(ccode, 20, 38);
}

void sendCode(int code) {
  //Check if ATimerNow is above 200.
  //Important! Remove this will cause the IR commands "remix".
  if (ATimerNow <= 200) {
    //Serial.println("Delay");
    delay(200);
  }
  sendUnwaitCode(code);
}

void convertDecToBin(int Dec, boolean Bin[]) {
  for (int i = 7 ; i >= 0 ; i--) {
    if (pow(2, i) <= Dec) {
      Dec = Dec - pow(2, i);
      Bin[8 - (i + 1)] = 1;
    } else {
    }
  }
}

void convertToByteCode(boolean Bin[], unsigned int code[]) {
  int ii = 2;
  for (int j = 0; j < 8; j++) {
    if (Bin[j] == 0) {
      code[ii] = 1500;
      ii ++;
      code[ii] = 500;
      ii ++;
    } else {
      code[ii] = 500;
      ii ++;
      code[ii] = 1500;
      ii ++;
    }
  }
  code[ii] = 1500;
}

void getCode(int i, int ccode[]) {
  boolean cBin[] = {0, 0, 0, 0, 0, 0, 0, 0};
  ccode[0] = 6000;
  ccode[1] = 500;
  convertDecToBin(i, cBin);
  convertToByteCode(cBin, ccode);
  return ccode;
}
