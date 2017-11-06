#include <Bridge.h>
#include <Console.h>
#include <Process.h>
#include <Mouse.h>
#include <Wire.h>
Process date;                 // process used to get the date
long linuxBaud = 250000;

uint32_t timer_s;
uint32_t timer_e;
const int TURN=20; const int POINT=19;const int TURN_END=100;
const int RED_PIN = 11;
const int GREEN_PIN = 10;
const int BLUE_PIN = 9;
const int Point_T=0;const int Point_X=1;const int Point_Y=2;
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
const int BUTTON_TIME = 5;
const int BUTTON_WORD = 6;

int arr[3][100];
int count = 10;
int SavedX = 0;
int SavedY = 0;
int16_t SavedY2 = 0;
int i = 0;
int j = 0;
int fire = 0; // related BUTTON_TIME
int htd = 0; // related BUTTON_WORD
int indexCount = 0;
int dIndexCount = 0;
String timeString;
void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  pinMode(BUTTON_TIME, INPUT);
  pinMode(BUTTON_WORD, INPUT);
  Serial.begin(9600);
  Mouse.begin();
  Bridge.begin(); // Initialize the Bridge
  while (!Serial);              // wait for Serial Monitor to open
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);

  if (digitalRead(BUTTON_TIME) == HIGH)
  {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);

    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
    AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    int16_t gyroX, gyroZ, kalX, kalY;
    int Sensitivity = 400;
    gyroX = GyX / Sensitivity / 1.1 * -1;
    gyroZ = GyZ / Sensitivity * -1;

    Mouse.move(gyroZ, gyroX);

    SavedX = SavedX + gyroZ;
    SavedY = SavedY + gyroX;
    SavedY2 = -SavedY;

    i++; //Print process
    if (i % TURN == POINT)
    {
      i = 0;

      if (j == 0) {
        timer_s = micros(); // Time of Start
      }
      timer_e = micros(); // Time of End

      // Add value into arrays
      arr[POINT_T][j] = (timer_e - timer_s) / 1000;
      arr[POINT_X][j] = SavedX;
      arr[POINT_Y][j] = SavedY2;

      j++;
      if (j == TURN_END)
      {
        j = 0;
      }
    }

    fire = 1;
    htd = 1;
  }

  else if (digitalRead(BUTTON_WORD) == HIGH)
  {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);

    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
    AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    int16_t gyroX, gyroZ, kalX, kalY;
    int Sensitivity = 400;
    gyroX = GyX / Sensitivity / 1.1 * -1;
    gyroZ = GyZ / Sensitivity * -1;

    Mouse.move(gyroZ, gyroX);

    SavedX = SavedX + gyroZ;
    SavedY = SavedY + gyroX;
    SavedY2 = -SavedY;

    i++; //Print process
    if (i % TURN == POINT)
    {
      i = 0;

      if (j == 0) {
        timer_s = micros(); // Time of Start
      }
      timer_e = micros(); // Time of End

      // Add value into arrays
      arr[POINT_T][j] = (timer_e - timer_s) / 1000;
      arr[POINT_X][j] = SavedX;
      arr[POINT_Y][j] = SavedY2;

      j++;
      if (j == TURN_END)
      {
        j = 0;
      }
    }

    fire = 1;
  }

  else if (fire == 1) {
    if ( fire == 1)
    {
      if (htd == 1)
      {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, HIGH);
        date.begin("/bin/date");    //  -> 다른 버튼 누를시 date 변경가능하게 함수 생성 , indexCount 0으로 초기화
        date.addParameter("+%Y%m%d%H%M%S");
        date.run();
        
        while (date.available() > 0) {
          timeString = date.readString();
          timeString.trim();
          Process p;
          p.runShellCommand("curl -k -X PUT -d '{ \"index\":\"" + String(timeString) + "\"}' 'https://smartpen-89fd5.firebaseio.com/dateIndex/" + dIndexCount + ".json'");

          htd = 0;
          dIndexCount++;
        }

      }

      Serial.println("Checking values");
      Serial.println("Transmission...ing");
      Serial.println(date.available());
      Serial.println("X'\t'Y");
      for (int k = 0; k < TURN_END; k++)
      {
        Serial.print(arr[POINT_X][k]);
        Serial.print('\t');
        Serial.println(arr[POINT_Y][k]);


      }
      uint32_t _time = micros();


      for (int idx = 0; idx < j; idx++) {

        Process p;

        p.runShellCommand("curl -k -X PUT -d '{ \"time\": " + String(arr[POINT_T][idx]) + ", \"x\": " + String(arr[POINT_X][idx]) + ", \"y\": " + String(arr[POINT_Y][idx]) + "}' 'https://smartpen-89fd5.firebaseio.com/dates/" + timeString + "/words/" + String(_time) + "/" + idx + ".json'");
        p.runShellCommand("curl -k -X PUT -d '{ \"index\": \"" + String(_time) + "\"}' 'https://smartpen-89fd5.firebaseio.com/dates/" + timeString + "/wordsIndex/" + indexCount + ".json'");


        while (p.running());
      }

      indexCount++;
      fire = 0;
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
      Serial.println("...END");
    }

    else {

      SavedX = 0; // Button  : LOW -> Reset!
      SavedY = 0;

      j = 0;
      for (int x = 0; x <= 1; x++) {
        for (int y = 0; y < TURN_END ; y++)
        {
          arr[x][y] = 0;
        }
      }
    }
  }
}
