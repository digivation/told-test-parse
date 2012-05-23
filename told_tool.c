/*
 * Aero Data to Told Sim Input Tool for MIPS Target
 *		M. A. Smith (matthew.smith.1@robins.af.mil
 *		v.5/18/2012
 * Usage: told_tool <input filename>
 * Info:  Reads in aero told data test cases and generates an ADA file
 *		  suitable for building into the CIP for processing by the TT00
 *		  modified TOLD test driver (from Trung Nguyen)
 */

#define NUM_INPUT_FIELDS  57
#define NUM_OUTPUT_FIELDS 31
#define DEBUG 1
 
#include <stdio.h>
#include <stdlib.h>

/* incoming data structure */
typedef struct data {
	float x;
	int fieldNum;
	struct data *next;
}; 

int main( int argc, char *argv[] ) {
	/* Initializations */
	char inputBuffer[250];			/* input buffer for lines, should be 201 characters maximum */
	char *ibPtr;	
	int n;							/* counter for parsing lines */
	char tempBuffer[30];			/* temp buffer */
	int *temp1,*temp2;				/* dynamic input array pointers, will hold file input */
	int readIndex = 0;				/* counter */
	int totalTestCases = 0;			/* total number of test cases */
	
	/* initialize data list */
	struct data *loadedData;
	struct data *tempData;
	loadedData = malloc( sizeof(struct data) );
	loadedData->next = 0;
	tempData = 0;

	/* ensure we are passed a filename */
	if( argc != 2 ) {
		printf( "Told Aero Data to MIPS Target Sim Data Tool\n" );
		printf( "usage: %s input\n", argv[0] );
	} else {
		/* open input file for analysis */
		FILE *told_input = fopen( argv[1], "r" );
		
		/* check for successful open */
		if( told_input == 0 ) {
			printf( "!! Error opening file %s\n", argv[1] );
		} else {
			printf( "== Opened data file %s for analysis\n", argv[1] );
			/* Read in AERO TOLD data from exported data file; format:
			   1) Header: six lines describing data format and containing the date
					TAKEOFF                                      04/11/12 12:50:33 PM
					CASE_NO      PALT      TOGW       OAT   WINDREP      GUST  WIND_DIR        RA    RWYDIR  SLOPE(%)       RCR       RSC       RFF       DVI     CG(%)   DRG_IDX      BTMS    COFMIN    DER_HT   CLRATE4
					CLALT4   CLRATE3    CLALT3   OB_HT_1   OB_HT_2   OB_HT_3   OB_HT_4   OB_HT_5   OB_HT_6   OB_HT_7   OB_HT_8   OB_HT_9   OB_HT10  OB_DIS_1  OB_DIS_2  OB_DIS_3  OB_DIS_4  OB_DIS_5  OB_DIS_6  OB_DIS_7
					OB_DIS_8  OB_DIS_9  OB_DIS10     ACCBL     WNGAI     ENGAI      BRKC    SPOILC      REVC  EOCS_REQ    THRUST      ELEV   OPER_WT  OP_WT_CG      FUEL   PAYLOAD   P/L_STA
					MSG_CODE  CATII_WT     HWIND     XWIND       EPR    FL_POS  FL_INDEX       COF       CFL    TO_RUN   ACC_ALT        VR      VMCG     VBMAX      VROT       VGO      VMCO      VMFR      VMSR      STAB
					ACCSPD_1  ACCSPD_2  ACCSPD_3  ACCSPD_4  ACCTIME1  ACCTIME2  ACCTIME3  ACCTIME4  DVR_CALC   LD_FACT  %MAX_THR
			   2) Following data is Test cases, 5 lines per test case:
					Lines 1-3: 57 fields containing input data
					Lines 4-6: 31 fields containing output data
			*/
			
			/* check for TAKEOFF header */
			temp1 = malloc( 7*sizeof(char) );
			temp2 = malloc( 8*sizeof(char) );
			if (temp1 == NULL || temp2 == NULL ) {
				free(temp1); free(temp2);
				printf( "!! Error allocating memory\n" );
				return;
			}
			
			fgets( inputBuffer, 210, told_input ); /* read the first line */
			sscanf( inputBuffer, "%7s %8s", temp1, temp2 );
			
			if( !strstr(temp1,"TAKEOFF") ) {
				printf("!! Input file %s does not appear to be a takeoff data file.\n!! Please check format.\n", argv[1]);
				return 1;
			}
			printf( "== Processing AERO TAKEOFF data generated %s\n", temp2 );

			
			/* read and discard the remainder of the header */
			for( readIndex=2; readIndex<7; readIndex++ ) {
				fgets( inputBuffer, 210, told_input );
			}
			
			/* we have now validated that TAKEOFF is present on line 1 of the header,
			   and discarded the remainder of the header. we are now at data */
			
			/* Processing:
			   1) Loop line by line through the file, until the end of the file
			     a) scan string for floats
				 b) extend inputDataFields array by number of input values
				 c) move floats from temp into inputDataFields
			   2) Search for number of final test case
			     a) should be final size of inputDataFields-NUM_INPUT_FIELDS+NUM_OUTPUT_FIELDS)+1
			   3) Present total testcases imported and ask user for range to output
			   3) Generate aero_told_data1_.ada and aero_told_data2_.ada containing requested 
			      testcase ranges.
			*/
			
			/* 1) Loop line by line through the file, until the end of the file */

			for( readIndex=0; ; readIndex++ ) {
				/* check for EOL */
				if( feof( told_input ) ){
					break;
				}
				/* read in line */
				fgets( inputBuffer, 210, told_input );
				
				if(DEBUG){
					printf("index: %i\n  reading: %s ***\n", readIndex, inputBuffer);
				}
				
				/* parse */
				n = 0;
				ibPtr = inputBuffer;
				while( sscanf( ibPtr, "%10s%n", tempBuffer, &n) ) {
					printf("found: %s\n", tempBuffer, n);
					ibPtr += n;
				}
				

			}
				

			

			printf( "== Input data parsed, read %i lines of data\n", readIndex);
			
			
			
			if(DEBUG) {	
				printf("Last line read: %s\n", inputBuffer ); 
				}
			
			
			/* CLEANUP TIME */
			free(temp1);
			free(temp2);
			fclose( told_input ); /* close the input file */

		}
	}
}
