#ifndef GPS_H 
#define GPS_H


#define UART_PIN_TX 17
#define UART_PIN_RX 16
#define UART_BUFF 1024
#define UART_PORT UART_NUM_1

void nmea_to_deg_min(double raw, int *deg, double *min);

void uart_init(void);

void gps_coordinates_print(void);

#endif