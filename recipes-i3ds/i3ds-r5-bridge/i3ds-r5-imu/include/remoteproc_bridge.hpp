///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_REMOTEPROC_BRIDGE_HPP
#define __I3DS_REMOTEPROC_BRIDGE_HPP

#include <string>
#include <cstdint>

namespace i3ds
{

int remoteproc_open(std::string dev);
  
int remoteproc_read(uint32_t address, uint8_t *buffer, size_t size);

int remoteproc_close();

} // namespace i3ds

#endif
