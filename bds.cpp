#include "bds.h"

void BDS::readSec(ifstream *file, Data *d, int D, int type, int scanFlags,int Ni,int Nj)
{
    char message[4];
    d->pointData.clear();
    for(int i=0;i<=3;i++){
        message[i]=file->get();
    }
    cout<<"Section 4: "<<endl;
    d->sectionLength=secLength (message[0],message[1],message[2]);
    cout<<"Length: "<<d->sectionLength<<endl;

    d->mapPoint=((message[3]&128)==0);
    d->simplePack=((message[3]&64)==0);
    d->fPoint=((message[3]&32)==0);
    d->addflag=((message[3]&16)==0);
    if (d->simplePack){
        cout<<"Simple Pack"<<endl;
        d->pointData=simple(file,d, D);
    }
    for (int i = 0; i < d -> pointData.size(); i++) {
        if(type==GRB_TEMP)
            d->pointData[i]=d->pointData[i] - 273.15;//Перевод из К в С
        else if(type==GRB_TEMP_POT)
            d->pointData[i]=d->pointData[i] - 273.15;//Перевод из К в С
        else if(type==GRB_PRESSURE)
            d->pointData[i]=d->pointData[i]/133.3224;//перевод в мм.рт.ст из Па
        else if(type==GRB_PRESSURE_MSL)
            d->pointData[i]=d->pointData[i]/133.3224;//перевод в мм.рт.ст из Па
    }
    if ((scanFlags&128)!=0)
            directionMinusI(&d->pointData, Ni, Nj);
    if((scanFlags&64)!=0)
        directionPlusJ(&d->pointData, Nj, Ni);

}

void BDS::ref(float* refValue,char a, char b, char c, char d)
{
    bool sign=((a&128)==0);
    char A=a&127;
    long B=((unsigned char)b<<16)+((unsigned char)c<<8)+(unsigned char)d;

    float Aa=B/(pow(2,24));
    float Bb=pow(16,A-64);
    if(sign){
        *refValue=Aa*Bb;
    }
    else{
        *refValue=-1*Aa*Bb;}
}

QList<float> BDS::simple(ifstream *file,Data *d, int D)

{
    char message[7];

    vector<int> dataBit;
    QList<float> dataF;
    string data;
    float pointD;
    for(int i=0;i<7;i++){
        message[i]=file->get();}

    d->E=int2(message[0],message[1]);
    ref(&d->refValue,message[2],message[3],message[4],message[5]);
    cout<<"refValue: "<<d->refValue<<endl;
    d->bit=message[6];
    cout<<"bit in data: "<<d->bit<<endl;
    for(int i=11;i<d->sectionLength;i++){
        data.push_back(file->get());}
    bitData(d->bit, data, &dataBit);

    for(int i=0;i<(int)dataBit.size();i++)
    {
        pointD=(d->refValue+dataBit[i]*pow(2,d->E))/pow(10,D);
        dataF.push_back(pointD);
        //cout<<type<<": "<<d->pointData[i]<<endl;
    }
    return dataF;

}

void BDS::bitData(size_t bit, string data, vector<int> *dataF)
{

    int cnt=data.size ()*8/bit;
    vector<bool> bits;
    int16_t rez;
    for (int i=0;i< (int) data.size ();i++) {
        char c = data[i];
        for (int j = 0; j <8; j++) {
            bits.push_back (c & 0x80);
            c <<= 1;

        }

    }

    for (int i=0;i<cnt;i++) {
        rez=0;
        for (int j=0;j<(int)bit;j++)
        {
            if(bits[j+bit*i])
            {
                switch(j)
                {
                case 0:
                    rez=rez|1;
                    break;
                case 1:
                    rez=rez|2;
                    break;
                case 2:
                    rez=rez|4;
                    break;
                case 3:
                    rez=rez|8;
                    break;
                case 4:
                    rez=rez|16;
                    break;
                case 5:
                    rez=rez|32;
                    break;
                case 6:
                    rez=rez|64;
                    break;
                case 7:
                    rez=rez|128;
                    break;
                }

            }

            //cout<<"data: "<<data<<endl;
        }
        dataF->push_back(rez);

    }

}

void BDS::directionMinusI(QList<float> *data, int Ni,int Nj)
{
    for (int i = 0; i < Ni/2; i++) {
        for (int j = 0; j < Nj; j++) {
            data->swapItemsAt(Ni*j+i,Ni*(j+1)-i-1);
        }
    }
}

void BDS::directionPlusJ(QList<float> *data, int Nj,int Ni)
{
    for (int i = 0; i < Ni; i++) {
        for (int j = 0; j < Nj/2; j++) {
            data->swapItemsAt(i+Ni*j,data->size()-Ni*(j+1)+i);
        }
    }
}
