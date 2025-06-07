
extern void main ();
extern void (* const isr_vector_table[]) (void);
extern void _stack_base ();
extern void _boot_checksum();

__attribute__ ((weak)) void isr_nmi       ();
__attribute__ ((weak)) void isr_hardfault ();
__attribute__ ((weak)) void isr_svcall    (); 
__attribute__ ((weak)) void isr_pendsv    (); 
__attribute__ ((weak)) void isr_systick   ();

__attribute__ ((section(".isr_vector"))) void (* const isr_vector_table[]) (void) =
{  &_stack_base,
   main,
   isr_nmi,
   isr_hardfault,
   0,
   0,
   0,
   &_boot_checksum,
   0,
   0,
   0,
   isr_svcall,
   0,
   0,
   isr_pendsv,
   isr_systick,
};

void isr_nmi       () { for(;;); }
void isr_hardfault () { for(;;); }
void isr_svcall    () { for(;;); }
void isr_pendsv    () { for(;;); }
void isr_systick   () { for(;;); }
