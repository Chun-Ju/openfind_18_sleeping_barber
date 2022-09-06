#ifndef   _DEFINECONSTANT_H_
#define  _DEFINECONSTANT_H_

#define  BARBER_COUNT      3
#define  CHAIR_COUNT       10
#define  INT_MAX_DIG       10
#define  BENCH_STR_LEN     (INT_MAX_DIG + 1) * CHAIR_COUNT + 1// format:2,3,4,5,6,7,9,10\0

#define  COMMAND_LEN       256

#define  TRUE              1
#define  FALSE             0

#define  SUCCESS           0
#define  ERR_BASE          0
#define  ERR_SEM_OPEN      (ERR_BASE-1)
#define  ERR_ARGS          (ERR_BASE-2)
#define  ERR_PTHD_CREATE   (ERR_BASE-3)

#endif
