/*
  TM16xxFonts.h - Font definition for TM16xx.

  Copyright (C) 2011 Ricardo Batista (rjbatista <at> gmail <dot> com)
  Modified by Maxint R&D. See https://github.com/maxint-rd/
  - Improvements for 7-segment alpha-numeric font
  - Additional 15-segment font

  This program is free software: you can redistribute it and/or modify
  it under the terms of the version 3 GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Segment labels
  -- A --
  |       |
  F       B
  -- G --
  E       C
  |       |
  -- D --  .DP

  The bits are displayed by the mapping below:
  -- 0 --
  |       |
  5       1
  -- 6 --
  4       2
  |       |
  -- 3 --  .7


  Common pinout 7-segments x 4 digits:
    12 11 10  9  8  7
     |  |  |  |  |  |
  +------------------------+     12, 9, 8, 6: GRD1-GRD4
  |   --   --   --   --    |
  |  |  | |  | |  | |  |   |     1: SEG_E    5: SEG_G
  |   --   --   --   --    |     2: SEG_D    7: SEG_B
  |  |  | |  | |  | |  |   |     3: SEG_DP  10: SEG_F
  |   -- . -- . -- . -- .  |     4: SEG_C   11: SEG_A
  +------------------------+
     |  |  |  |  |  |
     1  2  3  4  5  6

*/

#ifndef TM16XXFonts_h
#define TM16XXFonts_h

// definition for standard hexadecimal numbers
const PROGMEM byte TM16XX_NUMBER_FONT[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b00000000  // 10
};

const PROGMEM byte MINUS = 0b01000000;

// definition for error text
//const PROGMEM byte TM16XX_ERROR_DATA[] = {0b01111001, 0, 0, 0, 0, 0, 0, 0}; // E
const PROGMEM byte TM16XX_ERROR_DATA[] = {0b01111001, 0b01010000,0b01010000, 0, 0, 0, 0, 0}; // Err

// Definition for the displayable ASCII chars
// (note that non-alpha-numeric characters may display as a space or a simple dash)
const PROGMEM byte TM16XX_FONT_DEFAULT[] = {
  0b00000000, // (32)  <space>
  0b10000110, // (33)	!
  0b00100010, // (34)	"
  0b01001001, // (35)	#
  0b01001001, // (36)	$
  0b01001001, // (37)	%
  0b01001001, // (38)	&
  0b00000001, // (39)	'
  0b01001001, // (40)	(
  0b01001001, // (41)	)
  0b01001001, // (42)	*
  0b01001001, // (43)	+
  0b10000000, // (44)	,
  0b01000000, // (45)	-
  0b10000000, // (46)	.
  0b01001001, // (47)	/
  0b00111111, // (48)	0
  0b00000110, // (49)	1
  0b01011011, // (50)	2
  0b01001111, // (51)	3
  0b01100110, // (52)	4
  0b01101101, // (53)	5
  0b01111101, // (54)	6
  0b00000111, // (55)	7
  0b01111111, // (56)	8
  0b01101111, // (57)	9
  0b01001001, // (58)	:
  0b01001001, // (59)	;
  0b01001001, // (60)	<
  0b01001000, // (61)	=
  0b01001001, // (62)	>
  0b01010011, // (63)	?
  0b01001001, // (64)	@
  0b01110111, // (65)	A
  0b01111100, // (66)	B like b
  0b00111001, // (67)	C
  0b01011110, // (68)	D like d
  0b01111001, // (69)	E
  0b01110001, // (70)	F
  0b00111101, // (71)	G
  0b01110110, // (72)	H
  0b00000110, // (73)	I
  0b00001110, // (74)	J
  0b01001001, // (75)	K
  0b00111000, // (76)	L
  0b01001001, // (77)	M
  0b01010100, // (78)	N like n
  0b01011100, // (79)	O like o
  0b01110011, // (80)	P
  0b01100111, // (81)	Q like q
  0b01010000, // (82)	R like r
  0b01101101, // (83)	S
  0b01111000, // (84)	T like t
  0b00011100, // (85)	U like u
  0b00111110, // (86)	V
  0b01001001, // (87)	W
  0b01001001, // (88)	X
  0b01101110, // (89)	Y
  0b01011011, // (90)	Z like 2
  0b01001001, // (91)	[
  0b01001001, // (92)	\ (this can't be the last char on a line, even in comment or it'll concat)
  0b01001001, // (93)	]
  0b00000010, // (94)	^
  0b00001000, // (95)	_
  0b00000001, // (96)	`
  0b01110111, // (97)	a like A
  0b01111100, // (98)	b
  0b01011000, // (99)	c
  0b01011110, // (100)	d
  0b01111001, // (101)	e like E
  0b01110001, // (102)	f like F
  0b00111101, // (103)	g like G
  0b01110100, // (104)	h
  0b00000100, // (105)	i
  0b00001110, // (106)	j like J
  0b01001001, // (107)	k
  0b00111000, // (108)	l like L
  0b01001001, // (109)	m
  0b01010100, // (110)	n
  0b01011100, // (111)	o
  0b01110011, // (112)	p like P
  0b01100111, // (113)	q
  0b01010000, // (114)	r
  0b01101101, // (115)	s like S
  0b01111000, // (116)	t
  0b00011100, // (117)	u
  0b00111110, // (118)	v like V
  0b01001001, // (119)	w
  0b01001001, // (120)	x
  0b01101110, // (121)	y like Y
  0b01011011, // (122)	z like 2
  0b01001001, // (123)	{
  0b01001001, // (124)	|
  0b01001001, // (125)	}
  0b00000001, // (126)	~
};

#endif
