#include "parser.h"


QList<uniData> Parser::parse()
{
    IS  sec0;
    PDS sec1;
    GDS sec2;
    BMS sec3;
    BDS sec4;
    ES  sec5;

   QList<uniData> listData;

   file.open(name.toStdString(),ios::binary);
    if (file.is_open()){
      cout<<"File is open"<<endl;

      uniData data;
      do
      {
        sec0.readSec (&file);
        sec1.readSec (&file, &data.sec1);
        if(data.sec1.GDS)
            sec2.readSec (&file, &data.sec2);
        if(data.sec1.BMS)
            sec3.readSec (&file, &data.sec3, data.sec2.pointQuantity);
        sec4.readSec (&file,&data.sec4, data.sec1.D,data.sec1.indicator_of_parameter_and_units, data.sec2.scanFlags, data.sec2.Ni,data.sec2.Nj);
        sec5.readSec (&file);

        listData.append (data);
      }while(!checkEOF());

      file.close ();
      return listData;
    }
    else
    {
       qDebug() << "Нет файла";
       listData.clear();
       return listData;
    }

}

void Parser::setFile(QString fileName)
{
    name=fileName;
}

QList<QList<paramData>> Parser::parameterData(int param)
{
    QList<uniData> data;
    QList<paramData> pData;
    QList<QList<paramData>> fData;
    paramData p;

    data=parse();
    for (int i = 0; i < data.size(); i++){
        if(data[i].sec1.indicator_of_parameter_and_units==param) {

            for(int j = 0; j <data[i].sec2.Nj; j++) {
                p.Lo=(data[i].sec2.Lo1+data[i].sec2.Di*j);
                p.Lo=p.Lo / 1000;
                for(int k = 0; k <data[i].sec2.Ni; k++) {
                    p.La=(data[i].sec2.La1+data[i].sec2.Dj_N*k);
                    p.La=p.La / 1000;
                    p.parValue=data[i].sec4.pointData[j*data[i].sec2.Ni+k];
                    p.year=data[i].sec1.year;
                    p.month=data[i].sec1.month;

                    if(data[i].sec1.P2+data[i].sec1.hour>=24 && data[i].sec1.forecast_time_unit==static_cast<int>(TimeUnit::hour)) {
                        p.day=data[i].sec1.day+1;
                        p.hour=data[i].sec1.P2+data[i].sec1.hour-24;
                    }
                    else if(data[i].sec1.P2!=0 && data[i].sec1.forecast_time_unit==static_cast<int>(TimeUnit::hour))
                        p.hour=data[i].sec1.hour;
                    else {
                        p.hour=data[i].sec1.hour;
                        p.day=data[i].sec1.day;
                    }
                    p.minut=data[i].sec1.minut;
                    pData.push_back(p);
                }
            }
            fData.push_back(pData);
            pData.clear();
        }

    }
    return fData;
}

bool Parser::checkEOF()
{
    file.get();
    if (file.eof ())
        return true;
    else{
        file.unget();
        return false;
    }
}
