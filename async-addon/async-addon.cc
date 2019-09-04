/*
*   An example of an asynchronous C++ node addon.
*   Provided by paulhauner https://github.com/paulhauner
*   License: MIT
*   Tested in node.js v4.4.2 LTS in Ubuntu Linux
*/

#include <node.h>
#include <uv.h>
#include <iostream>
#include <unistd.h>


using namespace std;


namespace asyncAddon
 {

  using v8::Function;
  using v8::FunctionCallbackInfo;
  using v8::Persistent;
  using v8::Isolate;
  using v8::Context;
  using v8::Global;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using v8::Persistent;
  using v8::NewStringType;
  using v8::HandleScope;


  /**
  * Work structure is be used to pass the callback function and data 
  * from the initiating function to the function which triggers the callback.
  */
  
  struct Work
   {

    uv_work_t  request ;

    Persistent<Function> callback ;

    string result ;

   } ;

  
  /**
  * WorkAsync function is the "middle" function which does the work.
  * After the WorkAsync function is called, the WorkAsyncComplete function
  * is called.
  */

  static void workAsync( uv_work_t *req )
   {

    Work *work = static_cast<Work *>(req->data) ;
    
    sleep(3) ;

    work->result = "Async task successfully processed." ;

   } ;

  
  /**
  * WorkAsyncComplete function is called once we are ready to trigger the callback
  * function in JS.
  */

  static void workAsyncComplete( uv_work_t *req , int status )
   {

    Isolate *isolate = Isolate::GetCurrent() ;

    HandleScope handle_scope( isolate ) ;

    Local<Context> context = isolate -> GetCurrentContext() ;

    Local<Value> recv = Undefined( isolate ) ;

    Work *work = static_cast<Work *>(req->data) ;
    
    const char *result = work->result.c_str() ;

    Local<Value> argv[] = { String::NewFromUtf8( isolate , result , NewStringType::kNormal ).ToLocalChecked() } ;
    
    // https://stackoverflow.com/questions/13826803/calling-javascript-function-from-a-c-callback-in-v8/28554065#28554065

    printf( "%s\n\n" , "Async function callback calling ..." ) ;

    Local<Function>::New( isolate , work->callback )->Call( context , recv , 1 , argv );
    
    printf( "\n%s\n" , "... Async function callback returned." ) ;

    work->callback.Reset( isolate , work->callback ) ;

    delete work ;

    return ;

   } ;
  

  /**
  * DoTaskAsync is the initial function called from JS. This function returns
  * immediately, however starts a uv task which later calls the callback function
  */

  void doTaskAsync( const FunctionCallbackInfo<Value>& args )
   {

    Isolate* isolate = args.GetIsolate() ;

    HandleScope handle_scope( isolate ) ;
    
    Work * work = new Work() ;

    work->request.data = work ;
    
    // args[0] is where we pick the callback function out of the JS function params.
    // Because we chose args[0], we must supply the callback fn as the first parameter

    Local<Function> callback = Local<Function>::Cast( args[0] ) ;

    work->callback.Reset( isolate , callback ) ;
    
    uv_queue_work( uv_default_loop(), &work->request, workAsync, workAsyncComplete ) ;
    
    args.GetReturnValue().Set( Undefined( isolate ) ) ;

    return ;

  } ;
  
  
  /**
  * init function declares what we will make visible to node
  */

  void init( Local<Object> exports , Local<Object> module )
   {

    NODE_SET_METHOD( exports , "doTaskAsync" , doTaskAsync ) ;

   }


  NODE_MODULE(asyncAddon, init)


}  
