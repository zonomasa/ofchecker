ofchecker
=========

Lightweight overflow checker.

Simply detect memory corruption as we say OVERFLOW.
q
You can get following information about overflow.

 * Overflow bytes

 * Memory address of the block

 * Block size

 * Backtrace(with filename : line-number)


Detail
------

`ofchecker` detect overflow writing to a region allocated by malloc() and suchlike.

Glibc malloc has similar mechanism MALLOC_CHECK_. It is easy to use and lightweight,
and shows followings.

```
$ MALLOC_CHECK_=3 ./test/ofc_test
*** Error in `./test/ofc_test': free(): invalid pointer: 0x000000000153c010 ***
======= Backtrace: =========
/lib/x86_64-linux-gnu/libc.so.6(+0x741cf)[0x7f9c96c3e1cf]
/lib/x86_64-linux-gnu/libc.so.6(+0x82df6)[0x7f9c96c4cdf6]
./test/ofc_test(test_malloc+0x45)[0x400a92]
./test/ofc_test(main+0x13)[0x400d44]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5)[0x7f9c96bebec5]
./test/ofc_test[0x400989]
```

`ofchecker` can show more infomation.

```
$ ./bin/ofchecker ./test/ofc_test
<ofchecker> : Corrupted memory after payload : 9 byte
<ofchecker> : Address : 0x1f32010
<ofchecker> : Size : 7
<ofchecker> : ***  Start backtrace ***
./src/libofcbt.so(ofc_bt+0x18)[0x7f82df3bd7c8]
./src/libofc.so(free+0xc8)[0x7f82df5bfc18]
/home/tatezono/github/ofchecker/test/test.c:18
/home/tatezono/github/ofchecker/test/test.c:114
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5)[0x7f82df018ec5]
./test/ofc_test[0x400989]
<ofchecker> : ***   End backtrace  ***
```

ofcheker provides multiple byte overflow detection and showing backtrace when overflow is detected.

When you call malloc(), ofchecker simply add extra region(red-zone) after your payload filled by magic word, and when free(), check the red-zone wheter be overwritten or not.

ofchecker provides glibc like malloc(), calloc(), realloc(), posix_memalign() and free(). The functions creates or checks red-zone and then call glibc real malloc()(or calloc(), realloc(), free()). So you can use ofchecker by linking to your applications directly or by using LD_PRELOAD.

In addition, ofchecker script convert `memory addresses in backtrace` to `filename:line-number`.

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
    $ LD_PRELOAD="./src/libofc.so ./src/libofcbt.so" ./test/ofc_test

Furthermore you can use `bin/ofchecker` which be able to use LD_PRELOAD easily, and
convert backtrace infomation to user friendly style.

    $ ./bin/ofchecker ./test/ofc_test

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
