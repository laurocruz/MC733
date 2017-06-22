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

volatile float first_value = 1, second_value = 1;

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

    if (a == N_A) {
        number.i =  ntohl(d);
        first_value = number.f;
        //return SUCCESS;
    } else if (a == N_B) {
        number.i = ntohl(d);
        second_value = number.f;
        //return SUCCESS;
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

    if (a == SOMA) {
        number.f = first_value + second_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SUB) {
        number.f = first_value - second_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == MULT) {
        number.f = first_value * second_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == DIV) {
        number.f = first_value / second_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SEN) {
        number.f = (float) sin(first_value);
        printf("%f\n",number.f);
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == COS) {
        number.f = (float) cos(first_value);
        printf("%f\n",number.f);
        *((uint32_t *) &d) = htonl(number.i);
    }

    return SUCCESS;
}
