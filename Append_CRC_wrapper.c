

/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif

/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <math.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
void Append_CRC_Outputs_wrapper(const uint16_T *index,
			const uint16_T *value,
			int32_T *message)
{
   static unsigned char valueArray[5];
   
   //(unsigned char) *message;
   
   int IndexNumber = *index;
   int val = *value;
   sprintf(valueArray, "%i", val);  
   puts(valueArray);
   
   //Find the lenght of the value character array
   int lengthCounter = 0;

   //array terminates in \0 which allows us to find length of passed in value
   while (valueArray[lengthCounter] != '\0'){
     lengthCounter++;
   }
   
      //set size of message to zero initially
   int size = 0;
   
   //constant result of 65535
   uint16_T crc_reg = 0xFFFF;
   
   //polynomial
   unsigned int polynomial = 0xa001;
   
   //Start of Header
   *message = 0x01;
   
      //Other declarations
   int i = 0;
   int j = 0;
   int MSB = 0;
   unsigned char LSB;
   int messageIndex = 0;
   int messageSize = 0;
   
       switch (IndexNumber){
        case 1:
           //Lenght of message
           *(message + 1) = 0x6;
           
           //The actual message
           *(message + 2) = 'A';
           *(message + 3) = 'L';
           *(message + 4) = 'L';
           *(message + 5) = 'D';
           *(message + 6) = 'A';
           *(message + 7) = 'T';
           size = 6;
           break;
        case 2:
           *(message + 1) = 0x06;
           *(message + 2) = 'S';
           *(message + 3) = 'Y';
           *(message + 4) = 'S';
           *(message + 5) = 'S';
           *(message + 6) = 'S';
           *(message + 7) = 'L';
            size = 6;
            break;
        case 3:
           *(message + 1) = 0x06;
           *(message + 2) = 'S';
           *(message + 3) = 'Y';
           *(message + 4) = 'S';
           *(message + 5) = 'S';
           *(message + 6) = 'T';
           *(message + 7) = 'A';
            size = 6;
            break;
        case 4:

           *(message + 1) = 0x06;
           *(message + 2) = 'E';
           *(message + 3) = 'N';
           *(message + 4) = 'G';
           *(message + 5) = 'D';
           *(message + 6) = 'T';
           *(message + 7) = '1';
            size = 6;
        break;
        case 5:
           *(message + 1) = 0x06;
           *(message + 2) = 'F';
           *(message + 3) = 'L';
           *(message + 4) = 'T';
           *(message + 5) = 'H';
           *(message + 6) = 'I';
           *(message + 7) = 'S';
            size = 6;
            break;
        case 6:
           *(message + 1) = 0x08;
           *(message + 2) = 'S';
           *(message + 3) = 'T';
           *(message + 4) = 'R';
           *(message + 5) = 'C';
           *(message + 6) = 'M';
           *(message + 7) = 'D';
           *(message + 8) = '=';
            
            //insert the integer value (0 to 65000) will become part of the entire message.
            for (i = 9, j = 0; i < (9 + (lengthCounter)); i++, j++){
                *(message + i) = valueArray[j];
            }
            size = 7 + lengthCounter;
            *(message + 1) = size;
            break;
        case 7:
           *(message + 1) = 0x0c;
           *(message + 2) = 'P';
           *(message + 3) = 'W';
           *(message + 4) = 'R';
           *(message + 5) = 'D';
           *(message + 6) = 'M';
           *(message + 7) = 'D';
           *(message + 8) = '=';
            
            //insert the integer value (0 to 65000) will become part of the entire message.
            for (i = 9, j = 0; i < (9 + (lengthCounter)); i++, j++){
                *(message + i) = valueArray[j];
            }
           
           size = 7 + lengthCounter;
            *(message + 1) = size;
           break;
        case 8:
           *(message + 1) = 0x0E;
           *(message + 2) = 'P';
           *(message + 3) = 'S';
           *(message + 4) = 'S';
           *(message + 5) = 'W';
           *(message + 6) = 'R';
           *(message + 7) = 'D';
           *(message + 9) = '=';
           *(message + 10) = 'U';
           *(message + 11) = 'S';
           *(message + 12) = 'R';
           *(message + 13) = '1';
           *(message + 14) = '2';
           *(message + 15) = '3';
           *(message + 16) = 'P';
            size = 14;
        break;
    }
       
          messageSize = size;
   messageIndex = 2;
   
    while (messageSize--){
        crc_reg ^= (uint16_T)*(message + (messageIndex++));
        for (i = 0; i < 8; ++i){
            if (crc_reg & 1){
                crc_reg = (crc_reg >> 1) ^ 0xa001;
            }
            else{
                crc_reg >>= 1;
            }
        }
    }
   
   //retrieve MSB and LSB from crc to concatenat to end of message for checksum purposes
    LSB = crc_reg;
    MSB = crc_reg;
    MSB >>= 8;
    
    *(message + size + 2) = LSB;
    *(message + size + 3) = MSB;
 
    //End of transmission
    *(message + size + 4) = 0x04;
   
}
