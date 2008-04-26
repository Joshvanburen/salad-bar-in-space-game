#pragma once
/*
================================================================================
                           hat container class                      version 1.59
                             by AngleWyrm

  The hat is a container that provides storage and retrieval from a
  weighted random set, both with and without replacement. Items contained can
  have unequally distributed probabilities, and also don't have to be unique.

  The probability of an object being selected is weight/totalChances,
  where totalChances is the sum of the weights assigned to the objects
  in the hat. Optionally, weights may be specified as fractions using
  the _normalized() functions.

  An user-specified random number generator object may be passed to the hat:
      hat<string, my_rng_type> names( my_rng_instance );
  If not specified, std::rand() is used as the random number generator.


    INTERFACE:
      put(obj, wt)  stores an object in hat
      get()         examines a randomly selected object (with replacement)
      peek()        returns an iterator to a random object
      pull(buf)     removes a randomly selected object  (without replacement)
      get_weight(iter)  
      set_weight(iter, wt)
      total_chances()
      probability(iter)
      erase(iter)   deletes referenced item from hat

      // functions for using fractional weights (0, 1]
      // user must ensure weights sum to 1.0
      put_normalized(obj, float)  
      set_weight_normalized(obj, float)
      get_weight_normalized(iter)
        
    ADDITIONAL PUBLICs:
      clear()
      size()
      begin()
      end()
      empty()
      swap(in)
      max_size()

    PERFORMANCE:
      peek(), put(), and pull() are all logarithmic time complexity, O(logN)
================================================================================
*/
#include <cstdlib>   // rand
#include <vector>    // vector
#include <list>      // list
#include <stdexcept> // out_of_range
#include <functional> // unary_function
#include <cmath>
#include <iterator>

// for code clarity
typedef size_t index_type;
typedef size_t weight_type;
const float NORMALIZE = RAND_MAX; /* TODO: fix this lame hack! */

////////////////////////////////////////////////////////////////////////////////
// std::rand() unary functor


class default_rng_type
{
public:
    weight_type operator()( weight_type range )
    {
        // range validation
        if ( range <= 0 ) return 0;
        
        // handle cases where range doesn't evenly divide RAND_MAX,
        int result;
        int bucket_size = RAND_MAX / range;        
        do {
           result = std::rand() / bucket_size;
        }  while ( (weight_type)result >= range );
        
        return result;
    }    
};

////////////////////////////////////////////////////////////////////////////////
// boost::random adaptor
#ifdef BOOST_RANDOM_HPP
template< class generator_type >
class boost_adaptor{
public:
    // typedef
    typedef unsigned long value_type;
    
    // constructor
    boost_adaptor( generator_type gen ) 
    : generator(gen){};
    
    generator_type generator;

    // main operator() function
    value_type operator()( value_type range )
    {
        return boost::uniform_int<>(0, range-1)(generator);
    }
};    
#endif

////////////////////////////////////////////////////////////////////////////////
// hat container
template <class T, typename random_type = default_rng_type, 
          typename allocator_type = std::allocator<T> >
class hat {
public:

    /////////////////////////////////////////////////////////////////
    // internal iterator class
    class iterator{
    public:
        T& operator*(){return *items_iter; }
        bool operator!=(const iterator other)const
        { return ( (items_iter!=other.items_iter) || (index!=other.index) );}
        bool operator==( const iterator &other )const 
        { return ( (items_iter==other.items_iter) && (index==other.index) );}
        iterator& operator++(){ ++items_iter; ++index; return *this;}
        iterator operator++(int){iterator temp = *this; ++items_iter; ++index; return temp;}
        void operator=(typename std::list<T>::iterator itr){ items_iter = itr; }
        void operator=(const iterator &i){ items_iter = i.items_iter; index = i.index; }

        iterator(){ index = 0; }
        iterator(const iterator &i): items_iter(i.items_iter), index(i.index){}
        iterator( typename std::list<T>::iterator itr, index_type idx)
        :    items_iter(itr), index(idx){}

        typedef typename std::list<T>::iterator list_iterator;
        list_iterator items_iter;
        index_type index; // reverse lookup within tree vector
        
