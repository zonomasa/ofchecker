ofchecker
=========

Lightweight overflow checker.

Simply detect memory corruption as we say OVERFLOW.


Build from Source
-----------------

1. Make sure you have installed the dependency:
    * CMake 2.8.1 or later

2. Checkout and build:

    $ git clone https://github.com/zonomasa/ofchecker.git
    $ cd ofchecker
    $ cmake .
    $ make

Now that you have libofc.so and libofcbt.so in your 'src' dir.


Usage
-----

You can use ofchecker by directly linking with your application or just use LD_PRELOAD.

To use by LD_PRELOAD do:

    $ LD_PRELOAD="./src/libofc.so ./src/libofcbt.so" $(your_application)
    for example:
    $ LD_PRELOAD="./src/libofc.so ./src/libofcbt.so" vi


TODO
----

 * Now Prototype version.
   * Delete Debug message.
   * Add posix_memalign.


Contributing
------------

1. Fork it ( https://github.com/zonomasa/ofchecker/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request


Copyright
---------

 * Copyright (c) 2014 TATEZONO Masaki
 * License   Apache License, Version 2.0
