#include "easypap.h"

#include <stdbool.h>
#include <immintrin.h>

static long unsigned int *TABLE = NULL, *TABLE_NEXT = NULL;
static unsigned int *TABLE_INT = NULL, *TABLE_NEXT_INT = NULL;
static bool *tab_unstable;

static volatile int changement;

static unsigned long int max_grains;

#define table_next(i, j) TABLE_NEXT[(i)*DIM + (j)]
#define table(i, j) TABLE[(i)*DIM + (j)]
#define int_table_next(i, j) TABLE_NEXT_INT[(i)*DIM + (j)]
#define int_table(i, j) TABLE_INT[(i)*DIM + (j)]
#define unstable(i,j) tab_unstable[(i)*(GRAIN+2)+(j)]

#define RGB(r, v, b) (((r) << 24 | (v) << 16 | (b) << 8) | 255)

void sable_init ()
{
  TABLE = calloc (DIM * DIM, sizeof (long unsigned int));
  TABLE_NEXT = calloc (DIM * DIM, sizeof (long unsigned int));
  TABLE_INT = calloc (DIM * DIM, sizeof (unsigned int));
  TABLE_NEXT_INT = calloc (DIM * DIM, sizeof (unsigned int));
  tab_unstable = malloc((GRAIN+2) * (GRAIN+2) * sizeof(int));

  for (int i = 1; i < GRAIN + 1; i++){
    for (int j = 1; j < GRAIN + 1; j++){
      unstable(i,j) = 1;
    }
  }

  for (int i = 0; i < GRAIN + 2; i++){
    for (int j = 0; j < GRAIN + 2; j+=GRAIN+1){
      unstable(i,j) = 0;
    }
  }

  for (int i = 0; i < GRAIN + 2; i+=GRAIN+1){
    for (int j = 1; j < GRAIN + 1; j++){
      unstable(i,j) = 0;
    }
  }
}

void sable_finalize ()
{
  free (TABLE);
  free (TABLE_NEXT);
  free (TABLE_INT);
  free (TABLE_NEXT_INT);
  free(tab_unstable);
}

///////////////////////////// Production d'une image
void sable_refresh_img ()
{
  unsigned long int max = 0;
  for (int i = 1; i < DIM - 1; i++)
    for (int j = 1; j < DIM - 1; j++) {
      //int g = int_table (i, j);           //Pour afficher l'iamge avec l'initialisation avec 4partouti
      int g = table(i,j);
      int r, v, b;
      r = v = b = 0;
      if (g == 1)
        v = 255;
      else if (g == 2)
        b = 255;
      else if (g == 3)
        r = 255;
      else if (g == 4)
        r = v = b = 255;
      else if (g > 4)
        r = b = 255 - (240 * ((double)g) / (double)max_grains);

      cur_img (i, j) = RGB (r, v, b);
      if (g > max)
        max = g;
    }
  max_grains = max;
}

///////////////////////////// Configurations initiales

static void sable_draw_4partout (void);

void sable_draw (char *param)
{
  // Call function ${kernel}_draw_${param}, or default function (second
  // parameter) if symbol not found
  hooks_draw_helper (param, sable_draw_4partout);
}

void sable_draw_4partout (void)
{
  max_grains = 8;
  for (int i = 1; i < DIM - 1; i++)
    for (int j = 1; j < DIM - 1; j++)
      table (i, j) = 4;
}

void sable_draw_4partouti (void)
{
  max_grains = 8;
  for (int i = 1; i < DIM - 1; i++)
    for (int j = 1; j < DIM - 1; j++)
      int_table (i, j) = 4;
}

void sable_draw_DIM (void)
{
  max_grains = DIM;
  for (int i = DIM / 4; i < DIM - 1; i += DIM / 4)
    for (int j = DIM / 4; j < DIM - 1; j += DIM / 4)
      table (i, j) = i * j / 4;
}

void sable_draw_alea (void)
{
  max_grains = 5000;
  for (int i = 0; i<DIM>> 3; i++) {
    table (1 + random () % (DIM - 2), 1 + random () % (DIM - 2)) =
        1000 + (random () % (4000));
  }
}

static inline void swap_tab (void)
{
  long unsigned int *tmp = TABLE;

  TABLE     = TABLE_NEXT;
  TABLE_NEXT = tmp;
}

