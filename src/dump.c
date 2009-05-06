/*
   SFR base dumper

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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdint.h>

#include "sfr.h"
#include "misc.h"
#include "revert.h"

extern int errno;

void perror (const char *ptr)
{
  printf ("%s:%d: %s: %s\n", __FILE__, __LINE__, ptr, strerror (errno));
}

void print_title (struct Title *);
void print_day (struct DayDataSet *, int num);
void print_group (struct Group *);
void print_gday (struct GDayStr *);
void print_distance (struct Distance *);
void print_cpoint (struct CPoint *);
void print_team (struct Team *);
void print_competitor (struct Competitor *);
void print_cday (struct CDayStr *);
void print_split (struct Split *);
void print_tsp (struct Tsp *);

int main (int argc, char *argv[])
{
  int fd = 0, i, j, errno;
  struct Title title;
  struct DayDataSet day;
  struct Offsets offsets;
  struct Group group;
  struct GDayStr gday;
  struct Distance distance;
  struct CPoint cpoint;
  struct Team team;
  struct Competitor comp;
  struct CDayStr cday;
  struct Split split;
  struct Tsp tsp;

  int32_t sync;

  bzero (&title, sizeof (struct Title));
  bzero (&offsets, sizeof (struct Offsets));

  if (argc > 1) {
    if ((fd = open (argv[1], O_RDONLY)) == -1) {
      perror ("open");
      exit (-1);
    }
  }
  else {
    printf ("Usage: %s <file.sfr>\n", argv[0]);
    exit (0);
  }

  if (read (fd, &title, sizeof (struct Title)) == -1)
    perror ("read");
  else {
    revert_title (&title);
    print_title (&title);
  }

  for (i = 1; i <= title.DaysNum; i++) {
    bzero (&day, sizeof (struct DayDataSet));
    if (read (fd, &day, sizeof (struct DayDataSet)) == -1)
      perror ("read");
    else
      print_day (&day, i);
  }

  if (lseek (fd, title.TitleEnd, SEEK_SET) == -1)
    perror ("lseek");

  if (read (fd, &offsets, sizeof (struct Offsets)) == -1)
    perror ("read");

  revert_offsets (&offsets);
  if (read (fd, &sync, sizeof (int32_t)) == -1)
    perror ("read");
  revert_l (sync);
  printf ("sync pos: %d\n", sync);

  for (i = 0; i < sizeof (offsets.Groups) / sizeof (int32_t); i++)
    if (offsets.Groups[i] > 0) {
      putchar ('\n');
//          printf(" offset of group %d: %d\n", i, offsets.Groups[i]);
      if (lseek (fd, offsets.Groups[i], SEEK_SET) == -1)
	perror ("lseek");
      bzero (&group, sizeof (struct Group));
      if (read (fd, &group, sizeof (struct Group)) == -1)
	perror ("read");
      else {
	revert_group (&group);
	print_group (&group);
      }
      for (j = 0; j < title.DaysNum; j++) {
	bzero (&gday, sizeof (struct GDayStr));
	if (read (fd, &gday, sizeof (struct GDayStr)) == -1)
	  perror ("read");
	else {
	  revert_gday (&gday);
	  if (gday.DistCode >= 0)
	    print_gday (&gday);
	}
      }
    }
  for (i = 0; i < sizeof (offsets.Distances) / sizeof (int32_t); i++)
    if (offsets.Distances[i] > 0) {
      putchar ('\n');
//          printf(" offset of distance %d: %d\n", i, offsets.Distances[i]);
      if (lseek (fd, offsets.Distances[i], SEEK_SET) == -1)
	perror ("lseek");
      bzero (&distance, sizeof (struct Distance));
      if (read (fd, &distance, sizeof (struct Distance)) == -1)
	perror ("read");
      else {
	revert_distance (&distance);
	print_distance (&distance);
      }
    }
  for (i = 0; i < sizeof (offsets.CPoints) / sizeof (int32_t); i++)
    if (offsets.CPoints[i] > 0) {
      putchar ('\n');
//          printf(" offset of cpoint %d: %d\n", i, offsets.CPoints[i]);
      if (lseek (fd, offsets.CPoints[i], SEEK_SET) == -1)
	perror ("lseek");
      bzero (&cpoint, sizeof (struct CPoint));
      if (read (fd, &cpoint, sizeof (struct CPoint)) == -1)
	perror ("read");
      else {
	revert_cpoint (&cpoint);
	print_cpoint (&cpoint);
      }
    }
  for (i = 0; i < sizeof (offsets.Teams) / sizeof (int32_t); i++)
    if (offsets.Teams[i] > 0) {
      putchar ('\n');
//          printf(" offset of team %d: %d\n", i, offsets.Teams[i]);
      if (lseek (fd, offsets.Teams[i], SEEK_SET) == -1)
	perror ("lseek");
      bzero (&team, sizeof (struct Team));
      if (read (fd, &team, sizeof (struct Team)) == -1)
	perror ("read");
      else {
	revert_team (&team);
	print_team (&team);
      }
    }
  for (i = 0; i < sizeof (offsets.Competitors) / sizeof (int32_t); i++)
    if (offsets.Competitors[i] > 0) {
      putchar ('\n');
//          printf(" offset of competitor %d: %d\n", i, offsets.Competitors[i]);
      if (lseek (fd, offsets.Competitors[i], SEEK_SET) == -1)
	perror ("lseek");
      bzero (&comp, sizeof (struct Competitor));
      if (read (fd, &comp, sizeof (struct Competitor)) == -1)
	perror ("read");
      else {
	revert_competitor (&comp);
	print_competitor (&comp);
      }
      for (j = 0; j < title.DaysNum; j++) {
	bzero (&cday, sizeof (struct CDayStr));
	if (read (fd, &cday, sizeof (struct CDayStr)) == -1)
	  perror ("read");
	else {
	  revert_cday (&cday);
	  print_cday (&cday);
	}
      }
    }
  for (i = 0; i < sizeof (offsets.Splits) / sizeof (int32_t); i++)
    if (offsets.Splits[i] > 0) {
      putchar ('\n');
//          printf(" offset of split %d: %d\n", i, offsets.Splits[i]);
      if (lseek (fd, offsets.Splits[i], SEEK_SET) == -1)
	perror ("lseek");
      bzero (&split, sizeof (struct Split));
      if (read (fd, &split, sizeof (struct Split)) == -1)
	perror ("read");
      else {
	revert_split (&split);
	print_split (&split);
      }
      for (j = 0; j < split.recs; j++) {
	bzero (&tsp, sizeof (struct Tsp));
	if (read (fd, &tsp, sizeof (struct Tsp)) == -1)
	  perror ("read");
	else {
	  revert_tsp (&tsp);
	  if (tsp.kp > 0)
	    print_tsp (&tsp);
	}
      }
    }
  close (fd);
  return 0;
}

void print_title (struct Title *ptr)
{
  printf ("version: %d\nheader end: %d\n"
	  "title: %s\nreferee: %s\ngensec: %s\n"
	  "days: %d groups: %d distances: %d\n"
	  "cps: %d teams: %d competitors: %d\n"
	  "splits: %d\n"
	  "group: %d distance: %d cp: %d\n"
	  "team: %d competitor: %d split: %d\n"
	  "type: %d day: %d precision: %d\n"
	  "midnight: %d check: %d\n",
	  ptr->Version, ptr->TitleEnd,
	  finalize (ptr->Title), finalize (ptr->Referee),
	  finalize (ptr->GenSec), ptr->DaysNum, ptr->GroupsNum,
	  ptr->DistancesNum, ptr->CPNum, ptr->TeamsNum, ptr->CompetitorsNum,
	  ptr->SplitsNum, ptr->GroupCurrentCode, ptr->DistanceCurrentCode,
	  ptr->CPCurrentCode, ptr->TeamCurrentCode,
	  ptr->CompetitorCurrentCode, ptr->SplitCurrentCode, ptr->CompType,
	  ptr->CurrentDay, ptr->Precision, ptr->MidNight,
	  ptr->CheckPunchType);
}

void print_day (struct DayDataSet *ptr, int num)
{
  printf ("day: %d\nname: %s\ndate: %s\n",
	  num, finalize (ptr->DayName), finalize (ptr->DayDate));
}

void print_group (struct Group *ptr)
{
  printf ("group: %d, parent: %d\n"
	  "name: %s\n"
	  "start: %d, end: %d\n"
	  "money: %d\n",
	  ptr->Code, ptr->parent, finalize (ptr->Name),
	  ptr->SBornYear, ptr->EBornYear, ptr->Money);
}

void print_gday (struct GDayStr *ptr)
{
  printf ("distcode: %d max.razr: %d mc: %d kmc: %d "
	  "checktime: %d max.bals: %d\n",
	  ptr->DistCode, ptr->maxRazr, ptr->mc, ptr->kmc,
	  ptr->checktime, ptr->MaxBals);
}

void print_distance (struct Distance *ptr)
{
  int i;
  printf ("distance: %d\nname: %s\n"
	  "number: %d, day: %d, max.ball: %d\n"
	  "sequence: %d, length: %d, height: %d\n"
	  "cp num: %d\n",
	  ptr->Code, finalize (ptr->Name),
	  ptr->Number, ptr->Day, ptr->MaxBall,
	  ptr->Sequence, ptr->Length, ptr->Height, ptr->CPNum);
  for (i = 0; i < ptr->CPNum; i++)
    printf ("%4d  ", ptr->CP[i]);
  putchar ('\n');
  for (i = 0; i < ptr->CPNum; i++)
    printf ("%4dm ", ptr->CPD[i]);
  putchar ('\n');
}

void print_cpoint (struct CPoint *ptr)
{
  printf ("cpoint: %d, number: %d, station: %d\n"
	  "x: %d, y: %d, check: %d\n",
	  ptr->Code, ptr->num, ptr->Station, ptr->X, ptr->Y, ptr->check);
}

void print_team (struct Team *ptr)
{
  printf ("team: %d, parent: %d\n"
	  "name: %s\n"
	  "coach: %s, contacts: %s\n"
	  "entry: %s, payment: %s\n"
	  "money: %d\n",
	  ptr->Code, ptr->parent, finalize (ptr->Name),
	  finalize (ptr->Coach), finalize (ptr->Contacts),
	  finalize (ptr->EntryDate), finalize (ptr->PaymentDate), ptr->Money);
}

void print_competitor (struct Competitor *ptr)
{
  printf ("competitor: %d, number: %d\n"
	  "surname: %s, name: %s\n"
	  "group: %d, team: %d, qualif: %d, year: %d\n"
	  "comment: %s\n"
	  "chip: %d, money: %d, type: %d\n",
	  ptr->Code, ptr->StNum, finalize (ptr->Family), finalize (ptr->Name),
	  ptr->GroupCode, ptr->TeamCode, ptr->Qualif, ptr->BornYear,
	  finalize (ptr->Comment), ptr->chipNum, ptr->Money, ptr->type);
}

void print_cday (struct CDayStr *ptr)
{
  printf ("start: %s, finish: %s, bonus: %d, position: %d\n"
	  "started: %d, penalty: %d, ball: %d, score: %d, dsq: %d\n",
	  totime (ptr->Start), totime (ptr->Finish), ptr->Bonus, ptr->Position,
	  ptr->IsStart, ptr->Penalty, ptr->Ball, ptr->Score, ptr->dsq);
}

void print_split (struct Split *ptr)
{
  printf ("split: %d\n"
	  "number: %d, group: %d, chip: %d, day: %d, recs: %d\n",
	  ptr->Code, ptr->num, ptr->group, ptr->chip, ptr->day, ptr->recs);
}

void print_tsp (struct Tsp *ptr)
{
  printf ("kp: %d, check: %d, time: %s\n", ptr->kp, ptr->check, totime (ptr->tm)
    );
}
