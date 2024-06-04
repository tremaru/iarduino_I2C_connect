//   ПРИМЕР ДЛЯ МАСТЕРА (ПРОГРАММНАЯ ШИНА I2C):                               // * Строки со звёздочкой являются необязательными.
//   Организация передачи данных через регистры ведомого:                     //
//   из регистра 0   читается   значение типа byte.                           //
//   в  регистр  1   записываем значение типа byte.                           //
//   в  регистры 2:3 записываем значение типа int16_t.                        //
//   в  регистры 4:5 записываем что угодно, ведомый игнорирует эту запись.    // * см. пример ведомого устройства "exchangeMulti/Slave".
//   в  регистры 6:9 записываем значение типа float.                          //
//   из регистров 10:11 читаем значение типа uint16_t.                        //
//   Требуется установить библиотеку <iarduino_I2C_Software.h>                //   https://iarduino.ru/file/627.html
                                                                              //
//   ВАЖНО: Установите подтягивающие резисторы на линии SCL и SDA!            //   Внутрисхемная подтяжка микроконтроллера осуществляется высокоомным сопротивлением, не гарантирующим устойчивость от помех.
                                                                              //
#include <iarduino_I2C_Software.h>                                            //   Подключаем библиотеку для работы с программной шиной I2C, до подключения библиотеки iarduino_I2C_connect.
SoftTwoWire sWire(3,4);                                                       //   Создаём объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
                                                                              //
#include <iarduino_I2C_connect.h>                                             //   Подключаем библиотеку для соединения Arduino по шине I2C.
iarduino_I2C_connect obj;                                                     //   Создаём объект obj для работы c методами библиотеки iarduino_I2C_connect.
                                                                              //
byte     a=0;     // Значение читаемое из регистра 0.                         //
byte     b=0;     // Значение ранее прочитанное из регистра 0.                //
byte     c=0;     // Значение записываемое в регистр  1.                      //
int16_t  d=12345; // Значение записываемое в регистры 2 и 3.                  //
float    e;       // Значение записываемое в регистры 6,7,8,9.                //
uint16_t f;       // Значение читаемое из регистров 10 и 11.                  //
                                                                              //
void setup(){                                                                 //
     sWire.begin();                                                           //   Инициируем подключение к шине I2C в качестве ведущего (master) устройства.
     obj.begin(&sWire);                                                       //   Указываем мастеру объект для работы с шиной I2C (по умолчанию &Wire).
}                                                                             //
                                                                              //
void loop(){                                                                  //
//   Читаем из регистрa 0 один байт данных в переменную a:                    //
     a = obj.readByte(0x09, 0);                                               //   Читаем из ведомого с адресом 0x09 из его 0 регистра один байт в перемую a.
//   Задержка:                                                                //
     delay(100);                                                              // * Выполняем задержку, чтоб не слишком часто читать значения регистров ведомого.
//   Выполняем дальнейшие действия если изменилось значение a:                //
     if( b != a ){                                                            //
         b  = a;                                                              //
         c ++;                                                                // * Увеличиваем значение.
         d *= -1;                                                             // * Меняем знак числа d.
         e  = (float) millis() / 1000.0f;                                     // * Получаем значение float, как количество секунд прошедших с момента старта скетча.
     //  Записываем в регистр 1 значение типа byte из переменной c:           //
         obj.writeByte (0x09, 1, c);                                          //   Записываем ведомому с адресом 0x09 в его 1 регистр значение из переменной c.
     //  Записываем в регистры 2 и 3 одно число типа int16_t из переменной d: //   Число int16_t занимает 2 байта.
         obj.writeByte (0x09, 2, *((byte*)(&d)+0) );                          //   Записываем ведомому с адресом 0x09 в его 2 регистр значение из младшего байта переменной d.
         obj.writeByte (0x09, 3, *((byte*)(&d)+1) );                          //   Записываем ведомому с адресом 0x09 в его 3 регистр значение из старшего байта переменной d.
     //  obj.writeBytes(0x09, 2,   (byte*)(&d), 2 );                          // * Предыдущие 2 строки можно заменить на эту: записываем ведомому с адресом 0x09 начиная с 2 регистра значение начиная с адреса d, всего 2 байта.
     //  Записываем в регистры 4 и 5 значения типа byte:                      //
         obj.writeByte (0x09, 4, 100);                                        // * Записываем ведомому с адресом 0x09 в его 4 регистр значение 100. В примере ведомого "exchangeMulti/Slave", эта запись должна проигнорироваться.
         obj.writeByte (0x09, 5, 200);                                        // * Записываем ведомому с адресом 0x09 в его 5 регистр значение 200. В примере ведомого "exchangeMulti/Slave", эта запись должна проигнорироваться.
     //  Записываем в регистры 6-9 одно значение типа float из переменной e:  //
         obj.writeByte (0x09, 6, *((byte*)(&e)+0) );                          //   Записываем ведомому с адресом 0x09 в его 6 регистр значение из младшего байта переменной e.
         obj.writeByte (0x09, 7, *((byte*)(&e)+1) );                          //   Записываем ведомому с адресом 0x09 в его 7 регистр значение из второго  байта переменной e.
         obj.writeByte (0x09, 8, *((byte*)(&e)+2) );                          //   Записываем ведомому с адресом 0x09 в его 8 регистр значение из третьего байта переменной e.
         obj.writeByte (0x09, 9, *((byte*)(&e)+3) );                          //   Записываем ведомому с адресом 0x09 в его 9 регистр значение из старшего байта переменной e.
     //  obj.writeBytes(0x09, 6,   (byte*)(&e), 4 );                          // * Предыдущие 4 строки можно заменить на эту: записываем ведомому с адресом 0x09 начиная с 6 регистра значение из начиная с адреса e, всего 4 байта.
     //  Читаем из регистров 10 и 11 значение типа uint16_t в переменную f:   //
         *((byte*)(&f)+0) = obj.readByte(0x09, 10);                           //   Читаем из ведомого с адресом 0x09 из его 10 регистра один байт и сохраняем его как младший байт переменной f.
         *((byte*)(&f)+1) = obj.readByte(0x09, 11);                           //   Читаем из ведомого с адресом 0x09 из его 11 регистра один байт и сохраняем его как старший байт переменной f.
     //  obj.readBytes( 0x09, 10, (byte*)(&f), 2 );                           // * Предыдущие 2 строки можно заменить на эту: читаем из ведомого с адресом 0x09 начиная с 10 регистра значение записываемое по адресу f, всего 2 байта.
     }                                                                        //
}                                                                             //
                                                                              //
