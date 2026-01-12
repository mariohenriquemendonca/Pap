#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9 

#define LED_G 5       // LED verde
#define LED_R 4       // LED vermelho
#define BUZZER 2      // Buzzer

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myservo;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  myservo.attach(3);   // Pino do servo
  myservo.write(0);    // Posição inicial

  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  noTone(BUZZER);

  Serial.println("Put your card to the reader ...");
  Serial.println();
}

void loop()
{
  // Verifica se há um novo cartão
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  // Lê o cartão
  if (!mfrc522.PICC_ReadCardSerial())
    return;

  // Mostrar UID
  Serial.print("UID tag:");
  String content = "";

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);

    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  content.toUpperCase();

  //  UID autorizado (SUBSTITUI PELO TEU)
  if (content.substring(1) == "DF F7 BA DE")
  {
    Serial.println("Authorized access");

    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);

    myservo.write(180);   // Abre cancela
    delay(5000);
    myservo.write(0);     // Fecha cancela

    digitalWrite(LED_G, LOW);
  }
  else
  {
    Serial.println("Access denied");

    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 200);
    delay(1000);
    noTone(BUZZER);
    digitalWrite(LED_R, LOW);   //  LED vermelho desliga após erro
  }

  // Finaliza comunicação com o cartão
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
