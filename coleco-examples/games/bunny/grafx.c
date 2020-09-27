/********************************************/
/*                                          */
/*   GRAPHICS TABLES  -  WIN ICVGM v3.00    */
/*                                          */
/*        WARNING : RLE COMPRESSION         */
/*                                          */
/********************************************/

#include <coleco.h>

const u8 NAMERLE[] = {
  0x01, 0x24, 0x23, 0x9B, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x9B, 0x20, 0x03, 0x25, 0x26,
  0x24, 0x23, 0x86, 0x20, 0x00, 0x98, 0x93, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x87, 0x20, 0x0B,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x87, 0x20, 0x03, 0x25,
  0x26, 0x24, 0x23, 0x87, 0x20, 0x0B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95,
  0x96, 0x97, 0x87, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x93, 0x20, 0x00, 0x98, 0x86, 0x20, 0x03,
  0x25, 0x26, 0x24, 0x23, 0x8C, 0x20, 0x01, 0xD6, 0xD7, 0x8C, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23,
  0x9B, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x92, 0x20, 0x00, 0xC8, 0x87, 0x20, 0x03, 0x25, 0x26,
  0x24, 0x23, 0x86, 0x20, 0x05, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x84, 0x20, 0x01, 0xC9, 0xCA,
  0x87, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x86, 0x20, 0x04, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0x82,
  0x20, 0x04, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0x87, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x87, 0x20,
  0x03, 0xA4, 0xA5, 0xA6, 0xA7, 0x82, 0x20, 0x03, 0xD0, 0xD1, 0xD2, 0xD3, 0x88, 0x20, 0x03, 0x25,
  0x26, 0x24, 0x23, 0x86, 0x20, 0x06, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0x82, 0x20, 0x01,
  0xD4, 0xD5, 0x88, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x86, 0x20, 0x09, 0xAF, 0xB0, 0xB1, 0xB2,
  0xB3, 0xB4, 0xB5, 0xB6, 0xC7, 0xC7, 0x8A, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x87, 0x20, 0x09,
  0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0x20, 0xC7, 0xC7, 0x89, 0x20, 0x03, 0x25, 0x26, 0x24,
  0x23, 0x87, 0x20, 0x08, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0x20, 0xC7, 0x8A, 0x20, 0x03,
  0x25, 0x26, 0x24, 0x23, 0x8A, 0x20, 0x05, 0xC5, 0xC6, 0xC4, 0x20, 0xC7, 0xC7, 0x8A, 0x20, 0x03,
  0x25, 0x26, 0x24, 0x23, 0x9B, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x9B, 0x20, 0x03, 0x25, 0x26,
  0x24, 0x23, 0x9B, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x89, 0x20, 0x07, 0xD8, 0xD9, 0xDA, 0xDB,
  0xDC, 0xDD, 0xDE, 0xDF, 0x89, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x9B, 0x20, 0x03, 0x25, 0x26,
  0x24, 0x23, 0x9B, 0x20, 0x03, 0x25, 0x26, 0x24, 0x23, 0x9B, 0x20, 0x01, 0x25, 0x26, 0xFF};

