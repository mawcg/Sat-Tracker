#ifndef _CIRCUM_H
#define	_CIRCUM_H

#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>

#include "P13.h"
#include "Target.h"

extern int magdecl ( double l, double L, double e, double y, double *mdp);

class Circum {

    private:

	/* piggy back on millis() timer to poll GPS
	 */
	void startPolling() {
	    // Timer0 is already used for millis() - we'll just interrupt somewhere
	    // in the middle and call the "Compare A" handler (below)
	    OCR0A = 0xAF;
	    TIMSK0 |= _BV(OCIE0A);
	}

	/* flags
	 */
	bool gps_lock;			// set when we get a valid fix
	bool gps_ok;			// set when we get a valid line
	bool time_overridden;		// some element of time has been set by op
	bool loc_overridden;		// some element of location has been set by op

	Observer *obs;			// topocentric place

	float decimalYear();
	void newObserver (float lat, float lng, float hgt);

	/* implement on top of DateTime a running time based on elapsed millis()
	 */
	DateTime dt_now;
	float dt_TN0;
	long dt_DN0;
	uint32_t dt_m0;
	void getnow(int &year, uint8_t &month, uint8_t &day, uint8_t &h, uint8_t &m, uint8_t &s);
	void setnow(int year, uint8_t month, uint8_t day, uint8_t h, uint8_t m, uint8_t s);

    public:

	Adafruit_GPS *GPS;		// device handle -- only public because of interrupt handler
	double magdeclination;		// true az - magnetic az
	float latitude, longitude;	// degs +N, +E
	float altitude;			// altitude above MSL, m
	float hdop;			// horizontal degradation
	int nsats;			// number of satellites used

	Circum ();
	void sendNewValues (EthernetClient client);
	bool overrideValue (char *name, char *value);
	void checkGPS();
	DateTime now() {return dt_now;};
	float age (Satellite *sat);
	Observer *observer();
	void printSexa (EthernetClient client, float v);
	void printHMS (EthernetClient client, uint8_t h, uint8_t m, uint8_t s);
	void printDate (EthernetClient client, int y, uint8_t m, uint8_t d);

	typedef enum {
	    NORMAL, BADNEWS, GOODNEWS
	} PrintLevel;
	void printPL (EthernetClient client, PrintLevel pl);

};

extern Circum *circum;

#endif // _CIRCUM_H
