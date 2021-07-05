
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


#define SERVOMIN  80 // this is the 'minimum' pulse length count (out of 4096) (based on futaba s3003)
#define SERVOMAX  500 // this is the 'maximum' pulse length count (out of 4096) (based on futaba s3003)
#define SERVO_FREQ 60
bool started = false;
int max_servoes = 15;
int enable = 3;

void reset_pos()
{
  int deg1 = 90;
  int deg2 = 90;
  pwm.setPWM(0, 0, ang_to_pw(deg1));
  pwm.setPWM(1, 0, ang_to_pw(deg2));
  pwm.setPWM(2, 0, ang_to_pw(deg1));
  pwm.setPWM(3, 0, ang_to_pw(deg2));
  pwm.setPWM(4, 0, ang_to_pw(deg1));
  pwm.setPWM(5, 0, ang_to_pw(deg2));
  pwm.setPWM(6, 0, ang_to_pw(deg1));
  pwm.setPWM(7, 0, ang_to_pw(deg2));
  pwm.setPWM(8, 0, ang_to_pw(deg1));
  pwm.setPWM(9, 0, ang_to_pw(deg2));
  pwm.setPWM(10, 0, ang_to_pw(deg1));
  pwm.setPWM(11, 0, ang_to_pw(deg2));
  pwm.setPWM(12, 0, ang_to_pw(deg1));
  pwm.setPWM(13, 0, ang_to_pw(deg2));
  pwm.setPWM(14, 0, ang_to_pw(deg1));
  pwm.setPWM(15, 0, ang_to_pw(deg2));
}



void setup() {
  pinMode(enable, OUTPUT); // Enable/disable pin
  Serial.begin(9600);
  while (!Serial);
  pwm.begin();
  digitalWrite(enable, 0); // Enable the device
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~60 Hz updates
  delay(10);
  reset_pos();
  delay(800);
  digitalWrite(enable, 1); // Disable the servo
}

int ang_to_pw(int a)
{
  a = (a < 0) ? 0 : ((a > 180) ? 180 : a);
  return map(a, 0, 180, SERVOMIN, SERVOMAX);
}

void str_to_pos(String str)
{
  int pos = 0;
  pos = str.indexOf('#');
  while ( pos != -1)
  {
    String tmp;
    tmp = str.substring(pos + 1, str.indexOf('#', pos + 1));
    String ser = tmp.substring(0, tmp.indexOf('^'));
    int ser_num = ser.toInt();
    String deg = tmp.substring(tmp.indexOf('^') + 1);
    int deg_num = deg.toInt();
    if (ser_num > max_servoes)
    {
      str = str.substring(pos + 1);
      pos = str.indexOf('#');
      continue;
    }
    pwm.setPWM(ser_num, 0, ang_to_pw(deg_num));
    str = str.substring(pos + 1);
    pos = str.indexOf('#');
  }
}


void loop() {

  if (Serial.available() > 0) {
    String str = Serial.readStringUntil('@');
    if (str.equals("stop"))
    {
      digitalWrite(enable, 1);
      Serial.println("servo_controller stopped");
      started = false;
      delay(10);
    }
    if (str.equals("start"))
    {
      digitalWrite(enable, 0);
      Serial.println("servo_controller started");
      started = true;
      delay(10);
    }
    if (str.equals("reset"))
    {
      digitalWrite(enable, 0);
      reset_pos();
      Serial.println("servo_controller reseted");
      started = true;
      delay(10);
    }
    if (started)
    {
      str_to_pos(str);
    }
  }
}
