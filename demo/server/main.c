/**************************************************************************
*
* Copyright (C) 2006 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "config.h"
#include "server.h"
#include "address.h"
#include "bacdef.h"
#include "handlers.h"
#include "client.h"
#include "dlenv.h"
#include "bacdcode.h"
#include "npdu.h"
#include "apdu.h"
#include "iam.h"
#include "tsm.h"
#include "device.h"
#include "bacfile.h"
#include "datalink.h"
#include "dcc.h"
#include "getevent.h"
#include "net.h"
#include "txbuf.h"
#include "lc.h"
#include "version.h"

#include "libBBB.h"
#include "libBBB.c"
#include <math.h>
#define GPIO_PIN1	47 //p8_15
#define GPIO_PIN2	46 //p8_16

#include <sys/time.h>		/* for setitimer */
#include <unistd.h>		/* for pause */
#include <signal.h>		/* for signal */

#define INTERVAL 50		/* number of milliseconds to go off */


/* include the device object */
#include "device.h"
#include "trendlog.h"
#if defined(INTRINSIC_REPORTING)
#include "nc.h"
#endif /* defined(INTRINSIC_REPORTING) */
#if defined(BACFILE)
#include "bacfile.h"
#endif /* defined(BACFILE) */


/** @file server/main.c  Example server application using the BACnet Stack. */

/* (Doxygen note: The next two lines pull all the following Javadoc
 *  into the ServerDemo module.) */
/** @addtogroup ServerDemo */
/*@{*/

/** Buffer used for receiving */
static uint8_t Rx_Buf[MAX_MPDU] = { 0 };

/** Initialize the handlers we will utilize.
 * @see Device_Init, apdu_set_unconfirmed_handler, apdu_set_confirmed_handler
 */
static void Init_Service_Handlers(
    void)
{
    Device_Init(NULL);
    /* we need to handle who-is to support dynamic device binding */
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_HAS, handler_who_has);
    /* handle i-am to support binding to other devices */
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, handler_i_am_bind);
    /* set the handler for all the services we don't implement */
    /* It is required to send the proper reject message... */
    apdu_set_unrecognized_service_handler_handler
        (handler_unrecognized_service);
    /* Set the handlers for any confirmed services that we support. */
    /* We must implement read property - it's required! */
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY,
        handler_read_property);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE,
        handler_read_property_multiple);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_WRITE_PROPERTY,
        handler_write_property);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_WRITE_PROP_MULTIPLE,
        handler_write_property_multiple);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_RANGE,
        handler_read_range);
#if defined(BACFILE)
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_READ_FILE,
        handler_atomic_read_file);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_WRITE_FILE,
        handler_atomic_write_file);
#endif
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_REINITIALIZE_DEVICE,
        handler_reinitialize_device);
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_UTC_TIME_SYNCHRONIZATION,
        handler_timesync_utc);
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_TIME_SYNCHRONIZATION,
        handler_timesync);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_SUBSCRIBE_COV,
        handler_cov_subscribe);
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_COV_NOTIFICATION,
        handler_ucov_notification);
    /* handle communication so we can shutup when asked */
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL,
        handler_device_communication_control);
    /* handle the data coming back from private requests */
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_PRIVATE_TRANSFER,
        handler_unconfirmed_private_transfer);
#if defined(INTRINSIC_REPORTING)
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_ACKNOWLEDGE_ALARM,
        handler_alarm_ack);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_GET_EVENT_INFORMATION,
        handler_get_event_information);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_GET_ALARM_SUMMARY,
        handler_get_alarm_summary);
#endif /* defined(INTRINSIC_REPORTING) */
}

/** Main function of server demo.
 *
 * @see Device_Set_Object_Instance_Number, dlenv_init, Send_I_Am,
 *      datalink_receive, npdu_handler,
 *      dcc_timer_seconds, bvlc_maintenance_timer,
 *      Load_Control_State_Machine_Handler, handler_cov_task,
 *      tsm_timer_milliseconds
 *
 * @param argc [in] Arg count.
 * @param argv [in] Takes one argument: the Device Instance #.
 * @return 0 on success.
 */

/*
count algorithm specific variables

*/

#include "libBBB.h"
#include <math.h>
#define GPIO_PIN1	47 //p8_15
#define GPIO_PIN2	46 //p8_16

#include <sys/time.h>		/* for setitimer */
#include <unistd.h>		/* for pause */
#include <signal.h>		/* for signal */

#define INTERVAL 50		/* number of milliseconds to go off */

int flagIn = 0;
int flagOut = 0;
int countPeopleIn = 0;
int countPeopleOut = 0;
int count = 0;
float countP = 0;
float samplingTime = 50;
float timePeriod = 350;
int idleCount = 0;


void countUpdate();

int countPeopleInFunc ();
int countPeopleOutFunc ();


