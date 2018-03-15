#include "m_algorithm.h"

// for search min by horizontal and vertical because there is pointers with null
bool ALG::fun_min( double *  a, double * b)

{
 return  ( !a && !b ) ?  false : ( a ? ( b ? (*a < *b) : true ) : false );
}

bool fun_min_CNode(ALG::CNode * ptr_l , ALG::CNode * ptr_r)
{
    if(ptr_l && ptr_r)
      return *ptr_l < *ptr_r;
    else
      return false;
}

// constructor copy
ALG::CNode::CNode(const ALG::CNode & obj) throw(std::bad_alloc) :
    Matrix(new M_ARRAY() ),
    Min(obj.Min),
    way(obj.way)
{
     // copy Matrix
     for(auto i : * (obj.Matrix) )
      {
      std::vector<double*> row;
      for( auto s : i )
      {
       if(s)
        {
           decltype(s) ptr = nullptr;
               try
               {
                    ptr = new double( *s );
               }catch(const std::bad_alloc & ex )
                  {
                    this->destroy_matrix();
                    throw ;
                  }
        row.push_back( ptr );
        }
       else
       row.push_back( nullptr );
      }
      Matrix->push_back( std::move(row) );
      }

}


ALG::CNode::~CNode()
{
    // delete all value at table
    this->destroy_matrix();

}

bool ALG::CNode::empty() const
{
    return  !Matrix ;
}

bool ALG::CNode::operator <( const ALG::CNode & obj ) throw(std::invalid_argument)
{
  if ( this && &obj )
     {
        if( this == &obj)
            return false;

      return this->Min < obj.Min ;
      }

    throw ( std::invalid_argument(  msg ) );

}

bool ALG::CNode::operator ==( const ALG::CNode & obj) throw(std::invalid_argument)
{

    if ( this && &obj )
       return  this->Min == obj.Min ;
    else
       throw ( std::invalid_argument( msg ) );

}

void ALG::CNode::processing_min()
{
  Min  += processing_min_horizontal() + processing_min_vertical();
}




ALG::uint_ ALG::CNode::processing_min_horizontal()
{
    auto iter_begin = ++( Matrix->begin());
    auto iter_end   =     Matrix->end();
    uint_ sum_all_min {};

    for(  ;  iter_begin != iter_end  ; ++iter_begin )
    {
        auto & ref_row = *iter_begin;
        auto iter_row_begin = ++ ref_row.begin();
        auto iter_row_end   =    ref_row.end();

        auto iter_min = std::min_element( iter_row_begin, iter_row_end, fun_min);
        if( *iter_min )
        {
         auto value = **iter_min;

         for( ; iter_row_begin != iter_row_end ; ++iter_row_begin )
         {
            if(*iter_row_begin )
            **iter_row_begin -= value;
         }
         sum_all_min += value;
        }
    }
    return sum_all_min;
}

ALG::uint_ ALG::CNode::processing_min_vertical()
{
    auto iter_Matrix_begin = ++Matrix->begin();
    auto iter_Matrix_end   =   Matrix->end();

    double * min(nullptr) ;

    auto size_row = (*iter_Matrix_begin).size();
    uint_ sum_all_min(0);
    for( decltype(size_row) i{1}; i < size_row ; ++i )
    {
      auto iter = iter_Matrix_begin;
      min = *((*iter++).begin() + i);
      // find min value of column
      for ( ; iter != iter_Matrix_end ; ++iter)
       {
          auto p = *((*iter).begin() + i );
         if(  fun_min( p , min ) )
             min = p;
       }

      // values of column minus min
      if(min && *min ) // min != null and *min != 0
      {
      auto value_min = *min;
        for ( iter = iter_Matrix_begin; iter != iter_Matrix_end ; ++iter)
          {
             auto p = *((*iter).begin() + i );
            if( p )
             *p -= value_min;
          }
        sum_all_min += value_min;
      }


    }
    return sum_all_min;
}

std::vector<ALG::CNode*> ALG::CNode::find_min_transition_build_next_nodes()
{
    std::vector<ALG::CNode*> Nodes_array;
   if( Matrix )
   {
     auto arr = get_array_values();
     find_min_for_arr_values( arr );

     auto iter_begin_arr = arr.begin();
     auto iter_end_arr = arr.end();
     // find max transaction
     auto iter_value_max = std::max_element( iter_begin_arr, iter_end_arr);

     // find all equal transactions
    iter_begin_arr = arr.begin();
    for( ;iter_begin_arr != iter_end_arr ; )
    {
         iter_begin_arr = std::find_if( iter_begin_arr,
                                            arr.end(),
                                       [&iter_value_max]( ALG::X_Y & val)
                                       { return (*iter_value_max).value == val.value; }
                                      );
        if(iter_begin_arr != iter_end_arr)
         {

         // add new two nodes left and right
         auto vec = build_left_and_right_node( (*iter_begin_arr).x, (*iter_begin_arr).y);
          for( auto * i : vec)
          {
           i->processing_min();

           if( i->Matrix->size() == 3)
            {
               i->set_two_last_steps();
               i->destroy_matrix();
            }
           Nodes_array.push_back( i );
          }
         ++iter_begin_arr;
        }
    }

   }
   return Nodes_array;
}