        template <class Iterator>
        struct iterator_traits 
        {
            typedef typename Iterator::iterator_category iterator_category;
            typedef typename Iterator::value_type        value_type;
            typedef typename Iterator::difference_type   difference_type;
            typedef typename Iterator::pointer           pointer;
            typedef typename Iterator::reference         reference;
        };    
    };
    
    /////////////////////////////////////////////////////////////////
    // internal const_iterator class
    class const_iterator{
    public:
        const T& operator*() const {return *items_iter; }
        bool operator!=(const iterator other)const
        { return ( (items_iter!=other.items_iter) || (index!=other.index) );}
        bool operator==( const iterator &other )const 
        { return ( (items_iter==other.items_iter) && (index==other.index) );}
        const_iterator& operator++(){ ++items_iter; ++index; return *this;}
        const_iterator operator++(int){const_iterator temp = *this; ++items_iter; ++index; return temp;}
        void operator=(typename std::list<T>::iterator itr){ items_iter = itr; }
        void operator=(const iterator &i){ items_iter = i.items_iter; index = i.index; }

        const_iterator(){ index = 0; }
        const_iterator(const iterator &i): items_iter(i.items_iter), index(i.index){}
        const_iterator( typename std::list<T>::iterator itr, index_type idx)
        :    items_iter(itr), index(idx){}

        typedef typename std::list<T>::iterator list_iterator;
        list_iterator items_iter;
        index_type index; // reverse lookup within tree vector
        
        template <class Iterator>
        struct iterator_traits 
        {
            typedef typename Iterator::iterator_category iterator_category;
            typedef typename Iterator::value_type        value_type;
            typedef typename Iterator::difference_type   difference_type;
            typedef typename Iterator::pointer           pointer;
            typedef typename Iterator::reference         reference;
        };    
    };

    /////////////////////////////////////////////////////////////////
    // public interface and additional public utility functions
    typedef typename std::list<T, allocator_type>::value_type  value_type;
    typedef typename std::list<T, allocator_type>::size_type   size_type;
    typedef allocator_type allocator;

    // modifying functions
    T&       get();
    void     swap(T in) { items.swap(in); }
    void     put( T const & in_object, const weight_type& in_weight = 1 );
    void     put_normalized( T const & in_object, const float & normalized_weight );
    T&       pull( T& );
    void     erase( iterator );
    void     set_weight( iterator, const weight_type& );
    void     set_weight_normalized( iterator, const float& normalized_weight ); 
    void     clear(){ tree.clear(); items.clear(); }

    // constant (non-modifying) queries
    size_t      total_chances() const { return sum_chances; };
    size_type   size() const { return items.size(); }
    size_type   max_size() const { return items.max_size(); }
    bool        empty() const { return items.empty(); }

    weight_type get_weight( iterator i ) const{return tree[i.index].weight;};
    float       probability( iterator i ) const
    {  return float( tree[i.index].weight ) / float( sum_chances ); }; 
    float       get_weight_normalized( iterator i ) const
    {  return (float)tree[i.index].weight / NORMALIZE; };
    iterator peek();
    iterator begin(){ return iterator(items.begin(),0); };
    iterator end(){   return iterator(items.end(), items.size()); };

    weight_type get_weight( const_iterator i ) const{return tree[i.index].weight;};
    float  probability( const_iterator i ) const
    { return float( tree[i.index].weight ) / float( sum_chances ); }; 
    float  get_weight_normalized( const_iterator i ) const
    {  return (float)tree[i.index].weight / NORMALIZE; }
    const_iterator begin() const { return const_iterator( items.const_begin(), 0); };
    const_iterator end()   const { return const_iterator( items.const_end(),   0); };
    
    // constructors, destructor, assignment
	hat() {
		default_rng_type std_rand;
		random = std_rand;
		sum_chances = 0;
	}
    hat( random_type rng_instance)
    :    random(rng_instance){ sum_chances = 0;}
    hat( const  hat& other );
    ~hat(){ clear(); }
    hat<T, random_type, allocator_type >& operator=( 
       const hat<T,random_type, allocator_type>& );
        
protected:
    random_type random;
    /////////////////////////////////////////////////////////////////
    // internal node class
    class node
    {
    public:
        iterator item;         // points into items storage container
        weight_type weight;
        weight_type family_weight;  // sum of my weight + children (if any)
        node( iterator in, weight_type in_weight = 1)
        :   item(in), weight(in_weight), family_weight(in_weight){};       
    };

