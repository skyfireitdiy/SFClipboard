#include <Data.h>

Data::Data(const Data &other,QObject *parent):
    QObject(parent),
    type(other.type),
    text(other.text),
    html(other.html),
    image(other.image),
    color(other.color),
    urls(other.urls)
{
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
    return *this;
}


void Data::clear(){
    type=0;
    image=QVariant();
    color=QVariant();
    html="";
    text="";
    urls.clear();
}
