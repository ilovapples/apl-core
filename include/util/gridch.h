#ifndef GRIDCH_H
#define GRIDCH_H

#ifndef APLCORE__INCLUDE_WIDE_FUNCS
#define HORIZ_CH '-'
#define VERTI_CH '|'
#define UPLEFT_CH '+'
#define UPRIGHT_CH '+'
#define DOWNLEFT_CH '+'
#define DOWNRIGHT_CH '+'
#define UP_T_CH '+'
#define RIGHT_T_CH '+'
#define DOWN_T_CH '+'
#define LEFT_T_CH '+'
#define CROSS_CH '+'

#define HORIZ_ST "-"
#define VERTI_ST "|"
#define UPLEFT_ST "+"
#define UPRIGHT_ST "+"
#define DOWNLEFT_ST "+"
#define DOWNRIGHT_ST "+"
#define UP_T_ST "+"
#define RIGHT_T_ST "+"
#define DOWN_T_ST "+"
#define LEFT_T_ST "+"
#define CROSS_ST "+"
#else
#define HORIZ_CH L'\x2500'
#define VERTI_CH L'\x2502'
#define UPLEFT_CH L'\x250C'
#define UPRIGHT_CH L'\x2510'
#define DOWNLEFT_CH L'\x2514'
#define DOWNRIGHT_CH L'\x2518'
#define UP_T_CH L'\x2534'
#define RIGHT_T_CH L'\x251C'
#define DOWN_T_CH L'\x252C'
#define LEFT_T_CH L'\x2524'
#define CROSS_CH L'\x253C'

#define HORIZ_ST L"\x2500"
#define VERTI_ST L"\x2502"
#define UPLEFT_ST L"\x250C"
#define UPRIGHT_ST L"\x2510"
#define DOWNLEFT_ST L"\x2514"
#define DOWNRIGHT_ST L"\x2518"
#define UP_T_ST L"\x2534"
#define RIGHT_T_ST L"\x251C"
#define DOWN_T_ST L"\x252C"
#define LEFT_T_ST L"\x2524"
#define CROSS_ST L"\x253C"
#endif


#define HORIZ2_CH L'\x2550'
#define VERTI2_CH L'\x2551'
#define UPLEFT2_CH L'\x2554'
#define UPRIGHT2_CH L'\x2557'
#define DOWNLEFT2_CH L'\x255A'
#define DOWNRIGHT2_CH L'\x255D'
#define UP_T2_CH L'\x2534'
#define RIGHT_T2_CH L'\x251C'
#define DOWN_T2_CH L'\x252C'
#define LEFT_T2_CH L'\x2524'
#define CROSS2_CH L'\x253C'

#define HORIZ2_ST L"\x2550"
#define VERTI2_ST L"\x2551"
#define UPLEFT2_ST L"\x2554"
#define UPRIGHT2_ST L"\x2557"
#define DOWNLEFT2_ST L"\x255A"
#define DOWNRIGHT2_ST L"\x255D"
#define UP_T2_ST L"\x2569"
#define RIGHT_T2_ST L"\x2560"
#define DOWN_T2_ST L"\x2566"
#define LEFT_T2_ST L"\x2563"
#define CROSS2_ST L"\x253C"


#ifndef APLCORE__INCLUDE_WIDE_FUNCS
#define HORIZDD_CH L'~'
#define VERTIDD_CH L'|'

#define HORIZDD_ST L"~"
#define VERTIDD_ST L"|"
#else
#define HORIZDD_CH L'\x254C'
#define VERTIDD_CH L'\x254E'

#define HORIZDD_ST L"\x254C"
#define VERTIDD_ST L"\x254E"
#endif

#endif /* GRIDCH_H */
