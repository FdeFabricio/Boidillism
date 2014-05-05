#include "linkedlist.h"
#include <cstdio>

linkedList::linkedList()
{
    head = 0;
    tail = 0;
    count = 0;
}

linkedList::~linkedList()
{
    item *current;
    current = head;
    if (current == 0);
    int foundOne = 1;
    while(foundOne)
    {
        foundOne = 0;
        for(int i = 0; i < this->count ;i += 2)
        {
            item *c = current;
            current = current->next;
            delete c;
        }
    }
}

//-----------------------------------------------------------------

int linkedList::size()
{
	return count;
}

//-----------------------------------------------------------------

void linkedList::set(long index, long _value)
{
    item* current;
    current = head;
    if (current == 0)return;
    for (int i = 0; i < index; i++) current = current->next;
    current->value = _value;
}

//-----------------------------------------------------------------

long linkedList::get(long index)
{
    item* current;
    current = head;
    if (current==0)return 0;
    for(int i=0;i<index;i++){
        current = current->next;
    }
    return current->value;
}

//-----------------------------------------------------------------
int linkedList::push(long data)
{
    item *new_item;
    
    new_item = new item();
    if (new_item == 0) return false;
    new_item->next = 0;
    new_item->previous = 0;
    
    new_item->value = data;
    
    if ( head == 0 || tail == 0){
        tail = new_item;
        head = new_item;
        new_item -> next = 0;
        new_item -> previous = 0;
    } else {
        tail -> next = new_item;
        new_item -> previous = tail;
        new_item -> next = 0;
        tail = new_item;
    }
    count++;
    return true;
}

//-----------------------------------------------------------------

long linkedList::pop(){
    if(count>0){
        item* n=tail;
        tail=tail->previous;
        count--;
        long theItem = n->value;
        delete n;
        return theItem;
    }else return 0;
}

//-----------------------------------------------------------------

long linkedList::unshift(){
    item* n=head;
    head=head->next;
    count--;
    long theItem = n->value;
    delete n;
    return theItem;
}

//-----------------------------------------------------------------

int linkedList::shift( long data ){
    item* new_item;
    
    new_item = new item;
    if ( new_item == 0 ) return false;
    new_item->value = data;
    if ( head == 0 || tail==0 )
    {
        tail = new_item;
        head = new_item;
        new_item -> next = 0;
        new_item -> previous = 0;
    }
    else
    {
        head -> previous = new_item;
        new_item -> next = head;
        new_item -> previous = 0;
        head = new_item;
    }
    count++;
    return true;
}

//------------------------------------------------------------------


void linkedList::deleteMatch(long match){
    item* current;
    if(head==0||tail==0)return;
    int foundOne = 1;
    while(foundOne){
        current = head;
        foundOne = 0;
        for(int i=0;i<this->count;i++){
            if(current->value==match){
                if(current==head){
                    if(count>1){
                        current->next->previous = 0;
                    }
                    head = current->next;
                    delete current;
                    foundOne = 1;
                    this->count--;
                    break;
                }else if(current==tail){
                    current->previous->next = 0;
                    tail = current->previous;
                    delete current;
                    foundOne = 1;
                    this->count--;
                    break;
                }else{//for everyone else in between
                    current->previous->next = current->next;
                    current->next->previous = current->previous;
                    delete current;
                    foundOne = 1;
                    this->count--;
                    break;
                }
            }
            //advance if needed
            current = current->next;
        }
        
    }
}

void linkedList::deletePosition(int n)
{
    item* current;
    if (head == 0 || tail == 0) return;
    int foundOne = 1;
    while(foundOne)
    {
        current = head;
        foundOne = 0;
        for (int i = 0; i < this->count; i++)
        {
            if (i == n)
            {
                if(current == head)
                {
                    if(count>1) current->next->previous = 0;
                    head = current->next;
                    delete current;
                    foundOne = 1;
                    this->count--;
                    return;
                }else if(current==tail){
                    current->previous->next = 0;
                    tail = current->previous;
                    delete current;
                    foundOne = 1;
                    this->count--;
                    return;
                }else{//for everyone else in between
                    current->previous->next = current->next;
                    current->next->previous = current->previous;
                    delete current;
                    foundOne = 1;
                    this->count--;
                    return;
                }
            }
            //advance if needed
            current = current->next;
        }
        
    }
}

void linkedList::print()
{
    item *current;
    if (count < 1) printf("Empty List\n\n");
    int foundOne = 1;
    while(foundOne)
    {
        current = head;
        foundOne = 0;
        for (int i = 0; i < this->count; i++)
        {
            printf("%ld ", current->value);
            current = current->next;
        }
        printf("\n\n");
    }
}



