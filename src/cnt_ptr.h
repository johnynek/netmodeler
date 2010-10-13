#ifndef starsky__cnt_ptr_h
#define starsky__cnt_ptr_h

/**
 * This is a simple (pretty standard) template for a reference
 * counted pointer.  This allows you to treat the cnt_ptr like
 * a normal pointer (with * and ->) but not worry about deleting
 * it when it is no longer in use.
 *
 * Reference counting is not a cure all of course.  Keep in mind
 * that circular references can still cause memory leaks.
 */

namespace Starsky {

template<typename T>
class cnt_ptr {
  public: 
    cnt_ptr<T>(T* ptr = 0) {
      setPtr(ptr);
    }
    
    //Here is the destructor
    ~cnt_ptr() {
      decrement();
    }

    //Here is the copy constructor:
    cnt_ptr<T>(const cnt_ptr<T>& cp) {
      setAndInc(cp);
    }
    //Here is the assignment operator:
    cnt_ptr<T>& operator=(const cnt_ptr<T>& cp) {
      if( _ptr != cp._ptr ) {
        //This is not us:
        decrement();
        setAndInc(cp);
      }
      else {
        //Do nothing: x = x
      }
      return *this;
    }
    //Here is assignment from a pointer:
    cnt_ptr<T>& operator=(T* ptr) {
      if( _ptr != ptr ) {
        //This is not us:
        decrement();
	setPtr(ptr);
      }
      else {
        //Do nothing: x = x
      }
      return *this;
    }
    
    bool operator==(T* ptr) const {
      return (_ptr == ptr);
    }
    bool operator!=(T* ptr) const {
      return (_ptr != ptr);
    }
    //This should work even when S is a subtype of T or vice-versa
    template<typename S>
    bool operator==(const cnt_ptr<S>& cp) const {
      return (_ptr == cp._ptr );
    }
    //This should work even when S is a subtype of T or vice-versa
    template<typename S>
    bool operator!=(const cnt_ptr<S>& cp) const {
      return (_ptr != cp._ptr );
    }
    //This should work even when S is a subtype of T or vice-versa
    template<typename S>
    bool operator<(const cnt_ptr<S>& cp) const {
      return (_ptr < cp._ptr );
    }
    bool operator<(T* p) const {
      return (_ptr < p);
    }

    T* operator->() const { return _ptr; }
    T& operator*() const { return *_ptr; }

    /**
     * Sometimes we want to do a dynamic cast.  This
     * method does so without messing up the counts
     */
    template<typename S>
    cnt_ptr<S> dyn_cast() const {
      S* new_p = dynamic_cast<S*>(_ptr);
      cnt_ptr<S> ncp;
      ncp.setPtr(new_p, _count);
      return ncp;
    }
	    
    /**
     * This is a dangerous method.  You get access
     * to the underlying pointer.  If you delete it,
     * expect bad things.  Also, if all the cnt_ptr
     * objects go out of scope, expect them to delete
     * the memory you have a pointer to.  If you plan
     * to keep it longer and take ownership, then
     * do cnt_ptr<T>::get() followed by cnt_ptr<T>::disown()
     */
    T* const & get() const {
      return _ptr;
    }
    /**
     * This method tells all cnt_ptr objects managing
     * this pointer, to stop counting.  This is fairly
     * dangerous: if you delete the pointer before the
     * other cnt_ptr stop using it, you will likely get
     * a segfault.  If you don't delete the pointer you
     * will get a memory leak.
     */
    void disown() {
      //Don't delete when in deconstructor.
      if( *_count > 0 ) {
        ( *_count) = -(*_count);
      }
    }

    /**
     * It is common to check to see if a pointer is null,
     * this does so
     */
    bool isNull() const { return ( _ptr == 0 ); }
	
    /*
     * Never use this method outside this class!  It should
     * be protected, but it causes problems for the dyn_cast
     * method.  Seriously, if you call this method, I hope your
     * code explodes, rides your motorcycle, and drinks all your
     * milk.
     *
     * Used in the standard constructor and operator=(T*)
     * @param ptr the pointer to count
     * @param c the pointer to the count for this pointer.
     */
    void setPtr(T* ptr, int* c  = 0) {
      if( ptr != 0 ) {
	if( c == 0 ) {
          _count = new int;
          (*_count) = 1;
	}
	else {
          _count = c;
	  if( (*_count) > 0 ) {
            //Increment the count:
	    (*_count)++;
	  }
	  else if( (*_count) < 0 ) {
            /*
	     * We are not counting ptr anymore, but we are waiting to delete _count:
	     * We count negatively to zero.  When we hit zero, delete _count.
	     */
	    (*_count)--;
	  }
	  else {
            //This really shouldn't happen.
	  }
	}
      }
      else {
	//There is no meaningful count here:
        _count = 0;
      }
      _ptr = ptr;
    }

  protected:
    void decrement() {
      if( _count != 0 ) {
        if( *_count > 1 ) {
          //There are many counts, just decrement:
	  (*_count)--;
        }
        else if ( *_count == 1 ) {
          //This is the last ptr:
	  if( _ptr != 0 ) {
	    delete _ptr;
	    _ptr = 0;
	    //std::cerr << "delete" << std::endl;
	  }
	  delete _count;
	  _count = 0;
	}
	else if ( *_count < 0 ) {
          //We have disowned the pointer, but we must
	  //eventually delete the count:
	  (*_count)++;
	  /*
	   * Check to see if it is count deletion time:
	   */
	  if( *_count == 0 ) {
            delete _count;
	    _count = 0;
	  }
	}
      }
    }

    //Used in copy constructor and operator=
    void setAndInc(const cnt_ptr<T>& cp) {
      setPtr(cp._ptr, cp._count);
    }
   
    /*
     * Here is the data storage
     */
    T* _ptr;
    int* _count;
};
	
}

#endif
