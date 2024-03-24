#include "server.hpp"
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

auto main(int argc, char const *const *argv) -> int {
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("config,c", po::value<std::string>()->default_value("../src/cfg.ini"), "configuration file")
      ("log,l", po::value<std::string>()->default_value("../log"), "log file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }
    
    google::InitGoogleLogging(argv[0]);
    auto logdir = boost::filesystem::absolute(vm["log"].as<std::string>());
    google::SetLogDestination(google::GLOG_INFO, (logdir / "INFO_").string().c_str());
    google::SetLogDestination(google::GLOG_ERROR, (logdir / "ERROR_").string().c_str());

    LOG(INFO) << "Arguments parsed successfully";

    Server& s = Server::get_instance();
    s.load_cfg(vm);
    s.run();
    return 0;
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}