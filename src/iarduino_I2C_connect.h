//	Библиотека для удобства соединения нескольких Arduino по аппаратной шине I2C
//  Версия: 1.1.3
//  Последнюю версию библиотеки Вы можете скачать по ссылке: http://iarduino.ru/file/254.html
//  Подробное описание функции бибилиотеки доступно по ссылке: http://lesson.iarduino.ru/page/urok-26-3-soedinyaem-dve-arduino-po-shine-i2c/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_I2C_connect_h																																				//
#define iarduino_I2C_connect_h																																				//
#include <Wire.h>																																							//
																																											//
#if defined(ARDUINO) && (ARDUINO >= 100)																																	//
#include <Arduino.h>																																						//
#else																																										//
#include <WProgram.h>																																						//
#endif																																										//
																																											//	
#include	"iarduino_I2C_connect_I2C.h"																																	//	Подключаем библиотеку выбора реализации шины I2C.
																																											//
#if defined(TwoWire_h) || defined(__ARDUINO_WIRE_IMPLEMENTATION__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega2560__) || defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(RENESAS_CORTEX_M4) // Если подключена библиотека Wire или платы её поддерживают...
#include	<Wire.h>																																						//	Разрешаем использовать библиотеку Wire в данной библиотеке.
#endif																																										//
#if defined( iarduino_I2C_Software_h )																																		//	Если библиотека iarduino_I2C_Software подключена в скетче...
#include	<iarduino_I2C_Software.h>																																		//	Разрешаем использовать библиотеку iarduino_I2C_Software в данной библиотеке.
#endif																																										//
																																											//
class iarduino_I2C_connect{																																					//
		public:																																								//
	/**	Конструктор класса **/																																				//
		iarduino_I2C_connect(void       ){              flgAddr=false; selI2C = new iarduino_I2C_Select; _savePtrI2C((iarduino_I2C_Select*)selI2C); }						//	Переопределяем указатель selI2C на объект производного класса iarduino_I2C_Select, дублируем указатель selI2C в пространство имён iarduino_I2C_connect_NS.
		iarduino_I2C_connect(uint8_t adr){ valAddr=adr; flgAddr=true;  selI2C = new iarduino_I2C_Select; _savePtrI2C((iarduino_I2C_Select*)selI2C); }						//	Сохраняем адрес ведомого, переопределяем указатель selI2C на объект производного класса iarduino_I2C_Select, дублируем указатель selI2C в пространство имён iarduino_I2C_connect_NS.
																																											//
	/**	Пользовательские функции **/																																		//
		#if defined(TwoWire_h) || defined(__ARDUINO_WIRE_IMPLEMENTATION__)																									//
											void	begin		(TwoWire* i									){	selI2C->init(i);     flgInitM=true;						}	//	Функция выбора аппаратной шины I2C для мастера.
			template<size_t n>				void	begin		(TwoWire* i    , byte(&j)[n]				){	selI2C->init(i);     _beginSlave(j,n);					}	//	Функция выбора аппаратной шины I2C и инициализации работы в качестве ведомого, с указанием массива данных byte(&j)[n].
			template<size_t n>				void	begin		(                byte(&j)[n]				){	selI2C->init(&Wire); _beginSlave(j,n);					}	//	Функция использования основной I2C и инициализации работы в качестве ведомого, с указанием массива данных byte(&j)[n].
			template<size_t n, size_t m>	void	begin		(TwoWire* i    , byte(&j)[n], bool(&k)[m]	){	selI2C->init(i);     _beginSlave(j,n); _setMask(k,m);	}	//	Функция выбора аппаратной шины I2C и инициализации работы в качестве ведомого, с указанием массива данных byte(&j)[n] и маскировочного массива bool(&k)[m].
			template<size_t n, size_t m>	void	begin		(                byte(&j)[n], bool(&k)[m]	){	selI2C->init(&Wire); _beginSlave(j,n); _setMask(k,m);	}	//	Функция использования основной I2C и инициализации работы в качестве ведомого, с указанием массива данных byte(&j)[n] и маскировочного массива bool(&k)[m].
		#endif																																								//
		#if defined(iarduino_I2C_Software_h)																																//
											void	begin		(SoftTwoWire* i								){	selI2C->init(i);      flgInitM=true;					}	//	Функция выбора программной шины I2C для мастера.
			template<size_t n>				void	begin		(SoftTwoWire* i, byte(&j)[n]				){	selI2C->init(i);     _beginSlave(j,n);					}	//	Функция выбора программной шины I2C и инициализации работы в качестве ведомого, с указанием массива данных byte(&j)[n].
			template<size_t n, size_t m>	void	begin		(SoftTwoWire* i, byte(&j)[n], bool(&k)[m]	){	selI2C->init(i);     _beginSlave(j,n); _setMask(k,m);	}	//	Функция выбора программной шины I2C и инициализации работы в качестве ведомого, с указанием массива данных byte(&j)[n] и маскировочного массива bool(&k)[m].
		#endif																																								//
			template<size_t n>				void	writeMask	( bool(&i)[n]								){ _setMask(i,n); }												//	Функция указания маскировочного массива (массива флагов) разрешающего мастеру запись в массив данных.
											byte	readByte	( byte, byte								);																//	Функция чтения мастером одного     байта по шине I2C с указанием: (адрес устройства, адрес регистра).
											byte	writeByte	( byte, byte, byte							);																//	Функция записи мастером одного     байта по шине I2C с указанием: (адрес устройства, адрес регистра, байт для записи).
											bool	readBytes	( byte, byte, byte*, byte					);																//	Функция чтения мастером нескольких байт  по шине I2C с указанием: (адрес устройства, адрес первого регистра, массив для чтения данных, количество читаемых байт).
											bool	writeBytes	( byte, byte, byte*, byte					);																//	Функция записи мастером нескольких байт  по шине I2C с указанием: (адрес устройства, адрес первого регистра, массив для записи данных, количество записываемых байт).
	/**	Внутренние переменные **/																																			//
		iarduino_I2C_VirtualSelect*					selI2C;																													//	Объявляем указатель на объект полиморфного класса iarduino_I2C_VirtualSelect, но в конструкторе текущего класса этому указателю будет присвоена ссылка на производный класс iarduino_I2C_Select.
											uint8_t	valAddr;																												//	Определяем переменную для хранения адреса ведомого на шине I2C.
											bool	flgAddr =false;																											//	Определяем флаг наличия адреса ведомого в конструкторе класа.
											bool	flgInitM=false;																											//	Определяем флаг указания шины I2C для мастера.
											bool	flgBegS =false;																											//	Определяем флаг инициализации работы в качестве ведомого.
	/**	Внутренние функции **/																																				//
		private:																																							//
		#if defined(TwoWire_h) || defined(__ARDUINO_WIRE_IMPLEMENTATION__)																									//
											void	_initMaster	( void										){	selI2C->init(&Wire); flgInitM=true;						}	//  Функция указания шины I2C для мастера по умолчанию.
		#endif																																								//
											void	_beginSlave	( byte* , uint8_t							);																//  Функция инициализации работы в качестве ведомого.
											void	_setMask	( bool* , uint8_t							);																//  Функция указания маскировочного массива ведомого.
											void	_savePtrI2C	(iarduino_I2C_Select*						);																//	Функция сохранения объекта I2C в простанство имён iarduino_I2C_connect_NS.
};																																											//
																																											//
#endif																																										//
