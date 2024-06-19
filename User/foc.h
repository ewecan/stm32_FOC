#ifndef RTW_HEADER_foc_h_
#define RTW_HEADER_foc_h_
#include <math.h>

typedef struct {
  double ud,uq;
  float theta, udc,Tpwm;
} ExtU;

typedef struct {
  float Tcmp1,Tcmp2,Tcmp3;
  float sector;
} ExtY;
  
extern ExtU rtU;
extern ExtY rtY;
extern void foc_step(void);

#endif                                 /* RTW_HEADER_foc_h_ */
