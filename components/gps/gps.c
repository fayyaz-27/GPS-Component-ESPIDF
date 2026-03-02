#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>



#include "esp_log.h"
#include "driver/uart.h"
//#include "driver/gpio.h"
//#include "freertos/FreeRTOS.h"

#include "gps.h"


static char nmea_line[256];
static int line_idx = 0;

uint8_t rx_buff[UART_BUFF];


static const char *TAG = "UART_RX";

static const char *TAG_GPS = "GPS";

typedef struct {
    const char *name;

    int lat_deg;
    double lat_min_center;
    double lat_tol;

    int lon_deg;
    double lon_min_center;
    double lon_tol;
} gps_city_t;

/* ---- SIMPLE CITY WINDOWS (degrees + minutes tolerance) ---- */
static const gps_city_t cities[] = {
    { "Andheri East", 19, 7.5, 2.0, 72, 51.8, 1.0 },
    { "Andheri West", 19, 7.5, 2.0, 72, 49.2, 1.0 },
    { "Andheri",      19, 8.0, 4.5, 72, 50.5, 4.5 },
    { "Thane",        19,13.0, 4.0, 72, 59.0, 4.0 },
    { "Kalyan East",  19,14.0, 3.0, 73, 09.5, 3.0 },
    { "Kalyan West",  19,14.0, 3.0, 73, 06.5, 3.0 },
    { "Kalyan",       19,14.0, 4.5, 73, 08.0, 4.5 },
    { "Pune",         18,31.0, 6.0, 73, 51.0, 6.0 },
};


#define CITY_COUNT (sizeof(cities) / sizeof(cities[0]))


static const char* gps_match_city(
    int lat_deg, double lat_min,
    int lon_deg, double lon_min
) {
    for (int i = 0; i < CITY_COUNT; i++) {

        if (lat_deg != cities[i].lat_deg)
            continue;

        if (lon_deg != cities[i].lon_deg)
            continue;

        if (fabs(lat_min - cities[i].lat_min_center) > cities[i].lat_tol)
            continue;

        if (fabs(lon_min - cities[i].lon_min_center) > cities[i].lon_tol)
            continue;

        return cities[i].name;
    }

    return "Unknown";
}


void uart_init(void){

	
	uart_config_t uart_config = {
    	.baud_rate = 9600,
    	.data_bits = UART_DATA_8_BITS,
    	.parity = UART_PARITY_DISABLE,
    	.stop_bits = UART_STOP_BITS_1,
    	.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    	.source_clk = UART_SCLK_DEFAULT

	};
	
		uart_driver_install(UART_PORT, UART_BUFF, UART_BUFF, 0, NULL, 0);

		uart_param_config(UART_PORT, &uart_config);
		
		uart_set_pin(UART_PORT, UART_PIN_TX, UART_PIN_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	
	    ESP_LOGI(TAG, "UART Initialized");
	    
	    ESP_LOGI(TAG_GPS, "GPS Initialized");
	    
	    printf("\n\n");
}

void nmea_to_deg_min(double raw, int *deg, double *min)
{
    *deg = (int)(raw / 100);
    *min = raw - (*deg * 100);
}

void gps_coordinates_print(void){
	
	
	int length = uart_read_bytes(UART_PORT, rx_buff, UART_BUFF - 1, pdMS_TO_TICKS(100));
		
		if (length > 0) {
        for (int i = 0; i < length; i++) {
            char c = rx_buff[i];


if (c == '\n') {
    nmea_line[line_idx] = '\0';
    line_idx = 0;
    
    printf("RAW: %s\n", nmea_line);


    if (strncmp(nmea_line, "$GPRMC", 6) == 0) {

        char copy[256];
        strncpy(copy, nmea_line, sizeof(copy));
        copy[sizeof(copy) - 1] = '\0';

        char *token;
        char *saveptr;
        int field = 0;

        double raw_lat = 0.0, raw_lon = 0.0;
        char lat_dir = 0, lon_dir = 0;
        bool fix_valid = false;

        token = strtok_r(copy, ",", &saveptr);
        while (token) {
            field++;

            if (field == 3) {                 // STATUS
                if (token[0] != 'A') {
                    fix_valid = false;
                    break;
                }
                fix_valid = true;
            }
            else if (field == 4) {            // LATITUDE
                raw_lat = atof(token);
            }
            else if (field == 5) {            // N / S
                lat_dir = token[0];
            }
            else if (field == 6) {            // LONGITUDE
                raw_lon = atof(token);
            }
            else if (field == 7) {            // E / W
                lon_dir = token[0];
                break;
            }

            token = strtok_r(NULL, ",", &saveptr);
        }

        if (!fix_valid) {
            continue;
        }

        int lat_deg, lon_deg;
        double lat_min, lon_min;

        nmea_to_deg_min(raw_lat, &lat_deg, &lat_min);
        nmea_to_deg_min(raw_lon, &lon_deg, &lon_min);
        
        const char *city = gps_match_city(lat_deg, lat_min, lon_deg, lon_min);


        printf("LAT: %d° %.5f' %c\n", lat_deg, lat_min, lat_dir);
        printf("LON: %d° %.5f' %c\n", lon_deg, lon_min, lon_dir);
        printf("CITY: %s\n\n", city);
    }
} else if (c != '\r' && line_idx < sizeof(nmea_line) - 1) {
                nmea_line[line_idx++] = c;
            }
        }
	}
	
}


