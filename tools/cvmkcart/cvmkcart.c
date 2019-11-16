/*---------------------------------------------------------------------------------

	Copyright (C) 2018-2019
		Alekmaul 

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source
		distribution.

  Hex checker / BIN creator for colecovision.
  
  Based on :
    hex2bin converts an Intel hex file to binary.
	Copyright (C) 2012,  Jacques Pelletier
	checksum extensions Copyright (C) 2004 Rockwell Automation
	All rights reserved.
  
***************************************************************************/

//INCLUDES
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>

//DEFINES
#define CVMKCARTVERSION __BUILD_VERSION
#define CVMKCARTDATE __BUILD_DATE

#define MAX_LINE_SIZE 1024				// The data records can contain 255 bytes: this means 512 characters.
#define MEMORY_SIZE 1024*1024			// size in bytes (max = 1Mb)

#define NO_ADDRESS_TYPE_SELECTED 0
#define LINEAR_ADDRESS 1
#define SEGMENTED_ADDRESS 2

int quietmode=0;										// 0 = not quiet, 1 = i can't say anything :P
unsigned int maxlength = 64*1024;		// max rom length size
unsigned int nobanks = 2;						// number of banks
unsigned int padbyte = 0xFF;				// padding byte
unsigned char *memblocks;						// This will hold binary codes translated from hex file 
unsigned int norecord;							// record number for tracing

unsigned char datastring[MAX_LINE_SIZE];  
char dataline[MAX_LINE_SIZE]; 				// line inputted from file 
  
unsigned int nobytes;
unsigned int firstword, address, segment, upperaddr;
unsigned int lowaddr, highaddr, startaddr, lowbank;

  /* This mask is for mapping the target binary inside the
	 binary buffer. If for example, we are generating a binary
	 file with records starting at FFF00000, the bytes will be
	 stored at the beginning of the memory buffer. */
unsigned int maskaddr;

unsigned int physaddr, recordtype;
unsigned int temp;
int temp2;
unsigned int norecord;

// We will assume that when one type of addressing is selected, it will be valid for all the
//  current file. Records for the other type will be ignored. 
unsigned int	seglinselect = NO_ADDRESS_TYPE_SELECTED;

char *p;
  
//-- Read a line from file --------------------------------------------------------
int Getdataline(char* str,FILE *in)
{
	char *result;

	result = fgets(str,MAX_LINE_SIZE,in);
	if ((result == NULL) && !feof (in)) {
		printf("\nERROR: Can't read from file\n");
		return 0;
	}
	
	return 1;
}

//-- Print option of program ------------------------------------------------------
void PrintOptions(char *str) {
	printf("\n\nUsage: cvmkcart [options] romfilename ...");
	printf("\n  where romfilename is the COLECOVISION rom file to generate.");
	
	if(str[0]!=0)
		printf("\nThe [%s] parameter is not recognized.",str);
	
	printf("\n\nOptions are:");
	printf("\n\n--- Rom options ---");
	printf("\n-b[bank]          Number of 16k banks of finale rom (default is 2 for 64k rom, max is 64 for 1Mb rom).");
	printf("\n-p[value]         Padding byte value (default is 255).");
	printf("\n-iihxfilename     Name of HEX ihx filename.");
	printf("\n");
	printf("\n\n--- Misc options ---");
	printf("\n-q                quiet mode");
	printf("\n");
	
} //end of PrintOptions()

/// M A I N ////////////////////////////////////////////////////////////

