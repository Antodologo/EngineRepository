#include "AssertionManager.h"

// Needed for abort
#include <signal.h>

// Windows includes
#include "TypesWindows.h"

// Utils for strings
#include "StringUtils.h"

// Log assert messages
#include "LogManager.h"

/**********************************************************************************************************************/

bool AssertionManager::AssertHandler::DefaultHandler( const char* condition, const char* message, const char* file, const int line )
{
  // Assert message
  std::string assertMessage("");
  // Add condition (if any) to assert message
  if( condition ){
    assertMessage = std::string( condition );
    // Add separator if needed
    if( message ){
      assertMessage += "\n";
    }
  }
  // Add message (if any) to assert message
  if( message ){
    assertMessage += std::string( message );
  }

  // Log assert into log file
  LogAssert( assertMessage, file, line );

  // Show assert window and return true if user asked to halt execution
  return ShowAssertWindow( assertMessage );
}

/**********************************************************************************************************************/

void AssertionManager::AssertHandler::LogAssert( const std::string &assertMessage, const char* file, const int line )
{
  // Log message
  std::string logMessage = MakeStr("ASSERTION FAILED:  ")( assertMessage )
                            ("\n                      FILE: ")(file)(" LINE: ")(line).GetString();

  // Log into file with assert level
  LogManager::GetInstance().LogMessage( logMessage, LogManager::LOG_MESSAGE_LEVEL_ASSERT );
}

/**********************************************************************************************************************/

bool AssertionManager::AssertHandler::ShowAssertWindow( const std::string &windowMessage )
{
  // Break into debugger?
  bool breakIntoDebugger = true;

  // If there is a debugger show window for entering debugger. Otherwise only show assert
  if( IsDebuggerPresent() ){
    // Show assert window
    int returnCode = MessageBox( NULL, windowMessage.c_str(), "Assertion failed",
      MB_ABORTRETRYIGNORE | MB_ICONSTOP | MB_SETFOREGROUND | MB_DEFBUTTON2 );

    // Check option selected
    if(       returnCode == IDABORT ){
      // Abort program
      raise(SIGABRT); // raise abort signal
    }
    else if(  returnCode == IDRETRY ){
      // Break into debugger
      breakIntoDebugger = true;
    }
    else if(  returnCode == IDIGNORE ){
      // Ignore assert
      breakIntoDebugger = false;
    }
  }
  else{
    // Show another window when no debugger attached
    int returnCode = MessageBox( NULL, windowMessage.c_str(), "Assertion failed", MB_OKCANCEL | MB_ICONSTOP | MB_SETFOREGROUND );

    // Check option selected
    if(      returnCode == IDOK ){
      // All OK ignore assert
      breakIntoDebugger = false;
    }
    else if( returnCode == IDCANCEL ){
      // Abort program
      raise(SIGABRT); // raise abort signal
    }
  }

  return breakIntoDebugger;
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/

bool AssertionManager::Report( const char* condition, const char* message, const char* file, const int line )
{
  return mAssertHandler.GetHandlerFunction()(condition, message, file, line);
}

/**********************************************************************************************************************/