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
 *    Example demonstrating how to use the Ubidots service
 *
 * \author
 *    George Oikonomou - <george@contiki-os.org>,
 */
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include "sys/process.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "ubidots.h"
//#include "dev/i2cmaster.h"
//#include "dev/tmp102.h"
//#include "lpm.h"

#include "board-peripherals.h"
#include "batmon-sensor.h"
//#include "cc26xx-rf.h"
#include "rf-core/rf-ble.h"
#include "ti-lib.h"

#include <stdio.h>
#include <string.h>
/*---------------------------------------------------------------------------*/
/* Sanity check */
#if !defined(UBIDOTS_DEMO_CONF_TEMPERATURE) || !defined(UBIDOTS_DEMO_CONF_SEQUENCE)
#error "UBIDOTS_DEMO_CONF_TEMPERATURE or UBIDOTS_DEMO_CONF_SEQUENCE undefined."
#error "Make sure you have followed the steps in the README"
#endif
/*---------------------------------------------------------------------------*/
/* Our own process' name */
PROCESS(ubidots_demo_process, "Ubidots demo process with SensorTag");
AUTOSTART_PROCESSES(&ubidots_demo_process);
/*---------------------------------------------------------------------------*/
/* POST period */
#define POST_PERIOD (CLOCK_SECOND * 30)
static struct etimer et;
/*---------------------------------------------------------------------------*/
#define VARIABLE_BUF_LEN 64
static unsigned int sequence;

static char variable_buffer[VARIABLE_BUF_LEN];
/*---------------------------------------------------------------------------*/
/*
 * 'List' of HTTP reply headers that we want to be notified about.
 * Terminate with NULL
 */
static const char *headers[] = {
  "Vary",
  NULL
};
/*---------------------------------------------------------------------------*/
/*
 * An example of how to POST one more more values to the same variable. This
 * primarily shows how to use the value argument depending on whether you
 * want to send a JSON string, number or boolean.
 */
static void
post_sequence_number(void)
{
  if(ubidots_prepare_post(UBIDOTS_DEMO_CONF_SEQUENCE) == UBIDOTS_ERROR) {
    printf("post_variable: ubidots_prepare_post failed\n");
  }

  memset(variable_buffer, 0, VARIABLE_BUF_LEN);

  /*
   * Write your value to the buffer. The contents of the buffer will be used
   * verbatim as the value of the variable in your JSON string. So, if you
   * store a number in the buffer this will become a JSON number. If you
   * enclose the value in double quotes, this will essentially be a JSON string
   *
   * Some examples
   * To send your value as a JSON number:
   * snprintf(variable_buffer, VARIABLE_BUF_LEN, "%u", sequence);
   *
   * To send your value as a JSON string:
   * snprintf(variable_buffer, VARIABLE_BUF_LEN, "\"%u\"", sequence);
   *
   * To send a JSON boolean:
   * ubidots_enqueue_value(NULL, "true");
   */
  snprintf(variable_buffer, VARIABLE_BUF_LEN, "%u", sequence);

  /* Append the contents of the buffer to your HTTP POST's payload */
  if(ubidots_enqueue_value(NULL, variable_buffer) == UBIDOTS_ERROR) {
    printf("post_variable (string): ubidots_enqueue_value failed\n");
  }

  /*
   * You can make a series of calls to ubidots_enqueue_value() here, as long
   * as they all have NULL as the first argument. In doing so, you can send
   * multiple values for the same variable
   */
  if(ubidots_post() == UBIDOTS_ERROR) {
    printf("post_variable: ubidots_post failed\n");
  }
}
/*---------------------------------------------------------------------------*/
/*
 * An example of how to post a collection: multiple different variables in
 * a single HTTP POST using {"variable":k,"value":v} pairs
 */
static void
post_collection(void)
{
  //uint16_t temp;
  int value;
  if(ubidots_prepare_post(NULL) == UBIDOTS_ERROR) {
    printf("post_collection: ubidots_prepare_post failed\n");
  }

  /* Encode and enqueue the uptime as a JSON number */
//----------------------------------------------------------------------  
  /*Temperature Ambient*/
  value = tmp_007_sensor.value(TMP_007_SENSOR_TYPE_ALL);
  if(value == CC26XX_SENSOR_READING_ERROR) {
    printf("TMP: Ambient Read Error\n");
  }
  memset(variable_buffer, 0, VARIABLE_BUF_LEN);

  value = tmp_007_sensor.value(TMP_007_SENSOR_TYPE_AMBIENT);
  printf("TMP: Ambient=%d.%03d C\n", value / 1000, value % 1000);
  snprintf(variable_buffer, VARIABLE_BUF_LEN, "%u", value/1000);
    
  if(ubidots_enqueue_value(UBIDOTS_DEMO_CONF_TEMPERATURE, variable_buffer) == UBIDOTS_ERROR) {
    printf("post_collection: ubidots_prepare_post  ambient failed\n");
  }

//----------------------------------------------------------------------  
  /*Temperature Object*/
/*  memset(variable_buffer, 0, VARIABLE_BUF_LEN);
  value = tmp_007_sensor.value(TMP_007_SENSOR_TYPE_OBJECT);
  printf("TMP: Ambient=%d.%03d C\n", value / 1000, value % 1000);
  snprintf(variable_buffer, VARIABLE_BUF_LEN, "%u", value/1000);
    
  if(ubidots_enqueue_value(UBIDOTS_DEMO_CONF_TOBJ, variable_buffer) == UBIDOTS_ERROR) {
    printf("post_collection: ubidots_prepare_post obj failed\n");
  }
*/
//----------------------------------------------------------------------
  /*Battery voltage*/
  memset(variable_buffer, 0, VARIABLE_BUF_LEN);
  value = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);
  printf("Bat: Volt=%d mV\n", (value * 125) >> 5);
  snprintf(variable_buffer, VARIABLE_BUF_LEN, "%u", (value*125)>>5);
    
  if(ubidots_enqueue_value(UBIDOTS_DEMO_CONF_BATT, variable_buffer) == UBIDOTS_ERROR) {
    printf("post_collection: ubidots_prepare_post volt failed\n");
  }

