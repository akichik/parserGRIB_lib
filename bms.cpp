#include "bms.h"

void BMS::readSec(ifstream *file, BMS::Data *d, int pointQuantity)
{

    char message[80];

    int i;
    for(i=0;i<=2;i++){
        //cout<<file.tellg()<<endl;
        message[i]=file->get();
    }
    cout<<"Section 3: "<<endl;
    d->sectionLength=secLength (message[0],message[1],message[2]);
    cout<<"Length: "<<d->sectionLength<<endl;

    for(int i=3;i<d->sectionLength;i++){
        message[i]=file->get();
    }

    d->unusedBit=message[3];
    if(int2(message[4],message[5])==0){
        int cnt=pointQuantity;

       // for(i=0;i<cnt;i++)
         //  bitMap[i]=(message[i+8]==1)?true:false;
    }
    else {
        d->specificBitmap=true;
    }
}
