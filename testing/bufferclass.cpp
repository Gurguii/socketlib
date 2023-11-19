#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <new>

#define __BUFFSIZE (4 * 1024 * 1024) // 4Mb
using ui8 = uint8_t;

namespace gsocket::testing
{
class Buffer{
  private:
    ui8 *ptr;
  public:
    uint64_t size;
    Buffer(uint64_t size = __BUFFSIZE, ui8 filler = 0):ptr((uint8_t*)malloc(size)),size(size){
      if(ptr == NULL){
        throw std::bad_alloc(); 
      }
      memset(ptr,filler,size);
    }
    ~Buffer(){
     free(ptr);
     ptr = NULL;
    }
    ui8* const beg(){
     return ptr;
    }
    ui8* const end(){
     return ptr+size;
    }
    ui8 const at(uint64_t index){
      return *(ptr+index);
    }
    ui8* const from(uint64_t index){
      return ptr+index;
    }
  };
}
