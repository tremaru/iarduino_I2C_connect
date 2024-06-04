//   ПРИМЕР ДЛЯ МАСТЕРА (ПРОГРАММНАЯ ШИНА I2C):                               // * Строки со звёздочкой являются необязательными.
//   Организация передачи байтов через регистры ведомого:                     //
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
void setup(){                                                                 //
     sWire.begin();                                                           //   Инициируем подключение к шине I2C в качестве ведущего (master) устройства.
     obj.begin(&sWire);                                                       //   Указываем мастеру объект для работы с шиной I2C (по умолчанию &Wire).
}                                                                             //
                                                                              //
void loop(){                                                                  //
//   Задержка:                                                                //
     delay(100);                                                              // * Выполняем задержку, чтоб не слишком часто читать значение 0 регистра ведомого.
//   Читаем байт из регистрa 0:                                               //
     byte a = obj.readByte(0x09, 0);                                          //   Читаем из ведомого с адресом 0x09 из его 0 регистра один байт в перемую a.
     //   a = obj.readByte(0x09, 1);                                          //   Читаем из ведомого с адресом 0x09 из его 1 регистра один байт в перемую a.
     //   a = obj.readByte(0x09, 2);                                          //   Читаем из ведомого с адресом 0x09 из его 2 регистра один байт в перемую a.
     //       obj.readByte(0x09, 3, i, 7);                                    //   Читаем из ведомого с адресом 0x09, начиная с его 3 регистра, в массив i, 7 байт данных.
//   Если в нулевом регистре значение 0:                                      //
     if( a==0 ){                                                              //
     //  Ждём 2 секунды:                                                      //   
         delay(2000);                                                         // * За 2 секунды ведомый (в примере "exchangeByte/Slave") успеет 2-3 раза показать значения которые он сам себе изменил.
     //  Меняем значения всех 10 регистров ведомого:                          //
         obj.writeByte(0x09, 0, 10);                                          //   Записываем ведомому с адресом 0x09 в его 0 регистр значение 10.
         obj.writeByte(0x09, 1, 11);                                          //   Записываем ведомому с адресом 0x09 в его 1 регистр значение 11.
         obj.writeByte(0x09, 2, 12);                                          //   Записываем ведомому с адресом 0x09 в его 2 регистр значение 12.
         byte i[]={13,14,15,16,17,18,19};                                     //   Создаём массив из 7 значений для остальных регистров.
         obj.writeBytes(0x09, 3, i, 7);                                       //   Записываем ведомому с адресом 0x09, начиная с его 3 регистра, значения из массива i, всего 7 байт.
     }                                                                        //
}                                                                             //
                                                                              //
//   В скетче мастер обращается к одному ведому устройству с адресом 0x09.    //
//   Хотя можно обращаться к разным ведомым с разными адресами.               //
//   В этом примере мастер читает 0 регистр и если он сброшен, то записывает  //
//   во все 10 регистров ведомого, значения от 10 до 19 соответственно.       //
//   В примере "exchangeMulti" описана передача значений int, float и т.д.    //
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