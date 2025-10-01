//
// gcc -lm magni2.c -o magni.x
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

typedef struct oscillo_sample {
  double t_sec;
  double ch[4];
  double math;
} oscillo_sample_t;

typedef struct data_sample {
  int32_t t_sec;
  int32_t ch[4];
  int32_t math;
} data_sample_t;

// ------------------------------------------
//  PC_INPUT
// ------------------------------------------

struct {
  FILE * file_desc;
} read_sample_conf = {};

void close_file_with_samples() { fclose( read_sample_conf.file_desc ); }

int read_sample( data_sample_t * dsample ) {
  char     buffer[128];
  unsigned buffer_sz = 128;
  char *   read_st   = fgets( buffer, buffer_sz, read_sample_conf.file_desc );
  if ( read_st == NULL )
    return 0;
  oscillo_sample_t sample;
  int sscanf_st = sscanf( buffer, "%lf,%lf,%lf,%lf,%lf,%lf", &sample.t_sec, &sample.ch[0],
                          &sample.ch[1], &sample.ch[2], &sample.ch[3], &sample.math );
  *dsample      = ( data_sample_t ){ .t_sec = ( int32_t )floor( sample.t_sec * 1000000 + 2671 ),
                                     .ch[0] = ( int32_t )floor( sample.ch[0] * 1000000 ),
                                     .ch[1] = ( int32_t )floor( sample.ch[1] * 1000000 ),
                                     .ch[2] = ( int32_t )floor( sample.ch[2] * 1000000 ),
                                     .ch[3] = ( int32_t )floor( sample.ch[3] * 1000000 ),
                                     .math  = ( int32_t )floor( sample.math * 1000000 ) };

  // if ( !sscanf_st )  // -> title
  //   return 0;
  return 1;
}

char * read_arg_0( int argc, char * argv[] ) {
  if ( argc < 2 ) {
    fprintf( stderr, "no args provided, required: 1. ( path to the file with samples )" );
    abort();
  }
  return argv[1];
}

void open_file_with_samples( char * file_path ) {
  read_sample_conf.file_desc = fopen( file_path, "r" );
  if ( read_sample_conf.file_desc == NULL ) {
    fprintf( stderr, "cannot open file: %s", file_path );
    abort();
  }
  atexit( close_file_with_samples );
}

void cat_file_with_samples() {
  data_sample_t sample;
  int           line_num = 0;
  read_sample( &sample ); // title
  while ( read_sample( &sample ) ) {
    printf( "line(%d)\t-> time(%d), ch1(%d), ch2(%d), ch3(%d), ch4(%d), math(%d)\n", line_num,
            sample.t_sec, sample.ch[0], sample.ch[1], sample.ch[2], sample.ch[3], sample.math );
    ++line_num;
  }
}

// ------------------------------------------
//  INCIDENTS
// ------------------------------------------

typedef struct spark_incident {
  int     num;
  int32_t t_begin;
  int32_t t_end;
  int32_t amplitude;
} spark_incident_t;

#define spark_incidents_sz 256
spark_incident_t spark_incidents[spark_incidents_sz] = {};
unsigned         spark_incidents_i                   = 0;

void record_spark_incident( spark_incident_t const incident ) {
  spark_incidents[( spark_incidents_i++ ) % ( spark_incidents_sz )] = incident;
}

// ------------------------------------------
//  MOVING_WINDOW
// ------------------------------------------

#define moving_window_sz 16
int32_t moving_window[moving_window_sz] = {};
int     moving_window_i                 = 0;

void moving_window_add( int32_t el ) {
  moving_window[( ++moving_window_i ) % moving_window_sz] = el;
}

double moving_window_get() {
  int32_t res = 0;
  for ( int i = 0; i < moving_window_sz; ++i ) {
    res += moving_window[i] / moving_window_sz;
  }
  return res;
}

// ------------------------------------------
//  DETECT
// ------------------------------------------

double combine_low_falls( int32_t memored, int32_t next, bool is_growing ) {
  return ( is_growing ) ? next : ( memored * 0.999 + next * 0.001 );
}

double combine_low_delta( int32_t memored, int32_t next ) { return memored * 0.75 + next * 0.25; }

void detect_spark() {
  FILE * logfile = fopen( "magni_out.dat", "w" );
  int    num     = 0;

  data_sample_t    sample;
  spark_incident_t incident;
  int32_t          filtered_math_part = 0;
  int32_t          average_ampl       = 0;
  bool             prev_is_peak       = false;
  int              spark_t_begin      = 0;
  int32_t          prev_window        = 0;
  int              spark_appeared     = 0;

  while ( read_sample( &sample ) ) {

    filtered_math_part = combine_low_falls( filtered_math_part, sample.math,
                                            ( sample.math > filtered_math_part ) );
    moving_window_add( filtered_math_part );
    int32_t window     = moving_window_get();
    bool    is_growing = window > prev_window;
    average_ampl =
        combine_low_falls( average_ampl, sample.ch[1], ( sample.math > average_ampl ) );

    if ( prev_is_peak && !is_growing ) {
      incident = ( spark_incident_t ){
          .num       = num,
          .t_begin   = spark_t_begin,
          .t_end     = sample.t_sec,
          .amplitude = average_ampl,
      };
      record_spark_incident( incident );
      spark_appeared = 1;
    }

    if ( !prev_is_peak && is_growing ) {
      spark_t_begin = sample.t_sec;
    }

    prev_is_peak = is_growing;
    prev_window  = window;

    num++;
    fprintf( logfile, "%d,%d\n", window, spark_appeared );
    spark_appeared = 0;
  }

  fclose( logfile );
}

void cat_spark_incidents() {
  for ( int i = 0; i < spark_incidents_i; ++i ) {
    printf( "spark( %d ): begin_time( %d microsec), end_time( %d microsec), "
            "amplitude( %lf V) \n",
            i, spark_incidents[i].t_begin, spark_incidents[i].t_end,
            ( ( double )spark_incidents[i].amplitude ) / 1000000 );
  }
}

int main( int argc, char * argv[] ) {
  char * path_to_file_with_samples = read_arg_0( argc, argv );
  open_file_with_samples( path_to_file_with_samples );
  // cat_file_with_samples();
  detect_spark();
  cat_spark_incidents();
}