static inline void swap_tab_int (void)
{
  unsigned int *tmp = TABLE_INT;

  TABLE_INT     = TABLE_NEXT_INT;
  TABLE_NEXT_INT = tmp;
}

///////////////////////////// Version seq simple (seq)

static inline void compute_new_state (int y, int x)
{
  if (table (y, x) >= 4) {
    unsigned long int div4 = table (y, x) / 4;
    table (y, x - 1) += div4;
    table (y, x + 1) += div4;
    table (y - 1, x) += div4;
    table (y + 1, x) += div4;
    table (y, x) %= 4;
    changement = 1;
  }

}

static inline int compute_new_state_stable (int y, int x)
{
  int change = 0;
  if (table (y, x) >= 4) {
    unsigned long int div4 = table (y, x) / 4;
    table (y, x - 1) += div4;
    table (y, x + 1) += div4;
    table (y - 1, x) += div4;
    table (y + 1, x) += div4;
    table (y, x) %= 4;
    change = 1;
  }
  return change;
}

static inline int compute_new_state_vect (int x, int y)
{
  table_next(x,y) =  table(x,y)%4 + table(x-1,y)/4 + table(x+1,y)/4 + table(x,y-1)/4 + table(x,y+1)/4;;
  return (table(x,y) != table_next(x,y));
}

static inline int compute_new_state_int (int x, int y)
{
  int_table_next(x,y) = int_table(x,y)%4 + int_table(x-1,y)/4 + int_table(x+1,y)/4 + int_table(x,y-1)/4 + int_table(x,y+1)/4;;
  return (int_table(x,y) != int_table_next(x,y));
}

static inline __m256i compute_vect_long (int i, int j){

  __m256i bot = _mm256_loadu_si256((__m256i *)&table(i+1,j));
  __m256i curr = _mm256_loadu_si256 ((__m256i *)&table(i,j));
  __m256i right = _mm256_loadu_si256((__m256i *)&table(i,j+1));
  __m256i left = _mm256_loadu_si256((__m256i *)&table(i,j-1));
  __m256i top = _mm256_loadu_si256((__m256i *)&table(i-1,j));

  right = _mm256_srli_epi64(right,2);
  left = _mm256_srli_epi64(left,2);
  bot = _mm256_srli_epi64(bot,2);
  top = _mm256_srli_epi64(top,2);

  __m256i res = curr;
  res = _mm256_slli_epi64(res,62);
  res = _mm256_srli_epi64(res,62);

  res = _mm256_add_epi64(res,right);
  res = _mm256_add_epi64(res,left);
  res = _mm256_add_epi64(res,bot);
  res = _mm256_add_epi64(res,top);
  _mm256_storeu_si256 ((__m256i *)&table_next(i, j), res);

  __m256i ret = _mm256_xor_si256(res,curr);

  return ret;
}

static inline __m256i compute_vect_int (int i, int j){

  __m256i bot   = _mm256_loadu_si256((__m256i *)&int_table(i+1,j));
  __m256i curr  = _mm256_loadu_si256 ((__m256i *)&int_table(i,j));
  __m256i right = _mm256_loadu_si256((__m256i *)&int_table(i,j+1));
  __m256i left  = _mm256_loadu_si256((__m256i *)&int_table(i,j-1));
  __m256i top   = _mm256_loadu_si256((__m256i *)&int_table(i-1,j));

  right = _mm256_srli_epi32(right,2);
  left  = _mm256_srli_epi32(left,2);
  bot   = _mm256_srli_epi32(bot,2);
  top   = _mm256_srli_epi32(top,2);

  __m256i res = curr;
  res = _mm256_slli_epi32(res,30);
  res = _mm256_srli_epi32(res,30);

  res = _mm256_add_epi32(res,right);
  res = _mm256_add_epi32(res,left);
  res = _mm256_add_epi32(res,bot);
  res = _mm256_add_epi32(res,top);
  _mm256_storeu_si256 ((__m256i *)&int_table_next(i, j), res);

  __m256i ret = _mm256_xor_si256(curr,res);

  return ret;
}

