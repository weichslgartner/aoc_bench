After the nice experience of using absl::flat_hash_map, I wanted to try the folly::F14FastMap from folly.
So I just add folly to conanfile.txt

```
folly/2020.08.10.00
```

After hitting conan install, I recognized the boost dependencies and compiling stopped after some while with some fmt lib error.
So I checked the interweb and I found https://github.com/facebook/folly/issues/1262
It doesen't matter if you have fmt in your conanfile, folly wants fmt installed from source.
Fine, install fmt from source.

Now, compiling goes a bit further but stops with (https://github.com/facebook/folly/issues/1414):
```
$<COMPILE_LANG_AND_ID:lang,id> may only be used with binary targets
```
I have cmake 3.19. Time to give up conan.

Let's try folly from source.
Git clone, apt install as described and hit build.sh. 
This results in some weird errors.
Okay what about classic mkdir build and cmake.
Oh wow (https://github.com/facebook/folly/issues/838,https://github.com/facebook/folly/issues/976):

```
/usr/include/malloc.h:64:14: error: exception specification in declaration does not match previous declaration
extern void *memalign (size_t __alignment, size_t __size)
             ^
/usr/include/jemalloc/jemalloc.h:167:24: note: previous declaration is here
JEMALLOC_EXPORT void *  je_memalign(size_t alignment, size_t size)
                        ^
/usr/include/jemalloc/jemalloc.h:57:23: note: expanded from macro 'je_memalign'
#  define je_memalign memalign
                      ^
```

Okay what about installing jemalloc from source.
This works instantly.

So after fixing some final linker flags(https://github.com/facebook/folly/issues/1134) my folly finally compiles and can be installed
