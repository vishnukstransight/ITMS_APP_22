#ifndef UARTFUNCTIONS_H
#define UARTFUNCTIONS_H








int open_port_LED(void);
int open_port_GPS(void);
void send_file_via_serial(const char *file_path, int serial_fd);

#endif
