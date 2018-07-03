/*
* C++ Program to Implement Singly Linked List
*/
#pragma once
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;
/*
* Node Declaration
*/
struct node
{
	int info;
	struct node *next;
}*start;

/*
* Class Declaration
*/
class linkedList{
public:
	node* create_node(int);
	void insert_begin();
	void insert_pos();
	void insert_last();
	void delete_pos();
	void sort();
	void search();
	void update();
	void reverse();
	void display();
	linkedList()
	{
		start = NULL;
	}
};


#endif