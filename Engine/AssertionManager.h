#ifndef ASSERTIONMANAGER_H
#define ASSERTIONMANAGER_H

#include "Singleton.h"

/**
Assertion manager class
This class implements everything that has to do with assertions
*/
class AssertionManager : public Singleton <AssertionManager>
{
  /**********************************************************************************************************************/
  // ASSOCIATIONS
  /**********************************************************************************************************************/

  // Allow constructor calling only from Singleton
  friend class Singleton <AssertionManager>;

  /**********************************************************************************************************************/
  // CONSTANTS
  /**********************************************************************************************************************/

  /**********************************************************************************************************************/
  // TYPES
  /**********************************************************************************************************************/
public:

  typedef bool (*HandlerFunction)( const char* condition, const char* message, const char* file, const int line );

private:

  /**********************************************************************************************************************/

  /**
  Assert handler class
  Handler for asserts
  */
  class AssertHandler
  {
    // METHODS
  public:
    /**
    Constructor
    */
    AssertHandler( void )
      : mHandlerFunction(&AssertHandler::DefaultHandler) { }

    /**
    Default handler for assertions
    @param condition Condition that triggered the assertion
    @param message Assert message to display
    @param file File that triggered the assertion
    @param line Line of the file that triggered the assertion
    @return True if execution must be halted on assert call
    */
    static bool DefaultHandler(const char* condition, const char* message, const char* file, const int line);

    /**
    Set and get for assertion handler function
    */
    inline HandlerFunction GetHandlerFunction( void ){
      return mHandlerFunction;
    }
    inline void SetHandlerFunction( HandlerFunction newHandler){
      mHandlerFunction = newHandler;
    }

    /**
    Logs assert into log file
    @param assertMessage Assert message to log
    */
    static void LogAssert( const std::string &assertMessage, const char* file, const int line );

    /**
    Shows a window with the assert and the possible actions to take and returns true if execution must be halted
    @param windowMessage Assert message to display in window
    @return True if user selected to halt execution
    */
    static bool ShowAssertWindow( const std::string &windowMessage );

  private:
    // ATTRIBUTES
    HandlerFunction mHandlerFunction;   ///< Assertion handler
  };

  /**********************************************************************************************************************/

  /**********************************************************************************************************************/
  // METHODS
  /**********************************************************************************************************************/

public:

  /**
  Set and get for handler function
  */
  inline HandlerFunction GetHandler( void ){
    return mAssertHandler.GetHandlerFunction();
  }
  inline void SetHandler( HandlerFunction newHandler ){
    mAssertHandler.SetHandlerFunction( newHandler );
  }

  /**
  Report assertion as necessary
  @param condition Condition that triggered the assertion
  @param message Assert message to display
  @param file File that triggered the assertion
  @param line Line of the file that triggered the assertion
  @return True if execution must be halted on assert call
  */
  bool Report( const char* condition, const char* message, const char* file, int line );

private:

  // Constructor and destructor private for singleton (only one instance can be created)
  /**
  Private constructor for AssertionManager singleton
  */
  AssertionManager( void ) { }

  /**
  Private constructor for AssertionManager singleton
  */
  /*virtual*/ ~AssertionManager( void ) { } // Avoid virtual if not strictly necessary (Singletons don't use inheritance)

  /**********************************************************************************************************************/
  // ATTRIBUTES
  /**********************************************************************************************************************/

  AssertHandler mAssertHandler;
};

/**********************************************************************************************************************/

///< Halt execution ( Visual studio specific )
#define AssertHalt(   ) do{ __debugbreak();  }while(0)
///< Empty assert for release
#define AssertVoid( x ) do{ (void)sizeof(x); }while(0)

// Define asserts disabled if asserts are not needed
// NOTE: ASSERTS_DISABLED should be defined on the project properties (Preprocessor definitions) on ReleaseOptimized version
//#define ASSERTS_DISABLED

///< If assertions are enabled create macros otherwise empty macros
#ifndef ASSERTS_DISABLED

  #define AssertCondition( condition )                                                                            \
    do{                                                                                                           \
      if( !(condition) && AssertionManager::GetInstance().Report( #condition,         0, __FILE__, __LINE__ ) ){  \
        AssertHalt();                                                                                             \
      }                                                                                                           \
    }while(0)

  #define AssertMessage( condition, message )                                                                     \
    do{                                                                                                           \
      if( !(condition) && AssertionManager::GetInstance().Report( #condition, (message), __FILE__, __LINE__ ) ){  \
        AssertHalt();                                                                                             \
      }                                                                                                           \
    }while(0)

  #define AssertForced( message )                                                                                 \
    do{                                                                                                           \
      if(                 AssertionManager::GetInstance().Report(       NULL, (message), __FILE__, __LINE__ ) ){  \
        AssertHalt();                                                                                             \
      }                                                                                                           \
    }while(0)

#else

  #define AssertCondition( condition )        do{ AssertVoid(condition);                      }while(0)
  #define AssertMessage( condition, message ) do{ AssertVoid(condition); AssertVoid(message); }while(0)
  #define AssertForced( message )             do{ AssertVoid(message);                        }while(0)

#endif

/**********************************************************************************************************************/

#endif
