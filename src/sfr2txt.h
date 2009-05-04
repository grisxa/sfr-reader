
struct Groups {
  struct Group group;
  int day_offset;
};
struct Comps {
  struct Competitor comp;
  int day_offset;
};
struct Splits {
  struct Split split;
  int tsp_offset;
};
struct CSplit {
  struct Comps *comp;
  struct Team *team;
  struct Splits *split;
  struct Tsp *tsp;
  struct CDayStr *cday;
  struct Distance *dist;
};
#define CHIP_RECS 64

#if !HAVE_BZERO && HAVE_MEMSET
# define bzero(buf, bytes)      ((void) memset (buf, 0, bytes))
#endif
