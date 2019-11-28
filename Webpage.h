/* this class handles the web page and interactions
 */

#ifndef _WEBPAGE_H
#define _WEBPAGE_H

#include <avr/pgmspace.h>

#include "Sensor.h"
#include "Circum.h"
#include "Gimbal.h"
#include "Target.h"
#include "NV.h"

// persistent state info to fetch a TLE from a remote web site incrementally
typedef struct {
    bool running;			// set while reading a remote file
    char sat[30];			// scrubbed name of satellite we are looking for
    char buf[200];			// long enough for a complete name and TLE: ~30+70+70
    char *l0, *l1, *l2;			// start of each TLE line within buf[], l0==NULL until complete
    EthernetClient *remote;		// remote connection object
    int lineno;				// show line number as progress
} TLEFetch;

class Webpage
{

    public:

	Webpage();
	void checkEthernet();
	void setUserMessage (const __FlashStringHelper *ifsh);
	void setUserMessage (const __FlashStringHelper *ifsh, const char *msg, char state);

    private:

	EthernetServer *httpServer;

	const __FlashStringHelper *user_message_F;
	char user_message_s[100];

	void startTLEFetch (char *query_text);
	void resumeTLEFetch (void);
	TLEFetch tlef;

	void scrub (char *s);
	char readNextClientChar (EthernetClient client, uint32_t *to);
	void overrideValue (EthernetClient client);
	void sendMainPage (EthernetClient client);
	void sendNewValues (EthernetClient client);
	void sendPlainHeader (EthernetClient client);
	void sendHTMLHeader (EthernetClient client);
	void sendEmptyResponse (EthernetClient client);
	void send404Page (EthernetClient client);
	void reboot();
};

extern Webpage *webpage;

#endif // _WEBPAGE_H
