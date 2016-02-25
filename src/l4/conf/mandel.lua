local L4 = require("L4");
local ld = L4.default_loader;

local io_busses = {
  gui = ld:new_channel();
  fbdrv = ld:new_channel();
};

ld:start({ 
 caps = {
          gui = io_busses.gui:svr(),
          fbdrv = io_busses.fbdrv:svr(),
          icu = L4.Env.icu,
          sigma0 = L4.cast(L4.Proto.Factory, L4.Env.sigma0):create(L4.Proto.Sigma0),
        },
   },
           "rom/io rom/mandel.devs rom/mandel.io");


local fbdrv_fb = ld:new_channel();
ld:start({
    caps= {
     vbus=io_busses.fbdrv,
     fb = fbdrv_fb:svr(),
    },
  }, 
  "rom/fb-drv");

local mandelserver = ld:new_channel();
ld:start({ 
 caps = {mandelserver = mandelserver },
           },
           "rom/mandelclient" );
ld:start({ 
 caps = {mandelserver = mandelserver },
           },
           "rom/mandelclient" );
ld:start({ 
 caps = {mandelserver = mandelserver },
           },
           "rom/mandelclient" );
ld:start({ 
 caps = {mandelserver = mandelserver },
           },
           "rom/mandelclient" );
ld:start({ 
 caps = {mandelserver = mandelserver },
           },
           "rom/mandelclient" );
ld:start({ 
 caps = {
          mandelserver = mandelserver:svr(),
          fb = fbdrv_fb,
        },
           },
           "rom/mandelserver");