    std::vector< node, allocator_type > tree;
    std::list<T, allocator_type > items;
    weight_type sum_chances;
    index_type find_index( index_type index, weight_type target );
    void   update_weights( index_type index );
    
};


////////////////////////////////////////////////////////////////////////////////
// put
template <class T, typename random_type, typename allocator_type>
void hat<T, random_type, allocator_type>::put( 
   T const & in, const weight_type& in_weight )
{
    items.push_back( in );
    
    iterator iter( --items.end(), items.size()-1);
    
    weight_type weight = in_weight;
    if(weight < 1) weight = 1; // IS THIS MISUSE?
    sum_chances += weight;
    
    node new_node( iter, weight );
    
    tree.push_back( new_node );
    
    update_weights( tree.size() - 1 );
    
    tree[0].item = items.begin(); // reset root pointer
    tree[0].item.index = 0;
};

////////////////////////////////////////////////////////////////////////////////
// put_normalized float&
template <class T, typename random_type, typename allocator_type>
void hat<T, random_type, allocator_type>::put_normalized( 
   T const & in, const float & in_weight )
{   
    items.push_back( in );
    
    iterator iter( --items.end(), items.size()-1);
    
    float normalized = in_weight;
    if(normalized <= 0) normalized = 1.0/NORMALIZE;
    weight_type chances = int( normalized * NORMALIZE );
    
    sum_chances += chances;
    
    node new_node( iter, chances );
    
    tree.push_back( new_node );
    
    update_weights( tree.size() - 1 );
    
    tree[0].item = items.begin(); // reset root pointer
    tree[0].item.index = 0;
};

////////////////////////////////////////////////////////////////////////////////
// get()
template <class T, typename random_type, typename allocator_type>
T& hat<T, random_type, allocator_type>::get()
{
    weight_type target = random( tree[0].family_weight );
    index_type location = find_index( 0, target );
    return *tree[ location ].item;
};

////////////////////////////////////////////////////////////////////////////////
// peek
template <class T, typename random_type, typename allocator_type>
typename hat<T, random_type, allocator_type>::iterator 
hat<T, random_type, allocator_type>::peek()
{
    weight_type random_weight = random( tree[0].family_weight );
    return tree[ find_index(0, random_weight) ].item;
};

////////////////////////////////////////////////////////////////////////////////
// pull
template <class T, typename random_type, typename allocator_type>
T& hat<T, random_type, allocator_type>::pull( T& storage )
{
    // get random item
    weight_type random_weight = random( tree[0].family_weight );
    index_type index = find_index( 0, random_weight );
    
    storage = *tree[index].item;

    erase( tree[index].item );
    return storage;
};

////////////////////////////////////////////////////////////////////////////////
// update_weights
template <class T, typename random_type, typename allocator_type>
void hat<T, random_type, allocator_type>::update_weights( index_type index )
{
    // subscript validation
    if( index >= tree.size() ){
        throw std::out_of_range("hat::update_weights() invalid index");
    }

    // update the weight of the currently indexed node
    tree[index].family_weight =  tree[index].weight;

    // daughters are found at 2(x+1)
    if( 2 * index + 2/*daughter(index)*/ < tree.size() )
    {   // add daughter
        tree[index].family_weight += tree[ 2 * index + 2/*daughter(index)*/ ].family_weight;
    } // else no daughter

    // sons are found at 2x+1
    if( 2 * index + 1/*son(index)*/ < tree.size() )
    {   // add son
        tree[index].family_weight += tree[ 2 * index + 1/*son(index)*/ ].family_weight;
    } // else no son

    // check for parents, and update them
    if( index > 0 ) // not the root mother (has parent)
    {
        // calculate distance to parent
        if( index % 2 ) { // is a male node
            update_weights( (index-1)/2 ); // update parent
        }
        else { // is a female node
            update_weights( index/2 - 1 ); // update parent
        }
    } // else this is the root mother (no parents)
};

