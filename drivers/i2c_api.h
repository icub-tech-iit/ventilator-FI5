#ifndef __I2C_API_H__
#define __I2C_API_H__

typedef struct {
	enum {READ, WRITE} direction;
	int len;
	char *buf;
} i2c_xfer_t;

typedef struct {
	int xfer_num;
	i2c_xfer_t *xfers;
} i2c_xfer_list_t;

#endif
