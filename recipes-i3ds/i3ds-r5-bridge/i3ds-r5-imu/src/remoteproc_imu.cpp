///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_LOG_DYN_LINK
#include <boost/log/trivial.hpp>

#include <i3ds/time.hpp>

#include "remoteproc_imu.hpp"

i3ds::RemoteProcIMU::RemoteProcIMU(Context::Ptr context, NodeID node)
  : IMU(node),
    publisher_(context, node)
{
  BOOST_LOG_TRIVIAL(info) << "RemoteProc IMU with NodeID: " << node << " created";
  set_device_name("RemoteProc R5 IMU");
}

i3ds::RemoteProcIMU::~RemoteProcIMU()
{
  BOOST_LOG_TRIVIAL(info) << "RemoteProc IMU with NodeID: " << node() << " deleted";
}

void
i3ds::RemoteProcIMU::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "RemoteProc IMU with NodeID: " << node() << " do_activate()";
}

void
i3ds::RemoteProcIMU::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "RemoteProc IMU with NodeID: " << node() << " do_start()";

  running_ = true;

  worker_ = std::thread(&i3ds::RemoteProcIMU::Run, this);
}

void
i3ds::RemoteProcIMU::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "RemoteProc IMU with NodeID: " << node() << " do_stop()";

  running_ = false;

  if (worker_.joinable())
    {
      worker_.join();
    }
}

void
i3ds::RemoteProcIMU::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "RemoteProc IMU with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::RemoteProcIMU::is_sampling_supported(SampleCommand sample)
{
  BOOST_LOG_TRIVIAL(info) << "RemoteProc IMU with NodeID: " << node() << " is_period_supported()";

  // Only support batch size 20 and period 0.1 seconds
  return sample.batch_size == 20 && sample.period == 100000;
}

void
i3ds::RemoteProcIMU::Run()
{
  const uint32_t address_id = (node() << 8) + MeasurementTopic::endpoint;

  int retries = 0;
  
  while (running_) {

    // Clear frame.
    IMUMeasurement20_Initialize(&frame_);

    // Read IMU data into samples array.
    const int size = remoteproc_read(address_id, (uint8_t *) frame_.samples.arr, sizeof(frame_.samples.arr));

    // Error if size is less than zero or not multiple of sample size.
    if (size < 0 || (size % sizeof(IMUSample))) {

      retries++;

      BOOST_LOG_TRIVIAL(info) << "Error reading remoteproc, uxexpected size: " << size;

      if (retries < 10)
	{
	  continue;
	}
      else
	{
	  set_failure();
	  break;
	}
    }

    // Reset retries on every success.
    retries = 0;
    
    // Ignore if size is zero, wait for half 10 ms and try again.
    if (size == 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      continue;
    }

    // Compute number of samples read, set attributes and send measurement.
    const int n = size / sizeof(IMUSample);
    
    frame_.attributes.timestamp = i3ds::get_timestamp();
    frame_.attributes.validity = sample_valid;

    frame_.batch_size = n;
    frame_.samples.nCount = n;

    publisher_.Send<MeasurementTopic>(frame_);
  }
}
