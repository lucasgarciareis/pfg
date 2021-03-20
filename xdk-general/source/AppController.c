/* module includes ********************************************************** */

/* own header files */
#include "XdkAppInfo.h"

#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_APP_CONTROLLER

/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "XDK_Sensor.h"
#include "BCDS_Assert.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "BCDS_WlanConnect.h"
#include "cJSON.h"
#include "BCDS_NetworkConfig.h"
#include "PAL_socketMonitor_ih.h"
#include "PIp.h"

#include <Serval_HttpClient.h>

/* constant definitions ***************************************************** */
#define APP_TEMPERATURE_OFFSET_CORRECTION   (-3459)
#define  SERVER_IP								"httpbin.org"

/* Global variables ********************************************************* */
uint32_t light;
long int humidity, temperature;


/* local variables ********************************************************** */

static CmdProcessor_T *AppCmdProcessor;/**< Handle to store the main Command processor handle to be reused by ServalPAL thread */

static xTimerHandle sensorHandle = NULL;


static Sensor_Setup_T SensorSetup =
        {
                .CmdProcessorHandle = NULL,
                .Enable =
                        {
                                .Accel = false,
                                .Mag = false,
                                .Gyro = false,
                                .Humidity = true,
                                .Temp = true,
                                .Pressure = false,
                                .Light = true,
                                .Noise = false,
                        },
                .Config =
                        {
                                .Light =
                                        {
                                                .IsInteruptEnabled = false,

                                        },
                                .Temp =
                                        {
                                                .OffsetCorrection = APP_TEMPERATURE_OFFSET_CORRECTION,
                                        },
                        },
        };/**< Sensor setup parameters */

retcode_t writeNextPartToBuffer(OutMsgSerializationHandover_T* handover)
{
	static char  *payload;
	static cJSON *JSON;

	JSON = cJSON_CreateObject();

	cJSON_AddItemToObject(JSON,"temperature",cJSON_CreateNumber(temperature));
	cJSON_AddItemToObject(JSON,"humidity",cJSON_CreateNumber(humidity));
	cJSON_AddItemToObject(JSON,"light",cJSON_CreateNumber(light));
	payload = cJSON_Print(JSON);
	cJSON_Minify(payload);
	printf("payload %s \r\n",payload);

	cJSON_Delete(JSON);


	uint16_t payloadLength = (uint16_t) strlen(payload);
	uint16_t alreadySerialized = handover->offset;
	uint16_t remainingLength = payloadLength - alreadySerialized;
	uint16_t bytesToCopy;
	retcode_t rc;
	if (remainingLength <= handover->bufLen)
	{
		bytesToCopy = remainingLength;
		rc = RC_OK;
	}
	else
	{
		bytesToCopy = handover->bufLen;
		rc = RC_MSG_FACTORY_INCOMPLETE;
	}

	memcpy(handover->buf_ptr, payload + alreadySerialized, bytesToCopy);
	handover->offset = alreadySerialized + bytesToCopy;
	handover->len = bytesToCopy;
	free(payload);
	return rc;
}

static retcode_t onHTTPRequestSent ( Callable_T *callfunc, retcode_t status)
{
	(void) (callfunc);
		if (status != RC_OK){
			printf("Falha em enviar o HTTP request!\r\n");
		}
		return(RC_OK);
}

static retcode_t onHTTPResponseReceived(HttpSession_T *httpSession, Msg_T *msg_ptr, retcode_t status)
{
	(void) (httpSession);
	if (status == RC_OK && msg_ptr != NULL)
	{
		Http_StatusCode_T statusCode = HttpMsg_getStatusCode(msg_ptr);
		char const *contentType = HttpMsg_getContentType(msg_ptr);
		char const *content_ptr;
		unsigned int contentLength = 0;
		HttpMsg_getContent(msg_ptr,&content_ptr, &contentLength);
		char content[contentLength+1];
		strncpy(content, content_ptr, contentLength);
		content[contentLength] = 0;
		//printf("HTTP RESPONSE: %d [%s]\r\n", statusCode, contentType);
		//printf("%s\r\n", content);
	}
	else
	{
		Http_StatusCode_T statusCode = HttpMsg_getStatusCode(msg_ptr);
		printf("Falha ao receber a resposta HTTP!\r\n");
		printf("STATUS CODE: %d \r\n", statusCode);
	}

	return(RC_OK);
}