//----------------------------------------------------------------------
  /* Press */
  memset(variable_buffer, 0, VARIABLE_BUF_LEN);
  value = bmp_280_sensor.value(BMP_280_SENSOR_TYPE_PRESS);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    printf("BAR: Pressure=%d.%02d hPa\n", value / 100, value % 100);
  } else {
    printf("BAR: Pressure Read Error\n");
  }
  snprintf(variable_buffer, VARIABLE_BUF_LEN, "%u", value/100);

  if(ubidots_enqueue_value(UBIDOTS_DEMO_CONF_PRESS, variable_buffer) == UBIDOTS_ERROR) {
    printf("post_collection: ubidots_prepare_post press failed\n");
  }

 //----------------------------------------------------------------------
  /* And the sequence counter, again as a JSON number */
  memset(variable_buffer, 0, VARIABLE_BUF_LEN);
  snprintf(variable_buffer, VARIABLE_BUF_LEN, "%u", sequence);

  if(ubidots_enqueue_value(UBIDOTS_DEMO_CONF_SEQUENCE, variable_buffer) == UBIDOTS_ERROR) {
    printf("post_collection: ubidots_prepare_post seq failed\n");
  }

 
//----------------------------------------------------------------------
  /* Humidity */
  memset(variable_buffer, 0, VARIABLE_BUF_LEN);
  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_HUMIDITY);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    printf("HDC: Humidity=%d.%02d %%RH\n", value / 100, value % 100);
  } else {
    printf("HDC: Humidity Read Error\n");
  }
  snprintf(variable_buffer, VARIABLE_BUF_LEN, "%u", value/100);

  if(ubidots_enqueue_value(UBIDOTS_DEMO_CONF_HUM, variable_buffer) == UBIDOTS_ERROR) {
    printf("post_collection: ubidots_prepare_post hum failed\n");
  }

 
//----------------------------------------------------------------------
  /* optical */
  memset(variable_buffer, 0, VARIABLE_BUF_LEN);
  value = opt_3001_sensor.value(0);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    printf("OPT: Light=%d.%02d lux\n", value / 100, value % 100);
  } else {
    printf("OPT: Light Read Error\n");
  }


  snprintf(variable_buffer, VARIABLE_BUF_LEN, "%u",value/100);

  if(ubidots_enqueue_value(UBIDOTS_DEMO_CONF_LUX, variable_buffer) == UBIDOTS_ERROR) {
    printf("post_collection: ubidots_prepare_post opt failed\n");
  }

  //printf("POSTING TO UBI: ");
  //printf(variable_buffer);

  if(ubidots_post() == UBIDOTS_ERROR) {
    printf("post_collection: ubidots_prepare_post failed\n");
  }
}
/*---------------------------------------------------------------------------*/
/*
 * This is how to process the HTTP reply from the Ubidots server. In a real
 * scenario, we may wish to do something useful here, e.g. to test whether
 * the POST succeeded.
 *
 * This function here simply prints the entire thing, demonstrating how to use
 * the engine's API.
 */
static void
print_reply(ubidots_reply_part_t *r)
{
  switch(r->type) {
  case UBIDOTS_REPLY_TYPE_HTTP_STATUS:
    printf("HTTP Status: %ld\n", *((long int *)r->content));
    break;
  case UBIDOTS_REPLY_TYPE_HTTP_HEADER:
    printf("H: '%s'\n", (char *)r->content);
    break;
  case UBIDOTS_REPLY_TYPE_PAYLOAD:
    printf("P: '%s'\n", (char *)r->content);
    break;
  default:
    printf("Unknown reply type\n");
    break;
  }
}

/*---------------------------------------------------------------------------*/
static void
init_sensor_readings(void)
{
  SENSORS_ACTIVATE(hdc_1000_sensor);
  SENSORS_ACTIVATE(tmp_007_sensor);
  SENSORS_ACTIVATE(opt_3001_sensor);
  SENSORS_ACTIVATE(bmp_280_sensor);
  SENSORS_ACTIVATE(batmon_sensor);
//  init_mpu_reading(NULL);
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ubidots_demo_process, ev, data)
{
  PROCESS_BEGIN();

  /* Init the BLE advertisement daemon */
  rf_ble_beacond_config(0, BOARD_STRING);
  rf_ble_beacond_start();

  init_sensor_readings();
  
  ubidots_init(&ubidots_demo_process, headers);

  sequence = 0;

  while(1) {

    PROCESS_YIELD();
    //get_tmp_reading();

    if(ev == ubidots_event_established ||
       (ev == PROCESS_EVENT_TIMER && data == &et)) {
      leds_on(LEDS_GREEN);
      sequence++;

      //if(sequence & 1) {
      //  post_sequence_number();
      //} else {
        post_collection();
      //}
    } else if(ev == ubidots_event_post_sent) {
      leds_off(LEDS_GREEN);
      etimer_set(&et, POST_PERIOD);
    } else if(ev == ubidots_event_post_reply_received) {
      print_reply((ubidots_reply_part_t *)data);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
