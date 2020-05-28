#include "is.h"

void IS::readSec(ifstream *file)
{
    cout<<"Section 0: "<<endl;
    //string message;
    char message[8];
    for(int i=0;i<=7;i++){
        message[i]=file->get();
    }

   if(((char)message[0]=='G')&&((char)message[1]=='R')&&((char)message[2]=='I')&&((char)message[3]=='B'))
   {

       totalLength=secLength(message[4],message[5],message[6]);
       cout<<"Length of message: "<<totalLength<<endl;

       if(message[7]==1){
         editNumber=1;
       }
       else {
          editNumber=2;
       }
       cout<<"GRIB type: "<<editNumber<<endl;
   }
   else
   {
       cout << "Error, not read GRIB in sec 0" << endl;
   }
}
