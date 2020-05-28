#include "pds.h"

void PDS::readSec(ifstream *file, PDS::Data *d)
{
    char message[30];
    //string message;

    for(int i=0;i<=2;i++) {
        message[i]=file->get();
    }
    cout<<"Section 1: "<<endl;
    d->sectionLength = secLength(message[0],message[1],message[2]);
    cout<<"Length: "<<d->sectionLength<<endl;

    for(int i=3;i<d->sectionLength;i++){
        message[i]=file->get();
    }
    d->parameter_table_version_number = message[3];
    if(d->parameter_table_version_number == 2) cout<<"International exchange"<<endl;
    else if(d->parameter_table_version_number>=128 && d->parameter_table_version_number <= 254)   cout<<"Reserved for local use"<<endl;

    d->identification_of_center=centerIndentification(message[4]);
    cout<<"Indentification of center: "<<d->identification_of_center<<endl;

    d->generating_process_ID_number=(unsigned char)message[5];
    cout<<"Generating process ID number"<<d->generating_process_ID_number<<endl;


    d->grid_identification=message[7];
    cout<<"Grid Identification"<<d->grid_identification<<endl;

    d->GDS=(message[7]&128)!=0;
    d->BMS=(message[7]&64)!=0;

    d->indicator_of_parameter_and_units=message[8];
    //cout<<"indicator_of_parameter_and_units: "<<d->indicator_of_parameter_and_units<<endl;

    d->indicator_of_type_of_level_or_layer=message[9];
    cout<<"indicator_of_type_of_level_or_layer: "<<d->indicator_of_type_of_level_or_layer<<endl;

    d->info_of_the_level_or_layer=int2(message[10],message[11]);
    cout<<"info_of_the_level_or_layer: "<<d->info_of_the_level_or_layer<<endl;

    d->year=message[12]+message[24]*100-100;
    d->month=message[13];
    d->day=message[14];
    d->hour=message[15];
    d->minut=(int)message[16];
    d->refDate=UTC_mktime(d->year,d->month,d->day,d->hour,d->minut,0);
    cout<<"date "<<d->year<<"."<<d->month<<"."<<d->day<<endl;
    cout<<"time "<<d->hour<<":"<<d->minut<<endl;

    d->forecast_time_unit=message[17];
    d->P1=message[18];
    d->P2=message[19];
    d->time_range_indicator=message[20];
    d->periodSec=period (d->forecast_time_unit,d->P1,d->P2,d->time_range_indicator);
    d->curDate=UTC_mktime(d->year,d->month,d->day,d->hour,d->minut,d->periodSec);
    //21,22?
    cout<<"UTC: "<<d->curDate<<endl;
    d->number_missing_from_averages_or_accumulations=message[23];
    d->century=message[24];
    d->identification_of_sub_center=message[25];

    factorD(&(d->D),message[26],message[27]);//
    cout<<"factorD "<<d->D<<endl;
}

long long PDS::UTC_mktime(int year, int month, int day, int hour, int min, int sec)
{
    if (year<1970 || month<1 || month>12 || day<1
            || hour<0 || min<0 || sec<0)
        return -1;
    long long r = 0;

    for (int y=1970; y<year; y++) {
        r += 365*24*3600;
        if (isleapyear(y))
            r += 24*3600;
    }
    if (month > 1) {
        for (int m=1; m<month; m++) {
            if (m==2) {
                r += 28*24*3600;
                if (isleapyear(year))
                    r += 24*3600;
            }
            else if (m==1||m==3||m==5||m==7||m==8||m==10||m==12) {
                r += 31*24*3600;
            }
            else {
                r += 30*24*3600;
            }
        }
    }
    r += (day-1)*24*3600;
    r += hour*3600;
    r += min*60;
    r += sec;
    return r;
}

void PDS::factorD(int* D,char a, char b)
{
    bool sign=((a&128)!=0);
    a=a&127;
    if (sign)
        *D=-int2(a,b);
    else *D=int2(a,b);

}

