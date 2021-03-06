#include <nitro.h>
#include <nitroWiFi.h>

static char ca_name[] = "JP, Tokyo, Shinjuku, Ubiquitous, Headquarter, jey-s, jey-s@ubiquitous.co.jp";

static unsigned char    ca_modulo[] = {
    0xbd,0x3d,0x2b,0xc2,0x30,0x29,0xc3,0xe5,
    0xbe,0x73,0xc2,0xfb,0x00,0x2f,0x79,0x53,
    0xbf,0xdb,0x06,0xcc,0xd2,0x87,0x61,0xbb,
    0x58,0x4e,0x15,0xa3,0xb9,0x54,0x34,0x1f,
    0x84,0x1c,0x45,0x6a,0x35,0x28,0x92,0xc5,
    0xde,0x34,0x20,0x1e,0x4b,0x8b,0xe6,0xc5,
    0x2a,0x77,0xbb,0xaf,0x53,0xe2,0xe3,0x30,
    0xe2,0xb3,0x1e,0x76,0x93,0x71,0x9e,0xdd
};

static unsigned char    ca_exponent[] = {
    0x01,0x00,0x01
};

CPSCaInfo   ca_jeys = {
    ca_name,
    sizeof(ca_modulo),
    ca_modulo,
    sizeof(ca_exponent),
    ca_exponent
};
