#pragma once
#include <l4/sys/capability>
#include <l4/sys/cxx/ipc_iface>

struct Mandel : L4::Kobject_t<Mandel, L4::Kobject, 0x44>
{
  L4_INLINE_RPC(int, getPoint, (int * x, int * y, int * n));
  L4_INLINE_RPC(int, hasPoint, (int  * res));
  L4_INLINE_RPC(int, setPoint, (int x, int y, int res));
  L4_INLINE_RPC(int, getScreen, (int * w, int *h));
  typedef L4::Typeid::Rpcs<getPoint_t, hasPoint_t, setPoint_t, getScreen_t> Rpcs;
};
