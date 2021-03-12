/*----------------------------------------------------------------------------*/

/* --------------------------------------------------------------------------- |
 * INCLUDES & DEFINES ******************************************************** |
 * -------------------------------------------------------------------------- */
/* own header files */
#include "XdkAppInfo.h"
#undef BCDS_MODULE_ID /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_APP_CONTROLLER

/* system header files */
#include <stdio.h>
#include <time.h>
#include <Serval_HttpClient.h>

/* additional interface header files */
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "XDK_NoiseSensor.h"
#include "math.h"
#include "arm_math.h"
#include "BCDS_Assert.h"
#include "BCDS_WlanConnect.h"
#include "cJSON.h"
#include "BCDS_NetworkConfig.h"
#include "PAL_socketMonitor_ih.h"
#include "PIp.h"

/* --------------------------------------------------------------------------- |
 * HANDLES ******************************************************************* |
 * -------------------------------------------------------------------------- */

static CmdProcessor_T *AppCmdProcessor; /**< Handle to store the main Command processor handle to be used by run-time event driven threads */
xTimerHandle acousticHandle = NULL;

/* --------------------------------------------------------------------------- |
 * VARIABLES ***************************************************************** |
 * -------------------------------------------------------------------------- */

const float aku340ConversionRatio = pow(10, (-38 / 20));

const float SplRatio = pow(10, (-38 / 20)) * 20e-6;

static arm_rfft_instance_f32 rfft_instance;

static arm_cfft_radix4_instance_f32 cfft_instance;

/** Complex (interleaved) output from FFT. */
static float32_t *fftOutputComplex;

/** Magnitude of complex numbers in FFT output. */
static float32_t *fftOutputMag;

#define SAMPLING_FREQUENCY UINT32_C(50) //remember to change inside findHighestMagnitudeAndFrequency function

#define ACOUSTIC_BUFFER_SIZE UINT32_C(64)

static float32_t acousticSensorBuffer[ACOUSTIC_BUFFER_SIZE];

#define SERVER_IP "192.168.0.11"
//#define  SERVER_IP								"httpbin.org"

unsigned int counter = 0;

/* --------------------------------------------------------------------------- |
 * EXECUTING FUNCTIONS ******************************************************* |
 * -------------------------------------------------------------------------- */

void findHighestMagnitudeAndFrequency(float32_t *FFTmagnitudes, float32_t windowSize)
{

	float32_t maxVal;
	uint32_t maxIndex;
	float32_t freqVal;

	float32_t samplingFrequency = 50;

	arm_max_f32(FFTmagnitudes,
				windowSize / 2,
				&maxVal,
				&maxIndex);

	float32_t freqBin = samplingFrequency / windowSize;

	freqVal = freqBin * maxIndex;

	//printf("The maximal magnitude %f is at the frequency %f \n\r",(float) maxVal, (float) freqVal);
}

void processFFT(float32_t *window, float32_t windowSize)
{

	/* Process the data through the RFFT module, resulting complex output is
     * stored in fftOutputComplex
     */
	arm_rfft_f32(&rfft_instance, window, fftOutputComplex);

	/* Compute the magnitude of all the resulting complex numbers */
	arm_cmplx_mag_f32(fftOutputComplex,
					  fftOutputMag,
					  windowSize);
}

uint32_t initializeFFT(uint32_t windowSize)
{

	fftOutputComplex = (float32_t *)malloc(windowSize * sizeof(float32_t) * 2);
	fftOutputMag = (float32_t *)malloc(windowSize * sizeof(float32_t));

	// Initialize the CFFT/CIFFT module
	arm_status status = arm_rfft_init_f32(&rfft_instance,
										  &cfft_instance,
										  windowSize,
										  0,  // forward transform
										  1); // normal, not bitreversed, order

	return status;
}

float32_t calcSoundIntensityFromSoundPressureLevel(float32_t spl)
{
	return (pow(10, spl / 10) * 1e-12);
}

float calcSoundPressureLevel(float magnitude)
{

	float spl;

	if (magnitude == 0)
	{
		spl = 0;
	}

	else
	{
		spl = (20 * log10(magnitude / SplRatio) - 20);
	}

	return spl;
}

float calcSoundPressure(float acousticRawValue)
{
	return (acousticRawValue / aku340ConversionRatio);
}