const u8 PATTERNRLE[] = {
  0x0A, 0xC3, 0x24, 0xA5, 0x66, 0x3C, 0x5A, 0xDB, 0xE7, 0x00, 0xC3, 0x42, 0x83, 0x00, 0x1C,
  0x18, 0x00, 0x24, 0x7E, 0xE7, 0xC3, 0xE7, 0x7E, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x3C, 0x18, 0x00,
  0x00, 0xE0, 0x90, 0x48, 0x28, 0x1C, 0x3A, 0x7A, 0x7C, 0x00, 0x60, 0x30, 0x10, 0x82, 0x00, 0x1C,
  0x03, 0x08, 0x18, 0x3F, 0x79, 0xF0, 0xF9, 0x7F, 0x3E, 0x06, 0x06, 0xC0, 0x86, 0x0F, 0x06, 0x00,
  0x00, 0x07, 0x09, 0x12, 0x14, 0x38, 0x5C, 0x5E, 0x3E, 0x00, 0x06, 0x0C, 0x08, 0x82, 0x00, 0x0E,
  0xC0, 0x10, 0x18, 0xFC, 0x9E, 0x0F, 0x9F, 0xFE, 0x7C, 0x60, 0x60, 0x03, 0x61, 0xF0, 0x60, 0xB9,
  0x00, 0x44, 0x99, 0x66, 0xDD, 0x77, 0xDD, 0x77, 0x99, 0xBB, 0x99, 0x66, 0xDD, 0x77, 0xDD, 0x77,
  0xEE, 0xBB, 0x99, 0x66, 0xDD, 0x77, 0xDD, 0xBB, 0xEE, 0x33, 0x99, 0x66, 0xDD, 0x77, 0xEE, 0xBB,
  0xCC, 0x33, 0x99, 0x66, 0xDD, 0xBB, 0xEE, 0x33, 0xCC, 0x22, 0x99, 0x66, 0xEE, 0xBB, 0xCC, 0x33,
  0x88, 0x22, 0x99, 0xBB, 0xEE, 0x33, 0xCC, 0x22, 0x88, 0x00, 0xEE, 0xBB, 0xCC, 0x33, 0x88, 0x22,
  0x00, 0x00, 0xEE, 0x33, 0xCC, 0x22, 0x88, 0x82, 0x00, 0x03, 0xCC, 0x33, 0x88, 0x22, 0x83, 0x00,
  0x02, 0xCC, 0x22, 0x88, 0x84, 0x00, 0x01, 0x88, 0x22, 0x85, 0x00, 0x00, 0x88, 0x90, 0x00, 0x01,
  0x0C, 0x1E, 0x82, 0x3E, 0x37, 0x1C, 0x46, 0xAD, 0xDB, 0x76, 0x44, 0x12, 0x00, 0x00, 0xF3, 0xCC,
  0x30, 0xCC, 0xF3, 0xCC, 0x30, 0xCC, 0xF3, 0xCC, 0x30, 0xCC, 0xF3, 0xCC, 0x30, 0xCC, 0xCF, 0x33,
  0x0C, 0x33, 0xCF, 0x33, 0x0C, 0x33, 0xCF, 0x33, 0x0C, 0x33, 0xCF, 0x33, 0x0C, 0x33, 0x18, 0xDB,
  0x3C, 0xE7, 0x3C, 0xDB, 0x18, 0x00, 0x00, 0x18, 0x3C, 0x7E, 0x7E, 0x3C, 0x18, 0x82, 0x00, 0x03,
  0x18, 0x3C, 0x3C, 0x18, 0x84, 0x00, 0x81, 0x18, 0x8A, 0x00, 0x21, 0x30, 0x18, 0x88, 0xFE, 0x7F,
  0x18, 0x38, 0x70, 0x00, 0xE0, 0x30, 0xFE, 0x7F, 0x30, 0xE0, 0x00, 0x0C, 0x18, 0x11, 0x7F, 0xFE,
  0x18, 0x1C, 0x0E, 0x00, 0x07, 0x0C, 0x7F, 0xFE, 0x0C, 0x07, 0x00, 0xE7, 0x66, 0x85, 0x00, 0x01,
  0x18, 0x3C, 0x85, 0x00, 0x81, 0x66, 0x00, 0x24, 0xFE, 0x00, 0xFE, 0x00, 0xFE, 0x00, 0xFE, 0x00,
  0xF2, 0x00, 0x00, 0x1C, 0x82, 0x3F, 0x01, 0x7B, 0x70, 0x83, 0x00, 0x03, 0x80, 0x83, 0x3F, 0x7F,
  0x85, 0x00, 0x01, 0x1E, 0x3F, 0x83, 0x00, 0x81, 0x38, 0x01, 0xF8, 0xFD, 0x85, 0x00, 0x01, 0xF0,
  0xFD, 0x85, 0x00, 0x01, 0xD8, 0xF8, 0x82, 0x00, 0x04, 0x1F, 0x3F, 0x3F, 0x39, 0x39, 0x82, 0x00,
  0x04, 0x80, 0xC0, 0xC0, 0xC6, 0xCE, 0x84, 0x00, 0x02, 0x01, 0x33, 0x73, 0x84, 0x00, 0x02, 0x80,
  0xF1, 0xFD, 0x84, 0x00, 0x02, 0xC0, 0xF8, 0xFF, 0x84, 0x00, 0x0B, 0x0C, 0x1C, 0x9C, 0x7E, 0x7F,
  0x7F, 0xF0, 0xF0, 0xFE, 0xFE, 0x1E, 0xFF, 0x82, 0xE3, 0x1D, 0xF7, 0x7F, 0x7F, 0x1C, 0xFE, 0xE0,
  0xFF, 0x7F, 0xA3, 0xFF, 0x7F, 0x3E, 0x7F, 0x73, 0x73, 0xF3, 0xE3, 0xE1, 0xE1, 0x00, 0xFF, 0x9F,
  0xFF, 0xF7, 0xCF, 0xFF, 0xFD, 0x78, 0xF8, 0xE0, 0x84, 0xC0, 0x38, 0x00, 0x3F, 0x3F, 0x1F, 0x1C,
  0x1D, 0x1F, 0x1F, 0x0F, 0xCE, 0xEE, 0xFE, 0x7E, 0xFF, 0xEF, 0xE7, 0x83, 0x73, 0x73, 0x7B, 0x7F,
  0x7F, 0xF7, 0xF2, 0xE0, 0xFD, 0x9D, 0x9D, 0x9F, 0x1F, 0x3B, 0x39, 0x18, 0xFF, 0xCF, 0xCF, 0xCE,
  0x8E, 0x9C, 0x1C, 0x0C, 0xDC, 0xDC, 0xFC, 0xFC, 0x7C, 0x38, 0xF8, 0xF0, 0x10, 0x18, 0x3F, 0xFE,
  0x7C, 0x3E, 0x32, 0x20, 0x84, 0x00, 0x0D, 0x01, 0x03, 0x07, 0x00, 0x00, 0x03, 0x3F, 0xF0, 0xC0,
  0x80, 0x0F, 0x00, 0x00, 0xFF, 0x82, 0x00, 0x0D, 0x7E, 0xFF, 0x00, 0x00, 0x80, 0xFE, 0x03, 0x01,
  0x03, 0xFE, 0x00, 0x00, 0x1F, 0xFF, 0x82, 0x7F, 0x04, 0xC0, 0x00, 0x00, 0xC0, 0xFE, 0x82, 0xFF,
  0x20, 0x7E, 0x07, 0x0E, 0x0E, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x3F, 0xF8, 0xE0, 0xC0, 0x83, 0x87,
  0x8F, 0xCD, 0x00, 0x00, 0x7D, 0xFF, 0xC7, 0xBF, 0xF3, 0xC1, 0x00, 0xF8, 0xE0, 0x80, 0xC0, 0xE0,
  0xF0, 0xFB, 0x85, 0x00, 0x03, 0xE0, 0xF8, 0xEB, 0x7B, 0x82, 0x3F, 0x82, 0x1F, 0x08, 0x86, 0x07,
  0x33, 0x38, 0x18, 0x80, 0xC3, 0xFF, 0xFF, 0x82, 0x7F, 0x0B, 0xFF, 0xF1, 0xE0, 0xC0, 0xFC, 0xF2,
  0xFC, 0xFF, 0xFC, 0xF0, 0xE0, 0xE0, 0x82, 0x00, 0x06, 0x01, 0x03, 0x07, 0x06, 0x0D, 0x0F, 0x3F,
  0x82, 0xFF, 0x00, 0x7F, 0x83, 0xFF, 0x09, 0xF7, 0xBF, 0xFB, 0xDF, 0xFF, 0xFF, 0xC2, 0xC5, 0xE3,
  0xFF, 0x82, 0xFE, 0x10, 0xFC, 0xE0, 0xE0, 0xE7, 0x0F, 0x03, 0x00, 0x00, 0x08, 0x00, 0x00, 0xC0,
  0xF0, 0xFC, 0xFE, 0x7F, 0xFF, 0x86, 0x00, 0x04, 0x9E, 0x0B, 0x03, 0x06, 0x04, 0x83, 0x00, 0x3B,
  0xFF, 0xBF, 0x7C, 0xF0, 0xC3, 0x87, 0x0F, 0x0F, 0xF0, 0x0F, 0x3F, 0xFC, 0xFB, 0xF7, 0x87, 0x7B,
  0x03, 0xFF, 0xFF, 0x3F, 0xDF, 0xEF, 0x8F, 0x73, 0x85, 0xC3, 0xC7, 0xEF, 0xFE, 0xFD, 0xFD, 0xFE,
  0xBF, 0x9F, 0xBF, 0x7F, 0xFF, 0xBF, 0x7F, 0xEF, 0xE1, 0xC0, 0xE4, 0xE1, 0xE8, 0xE5, 0xC1, 0x97,
  0x00, 0x80, 0x80, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x1E, 0x1E, 0x3E, 0x3E, 0x82, 0x3F, 0x05, 0x1F,
  0xFC, 0xFD, 0xFB, 0xE7, 0x1F, 0x82, 0xFF, 0x12, 0xED, 0xDD, 0xFA, 0xF6, 0xFE, 0xFD, 0xFB, 0xF3,
  0xFE, 0x87, 0x03, 0x81, 0xC0, 0xE1, 0xFF, 0xFF, 0xDF, 0x3C, 0xC2, 0x83, 0xFF, 0x08, 0xDF, 0x7C,
  0x00, 0xF0, 0xFC, 0xFF, 0xFF, 0xEF, 0xEF, 0x84, 0x00, 0x05, 0xC0, 0xF0, 0xFF, 0x1F, 0x0F, 0x03,
  0x84, 0x00, 0x81, 0xFF, 0x01, 0xFE, 0xF0, 0x83, 0x00, 0x01, 0xE1, 0x80, 0x85, 0x00, 0x01, 0xFF,
  0xFE, 0x84, 0xFF, 0x15, 0x7F, 0x8F, 0x0F, 0xE7, 0xF8, 0xFE, 0xFF, 0xDF, 0xDF, 0xE7, 0xC7, 0x83,
  0x03, 0x01, 0x81, 0xE1, 0xFF, 0xBF, 0x77, 0xF8, 0xF8, 0xB8, 0x82, 0xC0, 0x02, 0x7F, 0x3F, 0x1F,
  0x84, 0x00, 0x03, 0xCF, 0x8F, 0x07, 0x07, 0x83, 0x03, 0x81, 0x00, 0x00, 0x3F, 0x82, 0x00, 0x19,
  0xFC, 0x00, 0x01, 0x02, 0x07, 0x0F, 0x1F, 0x3E, 0x7E, 0xFC, 0x01, 0x02, 0x07, 0x0D, 0x1B, 0x36,
  0x2B, 0x55, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x84, 0x00, 0x02, 0x01, 0xFF, 0x7E,
  0x82, 0x00, 0x21, 0x03, 0x1A, 0xBB, 0xC7, 0xD5, 0x00, 0x00, 0x01, 0x83, 0xC6, 0xFD, 0xFE, 0xFD,
  0x6B, 0xD7, 0xAB, 0x57, 0xAA, 0x56, 0xAC, 0x54, 0x84, 0x88, 0x08, 0x10, 0x10, 0x20, 0x04, 0x08,
  0x1F, 0x03, 0x00, 0x00, 0x01, 0x82, 0x00, 0x1A, 0xAA, 0xD5, 0x3A, 0x07, 0xC1, 0x30, 0x00, 0x0C,
  0xFE, 0x5F, 0xFF, 0x5F, 0xFF, 0x2F, 0x1F, 0x3D, 0xE8, 0xB0, 0xC0, 0xFC, 0xFC, 0x7C, 0xBC, 0xD8,
  0x1D, 0x0E, 0x02, 0x84, 0x00, 0x12, 0xD0, 0xE0, 0x08, 0x8E, 0xE6, 0x63, 0x30, 0x00, 0x44, 0xAA,
  0x2A, 0x4A, 0x8A, 0x8A, 0xE4, 0x00, 0x4E, 0xA2, 0xA2, 0x82, 0xA4, 0x1A, 0x44, 0x00, 0x73, 0xDB,
  0xC3, 0x73, 0x1B, 0xDB, 0x73, 0x00, 0xCF, 0x6C, 0x6C, 0x6F, 0xCC, 0x0C, 0x0F, 0x00, 0xBE, 0x30,
  0x30, 0x3C, 0x30, 0x30, 0xBE, 0x00, 0xF0, 0x84, 0xD8, 0x0B, 0xF0, 0x00, 0x70, 0xD8, 0x18, 0x30,
  0x60, 0x00, 0x60, 0x00, 0x06, 0x0E, 0x83, 0x06, 0x00, 0x0F, 0x83, 0x00, 0x00, 0x1C, 0x83, 0x00,
  0x06, 0x38, 0x6C, 0x0C, 0x18, 0x0C, 0x6C, 0x38, 0xFE, 0x00, 0xFE, 0x00, 0x82, 0x00, 0xFF};

