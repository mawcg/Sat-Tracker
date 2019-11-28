/* Class to organize variables stored in EEPROM.
 * The public class variables are mirrored in RAM, so change nv then call put(), or call get() then access nv.
 * The validity of EEPROM is checked using a magic constant that must match. All values will be inited to 0.
 */

#ifndef _NV_H
#define _NV_H

#include <EEPROM.h>

class NV {

    private:

	enum {
	    MAGIC  = 0x5a5aa5a5,
	    EEBASE = 400,
	    NBNO055CALBYTES = 22,
	};

    public:

	uint32_t magic;
	uint8_t IP[4];
	uint16_t mot0min, mot0max, mot1min, mot1max;
	uint8_t BNO055cal[NBNO055CALBYTES];

	void get() {
	    EEPROM.get (EEBASE, *this);
	    if (magic != MAGIC) {
		memset (this, 0, sizeof(*this));
		magic = MAGIC;
		EEPROM.put (EEBASE, *this);
	    }
	}

	void put() {
	    EEPROM.put (EEBASE, *this);
	}
};

extern NV *nv;

#endif // _NV_H
