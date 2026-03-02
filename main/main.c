// Project Name: GPS Driver for ESP32 by ESP-IDF 
// Author Name: Fayyaz Nisar Shaikh
// Email: fayyaz.nisar.shaikh@gmail.com
// Company: Viraj IoT Solutions - VISO


#include <stdbool.h>
#include <unistd.h>


//#include "freertos/FreeRTOS.h"
//#include "freertos/tasks.h"

#include "gps.h"


void app_main(void){
	
	
	uart_init();
		
	while(1){

		gps_coordinates_print(); // GPS Module Internally Produces 1 second delay, so need to add extra delay after this function block
		
    }
    
}