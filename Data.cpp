#include <Data.h>

Data::Data(const Data &other):type(other.type),image(other.image),color(other.color),html(other.html),urls(other.urls),text(other.text){
}

Data::Data(QObject *parent):QObject(parent),type(0){

}

const Data& Data::operator =(const Data & other){
    type=other.type;
    image=other.image;
    color=other.color;
    html=other.html;
    urls=other.urls;
    text=other.text;
}


void Data::clear(){
    type=0;
    image=QVariant();
    color=QVariant();
    html="";
    text="";
    urls.clear();
}
