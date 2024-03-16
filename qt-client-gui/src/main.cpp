#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("config,c", po::value<std::string>()->default_value("../src/cfg.ini"), "configuration file");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }
    Client &cl = Client::get_instance();
    cl.load_cfg(vm);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  return 1;
  }
}
