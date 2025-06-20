#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <atomic>
#include <thread>

namespace conf {
const unsigned el_n         = 2048;
const unsigned sleep_for_ms = 3;
} // namespace conf

static uint16_t b_overflow( unsigned n, unsigned mod ) { return n & ( mod - 1 ); }

static void * align( void * ptr_to_mem, uint8_t alignment ) {
  return ( void * )( ( intptr_t )( ( char * )ptr_to_mem + alignment - 1 ) & ~( alignment - 1 ) );
}

static unsigned align_block_sz( unsigned block_sz, unsigned alignment ) {
  return ( ( block_sz + alignment - 1 ) & ~( alignment - 1 ) );
}

template < unsigned sz > struct ring_map {
  uint16_t                buf[sz];
  std::atomic< uint16_t > popi   = sz - 1;
  std::atomic< uint16_t > pushi  = sz - 1;
  std::atomic< int16_t >  amount = sz;

  uint16_t pop( void ) {
    int16_t curr_amount = atomic_fetch_sub( &amount, 1 );
    if ( curr_amount <= 0 ) {
      ++amount;
      return sz;
    }
    return buf[b_overflow( atomic_fetch_sub( &popi, 1 ), sz )];
  }

  void push( uint16_t el ) {
    buf[b_overflow( atomic_fetch_sub( &pushi, 1 ), sz )] = el;
    ++amount;
  }

  ring_map() {
    for ( uint16_t i = 0; i < sz; ++i ) {
      buf[i] = i;
    }
  }
};

template < typename T, unsigned sz > struct mballocator {
  static_assert( sz > 0 && ( sz & ( sz - 1 ) ) == 0, "sz must be pow of 2" );
  ring_map< sz > rimp;
  char *         mem;
  uint16_t       block_size;

  mballocator( T * pmem, uint8_t alignment ) {
    // if ( alignment < 1 ) {
    //   throw std::invalid_argument( "alignement cannot be <1" );
    // }
    alignment  = std::max< uint8_t >( alignment, 1 );
    mem        = reinterpret_cast< char * >( align( pmem, alignment ) );
    block_size = align_block_sz( sizeof( T ), alignment );
  }

  T * mballoc() {
    uint16_t ret_section = rimp.pop();
    if ( ret_section == sz )
      return nullptr;
    char * res = mem + ( block_size * ret_section );
    return reinterpret_cast< T * >( res );
  }

  void mbdealloc( T * p_section ) { rimp.push( ( ( char * )p_section - mem ) / block_size ); }
};

// ------------------------------------------
//  APP
// ------------------------------------------

void allocate_n_fill_with_f( mballocator< size_t, conf::el_n > & mball ) {
  for ( int i = 0, sz = conf::el_n; i < sz; ++i ) {
    std::this_thread::sleep_for( std::chrono::milliseconds( conf::sleep_for_ms - 2 ) );
    size_t * some_p = mball.mballoc();
    if ( some_p == nullptr ) {
      std::cout << "returned nullptr in f on: " << i << std::endl;
      return;
    }
    if ( *some_p != 0x0 ) {
      static int err_count = 0;
      ++err_count;
      std::cout << "ERROR: using someone else's block: " << err_count << std::endl;
      std::cout << "BLOCK: " << *some_p << std::endl;
    }
    *some_p = 0xFFFFFFFFFFFFFFFF;
  }
}

void allocate_n_fill_with_a( mballocator< size_t, conf::el_n > & mball ) {
  for ( int i = 0, sz = conf::el_n; i < sz; ++i ) {
    std::this_thread::sleep_for( std::chrono::milliseconds( conf::sleep_for_ms - 1 ) );
    size_t * some_p = mball.mballoc();
    if ( some_p == nullptr ) {
      std::cout << "returned nullptr in a on: " << i << std::endl;
      return;
    }
    if ( *some_p != 0x0 ) {
      static int err_count = 0;
      ++err_count;
      std::cout << "ERROR: using someone else's block: " << err_count << std::endl;
      std::cout << "BLOCK: " << *some_p << std::endl;
    }
    *some_p = 0xAAAAAAAAAAAAAAAA;
  }
}

