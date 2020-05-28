#include "es.h"

void ES::readSec(ifstream *file)
{
    char message[4];
    for(int i=0;i<=3;i++){
        //cout<<file.tellg()<<endl;
        message[i]=file->get();
    }
    if(message[0]=='7' && message[1]=='7' && message[2]=='7' && message[3]=='7')
        cout<<"End message"<<endl<<endl;
}
