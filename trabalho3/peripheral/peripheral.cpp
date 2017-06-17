//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

#include "peripheral.h"

/* Functions to invert the bits */
#include <arpa/inet.h>
#include <byteswap.h>

#define N_A  600000004U
#define N_B  600000008U
#define SOMA 600000020U
#define SUB  600000030U
#define MULT 600000040U
#define DIV  600000050U

/* Funcionamento do periférico de float:
 * Passa numero a no end 600000004U
 * Passa numero b no end 600000008U
 * Resultado:
 *  - Soma : 600000020U
 *  - Sub  : 600000030U
 *  - Mult : 600000040U
 *  - Div  : 600000050U
 */

typedef union {
    uint32_t i;
    float f;
} Reading;

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
    }

    printf("%f %f\n", first_value, second_value);

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
        cout << "Soma" << endl;
        number.f = first_value + second_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == SUB) {
        cout << "Sub" << endl;
        number.f = first_value - second_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == MULT) {
        cout << "Mult" << endl;
        number.f = first_value * second_value;
        *((uint32_t *) &d) = htonl(number.i);
    } else if (a == DIV) {
        cout << "Div" << endl;
        number.f = first_value / second_value;
        *((uint32_t *) &d) = htonl(number.i);
    }
    return SUCCESS;
}