std::vector<ALG::X_Y>  ALG::CNode::get_array_values() const
{
    std::vector<ALG::X_Y> array_null_points;

  if( Matrix->size() != 1 )
  {
    //auto size_matrix = Matrix->size();
    //auto size_matrix_row = (*Matrix)[0].size();
    // find all values witch have 0 and their positions
    auto iter_row_beg = ++( Matrix->begin() ), // ++ because first name column
         iter_row_end =     Matrix->end() ;

    for( ; iter_row_beg != iter_row_end ; ++iter_row_beg)
    {
      auto iter     = ++(*iter_row_beg).begin() ,
           iter_end = (*iter_row_beg).end();

      for(  ; iter != iter_end ; ++iter)
      {
          if( ! (*iter) )
              continue;
          else if(  !(**iter) ) // true if == 0
          {
             ALG::uint_ x, y;
             auto step = std::distance( (*iter_row_beg).begin(), iter );
             auto val = (*( Matrix->begin() )).begin() + step;
             x = static_cast<ALG::uint_ > (**val );
             y = static_cast<ALG::uint_ > ( **( (*iter_row_beg).begin() ) );
             array_null_points.push_back( ALG::X_Y{ x, y , 0.0 } );
          }
      }

    }
 }
    return  array_null_points;
}

void ALG::CNode::find_min_for_arr_values( std::vector<ALG::X_Y> & obj)const
{

auto fun = [](double ** min, double * value )
{
    if(value)
    {
     if( !(*min) )
       *min = new double(*value);
     else if( **min > *value)
      **min = *value;
    }
};

    for( auto & i : obj )
    {
    double * min_column(nullptr),* min_row(nullptr);
    auto iter_colum = find_iter_name_column(i.x);
    auto iter_row   = find_iter_name_row(i.y);

    auto distance = std::distance( (*Matrix->begin()).begin(), iter_colum ) ;
    //min by vertical
    for( auto row_beg = ++Matrix->begin(), end = Matrix->end();  row_beg != end; ++row_beg)
    {
       auto it = (*row_beg).begin();
       if(  **it  != i.y )
       {
       double * ptr =  *( it + distance);
       fun( &min_column , ptr );
       }
    }
    // min by horizontal

    auto iter_x = (*iter_row).begin() + distance;
    for( auto iter = ++(*iter_row).begin(), end = (*iter_row).end(); iter != end; ++iter )
    {
        if(iter != iter_x )
        fun( &min_row, *iter);
    }
      // min can be nulll
      i.value += min_row ? *min_row : decltype(i.value)(0);
      i.value += min_column ? *min_column : decltype(i.value)(0);

      delete min_column;
      delete min_row;
    }
}

std::vector<ALG::CNode *> ALG::CNode::build_left_and_right_node( ALG::uint_ x, ALG::uint_ y) const
{
    // right node
    // we delete value Matrix[y][x] because step from y into x no allowed and set null_ptr
     std::vector<ALG::CNode *> value_return;
     value_return.push_back( new ALG::CNode());

     auto & row_name_column = *( this->Matrix->begin() );
     // find position name column witch is equal x
     auto iter = find_iter_name_column(x);

     auto distance = std::distance( row_name_column.begin() , iter );
     // copy Matrix
     for(auto i : * (this->Matrix) )
      {
      // row witch name is equal y we missing
      if( *( i.front())  == y )
        continue;

      std::vector<double*> row;
      for( auto s : i )
      {
       if(s)
        row.push_back( new double( *s ));
       else
        row.push_back( nullptr );
      }
      // value matrix[x][y] we delete and set null
      if( *(row.front()) == x )
      {

         auto iter_y = find_iter_name_column(y);
         if(  row_name_column.end()  != iter_y )
         {
         auto iter = row.begin() + std::distance( row_name_column.begin(), iter_y);
         delete  *iter;
         *iter = nullptr;
         }
      }
     // delete column x witch is equal distance
      auto column_for_delete =  row.begin() +  distance;
      delete  *column_for_delete;
      row.erase( column_for_delete );

      value_return.back()->Matrix->push_back( std::move(row) );
      }

      value_return.back()->way = this->way;  // copy
      value_return.back()->way.push_back( ALG::S_Way{ x, y } ); // add new value
      value_return.back()->Min = this->Min; // copy

      value_return.push_back( new ALG::CNode());

      for(auto i : * (this->Matrix) )
        {

       std::vector<double*> row;
       for( auto s : i )
       {
        if(s)
         row.push_back( new double( *s ));
        else
        row.push_back( nullptr );
       }

       value_return.back()->Matrix->push_back( std::move(row) );

      }

      auto iter_2 = value_return.back()->find_iter_name_row(y);


      auto ptr  = iter_2->begin() + distance;
      delete *ptr;
      *ptr = nullptr;

      value_return.back()->way = this->way; // copy
      value_return.back()->Min = this->Min; // copy

      return value_return;
}

