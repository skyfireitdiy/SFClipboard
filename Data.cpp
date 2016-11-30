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

Data::Data(QObject *parent):QObject(parent){

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


bool Data::operator ==(const Data & other)const{
    return this->type==other.type&&
            this->image==other.image&&
            this->text==other.text&&
            this->html==other.html&&
            this->color==other.color&&
            this->urls==other.urls;
}

bool Data::operator !=(const Data & other)const{
    return !operator ==(other);
}
