#ifndef __I2C_API_H__
#define __I2C_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
	enum {READ, WRITE} direction;
	int len;
	uint8_t *buf;
} i2c_xfer_t;

typedef struct {
	int xfer_num;
	i2c_xfer_t *xfers;
} i2c_xfer_list_t;

typedef void (*i2c_xfer_cb_t)(int status, void *arg);

#ifdef __cplusplus
}
#endif

#endif
