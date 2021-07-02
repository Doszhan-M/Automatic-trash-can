#include <Servo.h>
Servo myServo;   
    
    #define time_open 7000             // время открытой крышки 6 секунд
    #define Hmax      55               //высота срабатывания датчика
    
    #define PIN_GND      3             // пин земля датчика
    #define PIN_VCC      7             // пин питания датчика
    #define PIN_TRIG     6             // пин сигнала датчика                   
    #define PIN_ECHO     5             // пин сигнала датчика
    #define MOSFETpin    9             // пин сигнала мосфет
    long duration, cm;                 // переменные
    boolean open_flag;                 // переменные
    int pos = 0;  
    
    void setup() {
      
      Serial.begin (9600);               // Инициализируем взаимодействие по последовательному порту
      
      //Определяем вводы и выводы
      pinMode(PIN_GND, OUTPUT);
      digitalWrite(PIN_GND, LOW);
      pinMode(PIN_VCC, OUTPUT);
      digitalWrite(PIN_VCC, HIGH);
      pinMode(PIN_TRIG, OUTPUT);
      pinMode(PIN_ECHO, INPUT);
      pinMode(MOSFETpin, OUTPUT);         // пин мосфета в режиме выхода
      digitalWrite(MOSFETpin, LOW);
      delay (1000);                       //задержка для стабильности
      
      myServo.attach (A2);                 // пин сигнала серво
      myServo.write(72);
    }
    
  void open_cap()                     	   // функция открытия крышки
  {
   if (!open_flag) {
    digitalWrite(MOSFETpin, HIGH);         // подаем питание сервомотору
    delay (50);
    for (pos = 72; pos >= 15; pos -= 1)    // поворот с 80 град на 70 град со скоростью 40
    { 
    myServo.write(pos);              
    delay(15);                       
    }
    

  open_flag = 1;                         // флаг что крышка открыта
  }
 }
   
  void close_cap()                       // функция закрытия крышки
  {
    for (pos = 15; pos <= 72; pos += 1)  // поворот с 0 град на 80 град со скоростью 40
   { 
    myServo.write(pos);              
    delay(30);                       
   }

  digitalWrite(MOSFETpin, LOW);         // отключаем питание сервомотора 
  open_flag = 0;                        // флаг что крышка закрыта
  }

    
  void loop() 
  {

  // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(PIN_ECHO, HIGH);
  // Теперь осталось преобразовать время в расстояние
  cm = (duration / 2) / 29.1;
  Serial.print("Расстояние до объекта: ");
  Serial.print(cm);
  Serial.println(" см.");
         

  if (cm>1 && cm<Hmax)             				//если рука от 1 до Hmax см и если крышка закрыта,
   {
    open_cap();                                 // открыть крышку
    delay(time_open);                           // задержка для открытой крышки
   }
  else 
   {
    if (open_flag)                             // если крышка открыта   
    {                      
    close_cap();                               // закрыть крышку
    }
   }
  }
  
    
    
