// C library headers
#include <stdio.h>
#include <string.h>
#include<signal.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

void sig_handler(int signo)
{

    if (signo == SIGTERM)
        printf("received SIGTERM\n");
        close(serial_port);
        while(1)
        sleep(1);
    // A long long wait so that we can easily issue a signal to this process

    else if (signo == SIGKILL)
        printf("received SIGKILL\n");
        close(serial_port);
        while(1)
        sleep(1);

    else if (signo == SIGSTOP)
        printf("received SIGSTOP\n");
        close(serial_port);
        while(1)
        sleep(1);

}

struct frame
{
    int start_bit=0;                    //start bit
	int server_addr;                    //address of server
	int function_code;                  //function code
	char data[9];                    //data
	char crc[16];                       //error check
	int stop_bit=1;                     //stop bit

};

int main() {

// Open the serial
    int serial_port = open("/dev/ttyUSB0", O_RDWR);

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    //Master Side


    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL;

     // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
       {
          printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
          return 1;
       }

while(1)
{

     if (signal(SIGTERM, sig_handler) == SIG_ERR)
     {
         printf("\n can't catch SIGTERM\n");

         break;
     }

    else if (signal(SIGKILL, sig_handler) == SIG_ERR)
    {
        printf("\n can't catch SIGKILL\n");

        break;
    }

    else if (signal(SIGSTOP, sig_handler) == SIG_ERR)
    {
        printf("\n can't catch SIGSTOP\n");

        break;
    }

    else
    {

    //Frame

        struct frame *ptr;
        ptr = (struct frame *)malloc(sizeof(struct frame));

        (ptr)->server_addr = 1;          //scanf("%d" , &(ptr)->server_addr);
        (ptr)->function_code=04;         //scanf("%d" , &(ptr)->function_code);
        (ptr)->data=00001010;            //scanf("%s" , (ptr)->data);
        (ptr)->crc = CRC16_2(*ptr);      //CRC

        write(serial_port, *ptr, sizeof(*ptr));  // Write to serial port

        //server side
        // Allocate memory for read buffer, set size according to your needs
        char read_buf[256];

        memset(&read_buf,'\0', sizeof(read_buf));

        // Read bytes.
        int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

        // num_bytes is the number of bytes read.
        //num_bytes may be 0 if no bytes were received, and can also be -1 to signal an error.

        if (num_bytes < 0)
        {
          printf("Error reading: %s", strerror(errno));
          return 1;
        }

        printf("number of bytes %d", num_bytes);


        int my_server_addr = 01;                      //address of server at server side
        char data_address;                            //data address that needs to be read at the server side
        char value[16];                                   //value that needs to be written on coil or register
        char input_register[10000][16];               //input register block address from 30000-40000
        char holding_register[10000][16];             //holding register block to read/write from 40000-50000
        char data_read[16];                           //data that is to be read by the master

        if ( (ptr)->server_addr != my_server_addr)
        {
            printf(" Error occured!!  slave address does not matches");
            break;
        }

        else
        {
            switch( function_code )
            {
                case 04:
                read_input_register();

                case 06:
                write_holding_register();

            }
        }

        close(serial_port);
    }


}

    return 0;
}

//CRC16 Function
unsigned int CRC16_2(unsigned char *ptr)
{
    unsigned int crcs = 0xFFFF;
    for (int pos = 0; pos < 9; pos++)
    {
        crcs ^= (unsigned int)buf[pos];                                  // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--)                                    // Loop over each bit
        {
            if ((crcs & 0x0001) != 0)                                    // If the LSB is set
            {
                crcs >>= 1;                                              // Shift right and XOR 0xA001
                crcs ^= 0xA001;
            }
            else                                                        // Else LSB is not set
            {
                crcs >>= 1;                                              // Just shift right
            }
        }
    }

    // low byte conversion
    crcs = ((crcs & 0x00ff) << 8) | ((crcs & 0xff00) >> 8);
    return crcs;
}‍‍‍‍‍‍‍‍‍‍‍‍‍‍‍‍‍

// 01             04              00 0A 00 0A     50 0F
// slave address   Function code  Data            CRC

void read_input_register()
{

	data_address = (ptr)->data;

	for(int i=0; i<16; i++ )
	{
		data_read[i] = input_register[data_address][i];
	}

	(ptr)->server_addr = my_server_addr;
	(ptr)->function_code = 04;
	(ptr)->data = data_read;
	(ptr)->crc = crc;
}

// 01             06             00 0A 00 32    28 1D
// slave address  function code  Data           CRC

void write_holding_register()
{

    data_address = ( (ptr)->data &  00 11 00 00);
    value = ( (ptr)->data & 00 00 00 11 );

    for(int i=0; i<16; i++ )
	{
		holding_register[data_address][i] = value[i];
	}

	(ptr)->server_addr = my_server_addr;
	(ptr)->function_code = 06;
	(ptr)->crc = crc;

}
