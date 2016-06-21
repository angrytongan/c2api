#ifndef C2API_PRIV_H
#define C2API_PRIV_H

#include "hidapi/hidapi.h"

#define HID_C2_BUFFSIZE     200

#define HID_C2_VENDOR_ID    0x17a4

typedef struct c2rower_st {
    unsigned int id;
    unsigned int vendor_id;
    unsigned int product_id;
    wchar_t *serial_number;

    hid_device *device;

} c2rower_t;

typedef struct c2ctx_st {
    unsigned int num_rowers;
    c2rower_t *rowers;
    int last_error;

} c2ctx_t;

#endif /* C2API_PRIV_H */
