/*
   SFR base to text convertor

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
#include <sys/param.h>
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
#include "sfr2txt.h"

extern int errno;

void perror (const char *ptr)
{
  printf ("%s:%d: %s: %s\n", __FILE__, __LINE__, ptr, strerror (errno));
}

void usage (char *progname)
{
  char *ptr = NULL;
  if ((ptr = strrchr (progname, '/')) == NULL)
    ptr = progname;
  else
    ptr++;
  printf ("Usage: %s file.sfr [day]\n"
	  " file.sfr - SFR base file\n" " day      - day number\n", ptr);
}

char *qualification[] =
  { "", "3þ", "2þ", "1þ", "3ð", "2ð", "1ð", "êìñ", "ìñ" };

#define compare_code(a, b) if (a->Code < b->Code) return -1; return a->Code > b->Code;
int group_cmp (const void *a, const void *b)
{
compare_code ((&((struct Groups *) a)->group),
		(&((struct Groups *) b)->group))}
int distance_cmp (const void *a, const void *b)
{
compare_code (((struct Distance *) a), ((struct Distance *) b))}
int cpoint_cmp (const void *a, const void *b)
{
compare_code (((struct CPoint *) a), ((struct CPoint *) b))}
int team_cmp (const void *a, const void *b)
{
compare_code (((struct Team *) a), ((struct Team *) b))}
int comp_cmp (const void *a, const void *b)
{
compare_code ((&((struct Comps *) a)->comp), (&((struct Comps *) b)->comp))}
int split_cmp (const void *a, const void *b)
{
compare_code ((&((struct Splits *) a)->split),
		(&((struct Splits *) b)->split))}

int comp_num_cmp (const void *a, const void *b)
{
  if (((struct Comps *) a)->comp.StNum < ((struct Comps *) b)->comp.StNum)
    return -1;
  return ((struct Comps *) a)->comp.StNum > ((struct Comps *) b)->comp.StNum;
}

int split_comp_cmp (const void *a, const void *b)
{
    struct Split *aa = &((struct Splits *) a)->split;
    struct Split *bb = &((struct Splits *) b)->split;

    if (aa->day < bb->day)
	return -1;
    if (aa->day > bb->day)
	return 1;
    if (aa->group < bb->group)
	return -1;
    if (aa->group > bb->group)
	return 1;
    if (aa->num < bb->num)
	return -1;
    if (aa->num > bb->num)
	return 1;
    return 0;
}

int csplit_cmp (const void *a, const void *b)
{
  int32_t atime, btime;
  if (((struct CSplit *) a)->cday->dsq && !((struct CSplit *) b)->cday->dsq)
    return 1;
  if (!((struct CSplit *) a)->cday->dsq && ((struct CSplit *) b)->cday->dsq)
    return -1;
  atime =
    ((struct CSplit *) a)->cday->Finish - ((struct CSplit *) a)->cday->Start;
  btime =
    ((struct CSplit *) b)->cday->Finish - ((struct CSplit *) b)->cday->Start;
  if (atime < btime)
    return -1;
  return atime > btime;
}

int main (int argc, char *argv[])
{
  int fd = 0, errno;
  unsigned int i, j, k, day;
  unsigned int group_num, dist_num, team_num, comp_num, split_num;
  struct Title *title = NULL;
  struct DayDataSet *days = NULL;
  struct Offsets *offsets = NULL;
  struct Groups *groups = NULL;
  struct GDayStr *gdays = NULL;
  struct Distance *distances = NULL;
  struct CPoint *cpoints = NULL;
  struct Team *teams = NULL;
  struct Comps *comps = NULL;
  struct CDayStr *cdays = NULL;
  struct Splits *splits = NULL;
  struct Tsp *tsps = NULL;
  void *ptr = NULL;

  int32_t sync;

  // show info if not enough parameters given
  if (argc < 2) {
    usage (argv[0]);
    exit (EXIT_SUCCESS);
  }

  // open SFR base
  if ((fd = open (argv[1], O_RDONLY)) == -1) {
    perror ("open");
    exit (EXIT_FAILURE);
  }
  if ((title = malloc (sizeof (struct Title))) == NULL) {
    perror ("malloc");
    exit (EXIT_FAILURE);
  }
  // clear title structure
  bzero (title, sizeof (struct Title));

  // read competition title
  if (read (fd, title, sizeof (struct Title)) == -1)
    perror ("read");
  else
    revert_title (title);

  // show info if not enough parameters given
  if (argc < 3) {
    printf ("SFR base: %s, %d days\n", argv[1], title->DaysNum);
    close (fd);
    free (title);
    exit (EXIT_SUCCESS);
  }
  day = atoi (argv[2]) - 1;

  if ((offsets = malloc (sizeof (struct Offsets))) == NULL) {
    perror ("malloc");
    exit (EXIT_FAILURE);
  }

  // clear offsets structure
  bzero (offsets, sizeof (struct Offsets));


  // for each day read it's description
  for (i = 0; i < title->DaysNum; i++) {

    if ((ptr = realloc (days, sizeof (struct DayDataSet) * (i + 1))) == NULL) {
      perror ("realloc");
      continue;
    }
    else
      days = ptr;

    // clear day structure
    bzero (days + i, sizeof (struct DayDataSet));
    // read a day
    if (read (fd, days + i, sizeof (struct DayDataSet)) == -1)
      perror ("read");

    // print day header
    if (day == i)
      printf ("%s\n%s\n%s\n\nÑÏËÈÒÛ\n\n",
	      finalize (title->Title), finalize (days[i].DayName),
	      finalize (days[i].DayDate));
  }

  // seek to the end of competition's title
  if (lseek (fd, title->TitleEnd, SEEK_SET) == -1)
    perror ("lseek");


  if (read (fd, offsets, sizeof (struct Offsets)) == -1)
    perror ("read");
  else
    revert_offsets (offsets);

/*
  if (read (fd, &sync, sizeof (int32_t)) == -1)
    perror ("read");
  else
    revert_l (sync);
*/

  for (i = 0, k = 0; i < sizeof (offsets->Groups) / sizeof (int32_t); i++)
    if (offsets->Groups[i] > 0) {
      if (lseek (fd, offsets->Groups[i], SEEK_SET) == -1)
	perror ("lseek");

      if ((ptr = realloc (groups, sizeof (struct Groups) * (k + 1))) == NULL) {
	perror ("realloc");
	continue;
      }
      else
	groups = ptr;

      bzero (groups + k, sizeof (struct Groups));
      if (read (fd, &groups[k].group, sizeof (struct Group)) == -1)
	perror ("read");
      else
	revert_group (&groups[k].group);

      if ((ptr =
	   realloc (gdays,
		    sizeof (struct GDayStr) * title->DaysNum * (k + 1))) ==
	  NULL) {
	perror ("realloc");
	continue;
      }
      else {
	gdays = ptr;
	groups[k].day_offset = k;
      }


      for (j = 0; j < title->DaysNum; j++) {

	bzero (gdays + k * title->DaysNum + j, sizeof (struct GDayStr));
	if (read (fd, gdays + k * title->DaysNum + j, sizeof (struct GDayStr))
	    == -1)
	  perror ("read");
	else
	  revert_gday (gdays + k * title->DaysNum + j);
      }

      if ((ptr =
	   bsearch (groups + k, groups, k, sizeof (struct Groups),
		    group_cmp)) != NULL)
	memcpy (ptr, groups + k, sizeof (struct Groups));
      else {
	k++;
	qsort (groups, k, sizeof (struct Groups), group_cmp);
      }

    }
  group_num = k;

  for (i = 0, k = 0; i < sizeof (offsets->Distances) / sizeof (int32_t); i++)
    if (offsets->Distances[i] > 0) {
      if (lseek (fd, offsets->Distances[i], SEEK_SET) == -1)
	perror ("lseek");

      if ((ptr =
	   realloc (distances, sizeof (struct Distance) * (k + 1))) == NULL) {
	perror ("realloc");
	continue;
      }
      else
	distances = ptr;

      bzero (distances + k, sizeof (struct Distance));
      if (read (fd, distances + k, sizeof (struct Distance)) == -1)
	perror ("read");
      else
	revert_distance (distances + k);

      if ((ptr =
	   bsearch (distances + k, distances, k, sizeof (struct Distance),
		    distance_cmp)) != NULL)
	memcpy (ptr, distances + k, sizeof (struct Distance));
      else {
	k++;
	qsort (distances, k, sizeof (struct Distance), distance_cmp);
      }

    }
  dist_num = k;

  for (i = 0, k = 0; i < sizeof (offsets->CPoints) / sizeof (int32_t); i++)
    if (offsets->CPoints[i] > 0) {
      if (lseek (fd, offsets->CPoints[i], SEEK_SET) == -1)
	perror ("lseek");

      if ((ptr = realloc (cpoints, sizeof (struct CPoint) * (k + 1))) == NULL) {
	perror ("realloc");
	continue;
      }
      else
	cpoints = ptr;

      bzero (cpoints + k, sizeof (struct CPoint));
      if (read (fd, cpoints + k, sizeof (struct CPoint)) == -1)
	perror ("read");
      else
	revert_cpoint (cpoints + k);

      if ((ptr =
	   bsearch (cpoints + k, cpoints, k, sizeof (struct CPoint),
		    cpoint_cmp)) != NULL)
	memcpy (ptr, cpoints + k, sizeof (struct CPoint));
      else {
	k++;
	qsort (cpoints, k, sizeof (struct CPoint), cpoint_cmp);
      }

    }


  for (i = 0, k = 0; i < sizeof (offsets->Teams) / sizeof (int32_t); i++)
    if (offsets->Teams[i] > 0) {
      if (lseek (fd, offsets->Teams[i], SEEK_SET) == -1)
	perror ("lseek");
      if ((ptr = realloc (teams, sizeof (struct Team) * (k + 1))) == NULL) {
	perror ("realloc");
	continue;
      }
      else
	teams = ptr;

      bzero (teams + k, sizeof (struct Team));
      if (read (fd, teams + k, sizeof (struct Team)) == -1)
	perror ("read");
      else
	revert_team (teams + k);

      if ((ptr =
	   bsearch (teams + k, teams, k, sizeof (struct Team),
		    team_cmp)) != NULL)
	memcpy (ptr, teams + k, sizeof (struct Team));
      else {
	k++;
	qsort (teams, k, sizeof (struct Team), team_cmp);
      }

    }
  team_num = k;

  for (i = 0, k = 0; i < sizeof (offsets->Competitors) / sizeof (int32_t);
       i++)
    if (offsets->Competitors[i] > 0) {
      if (lseek (fd, offsets->Competitors[i], SEEK_SET) == -1)
	perror ("lseek");
      if ((ptr = realloc (comps, sizeof (struct Comps) * (k + 1))) == NULL) {
	perror ("realloc");
	continue;
      }
      else
	comps = ptr;

      bzero (comps + k, sizeof (struct Comps));
      if (read (fd, &comps[k].comp, sizeof (struct Competitor)) == -1)
	perror ("read");
      else
	revert_competitor (&comps[k].comp);

      if ((ptr =
	   realloc (cdays,
		    sizeof (struct CDayStr) * title->DaysNum * (k + 1))) ==
	  NULL) {
	perror ("realloc");
	continue;
      }
      else {
	cdays = ptr;
	comps[k].day_offset = k;
      }

      for (j = 0; j < title->DaysNum; j++) {

	bzero (cdays + k * title->DaysNum + j, sizeof (struct CDayStr));
	if (read (fd, cdays + k * title->DaysNum + j, sizeof (struct CDayStr))
	    == -1)
	  perror ("read");
	else
	  revert_cday (cdays + k * title->DaysNum + j);

      }

      if ((ptr =
	   bsearch (comps + k, comps, k, sizeof (struct Comps),
		    comp_cmp)) != NULL)
	memcpy (ptr, comps + k, sizeof (struct Comps));
      else {
	k++;
	qsort (comps, k, sizeof (struct Comps), comp_cmp);
      }
    }
  comp_num = k;

  for (i = 0, k = 0; i < sizeof (offsets->Splits) / sizeof (int32_t); i++)
    if (offsets->Splits[i] > 0) {
      if (lseek (fd, offsets->Splits[i], SEEK_SET) == -1)
	perror ("lseek");
      if ((ptr = realloc (splits, sizeof (struct Splits) * (k + 1))) == NULL) {
	perror ("realloc");
	continue;
      }
      else
	splits = ptr;

      bzero (splits + k, sizeof (struct Splits));
      if (read (fd, &splits[k].split, sizeof (struct Split)) == -1)
	perror ("read");
      else
	revert_split (&splits[k].split);

      if ((ptr =
	   realloc (tsps,
		    sizeof (struct Tsp) * CHIP_RECS * (k + 1))) == NULL) {
	perror ("realloc");
	continue;
      }
      else {
	tsps = ptr;
	splits[k].tsp_offset = k;
      }


      for (j = 0; j < splits[k].split.recs && j < CHIP_RECS; j++) {
	bzero (tsps + k * CHIP_RECS + j, sizeof (struct Tsp));
	if (read (fd, tsps + k * CHIP_RECS + j, sizeof (struct Tsp)) == -1)
	  perror ("read");
	else
	  revert_tsp (tsps + k * CHIP_RECS + j);

      }

      if ((ptr =
	   bsearch (splits + k, splits, k, sizeof (struct Splits),
		    split_comp_cmp)) != NULL) {
	int tsp = ((struct Splits *)ptr)->tsp_offset;
	splits[k].tsp_offset = tsp;
	memcpy (tsps + tsp * CHIP_RECS, tsps + k * CHIP_RECS, sizeof (struct Tsp) * CHIP_RECS);
	memcpy (ptr, splits + k, sizeof (struct Splits));
      }
      else {
	k++;
	qsort (splits, k, sizeof (struct Splits), split_comp_cmp);
      }
    }
  split_num = k;


