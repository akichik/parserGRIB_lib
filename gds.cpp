#include "gds.h"

void GDS::readSec(ifstream *file, Data* d)
{
    char message[6];
    //string message;

    for(int i=0;i<=5;i++){
        //cout<<file.tellg()<<endl;
        message[i]=file->get();
    }
    cout<<"Section 2: "<<endl;
    d->sectionLength=secLength (message[0],message[1],message[2]);
    cout<<"Length: "<<d->sectionLength<<endl;

    /*for(int i=3;i<sec2.sectionLength;i++){
            message[i]=file.get();
        }*/

    d->NV=message[3];
    d->PV_LV=message[4];
    d->DataType=dataType(message[5]);

    if ((d->DataType=="Latitude/Longitude Grib")||(d->DataType=="Gaussian Latitude/Longitude Grib"))
        typeLanLon(file, d);

}

string GDS::dataType(char dataType)
{
    string type;
    switch(dataType){
    case 0:
        type="Latitude/Longitude Grib";
        break;
    case 4:
        type="Gaussian Latitude/Longitude Grib";
        break;
    default:
        cout<<"I don't know this type";
    }
    return type;
}

void GDS::typeLanLon(ifstream *file, Data* d)
{
    char message[25];
    //string message;

    for(int i=0;i<=25;i++){
        //cout<<file.tellg()<<endl;
        message[i]=file->get();
    }

    d->Ni=int2(message[0],message[1]);
    cout<<"Ni: "<<d->Ni<<endl;
    d->Nj=int2(message[2],message[3]);
    cout<<"Nj: "<<d->Nj<<endl;
    d->pointQuantity=d->Ni * d->Nj;

    d->southLa1=(message[4]&128)!=0;
    if(d->southLa1)
        message[4]=message[4]&127;
    d->La1=secLength(message[4],message[5],message[6]);//+1


    d->westLo1=(message[7]&128)!=0;
    if(d->westLo1)
        message[7]=message[7]&127;
    d->Lo1=secLength(message[7],message[8],message[9]);//+1

    d->flags=message[10];

    d->southLa2=(message[11]&128)!=0;
    if(d->southLa2)
        message[11]=message[11]&127;
    d->La2=secLength(message[11],message[12],message[13]);//+1

    d->westLo2=(message[14]&128)!=0;
    if(d->westLo2)
        message[14]=message[14]&127;
    d->Lo2=secLength(message[14],message[15],message[16]);//+1


    d->Di=int2(message[17],message[18]);
    d->Dj_N=int2(message[19],message[20]);
    d->scanFlags=message[21];

}