int main( void ) {
  size_t *                          pmem = ( size_t * )calloc( conf::el_n + 42, sizeof( size_t ) );
  mballocator< size_t, conf::el_n > mball{ pmem, 0 };

#ifdef USING_0
  size_t * el_for_mbdealloc;

  for ( int i = 0; i < 17; ++i ) {
    size_t * some_p = mball.mballoc();
    if ( i == 7 )
      el_for_mbdealloc = some_p;
    if ( some_p == nullptr ) {
      std::cout << "bad_mballoc" << std::endl;
      break;
    }
    *some_p = 0xFFFFFFFFFFFFFFFF - i;
  }

  mball.mbdealloc( el_for_mbdealloc );
  size_t * el_for_fill = mball.mballoc();
  *el_for_fill         = 0x0;
#endif
  std::thread t1( allocate_n_fill_with_f, std::ref( mball ) );
  std::thread t2( allocate_n_fill_with_a, std::ref( mball ) );

  t1.join();
  t2.join();
  std::cout << "_______mem_explore_first_17______" << std::endl;
  uint8_t * pexplore = reinterpret_cast< uint8_t * >( pmem );
  for ( int i = 0, sz = 17 * sizeof( size_t ); i < sz; ++i ) {
    if ( i % 8 == 0 )
      std::cout << std::endl;
    std::cout << std::to_string( ( pexplore[i] ) ) << "\t";
  }
  free( pmem );
}

// ╭─ids@archlinux ~/pro/_works/mballocator  ‹master*›
// ╰─➤  g++ main.cpp -o main.x
// ╭─ids@archlinux ~/pro/_works/mballocator  ‹master*›
// ╰─➤  ./main.x
// bad_mballoc
// _______mem_explore_____

// 240	255	255	255	255	255	255	255
// 241	255	255	255	255	255	255	255
// 242	255	255	255	255	255	255	255
// 243	255	255	255	255	255	255	255
// 244	255	255	255	255	255	255	255
// 245	255	255	255	255	255	255	255
// 246	255	255	255	255	255	255	255
// 247	255	255	255	255	255	255	255
// 0	0	0	0	0	0	0	0
// 249	255	255	255	255	255	255	255
// 250	255	255	255	255	255	255	255
// 251	255	255	255	255	255	255	255
// 252	255	255	255	255	255	255	255
// 253	255	255	255	255	255	255	255
// 254	255	255	255	255	255	255	255
// 255	255	255	255	255	255	255	255
// 0	0	0	0	0	0	0	0	%

// ╭─ids@archlinux ~/pro/_works/mballocator  ‹master*›
// ╰─➤  g++ -O2 main.cpp -o main.x
// ╭─ids@archlinux ~/pro/_works/mballocator  ‹master*›
// ╰─➤  ./main.x
// returned nullptr in f on: 1353
// returned nullptr in a on: 695
// _______mem_explore_first_17______

// 170	170	170	170	170	170	170	170
// 255	255	255	255	255	255	255	255
// 255	255	255	255	255	255	255	255
// 170	170	170	170	170	170	170	170
// 255	255	255	255	255	255	255	255
// 255	255	255	255	255	255	255	255
// 170	170	170	170	170	170	170	170
// 255	255	255	255	255	255	255	255
// 255	255	255	255	255	255	255	255
// 170	170	170	170	170	170	170	170
// 255	255	255	255	255	255	255	255
// 255	255	255	255	255	255	255	255
// 170	170	170	170	170	170	170	170
// 255	255	255	255	255	255	255	255
// 255	255	255	255	255	255	255	255
// 170	170	170	170	170	170	170	170
// 255	255	255	255	255	255	255	255	%
