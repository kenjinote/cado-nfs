/* program to determine the size of the L1 cache */

/* we assume the L1 cache has size 2^k */
#define L1_NUM 11

#define N 1000000000

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int
main ()
{
  unsigned long T[L1_NUM][2] = {{10, 1021}, {11, 2039}, {12, 4093},
                                {13, 8191}, {14, 16381}, {15, 32749},
                                {16, 65521}, {17, 131071}, {18, 262139},
                                {19, 524287}, {20, 1048573}};
  unsigned long i, j, k, p, q, n, mask, L1 = 0;
  uint64_t t, mintime = ~((uint64_t) 0);
  char *s;

  for (i = 0; i < L1_NUM; i++)
    {
      n = 1 << T[i][0];
      p = T[i][1];
      s = malloc (n * sizeof (char));
      for (j = 0; j < n; j++)
        s[j] = 0;
      mask = n - 1;
      /* We compute k(j) = 2*j + 3*j^2 mod n.
         We have k(j+1) - k(j) = 6*j + 5. */
      t = microseconds ();
      for (j = k = 0, q = 5; j < N; j++)
        {
          /* invariant: q = 6j+5 */
          s[k] ++;
          k = (k + q) & mask;
          q += 6;
        }
      t = microseconds () - t;
      fprintf (stderr, "size=%lu time=%lu\n", n, t);
      if (t < mintime)
        mintime = t;
      if (t < mintime + (mintime / 10))
        L1 = n;
      free (s);
    }
  printf ("#define L1_CACHE_SIZE %lu\n", L1);
  return 0;
}
