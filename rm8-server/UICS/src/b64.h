#ifndef UICS_BASE64_H
#define UICS_BASE64_H

#include <string.h>
#include <stdlib.h>

static unsigned char to_uchar(char ch);


size_t islb64EncodeAlloc(const char *in, size_t inlen, char **out);
void islb64Encode(const char *in, size_t inlen,char *out, size_t outlen);
bool islb64IsBase64(char ch);
int islb64Decode(const char *in, size_t inlen,char *out, size_t * outlen);
int islb64DecodeAlloc(const char *in, size_t inlen,char **out, size_t * outlen);



#define islb64LENGTH(inlen) ((((inlen) + 2) / 3) * 4)

#ifndef DOXYGEN /* Code fragment ignored by Doxygen */
/**
 * With this approach this file works independent of the charset used (think
 * EBCDIC).  However, it does assume that the characters in the Base64
 * alphabet (A-Za-z0-9+/) are encoded in 0..255.  POSIX 1003.1-2001 require
 * that char and unsigned char are 8-bit quantities, though, taking care of
 * that problem.  But this may be a potential problem on non-POSIX C99
 * platforms.
 */
#define B64(x)		 	 		    \
      ((x) == 'A' ? 0				\
     : (x) == 'B' ? 1				\
     : (x) == 'C' ? 2				\
     : (x) == 'D' ? 3				\
     : (x) == 'E' ? 4				\
     : (x) == 'F' ? 5				\
     : (x) == 'G' ? 6				\
     : (x) == 'H' ? 7				\
     : (x) == 'I' ? 8				\
     : (x) == 'J' ? 9				\
     : (x) == 'K' ? 10				\
     : (x) == 'L' ? 11				\
     : (x) == 'M' ? 12				\
     : (x) == 'N' ? 13				\
     : (x) == 'O' ? 14				\
     : (x) == 'P' ? 15				\
     : (x) == 'Q' ? 16				\
     : (x) == 'R' ? 17				\
     : (x) == 'S' ? 18				\
     : (x) == 'T' ? 19				\
     : (x) == 'U' ? 20				\
     : (x) == 'V' ? 21				\
     : (x) == 'W' ? 22				\
     : (x) == 'X' ? 23				\
     : (x) == 'Y' ? 24				\
     : (x) == 'Z' ? 25				\
     : (x) == 'a' ? 26				\
     : (x) == 'b' ? 27				\
     : (x) == 'c' ? 28				\
     : (x) == 'd' ? 29				\
     : (x) == 'e' ? 30				\
     : (x) == 'f' ? 31				\
     : (x) == 'g' ? 32				\
     : (x) == 'h' ? 33				\
     : (x) == 'i' ? 34				\
     : (x) == 'j' ? 35				\
     : (x) == 'k' ? 36				\
     : (x) == 'l' ? 37				\
     : (x) == 'm' ? 38				\
     : (x) == 'n' ? 39				\
     : (x) == 'o' ? 40				\
     : (x) == 'p' ? 41				\
     : (x) == 'q' ? 42				\
     : (x) == 'r' ? 43				\
     : (x) == 's' ? 44				\
     : (x) == 't' ? 45				\
     : (x) == 'u' ? 46				\
     : (x) == 'v' ? 47				\
     : (x) == 'w' ? 48				\
     : (x) == 'x' ? 49				\
     : (x) == 'y' ? 50				\
     : (x) == 'z' ? 51				\
     : (x) == '0' ? 52				\
     : (x) == '1' ? 53				\
     : (x) == '2' ? 54				\
     : (x) == '3' ? 55				\
     : (x) == '4' ? 56				\
     : (x) == '5' ? 57				\
     : (x) == '6' ? 58				\
     : (x) == '7' ? 59				\
     : (x) == '8' ? 60				\
     : (x) == '9' ? 61				\
     : (x) == '+' ? 62				\
     : (x) == '/' ? 63				\
     : -1)
#endif /*!DOXYGEN*/

