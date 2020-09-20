/*---------------------------------------------------------------------------------

	Copyright (C) 2020
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

	binary to text converter for colecovision.
	
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BIN2TXTVERSION __BUILD_VERSION
#define BIN2TXTDATE __BUILD_DATE


//// M A I N   V A R I A B L E S ////////////////////////////////////////////////
int	convformat=1;			//convertion formation (1=c , 2 = asm)
int quietmode=0;			// 0 = not quiet, 1 = i can't say anything :P
FILE *fpi,*fpo;				// input and output file handler
unsigned int filesize;		// input file size
char filebase[256]="";		// input filename
char filename[256];			// output filename

//// F U N C T I O N S //////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void PrintOptions(char *str)
{
	printf("\n\nUsage : bin2txt [options] filename ...");
	printf("\n  where filename is a binary file");
	
	if(str[0]!=0)
		printf("\nThe [%s] parameter is not recognized.",str);
	
	printf("\n\nOptions are:");
	printf("\n\n--- Convert options ---");
	printf("\n-cc               Output in c style format");
	printf("\n-ca               Output in assembly style format");
	printf("\n\n--- Misc options ---");
	printf("\n-q                quiet mode");
	printf("\n");
	
} //end of PrintOptions()

/// M A I N ////////////////////////////////////////////////////////////

#include <math.h>

int main(int argc, char **arg)
{
	int i,j,k;
	unsigned char bytei;
    unsigned char tabascii[16];
    

	// Show something to begin :)
	if (quietmode == 0) {
		printf("\n=============================");
		printf("\n---bin2txt v"BIN2TXTVERSION" "BIN2TXTDATE"---");
		printf("\n------------------------------");
		printf("\n(c) 2020 Alekmaul ");
		printf("\n=============================\n");
	}
	
	//parse the arguments
	for(i=1;i<argc;i++)
	{
		if(arg[i][0]=='-')
		{
			if(arg[i][1]=='c') // convert options
			{
				if( strcmp(&arg[i][1],"cc") == 0)	//packed pixels
				{
					convformat=1;
				}
				else if( strcmp(&arg[i][1],"ca") == 0)	// lzss compressed pixels
				{
					convformat=2;
				}
				else
				{
					PrintOptions(arg[i]);
					return 1;					
				}
			}
			else if(arg[i][1]=='q') //quiet mode
			{
				quietmode=1;
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
			if(filebase[0]!=0) // if already defined... there's a problem
			{
				PrintOptions(arg[i]);
				return 1;
			}
			else
				strcpy(filebase,arg[i]);
		}
	}

	//make sure options are valid
	if( filebase[0] == 0 )
	{
		printf("\nERROR: You must specify a base filename.");
		PrintOptions("");
		return 1;
	}

	// open the file
	fpi = fopen(filebase,"rb");
	if(fpi==NULL)
	{
		printf("\nERROR: Can't open file [%s]",filebase);
		return 1;
	}
	if (filebase[strlen(filebase)-4] == '.') {
		filebase[strlen(filebase)-4] = '\0';
	}

	//Print what the user has selected
	if (quietmode == 0) {
		printf("\n****** O P T I O N S ***************");
		if(convformat == 1)
			printf("\nC Style format=ON");
		else
			printf("\nASM style format=ON");

		printf("\n************************************");
	}

	// get filesize
	fseek(fpi, 0, SEEK_END);
	filesize = ftell(fpi);
	fseek(fpi, 0, SEEK_SET);

	// open output file for writing
	if (convformat == 1)
		sprintf(filename,"%s.h",filebase);
	else if (convformat == 2)
		sprintf(filename,"%s.asm",filebase);

	if (quietmode == 0)
		printf("\nSaving text file: [%s]",filename);
	fpo = fopen(filename,"wb");
	if(fpo==NULL)
	{	
		printf("\nERROR: Can't open file [%s] for writing\n",filename);
		fclose(fpi);
		return 1;
	}

	// write header & content & footer
	if (convformat == 1) {
		// write asm header
		fprintf(fpo, "//----------------------------------------------------------------------\n");
		fprintf(fpo, "// bin2txt converted binary data\n");
		fprintf(fpo, "// binary file size : %d bytes\n",filesize);
		fprintf(fpo, "//----------------------------------------------------------------------\n\n");
		strupr(filebase);
		fprintf(fpo, "#ifndef %s_INC_\n", filebase);
		fprintf(fpo, "#define %s_INC_\n\n", filebase);
		fprintf(fpo, "#define %s_SIZE 0x%x\n\n", filebase,filesize);
		fprintf(fpo, "const unsigned char %s[%d]={\n", filebase,filesize);
		fprintf(fpo, "\t");
		// write const
        j = 0;
		for(i = 0; i < filesize; i++) {
			if(i) {
				if((i & 15) == 0) {
                    fprintf(fpo, ",\t // ");
                    fprintf(fpo, "%08x ",i-16);
                    for (k=0;k<16;k++) {
                        fputc( ( (tabascii[k]>=32) && (tabascii[k]<128)) ? tabascii[k] : '.',fpo);
                    }
                    j=0;
                    fprintf(fpo, "\n\t");
                }
				else
					fprintf(fpo, ",");
			}
			fread(&bytei, 1, 1, fpi);
            tabascii[j++]=bytei;
			fprintf(fpo, "0x%02X", bytei);
		}
        for (k=0;k<16-j;k++) {
            fprintf(fpo, "     ");
        }
        fprintf(fpo, " \t // ");
        fprintf(fpo, "%08x ",i-16);
        for (k=0;k<j;k++) {
            fputc( ( (tabascii[k]>=32) && (tabascii[k]<128)) ? tabascii[k] : '.',fpo);
        }

		// write footer
		fprintf(fpo, "\n};\n\n");
		fprintf(fpo, "#endif\n");
	}
	else if (convformat == 2) {
		// write asm header
		fprintf(fpo, ";----------------------------------------------------------------------\n");
		fprintf(fpo, "; bin2txt converted binary data\n");
		fprintf(fpo, "; binary file size : %d bytes\n",filesize);
		fprintf(fpo, ";----------------------------------------------------------------------\n\n");
		fprintf(fpo, ".section \".%s\" superfree\n\n", filebase);
		strupr(filebase);
		fprintf(fpo, ".define %s_SIZE %xh\n\n", filebase,filesize);
		fprintf(fpo, "%s:\n", filebase);
		
		// write const
		for(i = 0; i < filesize; i++) {
			if(i) {
				if((i & 31) == 0)
					fprintf(fpo, "\n\t.byte ");
				else
					fprintf(fpo, ",");
			}
			else
				fprintf(fpo, "\t.byte ");
			fread(&bytei, 1, 1, fpi);
			fprintf(fpo, "$%02X", bytei);
		}
	
		// write footer
		fprintf(fpo, "\n\n%s_end:\n\n", filebase);
		fprintf(fpo, ".ends\n");
	}
		
	// close the input & output file
	fclose(fpi);
	fclose(fpo);

	if (quietmode == 0)
		printf("\nDone!\n\n");

	return 0;
}
