#include "license.h"

//Returns true(1) if valid,else returns false(0)
int is_gpu_serial_number_valid(char * serial_number)
{
  //Array for stroring real serial number of GPU(index 0)
  char gpu_serial_number[NVML_DEVICE_SERIAL_BUFFER_SIZE];
  nvmlDevice_t* device;
  //For obtaining "nvmlDevice_t* device" and use it to get the serial number.
  nvmlDeviceGetHandleByIndex(0, device);
  //Get real serial number
  nvmlDeviceGetSerial(device, gpu_serial_number, NVML_DEVICE_SERIAL_BUFFER_SIZE);
  //Compare with the serial number stored in a file
  int result = memcmp(parameters[0], gpu_serial_number, NVML_DEVICE_SERIAL_BUFFER_SIZE);
  if(result != 0)
    return 0;
  return 1;
}

//Returns true(1) if valid,else returns false(0)
int is_expire_date_valid(char * expire_date)
{
  //Get current date
  time_t t = time(NULL);
  tm* timePtr = localtime(&t);
  int day = timePtr->tm_mday;
  int month = timePtr->tm_mon+1;
  int year = timePtr->tm_year+1900;
  //Components for comparing
  string date_string;
  stringstream string_stream;
  int date_int = 0;
  //Compare year
  date_string += expire_date[6];date_string += expire_date[7];date_string += expire_date[8];date_string += expire_date[9];
  string_stream << date_string;
  string_stream >> date_int;
  if (date_int < year)
      return 0;
  //Compare month
  date_string = "";
  date_string.clear();
  string_stream.clear();
  date_string += expire_date[3];date_string += expire_date[4];
  string_stream << date_string;
  date_int = 0;
  string_stream >> date_int;
  if (date_int < month)
      return 0;
  //Compare day
  date_string = "";
  date_string.clear();
  string_stream.clear();
  date_string += expire_date[0];date_string += expire_date[1];
  string_stream << date_string;
  date_int = 0;
  string_stream >> date_int;
  if (date_int < day)
      return 0;
  return 1;
}

//Returns true(1) if valid,else returns false(0)
int is_mac_address_valid(char * mac_address)
{
//Components for comparing
string adress_full_string;
struct ifaddrs *ifaddr=NULL;
struct ifaddrs *ifa = NULL;
int i = 0;
if (getifaddrs(&ifaddr) == -1)
{
   cout << "Can not get real MAC address." << '\n';
}
else
{
ifa = ifaddr;
ifa = ifa->ifa_next;
adress_full_string.push_back(mac_address[1][0]);adress_full_string.push_back(mac_address[1][1]);
adress_full_string.push_back(mac_address[1][3]);adress_full_string.push_back(mac_address[1][4]);
adress_full_string.push_back(mac_address[1][6]);adress_full_string.push_back(mac_address[1][7]);
adress_full_string.push_back(mac_address[1][9]);adress_full_string.push_back(mac_address[1][10]);
adress_full_string.push_back(mac_address[1][12]);adress_full_string.push_back(mac_address[1][13]);
struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
int k = 0;
for (i=0; i <s->sll_halen; i++)
{
  string temp;
  temp.push_back(adress_full_string[k]);
  temp.push_back(adress_full_string[k+1]);
  k+=2;
  stringstream int_stream(temp);
  int mac_part_int = 0;
  int_stream >> hex >> mac_part_int;

  if(mac_part_int != (s->sll_addr[i]))
  {
    freeifaddrs(ifaddr);
    return 0;
  }
}
  freeifaddrs(ifaddr);
  return 1;
}

//Check for validate
void is_gpu_valid()
{
  char parameters[3][NVML_DEVICE_SERIAL_BUFFER_SIZE] = {0, 0, 0};
  ifstream license_file(LICENSE_FILE_DIR, ios::in);
  if (license_file.is_open())
  {
    char file_character;
    int i = 0;
    int j =0;
    while (!license_file.eof() ) {

  	license_file.get(file_character);
    parameters[i][j] = file_character;
  		if (file_character == '\n'){
        i++;
        j = 0;
  		}
      else
        j++;
  	}
    fclose (license_file);
  }
  else
  {
    cout << "Failed to open GPU license file!\n";
  }

  if(!is_gpu_serial_number_valid(parameters[0]))
  {
    cout << "GPU serial number is not valid." << "The system will restart after 5 seconds." << '\n';
    usleep(5000000);
    system("shutdown -P now");
  }
  if(!is_expire_date_valid(parameters[1]))
  {
    cout << "Expire date is not valid." << "The system will restart after 5 seconds." << '\n';
    usleep(5000000);
    system("shutdown -P now");
  }
  if(!is_mac_address_valid(parameters[2]))
  {
    cout << "MAC address is not valid." << "The system will restart after 5 seconds." << '\n';
    usleep(5000000);
    system("shutdown -P now");
  }
}
