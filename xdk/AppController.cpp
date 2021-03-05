/*
* Licensee agrees that the example code provided to Licensee has been developed and released by Bosch solely as an example to be used as a potential reference for application development by Licensee.
* Fitness and suitability of the example code for any use within application developed by Licensee need to be verified by Licensee on its own authority by taking appropriate state of the art actions and measures (e.g. by means of quality assurance measures).
* Licensee shall be responsible for conducting the development of its applications as well as integration of parts of the example code into such applications, taking into account the state of the art of technology and any statutory regulations and provisions applicable for such applications. Compliance with the functional system requirements and testing there of (including validation of information/data security aspects and functional safety) and release shall be solely incumbent upon Licensee.
* For the avoidance of doubt, Licensee shall be responsible and fully liable for the applications and any distribution of such applications into the market.
*
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
*     (1) Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*
*     (2) Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*
*     (3)The name of the author may not be used to
*     endorse or promote products derived from this software without
*     specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*/
/*----------------------------------------------------------------------------*/
/**
 * @ingroup APPS_LIST
 *
 * @defgroup XDK_APPLICATION_TEMPLATE HTTP
 * @{
 *
 * @brief XDK Application Template
 *
 * @details XDK Application Template without any functionality.
 * Could be used as a starting point to develop new application based on XDK platform.
 *
 * @file
 **/
/* module includes ********************************************************** */

/* own header files */
#include "XdkAppInfo.h"
#undef BCDS_MODULE_ID /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_APP_CONTROLLER

/* own header files */
#include "AppController.h"

/* system header files */
#include <stdio.h>
#include <Serval_HttpClient.h>
/* additional interface header files */
#include "BCDS_CmdProcessor.h"
#include "XDK_Sensor.h"
#include "XDK_Utils.h"
#include "FreeRTOS.h"
#include "BCDS_Assert.h"
#include "task.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
//#include "PAL_Initialize_ih.h"
#include "PAL_socketMonitor_ih.h"
#include "PIp.h"
#include "timers.h"
#include "XdkSensorHandle.h"

/* constant definitions ***************************************************** */
#define XDK_APP_DELAY UINT32_C(1000)
#define APP_TEMPERATURE_OFFSET_CORRECTION (-3459)
/* local variables ********************************************************** */

static CmdProcessor_T *AppCmdProcessor; /**< Handle to store the main Command processor handle to be used by run-time event driven threads */

static xTaskHandle AppControllerHandle = NULL; /**< OS thread handle for Application controller to be used by run-time blocking threads */

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/**
 * @brief Responsible for controlling application control flow.
 * Any application logic which is blocking in nature or fixed time dependent
 * can be placed here.
 *
 * @param[in] pvParameters
 * FreeRTOS task handle. Could be used if more than one thread is using this function block.
 */

static Sensor_Setup_T SensorSetup =
    {
        .CmdProcessorHandle = NULL,
        .Enable =
            {
                .Accel = true,
                .Mag = true,
                .Gyro = true,
                .Humidity = true,
                .Temp = true,
                .Pressure = true,
                .Light = true,
                .Noise = false,
            },
        .Config =
            {

                .Accel =
                    {
                        .Type = SENSOR_ACCEL_BMA280,
                        .IsRawData = false,
                        .IsInteruptEnabled = false,

                    },
                .Gyro =
                    {
                        .Type = SENSOR_GYRO_BMG160,
                        .IsRawData = false,
                    },
                .Mag =
                    {
                        .IsRawData = false,
                    },
                .Light =
                    {
                        .IsInteruptEnabled = false,

                    },
                .Temp =
                    {
                        .OffsetCorrection = APP_TEMPERATURE_OFFSET_CORRECTION,
                    },

            },
};

retcode_t writeNextPartToBuffer(OutMsgSerializationHandover_T *handover)
{
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));

    Sensor_GetData(&sensorValue);

    float AccelX = ((long int)sensorValue.Accel.X * 0.00980665);
    float AccelY = ((long int)sensorValue.Accel.Y * 0.00980665);
    float AccelZ = ((long int)sensorValue.Accel.Z * 0.00980665);
    float temp = ((long int)sensorValue.Temp / 1000);
    unsigned int light = (unsigned int)sensorValue.Light;
    long int GyroscopeX = (long int)sensorValue.Gyro.X;
    long int GyroscopeY = (long int)sensorValue.Gyro.Y;
    long int GyroscopeZ = (long int)sensorValue.Gyro.Z;
    long int MagX = (long int)sensorValue.Mag.X;
    long int MagY = (long int)sensorValue.Mag.Y;
    long int MagZ = (long int)sensorValue.Mag.Z;
    long int Pressure = (long int)sensorValue.Pressure;
    long int Humidity = (long int)sensorValue.RH;

    char str[30];
    char str0[500];

    snprintf(str0, 100, "%s", "{\n\t\"xAceleration\": ");
    snprintf(str, 10, "%f", AccelX);
    strcat(str0, str);
    strcat(str0, ",\n\t\"yAceleration\": ");
    snprintf(str, 10, "%f", AccelY);
    strcat(str0, str);
    strcat(str0, ",\n\t\"zAceleration\": ");
    snprintf(str, 10, "%f", AccelZ);
    strcat(str0, str);
    strcat(str0, ",\n\t\"light\": ");
    snprintf(str, 10, "%d", light);
    strcat(str0, str);
    strcat(str0, ",\n\t\"xGyroscope\": ");
    snprintf(str, 10, "%ld", GyroscopeX);
    strcat(str0, str);
    strcat(str0, ",\n\t\"yGyroscope\": ");
    snprintf(str, 10, "%ld", GyroscopeY);
    strcat(str0, str);
    strcat(str0, ",\n\t\"zGyroscope\": ");
    snprintf(str, 10, "%ld", GyroscopeZ);
    strcat(str0, str);
    strcat(str0, ",\n\t\"xMagnetometer\": ");
    snprintf(str, 10, "%ld", MagX);
    strcat(str0, str);
    strcat(str0, ",\n\t\"yMagnetometer\": ");
    snprintf(str, 10, "%ld", MagY);
    strcat(str0, str);
    strcat(str0, ",\n\t\"zMagnetometer\": ");
    snprintf(str, 10, "%ld", MagZ);
    strcat(str0, str);
    strcat(str0, ",\n\t\"pressure\": ");
    snprintf(str, 10, "%ld", Pressure);
    strcat(str0, str);
    strcat(str0, ",\n\t\"temperature\": ");
    snprintf(str, 10, "%f", temp);
    strcat(str0, str);
    strcat(str0, ",\n\t\"humidity\": ");
    snprintf(str, 10, "%ld", Humidity);
    strcat(str0, str);
    strcat(str0, "\n}");

    printf("payload %s \r\n", str0);
    const char *payload = str0;
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
    return rc;
}

