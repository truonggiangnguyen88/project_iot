#include "global.h"

DHT dht(DHTPIN, DHTTYPE);
hw_timer_t *timer = NULL;  // Khởi tạo con trỏ cho timer
Servo servo;
void IRAM_ATTR onTimer() {
  // Hàm này sẽ được gọi mỗi khi timer ngắt
  //Serial.println("timer");
  SCH_Update();
  toggleLED();
}

//------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------
void setup() {
//---------------------------------SETUP PIN-----------------------------------//

  pinMode(DHTPIN, INPUT);
  pinMode(LIGHTPIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(FIRE_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT); 
  pinMode(FAN_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  //---------------------------------INIT FUNC----------------------------------//
  Serial.begin(9600);
  Wire.begin();
  lcd_init();
  dht.begin();         // Khởi động cảm biến DHT11
  servo.attach(MOTOR_PIN); // Gắn servo vào chân số 9
  servo.write(90); // Điều khiển servo quay đến góc 90 độ
  Serial.println("HELLO");



  //---------------------------------SCHEDULER-----------------------------------//
  SCH_Init();
    //SCH_Add_Task(toggleLED, 0, 1);
  SCH_Add_Task(sendDHT_temp, 0, 30);
  SCH_Add_Task(sendDHT_humi, 10, 30);
  SCH_Add_Task(send_light, 20, 30);
  SCH_Add_Task(fsm_lcd, 0, 1);
  SCH_Add_Task(device_control, 3, 1);

  
// ------------------------------------ INIT TIMER---------------------------------//
  timer = timerBegin(0, 80, true);
  // Gắn hàm onTimer cho timer interrupt
  timerAttachInterrupt(timer, &onTimer, true);
  // Đặt tần số ngắt 1 giây (1.000.000 micro giây)
  timerAlarmWrite(timer, 1000000, true);  // Ngắt mỗi 1 giây
  // Bật ngắt cho timer
  timerAlarmEnable(timer);
//-------------------------------------END TIMER---------------------------------//

}

void loop() {  

   SCH_Dispatch_Tasks();
    read_serial_data();
   key_is_press();
   fsm_password();


  //digitalWrite(LED_PIN, 0);
 // read_serial_data();
  //run_button();
  //readSensorValue(inputLight);
 // Chờ 2 giây trước khi đọc và gửi dữ liệu tiếp

  // // Kiểm tra xem timerFlag có được thiết lập từ ngắt không
  // if (timerFlag) {
  //   timerFlag = false;  // Reset cờ
  //   digitalWrite(LED, !digitalRead(LED));
  //   Serial.println("Timer Interrupt Triggered!");
  // }

  
  // fsm_password();
  // key_is_press();
  

}

/*bufer còi , quạt, đèn, morto
hàm ghi buffer
i2c lcd
bàn phims so
ham check gas, cháy->buffer
button bật đèn, bật quạt


*/