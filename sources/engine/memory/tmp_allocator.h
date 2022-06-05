#pragma once

class tmp_allocator
{
public:
  tmp_allocator(const char* pName = "tmp_allocator");
  //tmp_allocator(const tmp_allocator& x);
  //tmp_allocator(const tmp_allocator& x, const char* pName);

  //tmp_allocator& operator=(const tmp_allocator& x);

  void* allocate(size_t n, int flags = 0);
  void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
  void  deallocate(void* p, size_t n);

  //const char* get_name() const;
  //void        set_name(const char* pName);


};
//bool operator==(const tmp_allocator& a, const tmp_allocator& b);
//bool operator!=(const tmp_allocator& a, const tmp_allocator& b);

void clear_tmp_allocation();