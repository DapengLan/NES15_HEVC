#include <stdbool.h>
#include <stdint.h>
#include <string.h>     /* for memmove */
#include <time.h>       /* for timezone, localtime */
#include "bacdef.h"
#include "bacdcode.h"
#include "bacenum.h"
#include "bacapp.h"
#include "config.h"     /* the custom stuff */
#include "apdu.h"
#include "wp.h" /* WriteProperty handling */
#include "rp.h" /* ReadProperty handling */
#include "dcc.h"        /* DeviceCommunicationControl handling */
#include "version.h"
#include "device.h"     /* me */
#include "handlers.h"
#include "datalink.h"
#include "address.h"
/* os specfic includes */
#include "timer.h"
/* include the device object */
#include "device.h"
#include "ai.h"
#include "ao.h"
#include "av.h"
#include "bi.h"
#include "bo.h"
#include "bv.h"
#include "csv.h"
#include "lc.h"
#include "lsp.h"
#include "ms-input.h"
#include "mso.h"
#include "msv.h"
#include "trendlog.h"
#if defined(INTRINSIC_REPORTING)
#include "nc.h"
#endif /* defined(INTRINSIC_REPORTING) */
#if defined(BACFILE)
#include "bacfile.h"
#endif /* defined(BACFILE) */

void changeValue()
{
/*
typedef struct BACnet_Write_Property_Data {
    BACNET_OBJECT_TYPE object_type;
    uint32_t object_instance;
    BACNET_PROPERTY_ID object_property;
    uint32_t array_index;        use BACNET_ARRAY_ALL when not setting 
    uint8_t application_data[MAX_APDU];
    int application_data_len;
    uint8_t priority;    use BACNET_NO_PRIORITY if no priority 
    BACNET_ERROR_CLASS error_class;
    BACNET_ERROR_CODE error_code;
} BACNET_WRITE_PROPERTY_DATA;


BACNET_WRITE_PROPERTY_DATA *test;
*test.object_type = 13;
*test.object_instance = 0;
*test.object_property = 85;
*test.array_index = BACNET_ARRAY_ALL;
*test.application_data[0] = 5;
*test.application_data_len = 1;
*test.priority = BACNET_NO_PRIORITY;


Device_Write_Property_Local(test);
return 0;
*/

Multistate_Input_Present_Value_Set(0,4);

}
