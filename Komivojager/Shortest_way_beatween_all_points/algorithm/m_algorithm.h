#ifndef _ALGORITHM_
#define _ALGORITHM_


#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stack>
#include <stdexcept>
#include <assert.h>
//                  Matrix
//                               column name
// row name vector<double*>      [    0     ]   1       2     3      ...
//          vector<double*>      [    1     ] nullptr value  value   ...
//          vector<double*>      [    2     ] values nullptr value   ...
// diagonal is nullptr

#define msg std::string("ERROR File Name ") + std::string(__FILE__) + std::string(" Line ") + std::to_string(__LINE__)


namespace ALG
{

typedef u_int64_t uint_; // size fields for size path (2 fields),transition (2 fields)
typedef std::vector< std::vector<double * > > M_ARRAY;
template<typename T>
inline  T return_value( const T * ob )
{
 return *ob;
}


template<typename T>
inline  T return_value( const T & ob )
{
 return ob;
}


// for search min by horizontal and vertical because there is pointers with null
bool fun_min( double *  a, double * b);
// for array 0 values and their positions
struct X_Y
{
 uint_ x;
 uint_ y;
 double value;
 bool operator<( const X_Y & value ) const
 {
     return this->value < value.value;
 }
};

// step for transaction
struct S_Way
{
 uint_ m_x;
 uint_ m_y;
 bool operator<(const S_Way & obj)
 {
     return m_x < obj.m_y;
 }
};

class CNode
{
    M_ARRAY * Matrix;        // table ways
    uint_ Min;               // length
    std::vector<S_Way> way;  // transitions

    void processing_min();
    uint_ processing_min_horizontal();
    uint_ processing_min_vertical();



    std::vector<X_Y>   get_array_values() const;
    void  find_min_for_arr_values( std::vector<X_Y> & obj) const;

    std::vector<ALG::CNode*> build_left_and_right_node( ALG::uint_ x, ALG::uint_ y )   const;
    void set_two_last_steps();
    void destroy_matrix();
    const std::vector< double * >::iterator      find_iter_name_column( ALG::uint_ x)   const ;
    const std::vector< std::vector<double*> >::iterator find_iter_name_row( ALG::uint_ y)      const ;

    CNode() throw(std::bad_alloc) : Matrix( new M_ARRAY), Min(), way() { }
public:
    template< typename T>
    explicit CNode(const T & ar )  throw(std::bad_alloc) ;
    CNode(const CNode & )          throw(std::bad_alloc) ;
    CNode( CNode &&) = delete;
    CNode & operator=(const CNode &) = delete;
    CNode & operator=( CNode &&) = delete;
   ~CNode();

    bool empty() const ;
    std::vector<ALG::CNode*>  find_min_transition_build_next_nodes();

    const std::vector<ALG::S_Way> r_way() const { return way;}

   bool operator <( const ALG::CNode & obj ) throw(std::invalid_argument) ;

   bool operator ==( const ALG::CNode & obj) throw(std::invalid_argument) ;

};

template<typename T>
ALG::CNode::CNode(const T & ar)  throw(std::bad_alloc)  :
    Matrix(new M_ARRAY() ),
    Min()
{

    //copy ar and add  fill  name colums and row
    std::vector<double*> numbers_columns( std::distance(ar.begin(),ar.end()) + 1); // 1 names rows
    uint_ count = -1; // first plus will be 0
     for(auto & i : numbers_columns)
     {
        i = new double(++count);
     }
    Matrix->push_back(numbers_columns);
    count = 0;

    for(auto & i : ar)
     {
        std::vector<double*> row;
        row.push_back(new double(++count));
        for( auto s : i)
           {
            if(s)
             row.push_back( new double( return_value(s)) );
            else
            row.push_back( nullptr );

           }
        Matrix->push_back( std::move ( row ) );
     }

    this->processing_min();
}








class C_Commis_Voyageur
{
 std::stack  < ALG::CNode * > m_stack_nodes;
 std::vector < ALG::CNode * > m_new_modes;
 ALG::CNode * result;

bool next_node();
 public:
std::vector<ALG::S_Way> result_way() const;
bool first_result();
bool improve_result();

template <typename T>
 C_Commis_Voyageur( const T & ar) : result(nullptr)
{
  m_stack_nodes.push( new ALG::CNode(ar) );
}
~C_Commis_Voyageur();



};

} // namespace ALG

#undef mes // message

#endif
