#include <include/MC14051B.h>


/*starts of as inhibit on, meaning no passthrough*/
void MC14051_init(MC14051_t *dev)
{
    dev->inhibit = TAC_SEL_INH;
    PIN_HI(dev->inhibit);
    dev->status = CHL_1;
    dev->sel_A = TAC_SEL_A; 
    dev->sel_B = TAC_SEL_B;
    dev->sel_C = TAC_SEL_C;
    MC14051_CHL_SELECT(dev, CHL_1);
}
void MC14051_CHL_SELECT(MC14051_t *dev, switch_chl_t chl)
{
     
    uint8_t addr = (uint8_t)chl; //cast enum to unit8, if not done explicit, will be done implicit
    uint8_t mask = 0;
    uint8_t sel_val = 0;
    volatile uint8_t *lat;
    
    MC14051_INHIB_ON(dev);
    
    if((dev == NULL) || (chl > MC14051_CHLS))
    {
        return;
    }
    lat = dev->sel_A.lat; 
    mask = dev->sel_A.bitmask | dev->sel_B.bitmask | dev->sel_C.bitmask;
    /*enum starts at 0
    chl1 = 00
    chl2 = 01
    chl3 = 10
    chl4 = 11
    */
    if((addr & 0x01u) != 0u)
    {
        sel_val |= dev->sel_A.bitmask;
    }
    if((addr & 0x02u) != 0u)
    {
        sel_val |= dev->sel_B.bitmask;
    }
    if((addr & 0x04u) != 0u)
    {
        sel_val |= dev->sel_C.bitmask;
    }

    /* lat is a pointer that has copied the pointer from sel_A, so it points to LATA register
    derefence the pointer to get the actual content of the register AND that with the inverted mask, 
    to preserve the other content of the register, then OR in the selected pins for the mux.
    */
    *lat = (uint8_t)(*lat & (uint8_t)~mask) | sel_val;
    //update the status
    dev->status = chl;
    //remove the inhib
    MC14051_INHIB_OFF(dev);

}
void MC14051_INHIB_ON(MC14051_t *dev)
{
    PIN_HI(dev->inhibit);
}
void MC14051_INHIB_OFF(MC14051_t *dev)
{
    PIN_LO(dev->inhibit);
}
