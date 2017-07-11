///////////////////////////////////////////////////////////////////////////
// FILE: _algsort.h (Definitions of sorting and related operations)
//
// =========================================================================
//
//                          Open Watcom Project
//
// Copyright (c) 2004-2010 The Open Watcom Contributors. All Rights Reserved.
//
//    This file is automatically generated. Do not edit directly.
//
// =========================================================================
//
// Description: This header is part of the <algorithm> header.
///////////////////////////////////////////////////////////////////////////
#ifndef __ALGSORT_H_INCLUDED
#define __ALGSORT_H_INCLUDED

#ifndef _ENABLE_AUTODEPEND
 #pragma read_only_file;
#endif

#ifndef _ALGORITHM_INCLUDED
 #error The header _algsort.h requires algorithm.
#endif

#ifndef _FUNCTIONAL_INCLUDED
 #include <function>
#endif

#ifndef _ITERATOR_INCLUDED
 #include <iterator>
#endif

#ifndef _STDEXCEPT_INCLUDED
 #include <stdexcep>
#endif

namespace std {

namespace _ow {

    template< class Int >
    inline Int heap_parent( Int index )
        { return ( ( index - 1 )/2 ); }

    template< class Int >
    inline Int heap_left( Int index )
        { return ( 2*index + 1 ); }

    template< class Int >
    inline Int heap_right( Int index )
        { return ( 2*index + 2 ); }

    template< class RandomAccess >
    void heapify( RandomAccess first,
                  RandomAccess last,
                  typename
                    iterator_traits< RandomAccess >::difference_type index)
    {
        using std::swap;
        typedef typename iterator_traits< RandomAccess >::difference_type Int;

        Int size = last - first;
        Int L    = heap_left( index );   // Index of left child.
        Int R    = heap_right( index );  // Index of right child.
        Int largest = index;             // Index of largest child.

        if( L < size && first[largest] < first[L] ) largest = L;
        if( R < size && first[largest] < first[R] ) largest = R;
        if( largest != index ) {
            swap( first[index], first[largest] );
            heapify( first, last, largest );
        }
    }

} // namespace _ow

// push_heap( RandomAccess, RandomAccess )
// ***************************************
template< class RandomAccess >
void push_heap( RandomAccess first, RandomAccess last )
{
    typedef typename iterator_traits< RandomAccess >::difference_type Int;
    using std::swap;
    using _ow::heap_parent;

    if( first == last )
        throw std::domain_error( "heap underflow" );

    Int index = (last - first) - 1;
    while( index > 0 && first[heap_parent( index )] < first[index] ) {
        swap( first[heap_parent( index )], first[index] );
        index = heap_parent( index );
    }
}

// pop_heap( RandomAccess, RandomAccess )
// **************************************
template< class RandomAccess >
void pop_heap( RandomAccess first, RandomAccess last )
{
    using std::swap;

    if( first == last )
        throw std::domain_error( "heap underflow" );

    --last;
    if( first != last ) {
        swap( *first, *last );
        _ow::heapify( first, last, 0 );
    }
}

// make_heap( RandomAccess, RandomAccess )
// ***************************************
template< class RandomAccess >
void make_heap( RandomAccess first, RandomAccess last )
{
    typedef typename iterator_traits< RandomAccess >::difference_type Int;

    Int size = last - first;
    Int index;

    if( size <= 1 ) return;
    for( index = size/2 - 1; index >= 0; --index ) {
        _ow::heapify( first, last, index );
    }
}

// sort_heap( RandomAccess, RandomAccess )
// ***************************************
template< class RandomAccess >
void sort_heap( RandomAccess first, RandomAccess last )
{
    typedef typename iterator_traits< RandomAccess >::difference_type Int;
    using std::swap;

    Int size = last - first;
    Int index;

    if( size <= 1 ) return;
    for( index = size - 1; index > 0; --index ) {
        swap( first[0], first[index] );
        --last;
        _ow::heapify( first, last, 0 );
    }
}

namespace _ow {

    // Used for small subsequences when doing a QuickSort.
    template< class Bidirectional, class Compare>
    void insertion_sort( Bidirectional first,
                         Bidirectional last,
                         Compare comp)
    {
        if( first == last ) return;

        Bidirectional current = first;
        ++current;
        while( current != last ) {
            typename std::iterator_traits< Bidirectional >::value_type
            temp = *current;
            Bidirectional p1 = current;
            Bidirectional p2 = current;
            --p2;

            while( comp( temp, *p2 ) ) {
                *p1 = *p2;
                if( p2 == first ) {
                    --p1;
                    break;
                }
                --p1; --p2;
            }
            *p1 = temp;
            ++current;
        }
    }

