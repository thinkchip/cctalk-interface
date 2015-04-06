/*
 * Utility.c
 *
 *  Created on: Aug 21, 2014
 *      Author: THINKCHIP
 */
#include "Utility.h"


/******************************************************************************
*
*  Move one string to another
*
******************************************************************************/
void MoveStringtoString(uint8_t *string1,uint8_t *string2,uint8_t punt1,uint8_t punt2,uint8_t size){
    
    uint8_t i;
      for(i=0;i<size;i++){      
       *(string1+i+punt1)=*(string2+i+punt2); 
      }
  
}

