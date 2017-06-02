//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

#include "peripheral.h"

#define MEM_SIZE 536870912U

volatile uint32_t int_mem;

//////////////////////////////////////////////////////////////////////////////


/// Constructor
ac_tlm_peripheral::ac_tlm_peripheral( sc_module_name module_name , int k ) :
    sc_module( module_name ),
    target_export("iport")
{
    /// Binds target_export to the memory
    target_export( *this );
    int_mem = 0;

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
    if (a == MEM_SIZE+1) {
        int_mem = (uint32_t) d;
        //cout << "writem:" << endl;
        //cout << "addr: " << std::hex << a << ", data: " << d << endl;
        return SUCCESS;
    }
    
    cout << "Peripheral ERROR: Address value\n" << endl;
    return ERROR;
}

/** Internal Read
 * Note: Always read 32 bits
 * @param a is the address to read
 * @param d id the data that will be read
 * @returns A TLM response packet with SUCCESS and a modified d
 */
ac_tlm_rsp_status ac_tlm_peripheral::readm( const uint32_t &a , uint32_t &d )
{
    if (a == MEM_SIZE+1) {
        *((uint32_t *) &d) = int_mem;
        // writing 1 to the value (the peripheral messes up the value)
        int_mem = 0x01000000;
        //cout << "readm:" << endl;
        //cout << "addr: " << std::hex << a << ", data: " << d << endl;
        return SUCCESS;
    }
    cout << "Peripheral ERROR: Address value\n" << endl;
    return ERROR;
}