    template< class RandomAccess, class Compare >
    RandomAccess
    med3( RandomAccess seq,
          typename std::iterator_traits< RandomAccess >::difference_type left,
          typename std::iterator_traits< RandomAccess >::difference_type right,
          Compare comp)
    {
        using std::swap;
        typename std::iterator_traits< RandomAccess >::difference_type middle;

        middle = (left + right) / 2;
        if( comp( seq[middle], seq[left] ) ) swap( seq[left], seq[middle] );
        if( comp( seq[right], seq[left] ) ) swap( seq[left], seq[right] );
        if( comp( seq[right], seq[middle] ) ) swap( seq[middle], seq[right] );

        swap( seq[middle], seq[right - 1] );
        return seq + (right - 1);
    }

    // Based on the QuickSort algorithm in Mark Allen Weiss's "Data
    // Structures and Algorithm Analysis in C++" third edition; Addison
    // Wesley; ISBN=0-321-44146-X.
    //
    template< class RandomAccess, class Compare >
    void quick_sort( RandomAccess seq,
          typename std::iterator_traits< RandomAccess >::difference_type left,
          typename std::iterator_traits< RandomAccess >::difference_type right,
                   Compare comp)
    {
        using std::swap;
        typedef typename std::iterator_traits< RandomAccess >::difference_type
            difference_type; 

        if( right - left < 10 )
            insertion_sort( seq + left, seq + right + 1, comp );
        else {
            typename std::iterator_traits< RandomAccess >::value_type
                pivot = *med3( seq, left, right, comp );

            difference_type i = left;
            difference_type j = right - 1;
            for( ; ; ) {
                while( comp( seq[++i], pivot ) ) ;
                while( comp( pivot, seq[--j] ) ) ;
                if( i >= j ) break;
                swap( seq[i], seq[j] );
            }

            swap( seq[i], seq[right-1] );
            quick_sort( seq, left, i - 1, comp );
            quick_sort( seq, i + 1, right, comp );
        }
    }

} // namespace _ow


// sort( RandomAccess, RandomAccess, Compare )
// *******************************************
template< class RandomAccess, class Compare >
inline
void sort( RandomAccess first, RandomAccess last, Compare comp )
{
    if( first == last ) return;
    _ow::quick_sort( first, 0, (last - first) - 1, comp );
}

// sort( RandomAccess, RandomAccess )
// **********************************
template< class RandomAccess >
inline
void sort( RandomAccess first, RandomAccess last )
{
    if( first == last ) return;
    _ow::quick_sort(first, 0, (last - first) - 1,
      std::less< typename iterator_traits< RandomAccess >::value_type >( )
    );
}

// binary_search( ForwardIterator, ForwardIterator, const Type &, Compare )
// ************************************************************************
template< class ForwardIterator, class Type, class Compare >
inline
bool binary_search( ForwardIterator first, ForwardIterator last,
                    const Type &value,
                    Compare comp )
{
    typedef typename
        iterator_traits< ForwardIterator >::difference_type length_type;
        
    ForwardIterator p1( first );
    length_type len = last - first;   // This should really use distance( ).
    length_type mid = len/2;
    ForwardIterator p2( p1 );
    advance( p2, mid );

    while( len > 0 ) {
        if( comp( value, *p2 ) ) {
            len = mid;
            mid = len/2;
            p2  = p1;
            advance( p2, mid );
        }
        else if( comp( *p2, value ) ) {
            len = mid + ((len % 2) == 0 ? 0 : 1 ) - 1;
            mid = len/2;
            p1  = ++p2;
            advance( p2, mid );
        }
        else {
            return( true );
        }
    }
    return( false );
}

// binary_search( ForwardIterator, ForwardIterator, const Type & )
// ***************************************************************
template< class ForwardIterator, class Type >
inline
bool binary_search( ForwardIterator first, ForwardIterator last,
                    const Type &value )
{
    return( binary_search( first, last, value,
      std::less< typename iterator_traits< ForwardIterator >::value_type >( ) ) );
}

// min( const Type &, const Type & )
// *********************************
template< class Type >
inline const Type &min( const Type &x, const Type &y )
    { return( (y < x) ? y : x ); }

// max( const Type &, const Type & )
// *********************************
template< class Type >
inline const Type &max( const Type &x, const Type &y )
    { return( (x < y) ? y : x ); }

// min( const Type &, const Type &, Compare )
// ******************************************
template< class Type, class Compare >
inline const Type &min( const Type &x, const Type &y, Compare comp )
    { return( comp( y, x ) ? y : x ); }

// max( const Type &, const Type &, Compare )
// ******************************************
template< class Type, class Compare >
inline const Type &max( const Type &x, const Type &y, Compare comp )
    { return( comp( x, y ) ? y : x ); }

// min_element( ForwardIterator, ForwardIterator )
// ***********************************************
template< class ForwardIterator >
ForwardIterator min_element( ForwardIterator first, ForwardIterator last )
{
    if( first == last ) return( last );
    ForwardIterator tmp( first );
    ++first;
    while( first != last ) {
        if( *first < *tmp ) tmp = first;
        ++first;
    }
    return( tmp );
}

// min_element( ForwardIterator, ForwardIterator, Compare )
// ********************************************************
template< class ForwardIterator, class Compare >
ForwardIterator min_element( ForwardIterator first,
                             ForwardIterator last,
                             Compare comp )
{
    if( first == last ) return( last );
    ForwardIterator tmp( first );
    ++first;
    while( first != last ) {
        if( comp( *first, *tmp ) ) tmp = first;
        ++first;
    }
    return( tmp );
}

// max_element( ForwardIterator, ForwardIterator )
// ***********************************************
template< class ForwardIterator >
ForwardIterator max_element( ForwardIterator first, ForwardIterator last )
{
    if( first == last ) return( last );
    ForwardIterator tmp( first );
    ++first;
    while( first != last ) {
        if( *tmp < *first ) tmp = first;
        ++first;
    }
    return( tmp );
}

// max_element( ForwardIterator, ForwardIterator, Compare )
// ********************************************************
template< class ForwardIterator, class Compare >
ForwardIterator max_element( ForwardIterator first,
                             ForwardIterator last,
                             Compare comp )
{
    if( first == last ) return( last );
    ForwardIterator tmp( first );
    ++first;
    while( first != last ) {
        if( comp( *tmp, *first ) ) tmp = first;
        ++first;
    }
    return( tmp );
}

// next_permutation( Bidirectional, Bidirectional, Compare )
// *********************************************************
template< typename Bidirectional, typename Compare >
bool next_permutation( Bidirectional first, Bidirectional last, Compare comp )
{
    if( first == last ) return false;
    Bidirectional i = first;
    ++i;
    if( i == last ) return false;
    i = last;
    --i;

    for( ;; ) {
        Bidirectional ii = i;
        --i;
        if( comp( *i, *ii ) ) {
            Bidirectional j = last;
            while( !comp( *i, *--j ) ) /* null */ ;
            swap( *i, *j );
            reverse( ii, last );
            return true;
        }
        if( i == first ) {
            reverse( first, last );
            return false;
        }
    }
}

// next_permutation( Bidirectional, Bidirectional )
// ************************************************
template< typename Bidirectional >
bool next_permutation( Bidirectional first, Bidirectional last )
{
    return ( next_permutation( first, last,
        std::less< typename iterator_traits< Bidirectional >::value_type >( )
    ) );
}

// prev_permutation( Bidirectional, Bidirectional, Compare )
// *********************************************************
template< class Bidirectional, class Compare >
bool prev_permutation( Bidirectional first, Bidirectional last, Compare comp )
{
    if( first == last ) return false;
    Bidirectional i = first;
    ++i;
    if( i == last ) return false;
    i = last;
    --i;

    for( ;; ) {
        Bidirectional ii = i;
        --i;
        if( comp( *ii, *i ) ) {
            Bidirectional j = last;
            while( !comp( *--j, *i ) ) /* null */ ;
            swap( *i, *j );
            reverse( ii, last );
            return true;
        }
        if( i == first ) {
            reverse( first, last );
            return false;
        }
    }
}

// prev_permutation( Bidirectional, Bidirectional )
// ************************************************
template< typename Bidirectional >
bool prev_permutation( Bidirectional first, Bidirectional last )
{
    return ( prev_permutation( first, last,
        std::less< typename iterator_traits< Bidirectional >::value_type >( )
    ) );
}

} // namespace std

#endif
