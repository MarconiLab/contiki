/*
 * Copyright (c) 2015, George Oikonomou - <george@contiki-os.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*---------------------------------------------------------------------------*/
/**
 * \file
 *    Project specific configuration defines for the Ubidots demo
 *
 * \author
 *    George Oikonomou - <george@contiki-os.org>,
 */
/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/
/* Disable button shutdown functionality */
#define BUTTON_SENSOR_CONF_ENABLE_SHUTDOWN    0
/*---------------------------------------------------------------------------*/
/* User configuration */
#define UBIDOTS_DEMO_CONF_TEMPERATURE   "xxx" //ambient temp
#define UBIDOTS_DEMO_CONF_SEQUENCE      "xxx" //sequence
#define UBIDOTS_DEMO_CONF_PRESS  	"xxx" //press
#define UBIDOTS_DEMO_CONF_HUM 		"xxx" //hum
#define UBIDOTS_DEMO_CONF_LUX 		"xxx" //light
#define UBIDOTS_DEMO_CONF_BATT 		"xxx" //Volt
#define UBIDOTS_DEMO_CONF_TOBJ 		"xxx"// tempobject

#define UBIDOTS_CONF_IN_BUFFER_SIZE    1024
/*---------------------------------------------------------------------------*/
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC              nullrdc_driver
/*---------------------------------------------------------------------------*/
/* Change to match your configuration */
//#define NETSTACK_CONF_RDC		contikimac_driver
#define IEEE802154_CONF_PANID		0xABCD
//#define CC26XX_RF_CONF_CHANNEL          26
#define RF_CORE_CONF_CHANNEL            26
#define CC26XX_MODEL_CONF_CPU_VARIANT   2650 /* CC2650 */
#define CC26XX_RF_CONF_BLE_SUPPORT      1 /* Only available with CC2650 */
/*---------------------------------------------------------------------------*/
#endif /* PROJECT_CONF_H_ */
/*---------------------------------------------------------------------------*/