string PDS::centerIndentification(char a)
{
    string s;
    switch(a){
    case static_cast<int>(OrigCenters::USWS_NationalMetCenter):
        s="USWS_NationalMetCenter";
        break;
    case static_cast<int>(OrigCenters::USWS_NWSTelecommsGateway):
        s="USWS_NWSTelecommsGateway";
        break;
    case static_cast<int>(OrigCenters::USWS_FieldStations):
        s="USWS_FieldStations";
        break;
    case static_cast<int>(OrigCenters::JapaneseMA_Tokyo):
        s="JapaneseMA_Tokyo";
        break;
    case static_cast<int>(OrigCenters::NHC_Miami):
        s="NHC_Miami";
        break;
    case static_cast<int>(OrigCenters::CanadianMS_Montreal):
        s="CanadianMS_Montreal";
        break;
    case static_cast<int>(OrigCenters::USAF_GWC):
        s="USAF_GWC";
        break;
    case static_cast<int>(OrigCenters::USNavy_FNOC):
        s="USNavy_FNOC";
        break;
    case static_cast<int>(OrigCenters::NOAAFSL_BoulderCO):
        s="NOAAFSL_BoulderCO";
        break;
    case static_cast<int>(OrigCenters::UKMO_Bracknell):
        s="UKMO_Bracknell";
        break;
    case static_cast<int>(OrigCenters::FWS_Toulouse):
        s="FWS_Toulouse";
        break;
    case static_cast<int>(OrigCenters::ESA):
        s="ESA";
        break;
    case static_cast<int>(OrigCenters::ECforMediumRangeForecaste):
        s="ECforMediumRangeForecaste";
        break;
    case static_cast<int>(OrigCenters::DeBilt):
        s="DeBilt";
        break;
    default:
        s="Unknown";
    }
    return s;
}

string PDS::dataType(char a)
{
    string s;
    switch(a){
    case GRB_PRESSURE:
        s="Pressure";
        break;
    case static_cast<int>(dataTypes::GRB_PRESSURE_MSL):
        s="Pressure_Msl";
        break;
    case static_cast<int>(dataTypes::GRB_GEOPOT_HGT):
        s="Geopot_Hgt";
        break;
    case static_cast<int>(dataTypes::GRB_TEMP):
        s="Temp";
        break;
    case static_cast<int>(dataTypes::GRB_TEMP_POT):
        s="Temp_pot";
        break;
    case static_cast<int>(dataTypes::GRB_TMAX):
        s="temp_MAX";
        break;
    case static_cast<int>(dataTypes::GRB_TMIN):
        s="temp_MIN";
        break;
    case static_cast<int>(dataTypes::GRB_WIND_SPEED):
        s="wind_speed";
        break;
    default:
        s="Unknown";
    }
    return s;
}

int PDS::period(int unit, int P1, int P2, int range)
{
    int res, dur;
    switch (unit) {
    case 0: //	Minute
        res = 60; break;
    case 1: //	Hour
        res = 3600; break;
    case 2: //	Day
        res = 86400; break;
    case 10: //	3 hours
        res = 10800; break;
    case 11: //	6 hours
        res = 21600; break;
    case 12: //	12 hours
        res = 43200; break;
    case 254: // Second
        res = 1; break;
    case 3: //	Month
    case 4: //	Year
    case 5: //	Decade (10 years)
    case 6: //	Normal (30 years)
    case 7: //	Century (100 years)
    default:

        res = 0;

    }

    dur = 0;
    switch (range) {
    case 0:
        dur = (int)P1; break;
    case 1:
        dur = 0; break;
    case 2:
    case 3:

        dur = (int)P2; break;
    case 4:
        dur = (int)P2; break;
    case 10:
        dur = ((int)P1<<8) + (int)P2; break;
    default:
        dur = 0;
        // ok = false;
    }
    return res*dur;
}