void networkSetup(void) {

	WlanConnect_SSID_T connectSSID = (WlanConnect_SSID_T) "FOSSI";
	WlanConnect_PassPhrase_T connectPassPhrase = (WlanConnect_PassPhrase_T) "fossi7310";
	WlanConnect_Init();
	NetworkConfig_SetIpDhcp(0);
	WlanConnect_WPA(connectSSID,connectPassPhrase, NULL);
	PAL_initialize();
	PAL_socketMonitorInit();

	HttpClient_initialize();
}


static void AppControllerFire(void* pvParameters)
{
    BCDS_UNUSED(pvParameters);

    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));

        retcode = Sensor_GetData(&sensorValue);
        if (RETCODE_OK == retcode)
        {

                //printf("BME280 Environmental Conversion Data for Humidity:\n\rh =%ld %%rh\r\n",
                //        (long int) sensorValue.RH);
        		humidity = (long int) sensorValue.RH;

                //printf("BME280 Environmental Conversion Data for temperature :\n\rt =%ld mDeg\r\n",
                //        (long int) sensorValue.Temp);

        		temperature = (long int) sensorValue.Temp;

                //printf("Light sensor data obtained in millilux :%d \n\r", (unsigned int) sensorValue.Light);

        		light = sensorValue.Light;
            }

        if (RETCODE_OK != retcode)
        {
            Retcode_RaiseError(retcode);
        }

        Ip_Address_T destAddr;
        PAL_getIpaddress((uint8_t*) SERVER_IP, &destAddr);
        Ip_Port_T port = Ip_convertIntToPort(80);

        Msg_T* msg_prt;
        HttpClient_initRequest(&destAddr, port, &msg_prt);
        HttpMsg_setReqMethod(msg_prt, Http_Method_Post);
        HttpMsg_setReqUrl(msg_prt,"/post");
        HttpMsg_setHost(msg_prt, SERVER_IP);

        HttpMsg_setContentType(msg_prt, Http_ContentType_App_Json);

        Msg_prependPartFactory(msg_prt, &writeNextPartToBuffer);


        static Callable_T sentCallable;
        Callable_assign(&sentCallable, &onHTTPRequestSent);

        HttpClient_pushRequest(msg_prt, &sentCallable, &onHTTPResponseReceived);
}

static void AppControllerEnable(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    Retcode_T retcode = Sensor_Enable();


    if (RETCODE_OK == retcode)
    {

        uint32_t timerBlockTime = UINT32_MAX;
        xTimerStart(sensorHandle, timerBlockTime);

    }
}


static void AppControllerSetup(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    uint32_t oneSecondDelay = UINT32_C(1000);
    uint32_t timerAutoReloadOn = UINT32_C(1);

    Retcode_T retcode = Sensor_Setup(&SensorSetup);
    networkSetup();

    if (RETCODE_OK == retcode)
    {
        sensorHandle = xTimerCreate((const char *)"AppController", oneSecondDelay, timerAutoReloadOn, NULL, AppControllerFire);
        retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerEnable, NULL, UINT32_C(0));
    }
    if (RETCODE_OK != retcode)
    {
        printf("AppControllerSetup : Failed \r\n");
        Retcode_RaiseError(retcode);
        assert(0); /* To provide LED indication for the user */
    }
}

/* global functions ***********************************************************/

/** Refer interface header for description*/
void AppController_Init(void * cmdProcessorHandle, uint32_t param2)
{
    BCDS_UNUSED(param2);

    Retcode_T retcode = RETCODE_OK;

    if (cmdProcessorHandle == NULL)
    {
        printf("AppController_Init : Command processor handle is NULL \r\n");
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        AppCmdProcessor = (CmdProcessor_T*) cmdProcessorHandle;
        retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerSetup, NULL, UINT32_C(0));
    }

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
        assert(0); /* To provide LED indication for the user */
    }
}
