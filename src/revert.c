#include "revert.h"

void revert_title (struct Title *ptr)
{

  revert_l (ptr->Version);
  revert_l (ptr->TitleEnd);
  revert_l (ptr->DaysNum);
  revert_l (ptr->GroupsNum);
  revert_l (ptr->DistancesNum);
  revert_l (ptr->CPNum);
  revert_l (ptr->TeamsNum);
  revert_l (ptr->CompetitorsNum);
  revert_l (ptr->SplitsNum);
  revert_l (ptr->GroupCurrentCode);
  revert_l (ptr->DistanceCurrentCode);
  revert_l (ptr->CPCurrentCode);
  revert_l (ptr->TeamCurrentCode);
  revert_l (ptr->CompetitorCurrentCode);
  revert_l (ptr->SplitCurrentCode);
  revert_l (ptr->CompType);
  revert_l (ptr->CurrentDay);
  revert_l (ptr->Precision);
  revert_l (ptr->MidNight);
  revert_l (ptr->CheckPunchType);
}

void revert_offsets (struct Offsets *ptr)
{
  int i;
  for (i = 0; i < sizeof (ptr->Groups) / sizeof (int32_t); i++)
    revert_l (ptr->Groups[i]);
  for (i = 0; i < sizeof (ptr->Distances) / sizeof (int32_t); i++)
    revert_l (ptr->Distances[i]);
  for (i = 0; i < sizeof (ptr->CPoints) / sizeof (int32_t); i++)
    revert_l (ptr->CPoints[i]);
  for (i = 0; i < sizeof (ptr->Teams) / sizeof (int32_t); i++)
    revert_l (ptr->Teams[i]);
  for (i = 0; i < sizeof (ptr->Competitors) / sizeof (int32_t); i++)
    revert_l (ptr->Competitors[i]);
  for (i = 0; i < sizeof (ptr->Splits) / sizeof (int32_t); i++)
    revert_l (ptr->Splits[i]);
}

void revert_group (struct Group *ptr)
{
  revert_l (ptr->Code);
  revert_l (ptr->SBornYear);
  revert_l (ptr->EBornYear);
  revert_l (ptr->parent);
  revert_l (ptr->Money);
}

void revert_gday (struct GDayStr *ptr)
{
  revert_l (ptr->DistCode);
  revert_l (ptr->maxRazr);
  revert_l (ptr->mc);
  revert_l (ptr->kmc);
  revert_l (ptr->checktime);
  revert_l (ptr->MaxBals);
}

void revert_distance (struct Distance *ptr)
{
  int i;
  revert_l (ptr->Code);
  revert_l (ptr->Number);
  revert_l (ptr->Day);
  revert_l (ptr->MaxBall);
  revert_l (ptr->Sequence);
  revert_l (ptr->Height);
  revert_l (ptr->Length);
  revert_l (ptr->CPNum);
  for (i = 0; i < sizeof (ptr->CP) / sizeof (int32_t); i++)
    revert_l (ptr->CP[i]);
  for (i = 0; i < sizeof (ptr->CPD) / sizeof (int32_t); i++)
    revert_l (ptr->CPD[i]);
}

void revert_cpoint (struct CPoint *ptr)
{
  revert_l (ptr->Code);
  revert_l (ptr->num);
  revert_l (ptr->Station);
  revert_l (ptr->X);
  revert_l (ptr->Y);
  revert_l (ptr->check);
}

void revert_team (struct Team *ptr)
{
  revert_l (ptr->Code);
  revert_l (ptr->parent);
  revert_l (ptr->Money);
}

void revert_cday (struct CDayStr *ptr)
{
  revert_l (ptr->Start);
  revert_l (ptr->Finish);
  revert_l (ptr->Bonus);
  revert_l (ptr->Position);
  revert_l (ptr->IsStart);
  revert_l (ptr->Penalty);
  revert_l (ptr->Ball);
  revert_l (ptr->Score);
  revert_l (ptr->dsq);
}

void revert_competitor (struct Competitor *ptr)
{
  revert_l (ptr->Code);
  revert_l (ptr->StNum);
  revert_l (ptr->GroupCode);
  revert_l (ptr->TeamCode);
  revert_l (ptr->Qualif);
  revert_l (ptr->BornYear);
  revert_l (ptr->chipNum);
  revert_l (ptr->Money);
  revert_l (ptr->type);
}

void revert_tsp (struct Tsp *ptr)
{
  revert_s (ptr->kp);
  revert_s (ptr->check);
  revert_l (ptr->tm);
}

void revert_split (struct Split *ptr)
{
  revert_l (ptr->Code);
  revert_l (ptr->num);
  revert_l (ptr->group);
  revert_l (ptr->chip);
  revert_l (ptr->day);
  revert_l (ptr->recs);
}
