#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <regex.h>

/*
| MOVI | I   | 0IIIIIII |
| ADD  | R,R | 1000RRRR |
| SUB  | R,R | 1001RRRR |
| MUL  | R,R | 1010RRRR |
| DIV  | R,R | 1011RRRR |
| IN   | R   | 110000RR |
| OUT  | R   | 110001RR |
*/

typedef enum {
  MOVI = 0x0,
  ADD  = 0x8,
  SUB  = 0x9,
  MUL  = 0xA,
  DIV  = 0xB,
  IN   = 0x30,
  OUT  = 0x31
} opcode_t;

typedef enum { reg_load_format, ariphm_format, screen_format } instr_format_t;

typedef enum { A = 0, B, C, D } registr_t;

typedef struct {
  uint8_t immediat : 7;
  uint8_t opcode   : 1;
} instr_format_reg_load_t;

typedef struct {
  registr_t reg_2  : 2;
  registr_t reg_1  : 2;
  uint8_t   opcode : 4;
} instr_format_ariphm_t;

typedef struct {
  uint8_t reg_1  : 2;
  uint8_t opcode : 6;
} instr_format_screen_t;

typedef struct {
  const char * hex_pattern;
  const char * asm_pattern;
  void ( *print_as_asm )( uint8_t * );
  void ( *print_as_hex )( uint8_t * );
} instr_info_t;

typedef struct instruction {
  instr_info_t * info;
  uint8_t        val;
} instruction_t;

int instruction_set_n;

// -------------------------------------------------------
//                      INSTR: MOVI
// -------------------------------------------------------

void print_movi_asm( uint8_t * hexcode ) {
  instr_format_reg_load_t const * movi = ( instr_format_reg_load_t * )hexcode;
  printf( "MOVI %d", movi->immediat );
}

const char   pattern_hex_movi[] = "0x[0-7].";
instr_info_t movi_info          = {
             .hex_pattern  = pattern_hex_movi,
             .print_as_asm = print_movi_asm,
};

// -------------------------------------------------------
//                      INSTR: ADD
// -------------------------------------------------------

void print_add_asm( uint8_t * hexcode ) {
  instr_format_ariphm_t const * add = ( instr_format_ariphm_t * )hexcode;
  printf( "ADD %c %c", add->reg_1 + 65, add->reg_2 + 65 );
}

const char   pattern_hex_add[] = "0x8.";
instr_info_t add_info          = {
             .hex_pattern  = pattern_hex_add,
             .print_as_asm = print_add_asm,
};

// -------------------------------------------------------
//                      INSTR: SUB
// -------------------------------------------------------

void print_sub_asm( uint8_t * hexcode ) {
  instr_format_ariphm_t const * sub = ( instr_format_ariphm_t * )hexcode;
  printf( "SUB %c %c", sub->reg_1 + 65, sub->reg_2 + 65 );
}

const char   pattern_hex_sub[] = "0x9.";
instr_info_t sub_info          = {
             .hex_pattern  = pattern_hex_sub,
             .print_as_asm = print_sub_asm,
};

// -------------------------------------------------------
//                      INSTR: MUL
// -------------------------------------------------------

void print_mul_asm( uint8_t * hexcode ) {
  instr_format_ariphm_t const * mul = ( instr_format_ariphm_t * )hexcode;
  printf( "MUL %c %c", mul->reg_1 + 65, mul->reg_2 + 65 );
}

const char   pattern_hex_mul[] = "0xA.";
instr_info_t mul_info          = {
             .hex_pattern  = pattern_hex_mul,
             .print_as_asm = print_mul_asm,
};

// -------------------------------------------------------
//                      INSTR: DIV
// -------------------------------------------------------

void print_div_asm( uint8_t * hexcode ) {
  instr_format_ariphm_t const * div = ( instr_format_ariphm_t * )hexcode;
  printf( "DIV %c %c", div->reg_1 + 65, div->reg_2 + 65 );
}

