//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

/* Functions to sen and cos */
#include <math.h>
/* Functions to invert the bits */
#include <arpa/inet.h>
#include <byteswap.h>

#include "peripheral.h"
#include "p_def.h"

#define N_PROC 9

volatile float proc_a[N_PROC], proc_b[N_PROC];

//volatile float proc1_a_value, proc2_a_value, proc3_a_value, proc4_a_value, proc5_a_value;
//volatile float proc1_b_value, proc2_b_value, proc3_b_value, proc4_b_value, proc5_b_value;

volatile uint32_t int_mem;

/// Constructor
ac_tlm_peripheral::ac_tlm_peripheral( sc_module_name module_name , int k ) :
    sc_module( module_name ),
    target_export("iport")
{
    /// Binds target_export to the memory
    target_export( *this );
}

/// Destructor
ac_tlm_peripheral::~ac_tlm_peripheral() {

}

/** Internal Write
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_peripheral::writem( const uint32_t &a , const uint32_t &d )
{
    Reading number;

    if (a == MEM_SIZE+1) {
        int_mem = (uint32_t) d;
        return SUCCESS;
    }

    /* Valor a para cada processador */
    if (a >= N_A && a < N_B) {
        number.i =  ntohl(d);
        proc_a[(a-N_A)/4] = number.f;
    } else if (a >= N_B && a < SUM){
        number.i =  ntohl(d);
        proc_b[(a-N_B)/4] = number.f;
    } else {
        return ERROR;
    }

    return SUCCESS;

}

/** Internal Read
  * Note: Always read 32 bits
  * @param a is the address to read
  * @param d id the data that will be read
  * @returns A TLM response packet with SUCCESS and a modified d
*/
ac_tlm_rsp_status ac_tlm_peripheral::readm( const uint32_t &a , uint32_t &d )
{
    Reading number;

    /* Lock Periphel */
    if (a == MEM_SIZE+1) {
        *((uint32_t *) &d) = int_mem;
        int_mem = 0x01000000;

        return SUCCESS;
    }

    if (a >= SUM && a < SUB) {
        number.f = proc_a[(a-SUM)/4] + proc_b[(a-SUM)/4];
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a >= SUB && a < MUL) {
        number.f = proc_a[(a-SUB)/4] - proc_b[(a-SUB)/4];
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a >= MUL && a < DIV) {
        number.f = proc_a[(a-MUL)/4] * proc_b[(a-MUL)/4];
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a >= DIV && a < SEN) {
        number.f = proc_a[(a-DIV)/4] / proc_b[(a-DIV)/4];
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a >= SEN && a < COS) {
        number.f = sin  (proc_a[(a-SEN)/4]);
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a >= COS && a < END) {
        number.f = cos(proc_a[(a-COS)/4]);
    } else {
        return ERROR;
    }

    return SUCCESS;
}