////////////////////////////////////////////////////////////////////////////////
// find_index
template <class T, typename random_type, typename allocator_type>
index_type hat<T, random_type, allocator_type>::find_index( 
  index_type index, weight_type target )
{
    // subscript validation
    if( index >= tree.size() ){
        throw std::out_of_range("hat::find_index() invalid index");
    }
    // weight validation
    if( target > tree[index].family_weight )
    {
        throw std::out_of_range("hat::find_index() weight outside tree");
    }

    // divide weight into three groups: Self, Son & Daughter
    weight_type sum = tree[index].weight;
    if( target < sum )
    {
        return index;
    } // else outside myself, must be in children

    // check son (first-born is always a son)
    sum += tree[ 2*index+1/*son(index)*/ ].family_weight;
    if( target < sum )
    {
        return find_index( 2*index+1/*son(index)*/, target - tree[index].weight );
    } // else not in son's range

    // check daughter
    sum += tree[ 2*index+2/*daughter(index)*/ ].family_weight;
    if( target < sum )
    {
        return find_index( 2*index+2/*daughter(index)*/,
            target - tree[index].weight - tree[ 2 * index + 1/*son(index)*/ ].family_weight );
    }
    
    // Possible if a custom random generator fails to deliver [0, range)
    throw std::out_of_range("hat::find_index() bad index/weight pair");
};

////////////////////////////////////////////////////////////////////////////////
// set_weight
template <class T, typename random_type, typename allocator_type>
void hat<T, random_type, allocator_type>::set_weight( 
  iterator iter, const weight_type& in_weight )
{
    weight_type weight = in_weight;
    if( weight < 1 ) weight = 1;
    sum_chances = sum_chances - tree[iter.index].weight + weight;

    tree[iter.index].weight = weight;
    update_weights( iter.index );
};    

////////////////////////////////////////////////////////////////////////////////
// set_weight_normalized
template <class T, typename random_type, typename allocator_type>
void hat<T, random_type, allocator_type>::set_weight_normalized( 
  iterator iter, const float& in_weight )
{
    float normalized = in_weight;
    if( normalized <= 0 ){
        normalized = 1.0 / NORMALIZE;
    }
    int chances = int( normalized * NORMALIZE );    
    sum_chances = sum_chances - tree[iter.index].weight + chances;

    tree[iter.index].weight = chances;
    update_weights( iter.index );
};    

////////////////////////////////////////////////////////////////////////////////
// erase 
template <class T, typename random_type, typename allocator_type>
void hat<T, random_type, allocator_type>::erase( iterator iter )
{    
    index_type index = iter.index;
    items.erase( iter.iter );

    // copy last node over this node, then update the weights
    tree[index] = tree[ tree.size() - 1 ];
    tree[index].item.index = index;
    update_weights(index);

    // delete last node, then update those weights
    sum_chances -= tree[ iter.index ].weight;
    tree.pop_back();
    

    if( tree.empty() ) return;

    // calculate distance to previous parent
    if( tree.size() % 2 ) { // was a male node
        update_weights( (tree.size()-1)/2 ); // update parent
    }
    else { // was a female node
        update_weights( tree.size()/2 - 1 ); // update parent
    }
};    

////////////////////////////////////////////////////////////////////////////////
// copy constructor
template <class T, typename random_type, typename allocator_type>
hat<T, random_type, allocator_type>::hat( 
  const hat<T, random_type, allocator_type> &in )
{
    tree = in.tree;    // vector of nodes with iterators to be updated
    items = in.items;  // list of value_type items
    sum_chances = in.sum_chances;
    
    // point iterators at new list of items
    index_type index = 0;
    for( typename std::list<T, allocator_type>::iterator i = items.begin(); 
         i != items.end(); ++i )
    {
        tree[index++].item = i;
    }    
}

////////////////////////////////////////////////////////////////////////////////
// assignment operator
template <class T, typename random_type, typename allocator_type>
hat<T, random_type, allocator_type>& 
hat<T, random_type, allocator_type>::operator=( 
  const hat<T, random_type, allocator_type> &in )
{
    /* todo: return on self-assignment */
        
    tree  = in.tree;   // vector of nodes with iterators to be updated
    items = in.items;  // list of value_type items
    sum_chances = in.sum_chances;
    
    // point iterators at new list of items
    index_type index = 0;
    for( typename std::list<T, allocator_type>::iterator i = items.begin(); 
         i != items.end(); ++i )
    {
        tree[index++].item = i;
    }
}

