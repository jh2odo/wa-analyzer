/*
The MIT License (MIT)

Copyright (c) 2012 Jose Aguado (jh2odo)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE
*/
#include "message.h"

Message::Message()
{
    contact = Contact();
    type = "Private";
    group = Group();
    length = 0;
    id = "";
}

bool Message::isGroup(){
    if(group.getId().isEmpty()) {
        return false;
    }
    return true;
}

Contact Message::getContact(){
    return contact;
}

QString Message::getType(){
    return type;
}

Group Message::getGroup(){
    return group;
}

int Message::getLength(){
    return length;
}

QString Message::getId(){
    return id;
}

void Message::setContact(Contact contact){
    this->contact = contact;
}

void Message::setType(QString type){
    this->type = type;
}

void Message::setGroup(Group group){
    this->group = group;
}

void Message::setLength(int length){
    this->length = length;
}

void Message::setId(QString id){
    this->id = id;
}

