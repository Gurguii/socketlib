#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <new>

#define __BUFFSIZE (4 * 1024 * 1024) // 4Mb
namespace gsocket::testing
{
class Buffer{
  private:
    uint8_t *ptr;
  public:
    uint64_t size;
    Buffer(uint64_t size = __BUFFSIZE, uint8_t filler = 0):ptr((uint8_t*)malloc(size)),size(size){
      if(ptr == NULL){
        throw std::bad_alloc(); 
      }
      memset(ptr,filler,size);
    }
    ~Buffer(){
     free(ptr);
     ptr = NULL;
    }
    uint8_t* const beg(){
     return ptr;
    }
    uint8_t* const end(){
     return ptr+size;
    }
    uint8_t const at(uint64_t index){
      return *(ptr+index);
    }
    uint8_t* const from(uint64_t index){
      return ptr+index;
    }
  };
}
