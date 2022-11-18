//We learnt this saving method from Saurabh Parulekar and Amogh Gajare and have modified it as per the main code and its other related functions.

#include "pico/stdlib.h"                                                               //Standard RP2040 library
#include <stdio.h>                                                                     //Standard C library
#include "stdlib.h"                                                                    //Standard C library
#include "string.h"                                                                    //Standard C library
void encode(bool *record){
    /*Encodes a boolean array, which can the BOOT button presses
    into a string,for example if boolean array has recorded 11110001111,1:BOOT pressed,0:BOOT Released
    the first character tells you how to array starts, 1:t,0:f
    followed by the number of 1's and 0's alternatively
    Therefore the example string will be encoded as follows
    t-4-3-4-
    eg2:0000011110001
    f-5-4-3-1
    */
    int count_1 = 0;                                                                   //Counter
    int count_0 = 0;                                                                   //Counter
    if(record[0] == true)                                                              //intial character for encoding 1:t,0:f
        printf("t-");
    else
        printf("f-");
    for(int g=0;g<5000;g++){
        if(record[g] == 1){
            count_1++;
            if(record[g+1]==0){
                printf("%d-",count_1);
                count_1 = 0;
                }
        }
        else{
            count_0++;
            if(record[g+1]==1){
                printf("%d-",count_0);
                count_0 = 0;
                }
        }
    }
}

void decode(bool *sequence,char *upload_sequence){
    /*
    The  function converts the above encoded string, back into a boolean array
    */
    int flag=0;
                char* token;
                char* rest = upload_sequence;
                int rec_num = 0,num;
                int upp_bound=0;
                token = strtok_r(rest, "-", &rest);
                if(*token == 'f'){
                    while((token = strtok_r(rest, "-", &rest))) {
                        num = (int)atoi(token);
                        //printf("%d",num);
                        upp_bound = rec_num+num;
                        if(flag == 0){
                        for(rec_num=rec_num ;rec_num<upp_bound;rec_num++)
                            sequence[rec_num] = false;

                        flag = 1;}
                        else{
                            for(rec_num=rec_num;rec_num<upp_bound;rec_num++)
                            sequence[rec_num] = true;
                            flag = 0;
                        }    
                    }
                }
                else{
                    while((token = strtok_r(rest, "-", &rest))) {
                        num = (int)atoi(token);
                        upp_bound = rec_num+num;
                        if(flag == 0){
                        for(rec_num=rec_num ;rec_num<upp_bound;rec_num++)
                            sequence[rec_num] = true;
                        flag = 1;}
                        else{
                            for(rec_num=rec_num ;rec_num<upp_bound;rec_num++)
                            sequence[rec_num] = false;
                            flag = 0;
                        }    
                    }
                }
}