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

volatile float proc1_a_value, proc2_a_value, proc3_a_value, proc4_a_value;
volatile float proc1_b_value, proc2_b_value, proc3_b_value, proc4_b_value;

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
    if (a == N_A) {
        number.i =  ntohl(d);
        proc1_a_value = number.f;
    } else if (a == N_A + 4) {
        number.i = ntohl(d);
        proc2_a_value = number.f;
    } else if (a == N_A + 8) {
        number.i = ntohl(d);
        proc3_a_value = number.f;
    } else if (a == N_A + 12) {
        number.i = ntohl(d);
        proc4_a_value = number.f;
    }

    /* Valor b para cada processador */
    else if (a == N_B) {
        number.i = ntohl(d);
        proc1_b_value = number.f;
    } else if (a == N_B + 4) {
        number.i = ntohl(d);
        proc2_b_value = number.f;
    } else if (a == N_B + 8) {
        number.i = ntohl(d);
        proc3_b_value = number.f;
    } else if (a == N_B + 12) {
        number.i = ntohl(d);
        proc4_b_value = number.f;
    }
    else {
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

    /* Soma */
    if (a == SUM) {
        number.f = proc1_a_value + proc1_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SUM + 4) {
        number.f = proc2_a_value + proc2_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SUM + 8) {
        number.f = proc3_a_value + proc3_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SUM + 12) {
        number.f = proc4_a_value + proc4_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    }
    /* Subtração */
    else if (a == SUB) {
        number.f = proc1_a_value - proc1_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SUB + 4) {
        number.f = proc2_a_value - proc2_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SUB + 8) {
        number.f = proc3_a_value - proc3_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SUB + 12) {
        number.f = proc4_a_value - proc4_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    }
    /* Multiplicação */
    else if (a == MUL) {
        number.f = proc1_a_value * proc1_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == MUL + 4) {
        number.f = proc2_a_value * proc2_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == MUL + 8) {
        number.f = proc3_a_value * proc3_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == MUL + 12) {
        number.f = proc4_a_value * proc4_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    }
    /* Divisão */
    else if (a == DIV) {
        number.f = proc1_a_value / proc1_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == DIV + 4) {
        number.f = proc2_a_value / proc2_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == DIV + 8) {
        number.f = proc3_a_value / proc3_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == DIV + 12) {
        number.f = proc4_a_value / proc4_b_value;
        *((uint32_t *) &d) = htonl(number.i);
    }
    /* Seno */
    else if (a == SEN) {
        number.f = (float) sin(proc1_a_value);
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SEN + 4) {
        number.f = (float) sin(proc2_a_value);
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SEN + 8) {
        number.f = (float) sin(proc3_a_value);
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SEN + 12) {
        number.f = (float) sin(proc4_a_value);
        *((uint32_t *) &d) = htonl(number.i);
    }

    /* Coseno */
    else if (a == COS) {
        number.f = (float) cos(proc1_a_value);
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == COS + 4) {
        number.f = (float) cos(proc2_a_value);
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == COS + 8) {
        number.f = (float) cos(proc3_a_value);
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == COS + 12) {
        number.f = (float) cos(proc4_a_value);
        *((uint32_t *) &d) = htonl(number.i);
    }

    else {
        return ERROR;
    }

    return SUCCESS;
}
