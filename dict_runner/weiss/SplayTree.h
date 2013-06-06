        #ifndef SPLAY_TREE_H_
        #define SPLAY_TREE_H_

        #include "dsexceptions.h"
        #include <iostream>       // For NULL

        // SplayTree class
        //
        // CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // void remove( x )       --> Remove x
        // Comparable find( x )   --> Return item that matches x
        // Comparable findMin( )  --> Return smallest item
        // Comparable findMax( )  --> Return largest item
        // boolean isEmpty( )     --> Return true if empty; else false
        // void makeEmpty( )      --> Remove all items
        // void printTree( )      --> Print tree in sorted order


          // Node and forward declaration because g++ does
          // not understand nested classes.
        template <class Comparable>
        class SplayTree;

        template <class Comparable>
        class SBinaryNode
        {
            Comparable  element;
            SBinaryNode *left;
            SBinaryNode *right;

            SBinaryNode( ) : left( NULL ), right( NULL ) { }
            SBinaryNode( const Comparable & theElement, SBinaryNode *lt, SBinaryNode *rt )
              : element( theElement ), left( lt ), right( rt ) { }

            friend class SplayTree<Comparable>;
        };


        template <class Comparable>
        class SplayTree
        {
          public:
            explicit SplayTree( const Comparable & notFound );
            SplayTree( const SplayTree & rhs );
            ~SplayTree( );

            const Comparable & findMin( );
            const Comparable & findMax( );
            const Comparable & find( const Comparable & x );
            bool isEmpty( ) const;
            void printTree( ) const;

            void makeEmpty( );
            void insert( const Comparable & x );
            void remove( const Comparable & x );

            const SplayTree & operator=( const SplayTree & rhs );

          private:
            SBinaryNode<Comparable> *root;
            SBinaryNode<Comparable> *nullNode;
            const Comparable ITEM_NOT_FOUND;

            const Comparable & elementAt( SBinaryNode<Comparable> *t ) const;

            void reclaimMemory( SBinaryNode<Comparable> * t ) const;
            void printTree( SBinaryNode<Comparable> *t ) const;
            SBinaryNode<Comparable> * clone( SBinaryNode<Comparable> *t ) const;

                // Tree manipulations
            void rotateWithLeftChild( SBinaryNode<Comparable> * & k2 ) const;
            void rotateWithRightChild( SBinaryNode<Comparable> * & k1 ) const;
            void splay( const Comparable & x, SBinaryNode<Comparable> * & t ) const;
        };

        #include "SplayTree.cpp"
        #endif