void ALG::CNode::set_two_last_steps()
{

    // Matrix == 2
   auto iter_row = Matrix->begin();
   auto & row_first  =  *iter_row;
   auto & row_second =  *(++iter_row);
   auto & row_third  =  *(++iter_row);
   // colum name 0x/y  x1  x2
   // row        y1    v1  v2
   // row        y2    v3  v4

   if( *(++row_second.begin()) && row_third.back())   // if( v1 && v4)
   {

      way.push_back( ALG::S_Way{  **(++row_first.begin()), * row_second.front() } );
      way.push_back( ALG::S_Way{ *row_first.back(),* row_third.front() } );
   }else  // if( v3 && v2)
   {
       way.push_back( ALG::S_Way{  *row_first.back(),* row_second.front() } );
       way.push_back( ALG::S_Way{ **(++row_first.begin()), * row_third.front() } );
   }

}

void ALG::CNode::destroy_matrix()
{

    if(Matrix)
    {
      for( auto & s : *Matrix )
         for( auto * p : s)
             delete p;

     delete Matrix;
     Matrix = nullptr;
    }
}

const std::vector<double *>::iterator ALG::CNode::find_iter_name_column(ALG::uint_ x) const
{
   auto iter_ret = std::find_if( (*Matrix->begin()).begin(),
                                 (*Matrix->begin()).end(),
   [&x]( double * ptr ){  return ptr ? *ptr == x : false ;}

               );
   return iter_ret;
}

const std::vector< std::vector<double*> >::iterator ALG::CNode::find_iter_name_row(ALG::uint_ y) const
{
  auto iter_ret = std::find_if( Matrix->begin(),
                                Matrix->end(),
  [&y]( std::vector<double*> & row){ return *row.front()== y ;  }
          );
  return iter_ret;

}



bool ALG::C_Commis_Voyageur::first_result()
{
    if ( m_stack_nodes.empty() )
     return false;

    next_node();

    while(  ! m_new_modes.empty()  )
    {
     // find min_way of node

     auto iter_min =  std::min_element( m_new_modes.begin(),m_new_modes.end(), &fun_min_CNode );

     auto iter_element = m_new_modes.begin();
     auto iter_end = m_new_modes.end();

       for( ; iter_element != iter_end ; ++iter_element)
       {
         if( iter_element == iter_min )
             continue;
         m_stack_nodes.push( *iter_element );
       }

     ALG::CNode *  ptr = *iter_min;  // because when i call find... iter dont valid

     this->m_new_modes.clear(); // all pointers copy at stack and one min we set result

     if(  !ptr->empty() )
          this->m_new_modes = ptr->find_min_transition_build_next_nodes();
     else
          std::swap( result, ptr); // result  is nullptr

      delete ptr;
     }

    return true;
}

bool ALG::C_Commis_Voyageur::improve_result()
{

 bool improve = false;

    while( next_node() )
    {
        while( !m_new_modes.empty())
        {
          auto * ptr =  m_new_modes.back();


           if( *ptr < *result)
           {
            if( !ptr->empty() )
             {
             m_new_modes = ptr->find_min_transition_build_next_nodes();
               for(auto p : m_new_modes)
                   m_stack_nodes.push(p);
            }
            else if(*ptr < *result)
            {
                std::swap(result,ptr);
                improve = !improve;
            }


           }
           m_new_modes.clear();
         delete ptr;
        }
    }
    return improve;
}

bool ALG::C_Commis_Voyageur::next_node()
{
    if( !m_stack_nodes.empty() )
    {
     m_new_modes.push_back(   m_stack_nodes.top() );
     m_stack_nodes.pop();
     return true;
    }
    return false;
}

std::vector<ALG::S_Way> ALG::C_Commis_Voyageur::result_way() const
{
  return result->r_way();
}





ALG::C_Commis_Voyageur::~C_Commis_Voyageur()
{
     delete result;

     while( !m_stack_nodes.empty() )
     {
      delete m_stack_nodes.top();
      m_stack_nodes.pop();
     }
}


