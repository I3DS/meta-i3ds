///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <csignal>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <memory>

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <i3ds/communication.hpp>

#include "remoteproc_bridge.hpp"
#include "remoteproc_imu.hpp"

namespace po = boost::program_options;
namespace logging = boost::log;

volatile bool running;

void signal_handler(int signum)
{
  BOOST_LOG_TRIVIAL(info) << "Stopping " << signum;
  running = false;
}

int main(int argc, char *argv[])
{
  unsigned int node_id;
  std::string device;

  po::options_description desc("Allowed camera control options");
  desc.add_options()
  ("help,h", "Produce this message")
  ("node,n", po::value<unsigned int>(&node_id)->default_value(310), "Node ID of IMU")
  ("device,d", po::value<std::string>(&device)->default_value("/dev/rpmsg0"), "RemoteProc device")
  ("verbose,v", "Print verbose output")
  ("quiet,q", "Quiet ouput");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return -1;
    }

  if (vm.count("quiet"))
    {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::warning);
    }
  else if (!vm.count("verbose"))
    {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
    }

  po::notify(vm);

  BOOST_LOG_TRIVIAL(info) << "Using node ID: " << node_id;

  printf("\r\n I3DS R5 IMU bridge \r\n");

  sleep(1);

  printf("\r\n Open rpmsg dev! \r\n");

  if (i3ds::remoteproc_open(device))
    {
      BOOST_LOG_TRIVIAL(error) << "Failed to open remoteproc: " << device;
      return -1;
    }

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Server server(context);

  i3ds::RemoteProcIMU imu(context, node_id);

  imu.Attach(server);

  running = true;
  signal(SIGINT, signal_handler);

  server.Start();

  while (running)
    {
      sleep(1);
    }

  server.Stop();

  i3ds::remoteproc_close();

  printf("\r\n Quitting application .. \r\n");
  printf(" I3DS R5 IMU bridge end \r\n");

  return 0;
}
