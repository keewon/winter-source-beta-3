#include "random.h"

int rand_int(int left, int right)
{
  return  (int)(((double)rand() / RAND_MAX )*(double)(right-left + 1) + (double)left);
}

int random(int right)
{
  return  (int)(((double)rand() / RAND_MAX )*(double)(right));
}

void randomize_timer()
{
  srand(time(0));
  return;
}

void randomize ()
{
	randomize_timer();
}