retcode_t writeNextPartToBuffer(OutMsgSerializationHandover_T *handover)
{
	//printf("payload %s \r\n",str0);
	static char *payload;
	static cJSON *JSON;

	JSON = cJSON_CreateObject();

	cJSON_AddItemToObject(JSON, "data", cJSON_CreateFloatArray(acousticSensorBuffer, ACOUSTIC_BUFFER_SIZE));

	payload = cJSON_Print(JSON);
	cJSON_Minify(payload);
	printf("payload %s \r\n", payload);

	cJSON_Delete(JSON);

	uint16_t payloadLength = (uint16_t)strlen(payload);
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

static retcode_t onHTTPRequestSent(Callable_T *callfunc, retcode_t status)
{
	(void)(callfunc);
	if (status != RC_OK)
	{
		printf("Falha em enviar o HTTP request!\r\n");
	}
	return (RC_OK);
}

static retcode_t onHTTPResponseReceived(HttpSession_T *httpSession, Msg_T *msg_ptr, retcode_t status)
{
	(void)(httpSession);
	if (status == RC_OK && msg_ptr != NULL)
	{
		Http_StatusCode_T statusCode = HttpMsg_getStatusCode(msg_ptr);
		char const *contentType = HttpMsg_getContentType(msg_ptr);
		char const *content_ptr;
		unsigned int contentLength = 0;
		HttpMsg_getContent(msg_ptr, &content_ptr, &contentLength);
		char content[contentLength + 1];
		strncpy(content, content_ptr, contentLength);
		content[contentLength] = 0;
		printf("HTTP RESPONSE: %d [%s]\r\n", statusCode, contentType);
		//printf("%s\r\n", content);
	}
	else
	{
		Http_StatusCode_T statusCode = HttpMsg_getStatusCode(msg_ptr);
		printf("Falha ao receber a resposta HTTP!\r\n");
		printf("STATUS CODE: %d \r\n", statusCode);
	}

	return (RC_OK);
}

void networkSetup(void)
{

	WlanConnect_SSID_T connectSSID = (WlanConnect_SSID_T) "FOSSI";
	WlanConnect_PassPhrase_T connectPassPhrase = (WlanConnect_PassPhrase_T) "fossi7310";
	WlanConnect_Init();
	NetworkConfig_SetIpDhcp(0);
	WlanConnect_WPA(connectSSID, connectPassPhrase, NULL);
	PAL_initialize();
	PAL_socketMonitorInit();

	HttpClient_initialize();
}

static void readAcousticSensor(xTimerHandle xTimer)
{
	(void)xTimer;

	float acousticData, sp, spl;
	double si;

	if (RETCODE_OK == NoiseSensor_ReadRmsValue(&acousticData, 10U))
	{

		//sp = calcSoundPressure(acousticData);
		//spl = calcSoundPressureLevel(acousticData);
		//si = calcSoundIntensityFromSoundPressureLevel(spl);

		//printf("Sound pressure in Pa: %f \r\n", sp);
		//printf("Sound pressure level in dB: %f \n\r", spl);
		// Sound intensity value multiplied by 1000 for better visability
		//printf("Sound Intesity in mW / m^2: %lf \n\r", (si*1000));

		acousticSensorBuffer[counter] = acousticData;
		counter++;

		if (counter == ACOUSTIC_BUFFER_SIZE)
		{

			printf("Calculating FFT \n\r");
			processFFT(acousticSensorBuffer, ACOUSTIC_BUFFER_SIZE);

			findHighestMagnitudeAndFrequency(fftOutputMag, ACOUSTIC_BUFFER_SIZE);

			Ip_Address_T destAddr;
			PAL_getIpaddress((uint8_t *)SERVER_IP, &destAddr);
			Ip_Port_T port = Ip_convertIntToPort(54322);
			//Ip_Port_T port = Ip_convertIntToPort(80);

			Msg_T *msg_prt;
			HttpClient_initRequest(&destAddr, port, &msg_prt);
			HttpMsg_setReqMethod(msg_prt, Http_Method_Post);
			HttpMsg_setReqUrl(msg_prt, "/sound");
			//HttpMsg_setReqUrl(msg_prt,"/post");
			HttpMsg_setHost(msg_prt, SERVER_IP);

			HttpMsg_setContentType(msg_prt, Http_ContentType_App_Json);

			Msg_prependPartFactory(msg_prt, &writeNextPartToBuffer);

			static Callable_T sentCallable;
			Callable_assign(&sentCallable, &onHTTPRequestSent);

			HttpClient_pushRequest(msg_prt, &sentCallable, &onHTTPResponseReceived);

			memset(&acousticSensorBuffer, 0x00, sizeof(float32_t));
			counter = 0;
		}
	}
}

static void AppControllerEnable(void *param1, uint32_t param2)
{
	BCDS_UNUSED(param1);
	BCDS_UNUSED(param2);

	uint32_t timerBlockTime = UINT32_MAX;

	/* Enable necessary modules for the application and check their return values */
	if (RETCODE_OK == NoiseSensor_Enable())
	{
		xTimerStart(acousticHandle, timerBlockTime);
	}
}

static void AppControllerSetup(void *param1, uint32_t param2)
{
	BCDS_UNUSED(param1);
	BCDS_UNUSED(param2);
	Retcode_T retcode = RETCODE_OK;

	/* Setup the necessary modules required for the application */
	initializeFFT(ACOUSTIC_BUFFER_SIZE);
	networkSetup();

	if (RETCODE_OK == NoiseSensor_Setup(22050U))
	{
		uint32_t oneSecondDelay = SAMPLING_FREQUENCY;
		uint32_t timerAutoReloadOn = UINT32_C(1);

		acousticHandle = xTimerCreate((const char *)"readAcousticSensor", oneSecondDelay, timerAutoReloadOn, NULL, readAcousticSensor);
	}

	retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerEnable, NULL, UINT32_C(0));
	if (RETCODE_OK != retcode)
	{
		printf("AppControllerSetup : Failed \r\n");
		Retcode_RaiseError(retcode);
		assert(0); /* To provide LED indication for the user */
	}
}

void AppController_Init(void *cmdProcessorHandle, uint32_t param2)
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
		AppCmdProcessor = (CmdProcessor_T *)cmdProcessorHandle;
		retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerSetup, NULL, UINT32_C(0));
	}

	if (RETCODE_OK != retcode)
	{
		Retcode_RaiseError(retcode);
		assert(0); /* To provide LED indication for the user */
	}
}

/** ************************************************************************* */
