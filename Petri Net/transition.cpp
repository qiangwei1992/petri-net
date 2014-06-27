//
//  transition.cpp
//  Petri Net
//
//  Created by adore on 14-6-26.
//  Copyright (c) 2014年 adore. All rights reserved.
//
#include "place.h"
#include "transition.h"


transition::transition(int i,bool is): next(nullptr), pre(nullptr), time(i), nextnum(1), iscross(is){}
bool transition::pop(int i)
{
    if (i == 0) {
        if (!contain.empty() && contain.front().second >= time) {
            contain.erase(contain.begin());
            return 1;
        }
        return 0;
    }
    else { 
        if (i < contain.size() - 1 && contain[i].second >= time){
            contain.erase(contain.begin() + i);
            return 1;
        }
        return 0;
    }
}
bool transition::push(token& t, bool canrun)
{
    if (iscross == 1) {
         if (canrun == 0) return 0;  
    }
    if (this->next != nullptr) {
        if (nextnum == 1)
        {
            if (!this->next->subcapacity(t.getdirctions())) return 0;
        }
        else {
            if (t.getturn() == next1turn) {
                if (!this->next->subcapacity(t.getdirctions())) return 0;
            }
            else {
                if (!this->next2->subcapacity(t.getdirctions())) return 0;
            }
        }
    }
    if (this->pre != nullptr) {
        this->pre->addcapacity(t.getdirctions());
    }
    contain.push_back(pair(t, 1));
    return 1;
}
token& transition::top()
{
    if (contain.empty()) throw 0;
    else return contain[0].first;
}
void transition::update()
{
    for (auto& beg: contain){
        beg.second += 1;
    }
}
void transition::setnext2(place* next, int color)
{
    this->next2 = next;
    next1turn = color;
    nextnum = 2;
}
void transition::setnext(place* next)
{
    this->next = next;
}
void transition::setpre(place* pre)
{
    this->pre = pre;
}
void transition::setattach(place* next)
{
    this->setnext(next);
    this->next->setpre(this);
}
void transition::setattach2(place* next, int color)
{
    this->setnext2(next, color);
    this->next2->setpre(this);
}
place* transition::getnext() const
{
    return next;
}
place* transition::getpre() const
{
    return pre;
}
void transition::act()
{
    this->update();
    try {
        if (nextnum == 1) {
            if (this->next != nullptr) {
                if (!contain.empty()) {
                    token *k = new token(contain[0].first);
                    while(this->pop()){
                        this->next->push(*k);
                        k = new token(contain[0].first);
                    }
                }
            }
        }   
        else {
            if (this->next != nullptr && this->next2 != nullptr) {
                for (int i = 0; i != contain.size(); i++) {
                    if (!contain.empty()) {
                        token* k = new token(contain[i].first);
                        if (this->pop(i)) {
                            if (k->getturn() == next1turn){
                                this->next->push(*k);
                            }
                            else {
                                this->next2->push(*k);
                            }
                            --i;
                        }
                    }
                }
            }
        }
    }
    catch (int i){}
    this->next->act();
}
int transition::getnowcars(int x, int y)
{
    auto k = 0;
    for (auto beg = contain.begin(); beg != contain.end(); beg++)
    {
        if (beg->first.getdirctions() == x && beg->first.getturn() == y) {
            k++;
        }
    }
    return k;
    
}