int main(
    int argc,
    char *argv[])
{
    BACNET_ADDRESS src = {
        0
    };  /* address where message came from */
    uint16_t pdu_len = 0;
    unsigned timeout = 1;       /* milliseconds */
    time_t last_seconds = 0;
    time_t current_seconds = 0;
    uint32_t elapsed_seconds = 0;
    uint32_t elapsed_milliseconds = 0;
    uint32_t address_binding_tmr = 0;
    uint32_t recipient_scan_tmr = 0;
    uint16_t count = 0;

    /* allow the device ID to be set */
    if (argc > 1)
        Device_Set_Object_Instance_Number(strtol(argv[1], NULL, 0));
    printf("BACnet Server Demo\n" "BACnet Stack Version %s\n"
        "BACnet Device ID: %u\n" "Max APDU: %d\n", BACnet_Version,
        Device_Object_Instance_Number(), MAX_APDU);
    /* load any static address bindings to show up
       in our device bindings list */
    address_init();
    Init_Service_Handlers();
    dlenv_init();
    atexit(datalink_cleanup);
    /* configure the timeout values */
    last_seconds = time(NULL);
    /* broadcast an I-Am on startup */
    Send_I_Am(&Handler_Transmit_Buffer[0]);

//////////////////////////////////////
	initPin(GPIO_PIN1);
	initPin(GPIO_PIN2);
	setPinDirection(GPIO_PIN1, IN);
	setPinDirection(GPIO_PIN2, IN);

  struct itimerval it_val;	/* for setting itimer */

  /* Upon SIGALRM, call DoStuff().
   * Set interval timer.  We want frequency in ms, 
   * but the setitimer call needs seconds and useconds. */
  if (signal(SIGALRM, (void (*)(int)) countPeopleInFunc) == SIG_ERR) {
    perror("Unable to catch SIGALRM");
    exit(1);
  }
/*
  if (signal(SIGALRM, (void (*)(int)) countPeopleOutFunc) == SIG_ERR) {
    perror("Unable to catch SIGALRM");
    exit(1);
  }
*/
  it_val.it_value.tv_sec =     INTERVAL/1000;
  it_val.it_value.tv_usec =    (INTERVAL*1000) % 1000000;	
  it_val.it_interval = it_val.it_value;
  if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
    perror("error calling setitimer()");
    exit(1);
  }
////////////////////////////////////////////////////



    /* loop forever */
    for (;;) {
        /* input */
        current_seconds = time(NULL);

        /* returns 0 bytes on timeout */
        pdu_len = datalink_receive(&src, &Rx_Buf[0], MAX_MPDU, timeout);

        /* process */
        if (pdu_len) {
            npdu_handler(&src, &Rx_Buf[0], pdu_len);
        }
        /* at least one second has passed */
        elapsed_seconds = (uint32_t) (current_seconds - last_seconds);
        if (elapsed_seconds) {
            last_seconds = current_seconds;
            dcc_timer_seconds(elapsed_seconds);
#if defined(BACDL_BIP) && BBMD_ENABLED
            bvlc_maintenance_timer(elapsed_seconds);
#endif
            dlenv_maintenance_timer(elapsed_seconds);
            Load_Control_State_Machine_Handler();
            elapsed_milliseconds = elapsed_seconds * 1000;
            handler_cov_timer_seconds(elapsed_seconds);
            tsm_timer_milliseconds(elapsed_milliseconds);
            trend_log_timer(elapsed_seconds);
#if defined(INTRINSIC_REPORTING)
            Device_local_reporting();
#endif
        }
        handler_cov_task();
        /* scan cache address */
        address_binding_tmr += elapsed_seconds;
        if (address_binding_tmr >= 60) {
            address_cache_timer(address_binding_tmr);
            address_binding_tmr = 0;
        }
#if defined(INTRINSIC_REPORTING)
        /* try to find addresses of recipients */
        recipient_scan_tmr += elapsed_seconds;
        if (recipient_scan_tmr >= NC_RESCAN_RECIPIENTS_SECS) {
            Notification_Class_find_recipient();
            recipient_scan_tmr = 0;
        }
#endif
	
        /* output */

        /* blink LEDs, Turn on or off outputs, etc */
    }

    return 0;
}

void countUpdate() {
	idleCount = 0;
	countP += count * samplingTime / timePeriod ; //ceil?

	double temp = 0;
	temp = countP;
	int tempInt = round(temp);
	Multistate_Input_Present_Value_Set(0,tempInt);

	printf(" countP: %d\n", tempInt);
	count = 0;
	countPeopleIn = 0;
	countPeopleOut = 0;
	flagIn = 0;
	flagOut = 0;
}

int countPeopleInFunc () {

	int x = getPinValue(GPIO_PIN1);

	if (x == 0 )
		{
			idleCount = 0;
			if (flagOut == 0)
			{
				flagIn = 1;
				countPeopleIn += 1;
			}
			else {
				countPeopleOut -= 1;
				count -= 1;
				if(countPeopleOut ==0 )
					 flagOut = 0;
			}
		}
	else
	{
		idleCount +=1;
		if(idleCount >= 40 )
		{
			countUpdate();
		}
	}

//printf(" countIn: %d\n", countPeopleIn);
countPeopleOutFunc();
	}

int countPeopleOutFunc () {

	int x = getPinValue(GPIO_PIN2);

	if (x == 0 )
		{
			idleCount = 0;
			if (flagIn == 0)
			{
				flagOut = 1;
				countPeopleOut += 1;
			}
			else {
				countPeopleIn -= 1;
				count += 1;
				if(countPeopleIn ==0 )
					 flagIn = 0;
			}
		}
	else
	{
		idleCount +=1;
		if(idleCount >= 40 )
		{
			countUpdate();
		}
	}
//printf(" countOut: %d\n", countPeopleOut);
	}


/* @} */

/* End group ServerDemo */