int main(int argc, char **arg)
{
	FILE *fpi, *fpo;

	char romfile[256];
	char ihxfile[256];

	int i;

	// Show something to begin :)
	if (quietmode == 0) {
		printf("\n=================================");
		printf("\n--- CVMKCART v"CVMKCARTVERSION" "CVMKCARTDATE" ---");
		printf("\n---------------------------------");
		printf("\n(c) 2014-2019 Alekmaul ");
		printf("\n=================================\n");
	}
	
	// Init vars
	romfile[0] = '\0';
	ihxfile[0] = '\0';
	
	//parse the arguments
	for(i=1;i<argc;i++)
	{
		if(arg[i][0]=='-')
		{
			if(arg[i][1]=='q') //quiet mode
			{
				quietmode=1;
			}
			else if(arg[i][1]=='b') // number of banks
			{
				nobanks=atoi(&arg[i][2]);
				if ( (nobanks<2) || (nobanks>64))
				{
					PrintOptions(arg[i]);
					return 1;
				}
				maxlength=(nobanks+3)*16*1024; // 3 because we begin writing at 0xc000
			}
			else if(arg[i][1]=='p') // padding byte
			{
				padbyte=atoi(&arg[i][2]);
				if (padbyte>255)
				{
					PrintOptions(arg[i]);
					return 1;
				}
			}
			else if(arg[i][1]=='i') // ihx filename
			{
				strcpy(ihxfile,&arg[i][2]);
			}
			else //invalid option
			{
				PrintOptions(arg[i]);
				return 1;
			}
		}
		else
		{
			//its not an option flag, so it must be the filebase
			if(romfile[0]!=0) // if already defined... there's a problem
			{
				PrintOptions(arg[i]);
				return 1;
			}
			else {
				strcpy(romfile,arg[i]);
			}
		}
	}

	//make sure options are valid
	if( romfile[0] == 0 )
	{
		printf("\nERROR: You must specify a rom filename.");
		PrintOptions("");
		return 1;
	}
	if (strlen(ihxfile) == 0)
	{
		printf("\nERROR: You must specify an ihx file.");
		PrintOptions("");
		return 1;
	}
	
	// open the file
	fpi = fopen(ihxfile,"r");
	if (fpi==NULL)
	{
		printf("\nERROR: Can't open ihx file [%s]",ihxfile);
		return 0;
	}
	
	// open the file
	fpo = fopen(romfile,"wb");
	if (fpo==NULL)
	{
		printf("\nERROR: Can't create rom file [%s]",romfile);
		return 0;
	}
	
	// allocate a buffer 
	if ((memblocks = malloc (maxlength+0x8000)) == NULL)
    {
		printf("\nERROR: Can't allocate memory for output.\n");
		exit(1);
	}

	if (quietmode == 0) {
		printf("\n****** O P T I O N S ***************");
		printf("\nReading file     %s ...",ihxfile);
		printf("\nWriting rom file %s...",romfile);
		printf("\nRom size         %dk",(maxlength-0x4000)/1024);
		printf("\nNumber of banks  %d",nobanks);
		printf("\nPadding byte     %02x\n",padbyte);
	}
	
	// begining of reading file  
	startaddr = 0;
	maxlength+=0x8000; // rom 0..8000 if coleco bios and ram
  
	// fill unused bytes with FF or the value specified by the p option (or 255 for default value)
	memset (memblocks,padbyte,maxlength);
	
	// To begin, assume the lowest address is at the end of the memory.
	// While reading each records, subsequent addresses will lower this number.
	// At the end of the input file, this value will be the lowest address.
	// A similar assumption is made for highest address. It starts at the
	// beginning of memory. While reading each records, subsequent addresses will raise this number.
	// At the end of the input file, this value will be the highest address. 
	lowaddr = maxlength - 1 - 0x8000;
	highaddr = 0;
	segment = 0;
	upperaddr = 0;
	norecord = 0;
	lowbank=0;
	
    /* Max length must be in powers of 2: 1,2,4,8,16,32, etc. */
	maskaddr = maxlength;// -1;
	if (quietmode == 0) {
		printf("\nMaximum address set to %06x\n",maskaddr);
	}
	
	// Read the file & process the lines. 
	do // repeat until EOF(fpi) 
	{

		//Read a line from input file.
		Getdataline(dataline,fpi);
		norecord++;

		//Remove carriage return/line feed at the end of line.
		i = strlen(dataline);

		if (--i != 0) {
			if (dataline[i] == '\n') dataline[i] = '\0';

			// Scan the first two bytes and nb of bytes.
			// The two bytes are read in firstword since its use depend on the
			// record type: if it's an extended address record or a data record.
			sscanf (dataline, ":%2x%4x%2x%s",&nobytes,&firstword,&recordtype,datastring);
			p = (char *) datastring;

			// If we're reading the last record, ignore it. 
			switch (recordtype) {
				// Data record
				case 0:
					if (nobytes == 0) {
						printf("\nWARNING: 0 byte length Data record ignored");
						break;
					}
					address = firstword;
					
					// check bank number to update if new bank
					if (address==0xc000) {
						lowbank++;
						if (quietmode == 0) {
							printf("\nBank %d detected ...",lowbank);
						}
					}

					// add bank number if in bank
					if (address>=0xc000)
						address+=(lowbank-1)*0x4000;

					if (seglinselect == SEGMENTED_ADDRESS)
						physaddr = (segment << 4) + address;
					else
						// LINEAR_ADDRESS or NO_ADDRESS_TYPE_SELECTED
						//upperaddr = 0 as specified in the Intel spec. until an extended address record is read.
						physaddr = ((upperaddr << 16) + address) % maskaddr; // change & to %

					// Check that the physical address stays in the buffer's range. 
					if ((physaddr + nobytes) <= maxlength) {
						// Set the lowest address as base pointer. 
						if (physaddr < lowaddr)
							lowaddr = physaddr;

						// Same for the top address.
						temp = physaddr + nobytes -1;
						if (temp > highaddr)
							highaddr = temp;

						// Read the Data bytes. 
						// Bytes are written in the Memory block even if checksum is wrong.
						i = nobytes;

						do
						{
							sscanf (p, "%2x",&temp2);
							p += 2;

							// Overlapping record will erase the pad bytes 
							if (memblocks[physaddr] != padbyte) 
								printf("\nWARNING: Overlapped record (%d) detected %06x=%02x",norecord-1, physaddr,memblocks[physaddr]);
							memblocks[physaddr++] = temp2;
						} while (--i != 0);

						// Read the Checksum value. 
						sscanf (p, "%2x",&temp2);
					}
					else
					{
						if (seglinselect == SEGMENTED_ADDRESS)
							printf("\nWARNING: Data record (%d) skipped at %6X:%6X",norecord-1, segment,address);
						else
							printf("\nWARNING: Data record (%d) skipped at %6X",norecord-1, physaddr);
					}
					break;

				// End of file record
				case 1:
					// Simply ignore checksum errors in this line. 
					break;

				// Extended segment address record 
				case 2:
					// firstword contains the offset. It's supposed to be 0000 so we ignore it. 
					// First extended segment address record ? 
					if (seglinselect == NO_ADDRESS_TYPE_SELECTED)
						seglinselect = SEGMENTED_ADDRESS;

					// Then ignore subsequent extended linear address records 
					if (seglinselect == SEGMENTED_ADDRESS)
					{
						sscanf (p, "%4x%2x",&segment,&temp2);

						// Update the current address.
						physaddr = (segment << 4) % maskaddr;
					}
					break;

				// Start segment address record 
				case 3:
					// Nothing to be done since it's for specifying the starting address for execution of the binary code 
					break;

				// Extended linear address record 
				case 4:
					// firstword contains the offset. It's supposed to be 0000 so we ignore it. 
					// First extended linear address record ? 
					if (seglinselect == NO_ADDRESS_TYPE_SELECTED)
						seglinselect = LINEAR_ADDRESS;

					// Then ignore subsequent extended segment address records 
					if (seglinselect == LINEAR_ADDRESS)
					{
						sscanf (p, "%4x%2x",&upperaddr,&temp2);

						// Update the current address. 
						physaddr = (upperaddr << 16) % maskaddr;
					}
					break;

				// Start linear address record 
				case 5:
					// Nothing to be done since it's for specifying the starting address for execution of the binary code 
					break;
				default:
					printf("\nWARNING: Unknown record type");
					break;
            }
        }
	} while (!feof (fpi));
    
	// maxlength is set; the memory buffer is already filled with pattern before
	// reading the hex file. The padding bytes will then be added to the binary file.
	highaddr = startaddr + maxlength-1;
	startaddr=0xc000;
	lowaddr = startaddr;

	// change 0x8000 to last bank
	// and manage start adress to 0x8000
	memcpy(&memblocks[highaddr-0x4000+1],&memblocks[0x8000],0x4000);
	
	if (quietmode == 0) {
		printf("\n\nLowest address  = %08X",lowaddr);
		printf("\nHighest address = %08X",highaddr);
		printf("\nPad Byte        = %X",  padbyte);
		printf("\nRom size wrote  = %08X (%dk)",highaddr - lowaddr +1,(highaddr - lowaddr +1)/1024);
		printf("\nBanks detected  = %X\n", lowbank);
	}
	
	// write binary file 
	fwrite (&memblocks[lowaddr], 1, highaddr - lowaddr +1, fpo);
	free (memblocks);
    
	// close the files
	fclose(fpi);
	fclose(fpo);
	
	if (quietmode == 0)
		printf("\nDone!\n");

	// display map file
	if (quietmode == 0) {
		// open the map file
		strcpy(romfile,ihxfile);
		i = strlen(romfile);
		romfile[i-3]='m';romfile[i-2]='a';romfile[i-1]='p';
		fpi = fopen(romfile,"r");
		if (fpi==NULL)
		{
			printf("\nERROR: Can't open map file [%s]",romfile);
			return 0;
		}
		printf("\nArea                                    Addr        Size        Decimal Bytes (Attributes)");
		printf("\n--------------------------------        ----        ----        ------- ----- ------------\n");
		do {
			//Read a line from input file.
			Getdataline(dataline,fpi);
			
			// if line begin with _, print it (I know it is bad ...)
			if (dataline[0]=='_') {
				printf(dataline);
			}
		} while (!feof (fpi));
		fclose(fpi);
		printf("\n");
	}
	
	return 0;
}
