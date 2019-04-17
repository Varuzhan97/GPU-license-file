#ifndef LICENSE_H
#define LICENSE_H

#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/reboot.h>
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <cuda_runtime.h>
//Library for monitoring gpu.Must install CUDA Toolkit for using this header file.
#include <nvidia/gdk/nvml.h>

#define NVML_DEVICE_SERIAL_BUFFER_SIZE 30

using namespace std;
//License file direction
const string LICENSE_FILE_DIR = "/var/license/gpu_license";

//Validate gpu serial number.
int is_gpu_serial_number_valid(char * serial_number)

//Validate pc MAC address.
int is_mac_address_valid(char * mac_address)

//Validate expire date
int is_expire_date_valid(char * expire_date)

//Check for valid
void is_gpu_valid()

#endif /* LICENSE_H */
