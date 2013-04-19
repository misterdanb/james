#ifndef _tools_h_
#define _tools_h_

#define GET_BIT(x, n) (((x) >> (n)) & 0x01)
#define SET_BIT(x, n, val) (((x) & (~(0x01 << (n)))) | ((val) ? (0x01 << (n)) : 0x00))

#define GET_HIGH(x) (((x) >> 8) & 0xFF)
#define GET_LOW(x) ((x) & 0xFF)

#define JOIN_BYTES(h, l) ((((h) & 0xFF) << 8) | ((l) & 0xFF))

#define GET_SIGNED_VALUE(x) (((x) & 0x7F) - ((x) & 0x80))

#define GET_ABSOLUTE_VALUE(x) ((x) < 0 ? ((x) * (-1)) : (x))

#endif
