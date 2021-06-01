//This is client side.....Here info has to be taken from the user

void error(const char *msg)    //error function
{
    perror(msg);        //inbuilt error function
    exit(1);            //terminates the program
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

struct frame* arguments()
{
	int count, choice;
	struct frame *ptr;
    ptr = (struct frame *)malloc(sizeof(struct frame));

	while(1)
	{
        (ptr)->server_addr = 1;          //scanf("%d" , &(ptr)->server_addr);
        (ptr)->function_code=04;         //scanf("%d" , &(ptr)->function_code);
        (ptr)->data=00001010;            //scanf("%s" , (ptr)->data);
        (ptr)->crc = CRC16_2(*ptr);      //CRC

    }

    return ptr;

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
