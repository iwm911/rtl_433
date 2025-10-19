/** @file
    Declaration of all available decoders.
*/

#ifndef INCLUDE_RTL_433_DEVICES_H_
#define INCLUDE_RTL_433_DEVICES_H_

#include "r_device.h"

#define DEVICES \
    DECL(new_template) \
    DECL(generic_remote) \
    DECL(generic_temperature_sensor) \
    DECL(generic_motion) \
    DECL(tpms_citroen) \
    DECL(tpms_toyota) \
    DECL(tpms_ford) \
    DECL(tpms_renault) \
    DECL(tpms_pmv107j) \
    DECL(tpms_jansite) \
    DECL(tpms_elantra2012) \
    DECL(tpms_abarth124) \
    DECL(tpms_jansite_solar) \
    DECL(tpms_hyundai_vdo) \
    DECL(tpms_truck) \
    DECL(tpms_porsche) \
    DECL(tpms_ave) \
    DECL(tpms_renault_0435r) \
    DECL(tpms_tyreguard400) \
    DECL(tpms_kia) \
    DECL(tpms_eezrv) \
    DECL(tpms_nissan) \
    DECL(tpms_bmw) \
    DECL(tpms_bmwg3) \
    DECL(tpms_gm) \

    /* Add new decoders here. */

#define DECL(name) extern r_device name;
DEVICES
#undef DECL

#endif /* INCLUDE_RTL_433_DEVICES_H_ */
