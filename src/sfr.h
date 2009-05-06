/* 
   SFR base structure declaration
   Based on BaseMemStruct.h by Sport Service (www.sportsystem.ru)

   Copyright (C) 2009 Grigory Batalov <bga@altlinux.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.
*/

#ifndef SFR_H
#define SFR_H

#include <stdint.h>

struct Title {
  int32_t Version;		//Версия файла
  int32_t TitleEnd;		//Конец заголовка
  char Title[80];		//Название соревнований
  char Referee[20];		//Гл.судья
  char GenSec[20];		//Гл.секретарь
  int32_t DaysNum;		//число дней в многодневке
  int32_t GroupsNum;		//Кол-во групп
  int32_t DistancesNum;		//Кол-во дистанций
  int32_t CPNum;		//Кол-во КП
  int32_t TeamsNum;		//Кол-во команд
  int32_t CompetitorsNum;	//Кол-во участников
  int32_t SplitsNum;		//Кол-во сплитов
  int32_t GroupCurrentCode;	//Текущий идентификатор групп
  int32_t DistanceCurrentCode;	//Текущий идентификатор дистанций
  int32_t CPCurrentCode;	//Текущий идентификатор КП
  int32_t TeamCurrentCode;	//Текущий идентификатор команд
  int32_t CompetitorCurrentCode;	//Текущий идентификатор участников
  int32_t SplitCurrentCode;	//Текущий идентификатор сплитов
  int32_t CompType;		//Вид соревнований
  int32_t CurrentDay;		//Текущий день
  int32_t Precision;		//Точность анализа времен:  0-секунды 1-десятые
  int32_t MidNight;		//Переход через полночь: 0-запрещен 1-нарастающим итогом
  int32_t CheckPunchType;	//Тип проверки отметки                                        .
};

struct DayDataSet {
  char DayName[60];
  char DayDate[10];
};

struct Offsets {
  int32_t Groups[1024];
  int32_t Distances[4096];
  int32_t CPoints[1024];
  int32_t Teams[2048];
  int32_t Competitors[20480];
  int32_t Splits[20480];
};

struct Group {
//Запись, описывающая одну группу на время соревнований
  int32_t Code;			//Уникальный идентификатор
  char Name[16];		//Название группы
  int32_t SBornYear;		//С Года рождения
  int32_t EBornYear;		//До года
  int32_t parent;		//Подгруппа в..
  int32_t Money;		//Взнос
};

struct GDayStr {
  int32_t DistCode;		//Код дистанции
  int32_t maxRazr;		//Максимальный присваеваемый разряд
  int32_t mc;			//Количество мест (с первого), получающих мастера
  int32_t kmc;			//Количество мест (с первого), получающих кандидата
  int32_t checktime;		//Контрольное время
  int32_t MaxBals;		//Макс. кол-во очков
};

struct Distance {
//Запись, описывающая одну дистанцию на время соревнований
  int32_t Code;			//Уникальный идентификатор
  char Name[16];		//Название дистанции
  int32_t Number;		//Номер участника
  int32_t Day;			//День соревнований
  int32_t MaxBall;		//Макс. кол-во баллов
  int32_t Sequence;		//Последовательность взятия КП
  int32_t Height;		//Набор высот
  int32_t Length;		//Кол-во метров
  int32_t CPNum;		//Кол-во пунктов (минимальное чтобы не сняли)
  int32_t CP[256];		//Массив с номерами пунктов
  int32_t CPD[256];		//расстояние м-ду пунктами
};

struct CPoint {
//Запись, описывающая один КП на время соревнований
  int32_t Code;			//Уникальный идентификатор
  int32_t num;			//Номер КП
  int32_t Station;		//Номер станции на КП
  int32_t X;			//Х-координата
  int32_t Y;			//Y-координата
  int32_t check;		//Статус проверки
};

struct Team {
//Запись, описывающая одну команду на время соревнований
  int32_t Code;			//Уникальный идентификатор
  char Name[32];		//Название комманды
  int32_t parent;		//Входит в..
  char Coach[24];		//Руководитель
  char Contacts[32];		//Контакты
  char EntryDate[10];		//Дата заявки
  char PaymentDate[10];		//Дата оплаты
  int32_t Money;		//Полученная сумма
};

struct CDayStr {
  int32_t Start;		//Стартовое время
  int32_t Finish;		//Финишное время
  int32_t Bonus;		//Бонус
  int32_t Position;		//Место
  int32_t IsStart;		//Стартовал/не страртовал/хз
  int32_t Penalty;		//Штраф
  int32_t Ball;			//Баллы
  int32_t Score;		//Очки
  int32_t dsq;			//Признак снятия
};


struct Competitor {
//Запись, описывающая одного участника во время соревнований
  int32_t Code;			//Уникальный идентификатор
  int32_t StNum;		//Нагрудный номер
  char Name[16];		//Имя
  char Family[16];		//Фамилия
  int32_t GroupCode;		//Код группы
  int32_t TeamCode;		//Код команды
  int32_t Qualif;		//Разряд
  int32_t BornYear;		//Год рождения
  char Comment[32];		//Комментарий
  int32_t chipNum;		//Номер чипа
  int32_t Money;		//Сумма к оплате
  int32_t type;			//Тип участия (лично, вк, не участвует)
};

struct Tsp {
  int16_t kp;			// номер кп
  int16_t check;		// результат проверки
  int32_t tm;			// время прохождения
};

struct Split {
  int32_t Code;			//Уникальный идентификатор
  int32_t num;			//Номер участника
  int32_t group;		//Группа участника
  int32_t chip;			//Cерийник чипа
  int32_t day;			//День
  int32_t recs;			//Количество записей в чипе
};

#endif