const u8 COLORRLE[] = {
  0xDF, 0x41, 0x83, 0xEF, 0x83, 0xE7, 0x83, 0x23, 0x83, 0x2C, 0x83, 0xAB, 0x81, 0xA9, 0x01,
  0xA8, 0xA6, 0x83, 0x86, 0x81, 0x89, 0x81, 0x8B, 0x83, 0x23, 0x83, 0x2C, 0x83, 0x23, 0x83, 0x2C,
  0x83, 0x23, 0x83, 0x2C, 0x83, 0x23, 0x82, 0x2C, 0x00, 0xC1, 0x83, 0x23, 0x81, 0x2C, 0x81, 0xC1,
  0x83, 0x23, 0x00, 0x2C, 0x82, 0xC1, 0x83, 0x23, 0x83, 0xC1, 0x82, 0x23, 0x00, 0x21, 0x83, 0xC1,
  0x81, 0x23, 0x81, 0x21, 0x83, 0xC1, 0x02, 0x23, 0x21, 0x21, 0x84, 0xC1, 0x81, 0x21, 0x85, 0xC1,
  0x00, 0x21, 0xA6, 0xC1, 0x87, 0x41, 0x81, 0xF1, 0x00, 0xA1, 0x82, 0xF1, 0x04, 0xE1, 0xA1, 0x61,
  0x69, 0x6A, 0x84, 0x61, 0x87, 0x2A, 0x87, 0x42, 0x87, 0x2A, 0x87, 0x42, 0xFE, 0x41, 0xFE, 0x41,
  0xFE, 0x41, 0xFE, 0x41, 0xFE, 0x41, 0xCC, 0x41, 0xFE, 0xF1, 0xC0, 0xF1, 0x87, 0xB1, 0x8B, 0xE1,
  0x83, 0xED, 0x82, 0xE1, 0x83, 0xED, 0x84, 0xE1, 0x82, 0xED, 0xA6, 0xE1, 0x81, 0xEF, 0x97, 0xE1,
  0x04, 0x5F, 0x1F, 0x5F, 0x1F, 0x5F, 0x87, 0xEF, 0x82, 0xED, 0x97, 0xE1, 0x87, 0xE8, 0x82, 0xED,
  0x00, 0xE1, 0x82, 0xEF, 0x83, 0xE1, 0x84, 0xEF, 0x85, 0xE1, 0x00, 0xEF, 0xA8, 0xE1, 0x83, 0xEF,
  0x8B, 0xE1, 0x87, 0xEF, 0xA0, 0xE1, 0x86, 0xEF, 0xDF, 0xE1, 0x87, 0x71, 0xA2, 0xE1, 0x02, 0xF1,
  0xA1, 0xB1, 0x83, 0xE1, 0x82, 0xF1, 0xBC, 0xE1, 0x81, 0x91, 0x02, 0x81, 0x61, 0x61, 0xD0, 0xF1,
  0xFE, 0x41, 0xFC, 0x41, 0x83, 0x41, 0xFF};