const char   pattern_hex_div[] = "0xB.";
instr_info_t div_info          = {
             .hex_pattern  = pattern_hex_div,
             .print_as_asm = print_div_asm,
};

// -------------------------------------------------------
//                      INSTR: IN
// -------------------------------------------------------

void print_in_asm( uint8_t * hexcode ) {
  instr_format_screen_t const * in = ( instr_format_screen_t * )hexcode;
  printf( "IN %c", in->reg_1 + 65 );
}

const char   pattern_hex_in[] = "0xC[0-3]";
instr_info_t in_info          = {
             .hex_pattern  = pattern_hex_in,
             .print_as_asm = print_in_asm,
};

// -------------------------------------------------------
//                      INSTR: OUT
// -------------------------------------------------------

void print_out_asm( uint8_t * hexcode ) {
  instr_format_screen_t const * out = ( instr_format_screen_t * )hexcode;
  printf( "OUT %c", out->reg_1 + 65 );
}

const char   pattern_hex_out[] = "0xC[4-7]";
instr_info_t out_info          = {
             .hex_pattern  = pattern_hex_out,
             .print_as_asm = print_out_asm,
};

// -------------------------------------------------------
//                       DECODE
// -------------------------------------------------------

void check_regcompexec( int res, regex_t * regex_arr, int i ) {
  if ( res == 0 || res == REG_NOMATCH )
    return;
  char BUFFER[128];
  regerror( res, regex_arr + i, BUFFER, sizeof( BUFFER ) );
  fprintf( stderr, "Regex match failed: %s\n", BUFFER );
  for ( int j = 0; j < i; ++j )
    regfree( regex_arr + j );
  exit( EXIT_FAILURE );
}

void decode_hex_instruction_init( regex_t * regex_arr, instr_info_t * instr_set ) {
  for ( int i = 0; i < instruction_set_n; ) {
    int res = regcomp( regex_arr + i, instr_set[i].hex_pattern, REG_EXTENDED );
    check_regcompexec( res, regex_arr, ++i );
  }
}

instruction_t decode_hex_instruction( regex_t * regex_arr, instr_info_t * instr_set,
                                      char instr_hex[5] ) {
  instruction_t instr;
  instr.info = NULL;
  instr.val  = 0;
  regmatch_t matches[1];

  for ( int i = 0; i < instruction_set_n; ++i ) {
    int res = regexec( regex_arr + i, instr_hex, 1, matches, 0 );
    check_regcompexec( res, regex_arr, instruction_set_n );
    if ( !res ) {
      instr.info = instr_set + i;
      instr.val  = strtol( instr_hex, NULL, 16 );
      return instr;
    }
  }
  return instr;
}

// -------------------------------------------------------
//                         MAIN
// -------------------------------------------------------

int main() {
  instr_info_t instruction_set[] = { movi_info, add_info, sub_info, mul_info,
                                     div_info,  in_info,  out_info };
  instruction_set_n              = sizeof( instruction_set ) / sizeof( instruction_set[0] );
  regex_t instruction_regex[instruction_set_n];

  decode_hex_instruction_init( instruction_regex, instruction_set );
  for ( ;; ) {
    char instr_hex[5];
    int  res = scanf( "%4s", instr_hex );
    if ( res != 1 )
      break;
    instruction_t i = decode_hex_instruction( instruction_regex, instruction_set, instr_hex );
    if ( i.info == NULL ) {
      fprintf( stderr, "Unrecognized instr\n" );
      continue;
    }
    i.info->print_as_asm( &i.val );
    printf( "\n" );
  }
}

/*
[ids@archlinux cnpct]$ cat 001.dat
0x70 0xC7 0xC1 0x87 0x27 0xC5 0x8D 0xC1 0x87 0x6f 0xC5 0xC7
[ids@archlinux cnpct]$ ./AS2_notes.e < 001.dat
MOVI 112
OUT D
IN B
ADD B D
MOVI 39
OUT B
ADD D B
IN B
ADD B D
MOVI 111
OUT B
OUT D
[ids@archlinux cnpct]$
*/