static retcode_t onHTTPRequestSent(Callable_T *callfunc, retcode_t status)
{
    (void)(callfunc);
    if (status != RC_OK)
    {
        printf("Falha em enviar o HTTP request!\r\n");
    }
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
        printf("%s\r\n", content);
    }
    else
    {
        Http_StatusCode_T statusCode = HttpMsg_getStatusCode(msg_ptr);
        printf("Falha ao receber a resposta HTTP!\r\n");
    }

    return (RC_OK);
}

void networkSetup(void)
{

    WlanConnect_SSID_T connectSSID = (WlanConnect_SSID_T) "Lucas";
    WlanConnect_PassPhrase_T connectPassPhrase = (WlanConnect_PassPhrase_T) "cebola123";
    WlanConnect_Init();
    NetworkConfig_SetIpDhcp(0);
    WlanConnect_WPA(connectSSID, connectPassPhrase, NULL);
    PAL_initialize();
    PAL_socketMonitorInit();

    HttpClient_initialize();
}

static void AppControllerFire(void *pvParameters)
{
    BCDS_UNUSED(pvParameters);

    Ip_Address_T destAddr;
    PAL_getIpaddress((uint8_t *)"192.168.0.13", &destAddr);
    Ip_Port_T port = Ip_convertIntToPort(5000);

    Msg_T *msg_prt;
    HttpClient_initRequest(&destAddr, port, &msg_prt);
    HttpMsg_setReqMethod(msg_prt, Http_Method_Post);
    HttpMsg_setReqUrl(msg_prt, "/sensors");
    HttpMsg_setHost(msg_prt, "192.168.0.13");

    HttpMsg_setContentType(msg_prt, Http_ContentType_App_Json);

    Msg_prependPartFactory(msg_prt, &writeNextPartToBuffer);

    static Callable_T sentCallable;
    Callable_assign(&sentCallable, &onHTTPRequestSent);

    HttpClient_pushRequest(msg_prt, &sentCallable, &onHTTPResponseReceived);
}

/**
 * @brief To enable the necessary modules for the application
 *
 * @param [in] param1
 * A generic pointer to any context data structure which will be passed to the function when it is invoked by the command processor.
 *
 * @param [in] param2
 * A generic 32 bit value  which will be passed to the function when it is invoked by the command processor..
 */
static void AppControllerEnable(void *param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    Retcode_T retcode = Sensor_Enable();

    if (RETCODE_OK == retcode)
    {

        uint32_t timerBlockTime = UINT32_MAX;
        xTimerStart(AppControllerHandle, timerBlockTime);
    }
}

/**
 * @brief To setup the necessary modules for the application
 *
 * @param [in] param1
 * A generic pointer to any context data structure which will be passed to the function when it is invoked by the command processor.
 *
 * @param [in] param2
 * A generic 32 bit value  which will be passed to the function when it is invoked by the command processor..
 */
static void AppControllerSetup(void *param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);
    //Retcode_T retcode = RETCODE_OK;

    networkSetup();

    uint32_t oneSecondDelay = UINT32_C(10000);
    uint32_t timerAutoReloadOn = UINT32_C(1);

    Retcode_T retcode = Sensor_Setup(&SensorSetup);

    if (RETCODE_OK == retcode)
    {
        AppControllerHandle = xTimerCreate((const char *)"AppController", oneSecondDelay, timerAutoReloadOn, NULL, AppControllerFire);
        retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerEnable, NULL, UINT32_C(0));
    }
    if (RETCODE_OK != retcode)
    {
        printf("AppControllerSetup : Failed \r\n");
        Retcode_RaiseError(retcode);
        assert(0); /* To provide LED indication for the user*/
    }
}

/* global functions ********************************************************* */

/** Refer interface header for description */
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

/**@} */
/** ************************************************************************* */