static const signed char b64[0x100] = {
    B64 (0), B64 (1), B64 (2), B64 (3),
    B64 (4), B64 (5), B64 (6), B64 (7),
    B64 (8), B64 (9), B64 (10), B64 (11),
    B64 (12), B64 (13), B64 (14), B64 (15),
    B64 (16), B64 (17), B64 (18), B64 (19),
    B64 (20), B64 (21), B64 (22), B64 (23),
    B64 (24), B64 (25), B64 (26), B64 (27),
    B64 (28), B64 (29), B64 (30), B64 (31),
    B64 (32), B64 (33), B64 (34), B64 (35),
    B64 (36), B64 (37), B64 (38), B64 (39),
    B64 (40), B64 (41), B64 (42), B64 (43),
    B64 (44), B64 (45), B64 (46), B64 (47),
    B64 (48), B64 (49), B64 (50), B64 (51),
    B64 (52), B64 (53), B64 (54), B64 (55),
    B64 (56), B64 (57), B64 (58), B64 (59),
    B64 (60), B64 (61), B64 (62), B64 (63),
    B64 (64), B64 (65), B64 (66), B64 (67),
    B64 (68), B64 (69), B64 (70), B64 (71),
    B64 (72), B64 (73), B64 (74), B64 (75),
    B64 (76), B64 (77), B64 (78), B64 (79),
    B64 (80), B64 (81), B64 (82), B64 (83),
    B64 (84), B64 (85), B64 (86), B64 (87),
    B64 (88), B64 (89), B64 (90), B64 (91),
    B64 (92), B64 (93), B64 (94), B64 (95),
    B64 (96), B64 (97), B64 (98), B64 (99),
    B64 (100), B64 (101), B64 (102), B64 (103),
    B64 (104), B64 (105), B64 (106), B64 (107),
    B64 (108), B64 (109), B64 (110), B64 (111),
    B64 (112), B64 (113), B64 (114), B64 (115),
    B64 (116), B64 (117), B64 (118), B64 (119),
    B64 (120), B64 (121), B64 (122), B64 (123),
    B64 (124), B64 (125), B64 (126), B64 (127),
    B64 (128), B64 (129), B64 (130), B64 (131),
    B64 (132), B64 (133), B64 (134), B64 (135),
    B64 (136), B64 (137), B64 (138), B64 (139),
    B64 (140), B64 (141), B64 (142), B64 (143),
    B64 (144), B64 (145), B64 (146), B64 (147),
    B64 (148), B64 (149), B64 (150), B64 (151),
    B64 (152), B64 (153), B64 (154), B64 (155),
    B64 (156), B64 (157), B64 (158), B64 (159),
    B64 (160), B64 (161), B64 (162), B64 (163),
    B64 (164), B64 (165), B64 (166), B64 (167),
    B64 (168), B64 (169), B64 (170), B64 (171),
    B64 (172), B64 (173), B64 (174), B64 (175),
    B64 (176), B64 (177), B64 (178), B64 (179),
    B64 (180), B64 (181), B64 (182), B64 (183),
    B64 (184), B64 (185), B64 (186), B64 (187),
    B64 (188), B64 (189), B64 (190), B64 (191),
    B64 (192), B64 (193), B64 (194), B64 (195),
    B64 (196), B64 (197), B64 (198), B64 (199),
    B64 (200), B64 (201), B64 (202), B64 (203),
    B64 (204), B64 (205), B64 (206), B64 (207),
    B64 (208), B64 (209), B64 (210), B64 (211),
    B64 (212), B64 (213), B64 (214), B64 (215),
    B64 (216), B64 (217), B64 (218), B64 (219),
    B64 (220), B64 (221), B64 (222), B64 (223),
    B64 (224), B64 (225), B64 (226), B64 (227),
    B64 (228), B64 (229), B64 (230), B64 (231),
    B64 (232), B64 (233), B64 (234), B64 (235),
    B64 (236), B64 (237), B64 (238), B64 (239),
    B64 (240), B64 (241), B64 (242), B64 (243),
    B64 (244), B64 (245), B64 (246), B64 (247),
    B64 (248), B64 (249), B64 (250), B64 (251),
    B64 (252), B64 (253), B64 (254), B64 (255)
};



#endif // UICS_BASE64_H