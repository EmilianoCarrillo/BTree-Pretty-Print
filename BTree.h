//
// Created by Emiliano Carrillo Moncayo on 10/16/18.
//

#ifndef TAREA_10_BTREE_H
#define TAREA_10_BTREE_H


#include <iostream>
#include <stdlib.h>

using namespace std;

const int MAX = 4;
const int MIN = 2 ;
struct btnode
{
    int count ;
    int value[MAX + 1];
    btnode *child[MAX + 1] ;
    btnode()
    {
        for(int i=0; i<MAX+1; i++)
        {
            value[i]=0;
            child[i]=NULL;
        }
    }
} ;
class btree
{
private :
    btnode *root ;
public :
    btree( ) ;
    static bool result;
    void insert ( int val ) ;
    int setval ( int val, btnode *n, int *p, btnode **c ) ;
    static btnode * search ( int val, btnode *root, int *pos ) ;
    static int searchnode ( int val, btnode *n, int *pos ) ;
    void fillnode ( int val, btnode *c, btnode *n, int k ) ;
    void split ( int val, btnode *c, btnode *n,int k, int *y, btnode **newnode ) ;
    void del ( int val ) ;
    int delhelp ( int val, btnode *root ) ;
    void clear ( btnode *root, int k ) ;
    void copysucc ( btnode *root, int i ) ;
    void restore ( btnode *root, int i ) ;
    void rightshift (btnode*, int k ) ;
    void leftshift (btnode*, int k ) ;
    void merge (btnode*, int k ) ;
    void show( ) ;
    static void display ( btnode *root ) ;
    static void deltree ( btnode *root ) ;
    void printTree(btnode*,int=0,string="");
    string printPage(btnode*,int,string);
    ~btree( ) ;
} ;
bool btree::result=false;
btree :: btree( )
{
    root = NULL ;
}
void btree :: insert ( int val)
{
    int i ;
    btnode *c, *n ;
    int flag ;
    flag = setval ( val, root, &i, &c );
    if ( flag )
    {
        n = new btnode ;
        n -> count = 1 ;
        n -> value[1] = i ;
        n -> child[0] = root ;
        n -> child[1] = c ;
        root = n ;
    }
}
int btree :: setval ( int val, btnode *n, int *p, btnode **c )
{
    int k ;
    if ( n == NULL )
    {
        *p = val ;
        *c = NULL ;
        return 1 ;
    }
    else
    {
        if (searchnode ( val, n, &k ))
        {
            cout << endl << "Key value already exists." << endl ;
            btree::result=true;
            return 0;
        }
        if (setval(val, n -> child[k], p, c ))
        {
            if (n ->count < MAX )
            {
                fillnode ( *p, *c, n, k ) ;
                return 0 ;
            }
            else
            {
                split ( *p, *c, n, k, p, c ) ;
                return 1 ;
            }
        }
        return 0 ;
    }
}
btnode * btree :: search ( int val, btnode *root, int *pos )
{
    if ( root == NULL )
        return NULL ;
    else
    {
        if ( searchnode ( val, root, pos ) )
            return root ;
        else
            return search ( val, root -> child[*pos], pos ) ;
    }
}
int btree :: searchnode ( int val, btnode *n, int *pos )
{
    if ( val < n -> value[1] )
    {
        *pos = 0 ;
        return 0 ;
    }
    else
    {
        *pos = n -> count ;
        while ( ( val < n -> value[*pos] ) && *pos > 1 )
            ( *pos )-- ;
        if ( val == n -> value[*pos] )
            return 1 ;
        else
            return 0 ;
    }
}
void btree :: fillnode ( int val, btnode *c, btnode *n, int k )
{
    int i ;
    for ( i = n -> count ; i > k ; i-- )
    {
        n -> value[i + 1] = n -> value[i] ;
        n -> child[i + 1] = n -> child[i] ;
    }
    n -> value[k + 1] = val ;
    n -> child[k + 1] = c ;
    n -> count++ ;
}
void btree :: split ( int val, btnode *c, btnode *n,int k, int *y, btnode **newnode )
{
    int i, mid ;

    if ( k <= MIN )
        mid = MIN ;
    else
        mid = MIN + 1 ;

    *newnode = new btnode ;

    for ( i = mid + 1 ; i <= MAX ; i++ )
    {
        ( *newnode ) -> value[i - mid] = n -> value[i] ;
        ( *newnode ) -> child[i - mid] = n -> child[i] ;
    }

    ( *newnode ) -> count = MAX - mid ;
    n -> count = mid ;

    if ( k <= MIN )
        fillnode ( val, c, n, k );
    else
        fillnode ( val, c, *newnode, k - mid ) ;

    *y = n -> value[n -> count] ;
    ( *newnode ) -> child[0] = n -> child[n -> count] ;
    n -> count-- ;
}
void btree :: del ( int val )
{
    btnode * temp ;

    if ( ! delhelp ( val, root ) )
    {
        cout << endl << "Value " << val << " not found.\n" ;
        btree::result=true;
    }
    else
    {
        if ( root -> count == 0 )
        {
            temp = root ;
            root = root -> child[0] ;
            delete temp ;
        }
    }
}
int btree :: delhelp ( int val, btnode *root )
{
    int i ;
    int flag ;

    if ( root == NULL )
        return 0 ;
    else
    {
        flag = searchnode ( val, root, &i ) ;
        if ( flag )
        {
            if ( root -> child[i - 1] )
            {
                copysucc ( root, i ) ;
                flag = delhelp ( root -> value[i], root -> child[i] ) ;
                if ( !flag ){
                    cout << endl << "Value " << val << " not found.\n" ;
                    btree::result=true;
                }
            }
            else
                clear ( root, i ) ;
        }
        else
            flag = delhelp ( val, root -> child[i] ) ;
        if ( root -> child[i] != NULL )
        {
            if ( root -> child[i] -> count < MIN )
                restore ( root, i ) ;
        }
        return flag ;
    }
}
void btree :: clear ( btnode *root, int k )
{
    int i ;
    for ( i = k + 1 ; i <= root -> count ; i++ )
    {
        root -> value[i - 1] = root -> value[i] ;
        root -> child[i - 1] = root -> child[i] ;
    }
    root -> count-- ;
}
void btree :: copysucc ( btnode *root, int i )
{
    btnode *temp = root -> child[i] ;

    while ( temp -> child[0] )
        temp = temp -> child[0] ;

    root -> value[i] = temp -> value[1] ;
}
void btree :: restore (btnode *root, int i )
{
    if ( i == 0 )
    {
        if ( root -> child [1] -> count > MIN )
            leftshift (root, 1 ) ;
        else
            merge (root, 1 ) ;
    }
    else
    {
        if ( i == root -> count )
        {
            if ( root -> child[i - 1] -> count > MIN )
                rightshift (root, i ) ;
            else
                merge (root, i ) ;
        }
        else
        {
            if ( root -> child[i - 1] -> count > MIN )
                rightshift (root, i ) ;
            else
            {
                if ( root -> child[i + 1] -> count > MIN )
                    leftshift (root, i + 1 ) ;
                else
                    merge (root, i ) ;
            }
        }
    }
}
void btree :: rightshift (btnode* root, int k )
{
    int i ;
    btnode *temp ;

    temp = root -> child[k] ;

    for ( i = temp -> count ; i > 0 ; i-- )
    {
        temp -> value[i + 1] = temp -> value[i] ;
        temp -> child[i + 1] = temp -> child[i] ;
    }

    temp -> child[1] = temp -> child[0] ;
    temp -> count++ ;
    temp -> value[1] = root -> value[k] ;
    temp = root -> child[k - 1] ;
    root -> value[k] = temp -> value[temp -> count] ;
    root -> child[k] -> child [0] = temp -> child[temp -> count] ;
    temp -> count-- ;
}
void btree :: leftshift (btnode* root, int k )
{
    btnode *temp ;

    temp = root -> child[k - 1] ;
    temp -> count++ ;
    temp -> value[temp -> count] = root -> value[k] ;
    temp -> child[temp -> count] = root -> child[k] -> child[0] ;
    temp = root -> child[k] ;
    root -> value[k] = temp -> value[1] ;
    temp -> child[0] = temp -> child[1] ;
    temp -> count-- ;
    for ( int i = 1 ; i <= temp -> count ; i++ )
    {
        temp -> value[i] = temp -> value[i + 1] ;
        temp -> child[i] = temp -> child[i + 1] ;
    }
}
void btree :: merge (btnode* root, int k)
{
    btnode *temp1, *temp2 ;
    int i;
    temp1 = root -> child[k] ;
    temp2 = root -> child[k - 1] ;
    temp2 -> count++ ;
    temp2 -> value[temp2 -> count] = root -> value[k] ;
    temp2 -> child[temp2 -> count] = temp1-> child[0];
    for (i = 1 ; i <= temp1 -> count ; i++ )
    {
        temp2 -> count++ ;
        temp2 -> value[temp2 -> count] = temp1 -> value[i] ;
        temp2 -> child[temp2 -> count] = temp1 -> child[i] ;
    }
    for ( i = k ; i < root -> count ; i++ )
    {
        root -> value[i] = root -> value[i + 1] ;
        root -> child[i] = root -> child[i + 1] ;
    }
    root -> count-- ;
    delete temp1;
}

