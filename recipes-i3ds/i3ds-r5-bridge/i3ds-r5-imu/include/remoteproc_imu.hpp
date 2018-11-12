///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_REMOTEPROC_IMU_HPP
#define __I3DS_REMOTEPROC_IMU_HPP

#include <thread>

#include <i3ds/publisher.hpp>
#include <i3ds/imu_sensor.hpp>

#include "remoteproc_bridge.hpp"

namespace i3ds
{

class RemoteProcIMU : public IMU
{
public:

  typedef std::shared_ptr<RemoteProcIMU> Ptr;

  static Ptr Create(Context::Ptr context, NodeID id)
  {
    return std::make_shared<RemoteProcIMU>(context, id);
  }

  RemoteProcIMU(Context::Ptr context, NodeID id);
  virtual ~RemoteProcIMU();

  // Supported period.
  virtual bool is_sampling_supported(SampleCommand sample);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

private:

  // Run the receiver until stop is called.
  void Run();

  // Running flag.
  volatile bool running_;

  // Worker thread.
  std::thread worker_;

  // Publisher for IMU measurement.
  Publisher publisher_;

  // IMU measurement.
  IMUMeasurement20 frame_;
};

} // namespace i3ds

#endif
