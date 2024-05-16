//	Библиотека для удобства соединения нескольких Arduino по аппаратной шине I2C
//  Версия: 1.1.2
//  Последнюю версию библиотеки Вы можете скачать по ссылке: http://iarduino.ru/file/254.html
//  Подробное описание функции бибилиотеки доступно по ссылке: http://lesson.iarduino.ru/page/urok-26-3-soedinyaem-dve-arduino-po-shine-i2c/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_I2C_connect_h
#define iarduino_I2C_connect_h
#include <Wire.h>

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class iarduino_I2C_connect{																		//
	public:								byte	readByte		(byte, byte);					//	Функция чтения мастером одного байта по шине I2C с указанием: (адрес устройства, адрес регистра).
										byte	writeByte		(byte, byte, byte);				//	Функция записи мастером одного байта по шине I2C с указанием: (адрес устройства, адрес регистра, байт для записи).
		template< typename T, size_t j>	void	begin			(T(&i)[j]){I2C_func_BEGIN(i,j);}//	Функция инициализации слейва, с указанием массива данных, который будет доступен мастеру по шине I2C.
		template< typename T, size_t j>	void	writeMask		(T(&i)[j]){I2C_func_MASK (i,j);}//	Функция указания маскировочного массива (массива флагов) разрешающего мастеру запись в массив данных.
	private:							void	I2C_func_BEGIN	(uint8_t*, uint8_t);			//  Функция запускаемая функцией begin().
										void	I2C_func_MASK	(bool*,    uint8_t);			//  Функция запускаемая функцией writeMask().
};																								//

#endif