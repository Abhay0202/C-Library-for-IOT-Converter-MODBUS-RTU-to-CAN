//c code which read the input register with 0x10 address with serial

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

int main() {

    // Open the serial port
    int serial_port = open("/dev/ttyHS0", O_RDWR);    //rs485  uart  port

    // Check for errors
    if (serial_port < 0)
        {
            printf("Error %i from open: %s\n", errno, strerror(errno));
        }

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }
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

        unsigned char frame[6];

        frame[0]=0;              // start bit
        frame[1]=01;             // slave address   scanf("%c", &frame[1]);
        frame[2]=04;             // function code   scanf("%c", &frame[2]);
        frame[3]=00001010;       // data field      scanf("%c", &frame[3]);

        unsigned char check[4];

        int count;
        for(count=0; count<4; count++)
           {
             check[count] = frame[count];
           }

        frame[4]= CRC16_2(check[]);      //CRC

        frame[5]=1;                     //Stop bit

        write(serial_port, frame, sizeof(frame));  // Write to serial port

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

        // Display received data

        for(count=0; count<256; count++)
        {
            printf(" %d",read_buf[count]);
        }

        close(serial_port);
    }

}

return 0;

}

//CRC16 Function
unsigned int CRC16_2(unsigned char *ptr)
{
    unsigned int crc = 0xFFFF;
    for (int pos = 0; pos < 9; pos++)
    {
        crc ^= (unsigned int)buf[pos];                                  // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--)                                    // Loop over each bit
        {
            if ((crc & 0x0001) != 0)                                    // If the LSB is set
            {
                crc >>= 1;                                              // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else                                                        // Else LSB is not set
            {
                crc >>= 1;                                              // Just shift right
            }
        }
    }

    // low byte conversion
    crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
    return crc;
}‍‍‍‍‍‍‍‍‍‍‍‍‍‍‍‍‍