static inline __m512i compute_vect512_long (int i, int j){

  __m512i bot   = _mm512_loadu_si512((__m512i *)&table(i+1,j));
  __m512i curr  = _mm512_loadu_si512 ((__m512i *)&table(i,j));
  __m512i right = _mm512_loadu_si512((__m512i *)&table(i,j+1));
  __m512i left  = _mm512_loadu_si512((__m512i *)&table(i,j-1));
  __m512i top   = _mm512_loadu_si512((__m512i *)&table(i-1,j));

  right = _mm512_srli_epi64(right,2);
  left  = _mm512_srli_epi64(left,2);
  bot   = _mm512_srli_epi64(bot,2);
  top   = _mm512_srli_epi64(top,2);

  __m512i res = curr;
  res = _mm512_slli_epi64(res,62);
  res = _mm512_srli_epi64(res,62);

  res = _mm512_add_epi64(res,right);
  res = _mm512_add_epi64(res,left);
  res = _mm512_add_epi64(res,bot);
  res = _mm512_add_epi64(res,top);

  _mm512_storeu_si512 ((__m512i *)&table_next(i, j), res);
  __m512i ret = _mm512_xor_si512(curr,res);

  return ret;
}

static inline __m512i compute_vect512_int (int i, int j){

  __m512i bot   = _mm512_loadu_si512((__m512i *)&int_table(i+1,j));
  __m512i curr  = _mm512_loadu_si512 ((__m512i *)&int_table(i,j));
  __m512i right = _mm512_loadu_si512((__m512i *)&int_table(i,j+1));
  __m512i left  = _mm512_loadu_si512((__m512i *)&int_table(i,j-1));
  __m512i top   = _mm512_loadu_si512((__m512i *)&int_table(i-1,j));

  right = _mm512_srli_epi32(right,2);
  left  = _mm512_srli_epi32(left,2);
  bot   = _mm512_srli_epi32(bot,2);
  top   = _mm512_srli_epi32(top,2);

  __m512i res = curr;
  res = _mm512_slli_epi32(res,30);
  res = _mm512_srli_epi32(res,30);

  res = _mm512_add_epi32(res,right);
  res = _mm512_add_epi32(res,left);
  res = _mm512_add_epi32(res,bot);
  res = _mm512_add_epi32(res,top);

  _mm512_storeu_si512 ((__m512i *)&int_table_next(i, j), res);
  __m512i ret = _mm512_xor_si512(curr,res);

  return ret;
}


static void do_tile (int x, int y, int width, int height, int who)
{
  PRINT_DEBUG ('c', "tuile [%d-%d][%d-%d] traitÃ©e\n", x, x + width - 1, y,
               y + height - 1);

  monitoring_start_tile (who);
  for (int i = y; i < y + height; i++)
    for (int j = x; j < x + width; j++) {
      compute_new_state (i, j);

  }
  monitoring_end_tile (x, y, width, height, who);
}

static bool do_tile_unstable(int x, int y, int width, int height, int who)
{
  int change = 0;

  PRINT_DEBUG ('c', "tuile [%d-%d][%d-%d] traitÃ©e\n", x, x + width - 1, y,
               y + height - 1);

  monitoring_start_tile (who);
  for (int i = y; i < y + height; i++)
    for (int j = x; j < x + width; j++)
      change += compute_new_state_stable (i, j);

  monitoring_end_tile (x, y, width, height, who);

  if(change == 0){
    return 0;
  }
  return 1;
}

static int do_tile_stable(int x, int y, int width, int height, int who)
{
  int change = 0;

  PRINT_DEBUG ('c', "tuile [%d-%d][%d-%d] traitÃ©e\n", x, x + width - 1, y,
               y + height - 1);

  monitoring_start_tile (who);
  for (int i = y; i < y + height; i++)
    for (int j = x; j < x + width; j += (width - 1))
      change += compute_new_state_stable (i, j);

  for (int i = y; i < y + height ; i += (height - 1))
    for (int j = x; j < x + width; j ++)
      change += compute_new_state_stable (i, j);

  monitoring_end_tile (x, y, width, height, who);
  if(change == 0){
    return 0;
  }
 return 1;
}