void btree :: show( )
{
    ///display ( root ) ;
    printTree(root) ;
}
void btree :: display ( btnode *root )
{
    int i;
    if ( root != NULL )
    {
        for (i = 0 ; i < root -> count ; i++ )
        {
            display ( root -> child[i] ) ;
            cout << root -> value[i + 1] << "\t" ;
        }
        display ( root -> child[i] ) ;
    }
}
void btree :: deltree ( btnode *root )
{
    int i;
    if ( root != NULL )
    {
        for (i = 0 ; i < root -> count ; i++ )
        {
            deltree ( root -> child[i] ) ;
            delete ( root -> child[i] ) ;
        }
        deltree ( root -> child[i] ) ;
        delete ( root -> child[i] ) ;
    }
}

btree :: ~btree( )
{
    deltree ( root ) ;
}


string btree::printPage(btnode *myNode, int nChild, string prevString){
    prevString += "     ";
    for (int j = 1; j <= nChild; ++j)  prevString +=  "|  ";
    cout << prevString;

    cout << " `---." ;
    for (int i = 0; i < myNode->count; i++) {
        cout << myNode->value[i+1] << ".";
    }
    cout << endl;

    return prevString;
}

void btree::printTree(btnode *myNode,  int nChild, string prevString ) {
    if (myNode) {
        string aux = prevString;
        prevString = printPage(myNode, nChild, aux);
        for (int i = myNode->count; i >= 0; i--) {
            printTree(myNode->child[i], i, prevString);
        }
    }
}

#endif //TAREA_10_BTREE_H
