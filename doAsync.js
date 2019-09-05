/*
*   An example of an asynchronous C++ node addon.
*   Provided by paulhauner https://github.com/paulhauner
*   License: MIT
*   Tested in node.js v12.10.0 in openSUSE Linux Tumbleweed
*
*   In order to get latest node.js official release version X.X.X do:
*
*   cd [/Your/node.js/source/directory]
*   git checkout master ;
*   git pull ;
*   git checkout tags/vX.X.X ;
*   make distclean ;
*   ./configure --prefix=[/Your/node.js/destination/directory] ;
*   make -j N install ;  # set N for number of threads during make
*
*/


const addon = require('./async-addon/build/Release/asyncAddon');



function doTaskPromise( task )
 {

  let promise = new Promise
   (

    function( resolve , reject )
     {

      try
       {

        task
         (

          function( data )
           {

            resolve( data ) ;

            return ;

           }

         ) ;

       }

      catch( error )
       {

        console.log( error ) ;

       } ;

     }

   )

  return( promise ) ;

 }



async function doTaskAsync( task )
 {

  var result = undefined ;

  try
   {

    console.log( 'Async task started ...' ) ;

    var interval = setInterval
     (

      function()
       {

        console.log( 'Another operation.' ) ;

        return ;

       } , 500

     ) ;

    var result = await doTaskPromise( task )

    console.log( result ) ;

    clearInterval( interval ) ;

    console.log( '... Async task finished.' ) ;

   }

  catch( error )
   {

    console.log( error ) ;

   }

  return( result ) ;

 }



doTaskAsync( addon.doTaskAsync ) ;