static int do_tile_vect_int (int x, int y, int width, int height, int who)
{
  PRINT_DEBUG ('c', "tuile [%d-%d][%d-%d] traitÃ©e\n", x, x + width - 1, y,
               y + height - 1);

  monitoring_start_tile (who);

  __m256i zero = _mm256_setzero_si256 ();
  __m256i change_vect = zero;
  __m256i res;
  int change = 0;
  int reste = width%8;

  for (int i = y; i < y + height; i++){
    for (int j = x; j < x + width - reste; j+=8) {
      res = compute_vect_int (i, j);
      change_vect = _mm256_or_si256(res, change_vect);
    }
    int j = x + width - reste;
    for(int k = 0;k<reste;k++)
      change += compute_new_state_int(i,j+k);
  }

  monitoring_end_tile (x, y, width, height, who);

  int ret = _mm256_testc_si256(zero,change_vect);

  if (change > 0 || ret == 0)
    return 1;
  return 0;
}

static int do_tile_vect_long (int x, int y, int width, int height, int who)
{
  PRINT_DEBUG ('c', "tuile [%d-%d][%d-%d] traitÃ©e\n", x, x + width - 1, y,
               y + height - 1);

  monitoring_start_tile (who);

  __m256i zero = _mm256_setzero_si256();
  __m256i change_vect = zero;
  __m256i res;
  int change = 0;
  int reste = width%4;

  for (int i = y; i < y + height; i++){
    for (int j = x; j < x + width - reste; j+=4) {
      res = compute_vect_long (i, j);
      change_vect = _mm256_or_si256(res, change_vect);
    }
    int j = x + width - reste;
    for(int k = 0;k<reste;k++)
      change += compute_new_state_vect(i,j+k);
  }

  monitoring_end_tile (x, y, width, height, who);

  int ret = _mm256_testc_si256(zero, change_vect);

  if (change > 0 || ret == 0)
    return 1;
  return 0;
}

static int do_tile_vect512_int (int x, int y, int width, int height, int who)
{
  PRINT_DEBUG ('c', "tuile [%d-%d][%d-%d] traitÃ©e\n", x, x + width - 1, y,
               y + height - 1);

  monitoring_start_tile (who);

  int change = 0;
  int reste = width%16;
  __m256i zero   = _mm256_setzero_si256 ();
  __m512i change_vect = _mm512_setzero_si512 ();
  __m512i res;

  for (int i = y; i < y + height; i++){
    for (int j = x; j < x + width - reste; j+=16) {
      res = compute_vect512_int (i, j);
      change_vect = _mm512_or_si512 (change_vect, res);
    }
    int j = x + width - reste;
    for(int k = 0;k<reste;k++)
      change += compute_new_state_int(i,j+k);
  }

  __m256i change_vect256 = _mm512_cvtsepi64_epi32 (change_vect);

  int ret  = _mm256_testc_si256(zero,change_vect256);

  monitoring_end_tile (x, y, width, height, who);

  if (change > 0 || (ret == 0))
    return 1;
  return 0;
}

static int do_tile_vect512_long (int x, int y, int width, int height, int who)
{
  PRINT_DEBUG ('c', "tuile [%d-%d][%d-%d] traitÃ©e\n", x, x + width - 1, y,
               y + height - 1);

  monitoring_start_tile (who);

  int change = 0;
  int reste = width%8;
  __m256i zero   = _mm256_setzero_si256 ();
  __m512i change_vect = _mm512_setzero_si512 ();
  __m512i res;

  for (int i = y; i < y + height; i++){
    for (int j = x; j < x + width - reste; j+=8) {
      res = compute_vect512_long (i, j);
      change_vect = _mm512_or_si512 (change_vect, res);
    }
    int j = x + width - reste;
    for(int k = 0;k<reste;k++)
      change += compute_new_state_vect(i,j+k);
  }

  __m256i change_vect256 = _mm512_cvtsepi64_epi32 (change_vect);

  int ret  = _mm256_testc_si256(zero,change_vect256);

  monitoring_end_tile (x, y, width, height, who);

  if (change > 0 || ret == 0)
    return 1;
  return 0;
}