//////////
  qsort (comps, comp_num, sizeof (struct Comps), comp_num_cmp);

  for (i = 0; i < group_num; i++) {
    void *ptr = NULL;
    struct Distance dist, *dptr = NULL;
    struct Comps comp, *cptr = NULL;
    struct Team team, *tptr = NULL;
    struct CSplit *csplits = NULL;
    int n;

    dist.Code = gdays[groups[i].day_offset * title->DaysNum + day].DistCode;
    if ((dptr =
	 bsearch (&dist, distances, dist_num, sizeof (struct Distance),
		  distance_cmp)) != NULL)
      printf ("\n  %s %d ì %d êï\n",
	      finalize (groups[i].group.Name), dptr->Length, dptr->CPNum);
    else
      continue;


    for (j = k = 0; j < split_num; j++) {
      if (splits[j].split.group != dist.Code ||
	  splits[j].split.day != day + 1)
	    continue;

	if ((ptr =
	     realloc (csplits, sizeof (struct CSplit) * (k + 1))) == NULL) {
	  perror ("realloc");
	  continue;
	}
	else
	  csplits = ptr;
	bzero (csplits + k, sizeof (struct CSplit));

	comp.comp.StNum = splits[j].split.num;
	if ((cptr =
	     bsearch (&comp, comps, comp_num, sizeof (struct Comps),
		      comp_num_cmp)) == NULL
	    || cptr->comp.GroupCode != groups[i].group.Code)
		continue;

	  team.Code = cptr->comp.TeamCode;
	  if ((tptr = bsearch (&team, teams, team_num, sizeof (struct Team),
			       team_cmp)) == NULL)
	    continue;

	    csplits[k].comp = cptr;
	    csplits[k].team = tptr;
	    csplits[k].split = splits + j;
	    csplits[k].tsp = tsps + splits[j].tsp_offset * CHIP_RECS;
	    csplits[k].dist = dptr;
	    csplits[k].cday = cdays + cptr->day_offset * title->DaysNum + day;
	    k++;
    }
    qsort (csplits, k, sizeof (struct CSplit), csplit_cmp);
    for (j = 0; j < k; j++) {
      int start = 0, last = 0;
      csplits[j].cday->Position = j + 1;

      if (! csplits[j].cday->dsq) {
         printf (" %d %d %s %s %s ", j + 1,
	      csplits[j].comp->comp.StNum,
	      finalize (csplits[j].comp->comp.Family),
	      finalize (csplits[j].comp->comp.Name),
	      finalize (csplits[j].team->Name));

	start = last = csplits[j].cday->Start;
	printf ("lap: ");
	for (n = 0; n < csplits[j].split->split.recs && n < CHIP_RECS; n++)
	  if (csplits[j].tsp[n].check > 0 || csplits[j].tsp[n].tm == csplits[j].cday->Finish)
	    printf (" %s",
		    totime (csplits[j].tsp[n].tm - start));

	puts ("");
	printf ("split: ");
	for (n = 0; n < csplits[j].split->split.recs && n < CHIP_RECS; n++)
	  if (csplits[j].tsp[n].check > 0 || csplits[j].tsp[n].tm == csplits[j].cday->Finish) {
	    printf (" %s",
		    totime (csplits[j].tsp[n].tm - last));
	    last = csplits[j].tsp[n].tm;
	  }

	puts ("");
	printf ("speed: ");
	last = csplits[j].cday->Start;

	for (n = 0; n < csplits[j].split->split.recs && n < CHIP_RECS; n++)
	  if (csplits[j].dist->CPD[n - 1] > 0
	      && (csplits[j].tsp[n].check > 0 || csplits[j].tsp[n].tm == csplits[j].cday->Finish)) {
	    printf (" %s",
		    totime ((csplits[j].tsp[n].tm -
			   last) * 1000 / csplits[j].dist->CPD[n - 1]));
	    last = csplits[j].tsp[n].tm;
	  }
	puts ("");


      }
    }
    free (csplits);
  }

  close (fd);

  free (splits);
  free (tsps);
  free (cdays);
  free (comps);
  free (teams);
  free (cpoints);
  free (distances);
  free (gdays);
  free (groups);
  free (offsets);
  free (days);
  free (title);

  return 0;
}


