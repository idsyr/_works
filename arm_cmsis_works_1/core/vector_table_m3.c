extern int ( *const vector_table_common[] )( void );
extern int _stack_base;
extern int main();

static int default_handle() {
  for ( ;; )
    ;
}

__attribute__( ( weak, alias( "default_handle" ) ) ) int handle_nmi();
__attribute__( ( weak, alias( "default_handle" ) ) ) int handle_hardfault();
__attribute__( ( weak, alias( "default_handle" ) ) ) int handle_memory_management_fault();
__attribute__( ( weak, alias( "default_handle" ) ) ) int handle_bus_fault();
__attribute__( ( weak, alias( "default_handle" ) ) ) int handle_usage_fault();
__attribute__( ( weak, alias( "default_handle" ) ) ) int handle_svcall();
__attribute__( ( weak, alias( "default_handle" ) ) ) int handle_pendsv();
__attribute__( ( weak, alias( "default_handle" ) ) ) int handle_systick();

__attribute__( ( section( ".vector_table_common" ) ) ) int ( *const vector_table_common[] )( void ) = {
    ( int ( *const )() )&_stack_base, // initial SP value
    main,                             // reset
    handle_nmi,                       // NonMaskableInterrupt
    handle_hardfault,                 // Hard Fault
    handle_memory_management_fault,   // memmanagefault
    handle_bus_fault,                 // Bus Fault
    handle_usage_fault,               // Usage Fault
    0,                                // reserved
    0,                                // reserved
    0,                                // reserved
    0,                                // reserved
    handle_svcall,                    // SVCall
    0,                                // reserved for debug
    0,                                // reserved
    handle_pendsv,                    // PendSV
    handle_systick,                   // Systick
                                      // ...IRQ(0-67)
};