//   В скетче мастер обращается к одному ведому устройству с адресом 0x09.    //
//   Хотя можно обращаться к разным ведомым с разными адресами.               //
//   В этом примере мастер читает 0 регистр и если он изменился, то:          //
//   В регистр  1     записывает увеличивающийся байт (c).                    //
//   В регистры 2 и 3 записывает значение d=12345, которое меняет свой знак.  //
//   В регистры 4 и 5 записывает числа 100 и 200 соответственно.              //
//   В регистры 6 - 9 записывает количество секунд (e) с плавающей точкой.    //
//   Из регистров 10 и 11 читает значение типа uint16_t в переменную (f).     //
//   Вы сами решаете, какие числа передавать и в каких регистрах их хранить!  //
                                                                              //
//   ВАЖНО:                                                                   //
//   Ведомое устройство может менять и читать данные, которые в то же время   //
//   может менять и читать мастер по шине I2C. Так как в данном примере есть  //
//   многобайтные числа (int16_t, float, uint16_t), то возможна ситуация      //
//   когда мастер или слейв изменили часть числа, например, 2 байта из 4 байт //
//   значения float, другое устройство его прочитало и только потом были      //
//   заменены оставшиеся 2 байта числа. В результате прочитанное число не     //
//   соответствует исходному. Значит нужно создать условие при котором ведомое//
//   устройство не меняет свои данные и информирует мастера о их готовности.  //
//   Таким условием, в данном примере, является изменение ведомым значения    //
//   в 0 регистре, ведомый ждёт 1 секунду, а мастер читает и меняет данные.   //
                                                                              //
/*   ФУНКЦИИ БИБЛИОТЕКИ ДЛЯ ВЕДОМОГО:
 *   begin    ( [&объект I2C] , массив данных [, маскировочный массив] );     // Инициализации доступа к массиву данных по шине I2C.
 *   writeMask( маскировочный массив );                                       // Устаревшая функция указания маскировочного массива.
 *   
 *   Функцию begin() достаточно вызвать один раз в коде setup().
 *   Функция позволяет мастеру обращается к элементам массива данных как к регистрам ведомого по шине I2C.
 *   Маскировочный массив позволяет запретить запись в конкретные элементы массива данных по шине I2C.
 *   Если маскировочный массив не указан, то доступ на запись не ограничен.
 *   Если объект для работы с шиной I2C не указан, то используется &Wire.
 *
 *   ФУНКЦИИ БИБЛИОТЕКИ ДЛЯ МАСТЕРА:
 *   begin     ( &объект I2C );                        // Необязательная функция указания рабочей шины I2C.
 *   readByte  ( адрес, регистр );                     // Чтение одного    байта по шине I2C. Указывается адрес модуля и номер регистра.
 *   writeByte ( адрес, регистр, байт );               // Запись одного    байта по шине I2C. Указывается адрес модуля , номер регистра и байт для записи.
 *   readBytes ( адрес, регистр, массив, количество ); // Чтения нескольких байт по шине I2C. Указывается адрес модуля , номер первого регистра, массив для чтения данных, количество читаемых байт.
 *   writeBytes( адрес, регистр, массив, количество ); // Запись нескольких байт по шине I2C. Указывается адрес модуля , номер первого регистра, массив для записи данных, количество записываемых байт.
 * 
 *   Функцию begin() достаточно вызвать один раз в коде setup().
 *   Если функция begin() не вызвана, то используется шина I2C под управлением объекта &Wire.
 *   Функции readByte(), readBytes(), writeByte(), writeBytes() позволяют мастеру читать/записать данные, даже если ведомое является реальным устройством, а не Arduino с библиотекой iarduino_I2C_connect.
 */