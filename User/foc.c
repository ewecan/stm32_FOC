#include "foc.h"

ExtU rtU;
ExtY rtY;

/* Model step function */
void foc_step(void) {
    int sector;
    float T1;
    float rtb_ualpha;
    float rtb_ubeta;

    T1 = sinf(rtU.theta);
    rtb_ubeta = cosf(rtU.theta);
    rtb_ualpha = (float) rtU.ud * rtb_ubeta - (float) rtU.uq * T1;
    rtb_ubeta = (float) rtU.ud * T1 + (float) rtU.uq * rtb_ubeta;

    sector = 0;

    if (rtb_ubeta > 0.0F) {
        sector = 1;
    }

    if ((1.73205078F * rtb_ualpha - rtb_ubeta) / 2.0F > 0.0F) {
        sector += 2;
    }

    if ((-1.73205078F * rtb_ualpha - rtb_ubeta) / 2.0F > 0.0F) {
        sector += 4;
    }

    switch (sector) {
        case 1:
            T1 = (-1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc);
            rtb_ualpha = (1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc);
            break;

        case 2:
            T1 = (1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc);
            rtb_ualpha = -(1.73205078F * rtb_ubeta * rtU.Tpwm / rtU.udc);
            break;

        case 3:
            T1 = -((-1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc));
            rtb_ualpha = 1.73205078F * rtb_ubeta * rtU.Tpwm / rtU.udc;
            break;

        case 4:
            T1 = -(1.73205078F * rtb_ubeta * rtU.Tpwm / rtU.udc);
            rtb_ualpha = (-1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc);
            break;

        case 5:
            T1 = 1.73205078F * rtb_ubeta * rtU.Tpwm / rtU.udc;
            rtb_ualpha = -((1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc));
            break;

        default:
            T1 = -((1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc));
            rtb_ualpha = -((-1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm /
                                                                              rtU.udc));
            break;
    }

    rtb_ubeta = T1 + rtb_ualpha;
    if (rtb_ubeta > rtU.Tpwm) {
        T1 /= rtb_ubeta;
        rtb_ualpha /= T1 + rtb_ualpha;
    }

    rtb_ubeta = (rtU.Tpwm - (T1 + rtb_ualpha)) / 4.0F;
    T1 = T1 / 2.0F + rtb_ubeta;
    switch (sector) {
        case 1:
            rtY.Tcmp1 = T1;
            rtY.Tcmp2 = rtb_ubeta;
            rtY.Tcmp3 = rtb_ualpha / 2.0F + T1;
            break;

        case 2:
            rtY.Tcmp1 = rtb_ubeta;
            rtY.Tcmp2 = rtb_ualpha / 2.0F + T1;
            rtY.Tcmp3 = T1;
            break;

        case 3:
            rtY.Tcmp1 = rtb_ubeta;
            rtY.Tcmp2 = T1;
            rtY.Tcmp3 = rtb_ualpha / 2.0F + T1;
            break;

        case 4:
            rtY.Tcmp1 = rtb_ualpha / 2.0F + T1;
            rtY.Tcmp2 = T1;
            rtY.Tcmp3 = rtb_ubeta;
            break;

        case 5:
            rtY.Tcmp1 = rtb_ualpha / 2.0F + T1;
            rtY.Tcmp2 = rtb_ubeta;
            rtY.Tcmp3 = T1;
            break;

        case 6:
            rtY.Tcmp1 = T1;
            rtY.Tcmp2 = rtb_ualpha / 2.0F + T1;
            rtY.Tcmp3 = rtb_ubeta;
            break;
    }

    rtY.sector = (float) sector;
}
