/*
*   An example of an asynchronous C++ node addon.
*   Provided by paulhauner https://github.com/paulhauner
*   License: MIT
*   Tested in node.js v4.4.2 LTS in Ubuntu Linux
*/

const addon = require('./async-addon/build/Release/asyncAddon');


function doTaskAsyncCallback( data )
 {

  console.log( data ) ;

  return ;

 } ;


var interval = setInterval
 (

  function()
   {

    console.log( 'Another operation.' ) ;

    return ;

   } , 500

 ) ;


setTimeout
 (

  function()
   {

    console.log( 'Async task started ...' ) ;

    return ;

   } , 1

 ) ;


setTimeout
 (

  function()
   {

    console.log( '... Async task finished.' ) ;

    clearInterval( interval ) ;

    return ;

   } , 5000

 ) ;


addon.doTaskAsync( doTaskAsyncCallback ) ;


