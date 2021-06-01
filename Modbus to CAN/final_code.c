#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <memory.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <time.h>
#include "modbus.h"

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

modbus_t *ctx;
uint16_t tab_reg[64];// will store registers values
int rc,i;
int sockfd, newsockfd; /* can raw socket */
int func_code = 03;


int main() {

    //parameters are received from json files
    ctx = modbus_new_rtu("/dev/ttyHSL0", 9600, 'N', 8, 1);
	if (!ctx)
        {
            fprintf(stderr, "Failed to create the context: %s\n", modbus_strerror(errno));
            exit(1);
        }

    modbus_rtu_set_rts(ctx, MODBUS_RTU_RTS_UP);

	if (modbus_connect(ctx) == -1)
        {
            fprintf(stderr, "Unable to connect: %s\n", modbus_strerror(errno));
            modbus_free(ctx);
            exit(1);
        }

	else
        {
            printf("\nConnection done to Slave\n");
        }

	modbus_set_slave(ctx, 1);  //Set the Modbus address of the remote slave

if(func_code == 03)
    {
        rc = modbus_read_registers(ctx, 1,02,tab_reg);

        if (rc == -1)   //it can store only +ve integer values
            {
                fprintf(stderr, "%s\n", modbus_strerror(errno));
                exit(1);
            }

        for (i=0; i < rc; i++)
            {
                printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
            }

    }

    char command[100];
    strcpy(command, "cansend can0 123#00000000000000tab_reg[1]tab_reg[1]" );
    system(command);

    strcpy(command, "ip -details -statistics link show can0" );
    system(command);

    modbus_close(ctx);
    modbus_free(ctx);


return 0;
}