static int do_tile_stable_vect(int x, int y, int width, int height, int who)
{

  PRINT_DEBUG ('c', "tuile [%d-%d][%d-%d] traitÃ©e\n", x, x + width - 1, y,
               y + height - 1);

  monitoring_start_tile (who);

  int change = 0;
  int reste = width%8;
  __m256i zero   = _mm256_setzero_si256 ();
  __m512i change_vect = _mm512_setzero_si512 ();
  __m512i res;

  for (int i = y + 1; i < y + height - 1; i++)
    for (int j = x; j < x + width; j += (width - 1))
      change += compute_new_state_vect (i, j);

  for (int i = y; i < y + height ; i += (height - 1)){
    for (int j = x; j < x + width - reste; j+=8) {
      res = compute_vect512_long (i, j);
      change_vect = _mm512_or_si512 (change_vect, res);
    }
    int j = x + width - reste;
    for(int k = 0;k<reste;k++)
      change += compute_new_state_vect(i,j+k);
  }

  __m256i res1 = _mm512_cvtsepi64_epi32 (change_vect);

  int ret  = _mm256_testc_si256(zero,res1);

  monitoring_end_tile (x, y, width, height, who);

  if (change > 0 || ret == 0)
    return 1;
  return 0;
}


// Renvoie le nombre d'itérations effectuées avant stabilisation, ou 0
unsigned sable_compute_seq (unsigned nb_iter)
{

  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;
    // On traite toute l'image en un coup (oui, c'est une grosse tuile)
    do_tile (1, 1, DIM - 2, DIM - 2, 0);
    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_seq_vect_int (unsigned nb_iter)
{

  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;
    changement = do_tile_vect_int(1, 1, DIM - 2, DIM - 2, 0);

    swap_tab_int();
    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_seq_vect512_int (unsigned nb_iter)
{

  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;
    changement = do_tile_vect512_int(1, 1, DIM - 2, DIM - 2, 0);

    swap_tab_int();
    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_seq_vect_long (unsigned nb_iter)
{

  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;
    changement = do_tile_vect_long(1, 1, DIM - 2, DIM - 2, 0);

    swap_tab();
    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_seq_vect512_long (unsigned nb_iter)
{

  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;
    changement = do_tile_vect512_long(1, 1, DIM - 2, DIM - 2, 0);

    swap_tab();
    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_seq_unstable (unsigned nb_iter)
{

  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;
    changement = do_tile_unstable(1, 1, DIM - 2, DIM - 2, 0);

    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_vect_omp_int (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    #pragma omp parallel for reduction(+:changement)
    for (int y = 0; y < DIM; y += TILE_SIZE)
        changement += do_tile_vect_int(1, y + (y == 0),
                DIM - 2,
                TILE_SIZE - ((y + TILE_SIZE == DIM) + (y == 0)),
                omp_get_thread_num());

    swap_tab_int();

    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_vect512_omp_int (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    #pragma omp parallel for reduction(+:changement)
    for (int y = 0; y < DIM; y += TILE_SIZE)
        changement += do_tile_vect512_int(1, y + (y == 0),
                DIM - 2,
                TILE_SIZE - ((y + TILE_SIZE == DIM) + (y == 0)),
                omp_get_thread_num());

    swap_tab_int();

    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_vect_omp_long (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    #pragma omp parallel for reduction (+:changement)
    for (int y = 0; y < DIM; y += TILE_SIZE)
        changement += do_tile_vect_long(1, y + (y == 0),
                DIM - 2,
                TILE_SIZE - ((y + TILE_SIZE == DIM) + (y == 0)),
                omp_get_thread_num());

    swap_tab();

    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_vect512_omp_long (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    #pragma omp parallel for reduction (+:changement)
    for (int y = 0; y < DIM; y += TILE_SIZE)
        changement += do_tile_vect512_long(1, y + (y == 0),
                DIM - 2,
                TILE_SIZE - ((y + TILE_SIZE == DIM) + (y == 0)),
                omp_get_thread_num());

    swap_tab();

    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_omp (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    #pragma omp parallel
    {
      #pragma omp for reduction (+:changement)
      for (int y = 0; y < DIM; y += TILE_SIZE)
          changement += do_tile_unstable(1, y + (y == 0),
                  DIM - 2,
                  TILE_SIZE - ((y + TILE_SIZE == DIM) + (y == 0)),
                  omp_get_thread_num());

      #pragma omp for reduction (+:changement)
      for (int y = TILE_SIZE; y < DIM; y += 2*TILE_SIZE)
          changement += do_tile_unstable(1, y,
                  DIM - 2,
                  TILE_SIZE - (y + TILE_SIZE == DIM),
                  omp_get_thread_num());
    }

    if (changement == 0)
      return it;
  }
  return 0;
}

///////////////////////////// Version seq tuilée (tiled)

unsigned sable_compute_tiled (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    for (int y = 0; y < DIM; y += TILE_SIZE)
      for (int x = 0; x < DIM; x += TILE_SIZE){
        do_tile (x + (x == 0), y + (y == 0),
                 TILE_SIZE - ((x + TILE_SIZE == DIM) + (x == 0)),
                 TILE_SIZE - ((y + TILE_SIZE == DIM) + (y == 0)),
                 0 );
      }

    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_tiled_stable (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    for (int i = 1; i < GRAIN + 1; i+=1)
        for (int j = 1; j < GRAIN + 1; j+=1){;
            if(!unstable(i,j)){
              if(unstable(i-1,j) || unstable(i+1,j) || unstable(i,j-1) || unstable(i,j+1))
                unstable(i,j) = do_tile_stable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
            }else{
              unstable(i,j) = do_tile_unstable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
              changement += unstable(i,j);
            }
        }

    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_tiled_stable_vect (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    for (int i = 1; i < GRAIN + 1; i+=1)
        for (int j = 1; j < GRAIN + 1; j+=1){
          if(!unstable(i,j)){
            if(unstable(i-1,j) || unstable(i+1,j) || unstable(i,j-1) || unstable(i,j+1))
              unstable(i,j) = do_tile_stable_vect ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                    TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                    TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                    0);
          }else{
            unstable(i,j) = do_tile_vect512_long ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                    TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                    TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                    0);
            changement += unstable(i,j);
          }
      }

    swap_tab();
    if (changement == 0)
      return it;
  }
  return 0;
}

unsigned sable_compute_tiled_stable_omp (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    #pragma omp parallel
    {
      #pragma omp for collapse(2) reduction(+:changement)
      for (int i = 1; i < GRAIN + 1; i+=2)
        for (int j = 1; j < GRAIN + 1; j+=2){
            if(!unstable(i,j)){
              if(unstable(i-1,j) || unstable(i+1,j) || unstable(i,j-1) || unstable(i,j+1))
                unstable(i,j) = do_tile_stable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
            }else{
              unstable(i,j) = do_tile_unstable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
              changement += unstable(i,j);
            }
        }

      #pragma omp for collapse(2) reduction(+:changement)
      for (int i = 2; i <GRAIN+1; i+=2)
        for (int j = 1; j < GRAIN+1; j+=2){
            if(!unstable(i,j)){
              if(unstable(i-1,j) || unstable(i+1,j) || unstable(i,j-1) || unstable(i,j+1))
                unstable(i,j) = do_tile_stable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
            }else{
              unstable(i,j) = do_tile_unstable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
              changement += unstable(i,j);
            }
        }

      #pragma omp for collapse(2) reduction(+:changement)
      for (int i = 1; i <GRAIN+1; i+=2)
        for (int j = 2; j < GRAIN+1; j+=2){
            if(!unstable(i,j)){
              if(unstable(i-1,j) || unstable(i+1,j) || unstable(i,j-1) || unstable(i,j+1))
                unstable(i,j) = do_tile_stable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
            }else{
              unstable(i,j) = do_tile_unstable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
              changement += unstable(i,j);
            }
        }

      #pragma omp for collapse(2) reduction(+:changement)
      for (int i = 2; i <GRAIN+1; i+=2)
        for (int j = 2; j < GRAIN+1; j+=2){
            if(!unstable(i,j)){
              if(unstable(i-1,j) || unstable(i+1,j) || unstable(i,j-1) || unstable(i,j+1))
                unstable(i,j) = do_tile_stable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
            }else{
              unstable(i,j) = do_tile_unstable ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                      TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                      TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                      omp_get_thread_num());
              changement += unstable(i,j);
            }
        }
    }

    if (changement == 0)
      return it;
  }

  return 0;
}

unsigned sable_compute_tiled_omp_vect (unsigned nb_iter)
{
  for (unsigned it = 1; it <= nb_iter; it++) {
    changement = 0;

    #pragma omp parallel for collapse(2)
    for (int i = 1; i < GRAIN + 1; i+=1)
      for (int j = 1; j < GRAIN + 1; j+=1){
          if(!unstable(i,j)){
            if(unstable(i-1,j) || unstable(i+1,j) || unstable(i,j-1) || unstable(i,j+1))
              unstable(i,j) = do_tile_stable_vect ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                    TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                    TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                    0);
          }else{
            unstable(i,j) = do_tile_vect512_long ((j-1) * TILE_SIZE + ((j-1)==0), (i-1) * TILE_SIZE + ((i-1)==0),
                                    TILE_SIZE - ((((j-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((j-1) == 0)),
                                    TILE_SIZE - ((((i-1) *TILE_SIZE) + TILE_SIZE == DIM) + ((i-1) == 0)),
                                    0);
            changement += unstable(i,j);
          }
      }

    swap_tab();
    if (changement == 0)
      return it;
  }

  return 0;
}

unsigned sable_invoke_ocl (unsigned nb_iter)
{
  size_t global[2] = {SIZE,SIZE};
  size_t local[2] = {TILEX, TILEY};
  int TABLE_READ[SIZE/TILEX * SIZE/TILEY];
  int TABLE_WRITE[SIZE/TILEX * SIZE/TILEY];

  cl_int err;
  cl_mem diff;
  cl_mem instable;
  cl_mem stable;


  int ret[1];
  ret[0] = 0;

  if(index_it == 0){
    for(int i=0; i<SIZE/TILEX * SIZE/TILEY;i++){
      TABLE_READ[i] = 0;
      TABLE_WRITE[i] = 1;
    }
    diff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, NULL);
    instable = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof (int) * SIZE/TILEX * SIZE/TILEY, NULL, NULL);
    stable = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof (int) * SIZE/TILEX * SIZE/TILEY, NULL, NULL);
    err = clEnqueueWriteBuffer (queue, cur_buffer, CL_TRUE, 0, sizeof (int) * DIM * DIM, TABLE_INT, 0, NULL, NULL);
    check (err, "Failed to write to extra buffer");
    err = clEnqueueWriteBuffer (queue, instable, CL_TRUE, 0, sizeof (int) * SIZE/TILEX * SIZE/TILEY, TABLE_READ,0, NULL, NULL);
    check (err, "Failed to write to extra buffer");
    err = clEnqueueWriteBuffer (queue, stable, CL_TRUE, 0, sizeof (int) * SIZE/TILEX * SIZE/TILEY, TABLE_WRITE,0, NULL, NULL);
    check (err, "Failed to write to extra buffer");
  }

  err = clEnqueueWriteBuffer (queue, diff, CL_TRUE, 0, sizeof(int), ret, 0, NULL, NULL);
  check (err, "Failed to write to extra buffer");

  for (unsigned it = 1; it <= nb_iter; it++) {
    err = 0;
    err |= clSetKernelArg (compute_kernel, 0, sizeof (cl_mem), &cur_buffer);
    err |= clSetKernelArg (compute_kernel, 1, sizeof (cl_mem), &next_buffer);
    err |= clSetKernelArg (compute_kernel, 2, sizeof (cl_mem), &diff);
    err |= clSetKernelArg (compute_kernel, 3, sizeof (cl_mem), &instable);
    err |= clSetKernelArg (compute_kernel, 4, sizeof (cl_mem), &stable);

    check (err, "Failed to set kernel arguments");
    err = clEnqueueNDRangeKernel (queue, compute_kernel, 2, NULL, global, local, 0, NULL, NULL);
    check (err, "Failed to execute kernel");
    if(index_it%10==0){
      err = clEnqueueReadBuffer( queue, diff, CL_TRUE, 0, sizeof(unsigned), ret, NULL, NULL, NULL );
      check (err, "Failed to read the mask");
    }

    {
      if(index_it%10==0)
        if(ret[0] == 0){
          return it;
        }
      cl_mem tmp  = cur_buffer;
      cur_buffer = next_buffer;
      next_buffer = tmp;
      cl_mem tmp2  = instable;
      instable = stable;
      stable = tmp2;
    }

  }
  index_it ++;
  return 0;
}
