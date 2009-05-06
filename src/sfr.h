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
  int32_t Version;		//������ �����
  int32_t TitleEnd;		//����� ���������
  char Title[80];		//�������� ������������
  char Referee[20];		//��.�����
  char GenSec[20];		//��.���������
  int32_t DaysNum;		//����� ���� � �����������
  int32_t GroupsNum;		//���-�� �����
  int32_t DistancesNum;		//���-�� ���������
  int32_t CPNum;		//���-�� ��
  int32_t TeamsNum;		//���-�� ������
  int32_t CompetitorsNum;	//���-�� ����������
  int32_t SplitsNum;		//���-�� �������
  int32_t GroupCurrentCode;	//������� ������������� �����
  int32_t DistanceCurrentCode;	//������� ������������� ���������
  int32_t CPCurrentCode;	//������� ������������� ��
  int32_t TeamCurrentCode;	//������� ������������� ������
  int32_t CompetitorCurrentCode;	//������� ������������� ����������
  int32_t SplitCurrentCode;	//������� ������������� �������
  int32_t CompType;		//��� ������������
  int32_t CurrentDay;		//������� ����
  int32_t Precision;		//�������� ������� ������:  0-������� 1-�������
  int32_t MidNight;		//������� ����� �������: 0-�������� 1-����������� ������
  int32_t CheckPunchType;	//��� �������� �������                                        .
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
//������, ����������� ���� ������ �� ����� ������������
  int32_t Code;			//���������� �������������
  char Name[16];		//�������� ������
  int32_t SBornYear;		//� ���� ��������
  int32_t EBornYear;		//�� ����
  int32_t parent;		//��������� �..
  int32_t Money;		//�����
};

struct GDayStr {
  int32_t DistCode;		//��� ���������
  int32_t maxRazr;		//������������ ������������� ������
  int32_t mc;			//���������� ���� (� �������), ���������� �������
  int32_t kmc;			//���������� ���� (� �������), ���������� ���������
  int32_t checktime;		//����������� �����
  int32_t MaxBals;		//����. ���-�� �����
};

struct Distance {
//������, ����������� ���� ��������� �� ����� ������������
  int32_t Code;			//���������� �������������
  char Name[16];		//�������� ���������
  int32_t Number;		//����� ���������
  int32_t Day;			//���� ������������
  int32_t MaxBall;		//����. ���-�� ������
  int32_t Sequence;		//������������������ ������ ��
  int32_t Height;		//����� �����
  int32_t Length;		//���-�� ������
  int32_t CPNum;		//���-�� ������� (����������� ����� �� �����)
  int32_t CP[256];		//������ � �������� �������
  int32_t CPD[256];		//���������� �-�� ��������
};

struct CPoint {
//������, ����������� ���� �� �� ����� ������������
  int32_t Code;			//���������� �������������
  int32_t num;			//����� ��
  int32_t Station;		//����� ������� �� ��
  int32_t X;			//�-����������
  int32_t Y;			//Y-����������
  int32_t check;		//������ ��������
};

struct Team {
//������, ����������� ���� ������� �� ����� ������������
  int32_t Code;			//���������� �������������
  char Name[32];		//�������� ��������
  int32_t parent;		//������ �..
  char Coach[24];		//������������
  char Contacts[32];		//��������
  char EntryDate[10];		//���� ������
  char PaymentDate[10];		//���� ������
  int32_t Money;		//���������� �����
};

struct CDayStr {
  int32_t Start;		//��������� �����
  int32_t Finish;		//�������� �����
  int32_t Bonus;		//�����
  int32_t Position;		//�����
  int32_t IsStart;		//���������/�� ����������/��
  int32_t Penalty;		//�����
  int32_t Ball;			//�����
  int32_t Score;		//����
  int32_t dsq;			//������� ������
};


struct Competitor {
//������, ����������� ������ ��������� �� ����� ������������
  int32_t Code;			//���������� �������������
  int32_t StNum;		//��������� �����
  char Name[16];		//���
  char Family[16];		//�������
  int32_t GroupCode;		//��� ������
  int32_t TeamCode;		//��� �������
  int32_t Qualif;		//������
  int32_t BornYear;		//��� ��������
  char Comment[32];		//�����������
  int32_t chipNum;		//����� ����
  int32_t Money;		//����� � ������
  int32_t type;			//��� ������� (�����, ��, �� ���������)
};

struct Tsp {
  int16_t kp;			// ����� ��
  int16_t check;		// ��������� ��������
  int32_t tm;			// ����� �����������
};

struct Split {
  int32_t Code;			//���������� �������������
  int32_t num;			//����� ���������
  int32_t group;		//������ ���������
  int32_t chip;			//C������� ����
  int32_t day;			//����
  int32_t recs;			//���������� ������� � ����
};

#endif
