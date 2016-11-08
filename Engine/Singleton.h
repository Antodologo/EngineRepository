#ifndef SINGLETON_H
#define SINGLETON_H

// For asserts
#include "Headers.h"

/**
Class Singleton
Base class to create singletons controlling construction and destruction of singleton
*/
template < class T >
class Singleton
{
  /**********************************************************************************************************************/
  // ASSOCIATIONS
  /**********************************************************************************************************************/

  /**********************************************************************************************************************/
  // CONSTANTS
  /**********************************************************************************************************************/

  /**********************************************************************************************************************/
  // TYPES
  /**********************************************************************************************************************/

  /**********************************************************************************************************************/
  // METHODS
  /**********************************************************************************************************************/

public:

  /**
  Constructor
  */
  Singleton( void ) { }

  /**
  Destructor
  */
  ~Singleton( void ) { }

  /**
  Singleton creation
  */
  inline void static CreateSingleton( void )
  {
    // Create singleton. Assert if singleton already exists
    if( !mSingleton ){
      mSingleton = new T();
    }
    else{
      ShowAssertForSingleton( "Singleton already created. FIX IMMEDIATELY" );
    }
  }

  /**
  Singleton destruction
  */
  inline void static DestroySingleton( void )
  {
    // Destroy singleton. Assert if singleton is already destroyed or never created
    if( mSingleton ){
      // Inline safe delete
      delete mSingleton;
      mSingleton = NULL;
    }
    else{
      ShowAssertForSingleton( "Singleton already destroyed or never created. FIX IMMEDIATELY" );
    }
  }

  /**
  Returns a reference to the singleton
  @return Singleton reference
  */
  inline static T &GetInstance( void )
  {
    return *mSingleton;
  }

  /**
  Returns a pointer to the singleton
  @return Singleton pointer
  */
  inline static T *GetInstancePtr( void )
  {
    return mSingleton;
  }

private:

  /**
  Shows and assert for an error found in assert management
  This function should be reimplemented for every platform with the desired assert behaviour
  @param windowMessage Message to show to the user to identify the error
  */
  inline static void ShowAssertForSingleton( const std::string &windowMessage )
  {
    MessageBox( NULL, windowMessage.c_str(), "Assertion failed in Singleton", MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
  }

  /**********************************************************************************************************************/
  // ATTRIBUTES
  /**********************************************************************************************************************/

  static T *mSingleton; ///< Singleton instance

};

/**********************************************************************************************************************/

//Singleton
template < class T >
T *Singleton<T>::mSingleton = NULL;

/**********************************************************************************************************************/

#endif